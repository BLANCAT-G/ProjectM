#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "rapidjson/document.h"
#include <iostream>

USING_NS_CC;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	auto winsize = Director::getInstance()->getWinSize();

	auto plabel = Label::createWithTTF("GameScene", "fonts/arial.ttf", 34);
	plabel->setPosition(winsize.width / 2, winsize.height / 2 + 50);
	plabel->setTextColor(Color4B(255, 255, 255, 255));
	this->addChild(plabel);

	
	
	return true;
}


void GameScene::onOpen(network::WebSocket* ws) {
	 ws->send("hi");
}

 void GameScene::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data){
	CCLOG("서버로부터 메시지: %s", data.bytes);
}

void GameScene::onClose(network::WebSocket* ws)  {
	CCLOG("WebSocket 닫힘");
	delete ws;
}

 void GameScene::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error) {
	CCLOG("WebSocket 에러 발생");
}



