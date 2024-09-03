import serial
import json
import threading
from flask import Flask, jsonify
from flask_socketio import SocketIO, emit
from flask_cors import CORS

# Configurações do Serial
ser = serial.Serial('/dev/ttyUSB0', 115200)

# Variáveis de placar
placar = {"jogador1": 0, "jogador2": 0, "jogos1": 0, "jogos2": 0}

# Configuração do Flask e SocketIO
app = Flask(__name__)
CORS(app)
socketio = SocketIO(app, cors_allowed_origins="*")

def ler_serial():
    while True:
        try:
            if ser.in_waiting > 0:
                linha = ser.readline().decode('utf-8').strip()
                print(linha)
                try:
                    dados = json.loads(linha)
                    if "jogador1" in dados and "jogador2" in dados and "jogos1" in dados and "jogos2" in dados:
                        placar['jogador1'] = dados['jogador1']
                        placar['jogador2'] = dados['jogador2']
                        placar['jogos1'] = dados['jogos1']
                        placar['jogos2'] = dados['jogos2']
                        print(f"Placar atualizado: {placar}")
                        # Emitir evento de mudança no placar via WebSocket
                        socketio.emit('placar_atualizado', placar)
                except json.JSONDecodeError:
                    print("Erro ao decodificar JSON")
        except serial.SerialException as e:
            print(f"Erro na leitura da porta serial: {e}")
            break

@app.route('/placar', methods=['GET'])
def get_placar():
    return jsonify(placar)

if __name__ == '__main__':
    # Iniciar o thread que lê a porta serial
    thread_serial = threading.Thread(target=ler_serial, daemon=True)
    thread_serial.start()

    # Iniciar o servidor Flask com SocketIO
    socketio.run(app, host='0.0.0.0', port=5000)
