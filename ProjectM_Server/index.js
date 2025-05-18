const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const bodyParser = require('body-parser');
const cors = require('cors');

const app = express();
const server = http.createServer(app);
const io = new Server(server, {
  cors: { origin: '*' }
});

app.use(cors());
app.use(bodyParser.json());

let games = {};  // 게임 ID → 상태 저장

// REST: 매치 생성
app.post('/match', (req, res) => {
  const { userId } = req.body;
  const gameId = "1111";
  console.log(userId);
  games[gameId] = {
    players: [userId],
    board: Array(4).fill(null),
    currentTurn: 0,
    status: 'waiting'
  };
  res.send({gameId:"1111"});
});

// REST: 매치 참가
app.post('/match/join', (req, res) => {
  const { gameId, userId } = req.body;
  const game = games[gameId];
  if (!game || game.players.length >= 4) return res.status(400).send("Full or invalid");
  game.players.push(userId);
  if (game.players.length === 4) game.status = 'playing';
  res.send({ success: true });
});

// WebSocket 처리
io.on('connection', (socket) => {
  socket.on('join-game', ({ gameId, userId }) => {
    socket.join(gameId);
    socket.gameId = gameId;
    socket.userId = userId;
  });

  socket.on('submit-turn', ({ gameId, move }) => {
    const game = games[gameId];
    if (!game) return;
    
    // 턴 검사 (간단 버전)
    const expectedUser = game.players[game.currentTurn];
    if (socket.userId !== expectedUser) return;

    // 보드 업데이트
    game.board[game.currentTurn] = move;
    game.currentTurn = (game.currentTurn + 1) % 4;

    io.to(gameId).emit('update', {
      board: game.board,
      currentTurn: game.currentTurn
    });
  });
});

server.listen(8080, () => {
  console.log('Server running on port 8080');
});
