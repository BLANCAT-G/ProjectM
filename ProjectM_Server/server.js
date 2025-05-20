const express = require('express');
const http = require('http');
const bodyParser = require('body-parser');
const cors = require('cors');
const {WebSocketServer}=require("ws");
const wss=new WebSocketServer({port: 8080});

const app = express();
const server = http.createServer(app);

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

server.listen(8080, () => {
  console.log('Server running on port 8080');
});

wss.on("connection",ws=>{
    ws.on("message",data=>{
        console.log('Received fron user: '+data);
    });

    ws.on("close",()=>{
        wss.clients.forEach(user=>{
            user.send('out 1');
        });
    });
});
