#include "LobbyScene.h"
#include "ui/CocosGUI.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "rapidjson/document.h"
#include <iostream>

USING_NS_CC;

Scene* LobbyScene::createScene()
{
	return LobbyScene::create();
}

bool LobbyScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	
	_userId = std::to_string(random(0, 1000));

	auto winsize = Director::getInstance()->getWinSize();

	auto plabel = Label::createWithTTF("LobbyScene", "fonts/arial.ttf",34);
	plabel->setPosition(winsize.width / 2, winsize.height / 2 + 50);
	plabel->setTextColor(Color4B(255, 255, 255, 255));
	this->addChild(plabel);

	std::string buttonSpritePath = "kenney_ui/PNG/Extra/Double/";
	auto createMatchButton = ui::Button::create(buttonSpritePath + "button_rectangle_depth_line.png", buttonSpritePath + "button_rectangle_line.png");
	createMatchButton->setPosition(Vec2(winsize.width / 2, winsize.height / 2 - 60));
	createMatchButton->addClickEventListener(CC_CALLBACK_0(LobbyScene::startMatch, this, _userId));
	this->addChild(createMatchButton);


	return true;
}

void LobbyScene::startMatch(const std::string& userId) 
{
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
		if (d.HasMember("gameId")) _gameId = d["gameId"].GetString();

		});

	network::HttpClient::getInstance()->send(request);
	request->release();
}

void LobbyScene::joinMatch(const std::string& userId, const std::string& gameId) 
{
	if (_ws) _ws->close();
	_ws = new network::WebSocket();
	_ws->init(*this, "ws://localhost:8080");
	_gameId = gameId;
}

void LobbyScene::onOpen(network::WebSocket* ws) {
	ws->send("hi");
}

void LobbyScene::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data) 
{
	CCLOG("서버로부터 메시지: %s", data.bytes);
}

void LobbyScene::onClose(network::WebSocket* ws) 
{
	CCLOG("WebSocket 닫힘");
	delete ws;
}

void LobbyScene::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error) 
{
	CCLOG("WebSocket 에러 발생");
}



