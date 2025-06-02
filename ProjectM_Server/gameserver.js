const express = require('express');
const http = require('http');
const {WebSocketServer}=require("ws");
const wss=new WebSocketServer({port: 8080});

let games={};
const tileDeck=require("./tiledeck.js");
const userMap=new Map();
const reverserMap=new Map();
const TURN_TIME_LIMIT=10000;
const CALL_TIME_LIMIT=3000;

class GameRoom {
    constructor(roomId) {
        this.roomId = roomId;
        this.players = []; // { id, ws }
        this.turnIndex = 0;
        this.deck=tileDeck.reset().copydeck();
        this.timer = null;
        this.turnTimeLimit = 10 * 1000; // 10초
        this.started = false;
        this.board=[];
        this.playerDeck=[];
    }

    addPlayer(id, ws) {
        this.players.push({ id, ws });
        this.playerDeck.push([]);
        this.broadcast({
            type: "join",
            players: this.players.map(p => p.id)
        });

        if (this.players.length === 4) {
            this.startGame();
        }
    }


    startGame() {
        this.started = true;
        this.broadcast({ type: "start" });
        this.startTurn();
    }

    startTurn() {
        const current = this.players[this.turnIndex];
        this.broadcast({
            type: "your_turn",
            playerId: current.id,
            turnTime: this.turnTimeLimit / 1000
        });

        this.timer = setTimeout(() => {
            this.handleTimeout(current.id);
        }, this.turnTimeLimit);
    }

    receiveAction(playerId, action) {
        if (this.players[this.turnIndex].id !== playerId) {
            this.sendTo(playerId, { type: "error", message: "Not your turn." });
            return;
        }

        clearTimeout(this.timer);

        this.broadcast({
            type: "action_result",
            playerId,
            action
        });

        this.nextTurn();
    }

    handleTimeout(playerId) {
        this.broadcast({
            type: "turn_timeout",
            playerId
        });
        this.nextTurn();
    }

    nextTurn() {
        this.turnIndex = (this.turnIndex + 1) % this.players.length;
        this.startTurn();
    }

    sendTo(playerId, message) {
        const player = this.players.find(p => p.id === playerId);
        if (player) {
            player.ws.send(JSON.stringify(message));
        }
    }

    broadcast(message) {
        this.players.forEach(p => {
            p.ws.send(JSON.stringify(message));
        });
    }
}


wss.on("connection",ws=>{
    ws.on("message",msg=>{
        const data=JSON.parse(msg);
        console.log("수신: ",data);
        let game
        switch(data.type){
            case 'init':

                ws.playerId=data.playerId;
                ws.gameId=null;

                break;
            case 'match':

                let gameId;
                while(true){ // 중복되지 않은 랜덤 gameId 생성
                    gameId=Math.random().toString(36).substring(2,8);
                    if(!games[gameId]) break;
                }

                games[gameId]=new GameRoom(gameId);
                games[gameId].addPlayer(data.userId,ws);
                ws.gameId=gameId;

                break;
            case 'join':

                game=games[data.gameId];
                if(!game){
                    ws.send(JSON.stringify({type:"join",status:"invalid"}));
                }else{
                    if(game.players.length>=4){
                        ws.send(JSON.stringify({type:"join",status:"full"}));
                    }
                    else{
                        game.addPlayer(data.playerId,ws);
                    }
                }
                
                break;
            case 'ready':
                break;
            
        }
    });

    ws.on("close",()=>{
        if(ws.gameId){
            
        }
    })
});

wss.broadcast=(game,msg)=>{
    game.players.forEach(player=>{
        if(userMap.get(player).readyState===WebSocket.OPEN){
            userMap.get(player).send(msg);
        }
    });
};
