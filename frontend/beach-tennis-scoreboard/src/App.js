import React, { useState, useEffect } from 'react';
import io from 'socket.io-client';
import './App.css';
import { Display } from "react-7-segment-display";

const socket = io('http://127.0.0.1:5000');

const App = () => {
  const [teamAScore, setTeamAScore] = useState(0);
  const [teamBScore, setTeamBScore] = useState(0);
  const [teamAScoreGame, setTeamAScoreGame] = useState(0);
  const [teamBScoreGame, setTeamBScoreGame] = useState(0);

  useEffect(() => {
    socket.on('placar_atualizado', (data) => {
      setTeamAScore(data.jogador1);
      setTeamBScore(data.jogador2);
      setTeamAScoreGame(data.jogos1);
      setTeamBScoreGame(data.jogos2);
    });

    return () => {
      socket.off('placar_atualizado');
    };
  }, []);

  return (
    <div className="app">
      <h1>Scoreboard</h1>
      <div className="scoreboard">
        <div className="score-sectionA">
          <h2>Team A</h2>
          <Display value={teamAScore} />
          <div className="scoreGame">
            <Display value={teamAScoreGame} height={100} count={1} />
          </div>
        </div>
        <div className="score-sectionB">
          <h2>Team B</h2>
          <Display value={teamBScore} />
          <div className="scoreGame">
            <Display value={teamBScoreGame} height={100} count={1} />
          </div>
        </div>
      </div>
    </div>
  );
};

export default App;
