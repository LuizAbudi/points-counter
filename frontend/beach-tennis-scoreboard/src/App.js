import React, { useState, useEffect } from 'react';
import io from 'socket.io-client';
import './App.css';
import { Display } from "react-7-segment-display";

const socket = io('http://127.0.0.1:5000');

const App = () => {
  const [pontosJogador1, setPontosJogador1] = useState(0);
  const [pontosJogador2, setPontosJogador2] = useState(0);
  const [jogosJogador1, setJogosJogador1] = useState(0);
  const [jogosJogador2, setJogosJogador2] = useState(0);

  useEffect(() => {
    socket.on('placar_atualizado', (data) => {
      setPontosJogador1(data.jogador1);
      setPontosJogador2(data.jogador2);
      setJogosJogador1(data.jogos1);
      setJogosJogador2(data.jogos2);
    });

    return () => {
      socket.off('placar_atualizado');
    };
  }, []);

  return (
    <div className="app">
      <h1>Placar</h1>
      <div className="scoreboard">
        <div className="score-section1">
          <h2>Jogador1</h2>
          <Display value={pontosJogador1} />
          <div className="scoreGame">
            <Display value={jogosJogador1} height={100} count={1} />
          </div>
        </div>
        <div className="score-section2">
          <h2>Jogador 2</h2>
          <Display value={pontosJogador2} />
          <div className="scoreGame">
            <Display value={jogosJogador2} height={100} count={1} />
          </div>
        </div>
      </div>
    </div>
  );
};

export default App;
