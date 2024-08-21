#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define CE_PIN 7
#define CSN_PIN 8

#define MSG 0
#define ACK 1
#define RTS 2
#define CTS 3

#define JOGADOR1 4
#define JOGADOR2 9

RF24 radio(CE_PIN, CSN_PIN);
uint64_t address[2] = { 0x3030303030LL, 0x3030303030LL};

int POS_REDE    = 0;
int POS_DESTINO = 1;
int POS_ORIGEM  = 2;
int POS_TIPO    = 3;
int POS_JOGADOR = 4;

byte MENSAGEM[5] = "     ";

uint8_t ID_SERVER = 22;
uint8_t REDE = 12;

int PONTOS_J1 = 0;
int PONTOS_J2 = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  if (!radio.begin()) {
    Serial.println(F("O hardware do rádio não está respondendo!!"));
    while (1) {}
  }

  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(100);
  radio.setPayloadSize(sizeof(MENSAGEM));
  radio.setAutoAck(false);
  radio.setCRCLength(RF24_CRC_DISABLED);
  radio.setDataRate(RF24_2MBPS);

  radio.openWritingPipe(address[0]);
  radio.openReadingPipe(1, address[1]);

  printf_begin();  // Necessário apenas uma vez para impressão de detalhes
  radio.printPrettyDetails(); // Função que imprime dados legíveis para humanos
}

void pontuar(uint8_t jogador) {
    if (jogador == JOGADOR1) {
        PONTOS_J1 = calcularPontuacao(PONTOS_J1);
    } else if (jogador == JOGADOR2) {
        PONTOS_J2 = calcularPontuacao(PONTOS_J2);
    }
    verificaGanhador();
    exibirPontuacao();
}

void verificaGanhador() {
    if (PONTOS_J1 == 45) {
        Serial.println("Jogador 1 venceu");
        PONTOS_J1 = 0;
        PONTOS_J2 = 0;
    } else if (PONTOS_J2 == 45) {
        Serial.println("Jogador 2 venceu");
        PONTOS_J1 = 0;
        PONTOS_J2 = 0;
    }
}

void exibirPontuacao() {
  Serial.print("Jogador 1: ");
  Serial.println(String(PONTOS_J1));
  Serial.print("Jogador 2: ");
  Serial.println(String(PONTOS_J2));
}

int calcularPontuacao(int pontosJogador) {
    if (pontosJogador == 30) {
        return 40;
    } 
    if (pontosJogador == 40) {
        return 45;
    }
    return pontosJogador + 15;
}

bool aguardaMsg(int tipo) {
  radio.startListening();
  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < 1000) {
    if (radio.available()) {
      uint8_t bytes = radio.getPayloadSize();
      radio.read(&MENSAGEM[0], bytes);
      if (MENSAGEM[POS_DESTINO] == ID_SERVER && MENSAGEM[POS_TIPO] == tipo) {
        radio.stopListening();
        return true;
      }
    }
    radio.flush_rx();
    delay(10);
  }
  radio.stopListening();
  return false;
}

bool sendPacket(byte *pacote, int tamanho, int destino, int tipo) {
  pacote[POS_REDE] = REDE;
  pacote[POS_DESTINO] = destino;
  pacote[POS_ORIGEM] = ID_SERVER;
  pacote[POS_TIPO] = tipo;
  pacote[POS_JOGADOR] = 0;

  while (1) {
    radio.startListening();
    delayMicroseconds(70);
    radio.stopListening();
    if (!radio.testCarrier()) {
      return radio.write(&pacote[0], tamanho);
    } else {
      Serial.println("Meio Ocupado");
      delayMicroseconds(270);
    }
    radio.flush_rx();
  }
}

void loop() {
  // Aguardando RTS
  radio.startListening();
  if (radio.available()) {
    uint8_t bytes = radio.getPayloadSize();
    radio.read(&MENSAGEM[0], bytes);

    // Se a rede for a minha (12)
    if (MENSAGEM[POS_REDE] == REDE && (MENSAGEM[POS_JOGADOR] == JOGADOR1 || MENSAGEM[POS_JOGADOR] == JOGADOR2)) {
      // Se o destino da mensagem for minha origem e o controle for RTS
      if (MENSAGEM[POS_DESTINO] == ID_SERVER && MENSAGEM[POS_TIPO] == RTS) {
        bool report = sendPacket(&MENSAGEM[0], sizeof(MENSAGEM), MENSAGEM[POS_ORIGEM], CTS);
        report = aguardaMsg(MSG);
        if (report) {
          pontuar(MENSAGEM[POS_JOGADOR]);
          report = sendPacket(&MENSAGEM[0], sizeof(MENSAGEM), MENSAGEM[POS_ORIGEM], ACK);
        }
      }
    }
  }
  radio.flush_rx();
  delay(10);
}
