#pragma once

#include "cocos2d.h"
#include "network/WebSocket.h"

USING_NS_CC;

class GameScene : public Scene
{
private:
	~GameScene();

	std::string _gameId="null";
public:

	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameScene);


	void handleMessage(const std::string& msg);


};

