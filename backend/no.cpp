#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define CE_PIN 7
#define CSN_PIN 8

#define MSG 0
#define ACK 1
#define RTS 2
#define CTS 3

#define TIMEOUT 1500  // milisegundos

RF24 radio(CE_PIN, CSN_PIN);
uint64_t address[2] = { 0x3030303030LL, 0x3030303030LL };

int POS_REDE    = 0;
int POS_DESTINO = 1;
int POS_ORIGEM  = 2;
int POS_TIPO    = 3;
int POS_JOGADOR = 4;

byte MENSAGEM[5] = "     ";

uint8_t REDE      = 12;
uint8_t ID_SERVER = 22;
uint8_t ID_NO     = 9; // 9 no outro jogador

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  // Initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}
  }

  radio.setPALevel(RF24_PA_MAX);  // RF24_PA_MAX is default.
  radio.setChannel(100);
  radio.setPayloadSize(sizeof(MENSAGEM));
  radio.setAutoAck(false);
  radio.setCRCLength(RF24_CRC_DISABLED);
  radio.setDataRate(RF24_2MBPS);

  radio.openWritingPipe(address[0]);     // always uses pipe 0
  radio.openReadingPipe(1, address[1]);  // using pipe 1

  printf_begin();
  radio.printPrettyDetails();
}

bool aguardaMsg(int tipo) {
  radio.startListening();
  unsigned long tempoInicio = millis();
  while (millis() - tempoInicio < TIMEOUT) {
    if (radio.available()) {
      uint8_t bytes = radio.getPayloadSize();
      radio.read(&MENSAGEM[POS_REDE], bytes);
      if (MENSAGEM[POS_DESTINO] == ID_NO && MENSAGEM[POS_TIPO] == tipo) {
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
  pacote[POS_ORIGEM] = ID_NO;
  pacote[POS_TIPO] = tipo;
  pacote[POS_JOGADOR] = ID_NO;

  Serial.println("enviando");

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
  if (Serial.available()) {

    char c = Serial.read();
    Serial.println(c);

    unsigned long start_timer = micros();
    bool report = sendPacket(&MENSAGEM[0], sizeof(MENSAGEM), ID_SERVER, RTS);
    report = aguardaMsg(CTS);
    if (report) {
      sendPacket(&MENSAGEM[0], sizeof(MENSAGEM), ID_SERVER, MSG);
      report = aguardaMsg(ACK);
    }

    unsigned long end_timer = micros();
    if (report) {
      Serial.println("Sucesso!");
    } else {
      Serial.println("FALHA!");
    }
  
    while(Serial.available() > 0) {
      Serial.read();
    }
  }

  radio.flush_rx();
  delay(10);
}
