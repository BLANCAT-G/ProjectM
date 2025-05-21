const express = require('express');
const http = require('http');
const { json } = require('stream/consumers');
const {WebSocketServer}=require("ws");
const wss=new WebSocketServer({port: 8080});

let games={};
const userMap=new Map();

wss.on("connection",ws=>{
    ws.on("message",msg=>{
        const data=JSON.parse(msg);
        switch(data.type){
            case 'match':
                const gameId=Math.random().toString(36).substring(2,8);
                games[gameId]= {
                    players: [data.userId],
                    currentTurn: 0,
                    status: 'waiting'
                };
                userMap.set(data.userId,ws);
                const msg_match={
                    type:"match",
                    gameId: gameId
                };
                ws.send(JSON.stringify(msg_match));
                break;
            case 'join':
                const msg_join={
                    type:"join",
                    status: "",
                    playerNum: 0
                };
                const game=games[data.gameId];
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
                            userMap.set(data.userId,ws);
                        }
                    }
                }
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

                const sgame=games[data.gameId];
                if(!sgame || sgame.players.length<2){
                    msg_start.status="invalid";
                }else{
                    msg_start.status="valid";
                }
                
                if(msg_start.status=="valid"){
                    wss.broadcast(sgame,JSON.stringify(msg_start));
                }else{
                    ws.send(JSON.stringify(msg_start));
                }
                break;
            case 'move':
                break;
            
        }
    });

    ws.on("close",()=>{
    })
});

wss.broadcast=(game,msg)=>{
    game.players.forEach(player=>{
        userMap.get(player).send(msg);
    });
};
