import React, { useState, useEffect } from 'react';
import io from 'socket.io-client';
import './App.css';
import { Display } from "react-7-segment-display";

const socket = io('http://localhost:8000');

const App = () => {
  const [teamAScore, setTeamAScore] = useState(0);
  const [teamBScore, setTeamBScore] = useState(0);
  const [teamAScoreGame, setTeamAScoreGame] = useState(0);
  const [teamBScoreGame, setTeamBScoreGame] = useState(0);

  useEffect(() => {
    socket.on('update_score', (data) => {
      setTeamAScore(data.team_a_score);
      setTeamBScore(data.team_b_score);
      setTeamAScoreGame(data.team_a_score_game);
      setTeamBScoreGame(data.team_b_score_game);
    });

    return () => {
      socket.off('update_score');
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
