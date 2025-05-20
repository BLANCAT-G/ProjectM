const express = require('express');
const http = require('http');
const { json } = require('stream/consumers');
const {WebSocketServer}=require("ws");
const wss=new WebSocketServer({port: 8080});

let games={};

wss.on("connection",ws=>{
    ws.on("message",msg=>{
        const data=JSON.parse(msg);
        switch(data.type){
            case 'match':
                const gameId=Math.random().toString(36).substring(2,8);
                games[gameId]== {
                    players: [data.userId],
                    currentTurn: 0,
                    status: 'waiting'
                };
                const msg={
                    type:"match",
                    gameId: gameId
                };
                ws.send(JSON.stringify(msg));
                break;
            case 'join':
                if(games[data.gameId].players.length==4){
                    console.log("full");
                }
                break;
            case 'move':
                break;
            
        }
    });

    ws.on("close",()=>{
        wss.clients.forEach(user=>{
            user.send('out 1');
        });
    })
});
