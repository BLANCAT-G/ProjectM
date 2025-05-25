#pragma once

#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "network/WebSocket.h"

USING_NS_CC;
using namespace ui;

class GameScene : public Scene
{
private:
	~GameScene();

	std::string _gameId="null";
	Button* skipButton;

public:

	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameScene);


	void handleMessage(const std::string& msg);

	void startMyTurn();

	void skipTurn();
};

