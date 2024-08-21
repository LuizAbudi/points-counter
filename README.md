# Beach Tennis Points Counter

## Descrição

O **Beach Tennis Points Counter** é um sistema para gerenciar e exibir a pontuação de um jogo de beach tennis em tempo real. O projeto é composto por um backend em FastAPI, que se comunica com Arduinos para receber dados de pontuação, e um frontend em React que exibe a pontuação atualizada.

## Arquitetura

- **Frontend**: Construído com React e utiliza a biblioteca `react-7-segment-display` para exibir a pontuação dos times.
- **Backend**: Construído com FastAPI e utiliza WebSockets para receber dados em tempo real dos Arduinos e endpoints HTTP para interagir com o frontend.
- **Arduinos**: Dispositivos que enviam dados de pontuação para o backend via WebSocket.

## Funcionalidades

- **Recebimento de Dados**: O backend recebe dados de pontuação dos Arduinos através de WebSockets.
- **Exibição de Pontuação**: O frontend exibe a pontuação atual dos times em 7 segmentos, atualizando em tempo real conforme os dados recebidos.
- **APIs**: O backend fornece APIs REST para obter e enviar dados de pontuação.

## Tecnologias Utilizadas

- **Frontend**: React, `react-7-segment-display`
- **Backend**: FastAPI, WebSocket
- **Comunicação**: WebSocket entre Arduinos e backend, HTTP entre frontend e backend

## Instalação

### Backend

1. Clone o repositório:

   ```bash
   git clone https://github.com/LuizAbudi/points-counter.git
   cd seu-repositorio
   ```

2. Crie e ative um ambiente virtual:

   ```bash
   python -m venv venv
   source venv/bin/activate  # No Windows use `venv\Scripts\activate`
   ```

3. Instale as dependências:

   ```bash
   pip install -r requirements.txt
   ```

4. Execute o servidor FastAPI:
   ```bash
   uvicorn app:app --reload
   ```

### Frontend

1. Navegue até o diretório do frontend e instale as dependências:

   ```bash
   cd frontend
   npm i
   ```

2. Execute o servidor de desenvolvimento:
   ```bash
   npm start
   ```

## Uso

1. **Arduinos**: Configure seus Arduinos para enviar dados de pontuação para o backend via WebSocket no endpoint `ws://localhost:8000/ws/game`.

2. **Frontend**: Acesse o frontend em [http://localhost:3000](http://localhost:3000) para visualizar a pontuação em tempo real.

## APIs

### WebSocket

- **Endpoint**: `/ws/game`
- **Método**: `WebSocket`
- **Descrição**: Recebe dados de pontuação dos Arduinos e envia confirmações.

### API REST

- **POST** `/api/game_data/`

  - **Descrição**: Recebe dados de pontuação do frontend ou outras fontes.
  - **Corpo da Requisição**:
    ```json
    {
      "score_team_a": 1,
      "score_team_b": 2
    }
    ```

- **GET** `/api/game_data/`
  - **Descrição**: Retorna dados de pontuação simulados para teste.
  - **Resposta**:
    ```json
    {
      "status": "Data sent",
      "data": {
        "score_team_a": 1,
        "score_team_b": 2
      }
    }
    ```
