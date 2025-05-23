#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "rapidjson/document.h"
#include "json/document.h";
#include "json/stringbuffer.h"
#include "json/writer.h"

USING_NS_CC;

GameScene* GameScene::create(network::WebSocket* ws) {
	GameScene* pRet = new GameScene();
	if (pRet && pRet->init()) {
		pRet->setWebSocket(ws);
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

Scene* GameScene::createScene(network::WebSocket* ws) {
	Scene* newScene = GameScene::create(ws);
	newScene->init();
	return newScene;
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

	rapidjson::Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

	d.AddMember("type", "match", allocator);
	d.AddMember("userId", "11", allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	_ws->send(buffer.GetString());

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

 void GameScene::setWebSocket(network::WebSocket* ws) {
	 _ws = ws;
 }



