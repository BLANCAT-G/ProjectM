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

wss.on("connection",ws=>{
    ws.on("message",msg=>{
        const data=JSON.parse(msg);
        console.log("수신: ",data);
        let game
        switch(data.type){
            case 'init':
                userMap.set(data.userId,ws);
                reverserMap.set(ws,data.userId);
                break;
            case 'match':
                let gameId;
                while(true){ // 중복되지 않은 랜덤 gameId 생성
                    gameId=Math.random().toString(36).substring(2,8);
                    if(!games[gameId]) break;
                }

                games[gameId]= { // 게임 정보
                    players: [data.userId],
                    deck: [],
                    currentTurn: 0,
                    readyPlayer: 0,
                    status: 'waiting'
                };
                
                const msg_match={
                    type:"match",
                    gameId: gameId
                };
                console.log("송신: ",msg_match);
                ws.send(JSON.stringify(msg_match));
                break;
            case 'join':
                const msg_join={
                    type:"join",
                    status: "",
                    playerNum: 0
                };
                game=games[data.gameId];
                if(!game ){
                    msg_join.status="invalid";
                }
                else{
                    if(game.players.length==4){
                        msg_join.status="full";
                    }
                    else{
                        let k=true;
                        game.players.forEach(player=>{
                            if(player==data.userId){
                                msg_join.status="invalid";
                                k=false;
                            }
                        });
                        if(k) {
                            game.players.push(data.userId);
                            msg_join.playerNum=game.players.length;
                            msg_join.status="accepted";
                        }
                    }
                }
                console.log("송신: ",msg_join);
                if(msg_join.status=="accepted"){
                    wss.broadcast(game,JSON.stringify(msg_join));
                }
                else{
                    ws.send(JSON.stringify(msg_join));
                }
                
                break;
            case 'start':
                const msg_start={
                    type:"start",
                    status: "",
                };

                game=games[data.gameId];
                if(!game || game.players.length<1){
                    msg_start.status="invalid";
                }else{
                    msg_start.status="valid";
                    tileDeck.reset();
                    game.deck=tileDeck.copydeck();
                    console.log(game.deck);
                }
                console.log("송신: ",msg_start);
                if(msg_start.status=="valid"){
                    wss.broadcast(game,JSON.stringify(msg_start));
                }else{
                    ws.send(JSON.stringify(msg_start));
                }
                break;
            case 'ready':
                break;
            
        }
    });

    ws.on("close",()=>{
    })
});

wss.broadcast=(game,msg)=>{
    game.players.forEach(player=>{
        if(userMap.get(player).readyState===WebSocket.OPEN){
            userMap.get(player).send(msg);
        }
    });
};
