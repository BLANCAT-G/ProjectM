#include "GameScene.h"
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

	GameScene::startMatch("abc");
	
	
	return true;
}

void GameScene::startMatch(const std::string& userId) {
	network::HttpRequest* request = new network::HttpRequest();
	request->setUrl("http://localhost:8080/match");
	request->setRequestType(network::HttpRequest::Type::POST);
	std::string data = "{\"userId\":\"" + userId + "\"}";
	request->setRequestData(data.c_str(), data.length());

	request->setHeaders({ "Content-Type: application/json" });

	request->setResponseCallback([this, userId](network::HttpClient* client, network::HttpResponse* response) {
		if (!response || !response->isSucceed()) return;
		std::string res(response->getResponseData()->begin(), response->getResponseData()->end());;
		rapidjson::Document d;
		
		d.Parse(res.c_str());
		if (d.HasMember("gameId")) {
			_gameId = d["gameId"].GetString();
		}
	});

	network::HttpClient::getInstance()->send(request);
	request->release();
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



