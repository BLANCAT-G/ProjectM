const tiledeck={
    deck: [],
    set: function(){
        let tileset=[]
        for(let n=1;n<10;n++){
            tileset.push(n.toString()+'m');
            tileset.push(n.toString()+'s');
            tileset.push(n.toString()+'p');
        }
        for(let n=1;n<4;n++){
            tileset.push(n.toString()+'z');
        }
        return tileset;
    },
    shuffle: function(arr){
        let shuffle=[];
        while(arr.length){
            const lastIdx=arr.length-1;
            let roll=Math.floor(Math.random()*arr.length);
            let tmp=arr[lastIdx];
            arr[lastIdx]=arr[roll];
            arr[roll]=tmp;
            shuffle.push(arr.pop())
        }
        return shuffle;
    },
    reset: function(){
        tmpset=this.set();
        tmpdeck=tmpset.concat(tmpset);
        tmpdeck=tmpdeck.concat(tmpset);
        this.deck=this.shuffle(tmpdeck);
    },
    copydeck: function(){
        let copy=JSON.parse(JSON.stringify(this.deck));
        return copy;
    }
}

module.exports=tiledeck;