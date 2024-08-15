# app.py
from fastapi import FastAPI, WebSocket
from pydantic import BaseModel
from typing import List

app = FastAPI()


class GameData(BaseModel):
    score_team_a: int
    score_team_b: int


@app.websocket("/ws/game")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    while True:
        data = await websocket.receive_json()
        await websocket.send_json({
            "team_a_score": data.get("score_team_a", 0),
            "team_b_score": data.get("score_team_b", 0),
        })


@app.post("/api/game_data/")
async def receive_game_data(game_data: GameData):
    return {"status": "Data received", "data": game_data.dict()}


@app.get("/api/game_data/")
async def send_game_data():
    data = {"score_team_a": 1, "score_team_b": 2}
    return {"status": "Data sent", "data": data}
