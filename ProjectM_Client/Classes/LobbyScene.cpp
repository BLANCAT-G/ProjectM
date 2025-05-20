#define RAPIDJSON_HAS_STDSTRING 1

#include "LobbyScene.h"
#include "ui/CocosGUI.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "rapidjson/document.h"
#include "json/document.h";
#include "json/stringbuffer.h"
#include "json/writer.h"



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

	_ws = new network::WebSocket();
	_ws->init(*this, "ws://localhost:8080");
	
	_userId = std::to_string(random(0, 100000));

	auto winsize = Director::getInstance()->getWinSize();

	auto plabel = Label::createWithTTF("LobbyScene", "fonts/arial.ttf",34);
	plabel->setPosition(winsize.width / 2, winsize.height / 2 + 100);
	plabel->setTextColor(Color4B(255, 255, 255, 255));
	this->addChild(plabel);

	std::string SpritePath = "kenney_ui/PNG/Extra/Double/";
	auto createMatchButton = ui::Button::create(SpritePath + "button_rectangle_depth_line.png", SpritePath + "button_rectangle_line.png");
	createMatchButton->setPosition(Vec2(winsize.width / 2, winsize.height / 2 - 60));
	createMatchButton->addClickEventListener(CC_CALLBACK_0(LobbyScene::startMatch, this, _userId));
	this->addChild(createMatchButton);

	auto joinMatchButton= ui::Button::create(SpritePath + "button_rectangle_depth_line.png", SpritePath + "button_rectangle_line.png");
	joinMatchButton->setPosition(Vec2(winsize.width / 2+100, winsize.height / 2 - 150));
	joinMatchButton->addClickEventListener(CC_CALLBACK_0(LobbyScene::joinMatch, this, _userId, _gameId));
	this->addChild(joinMatchButton);


	ui::EditBox* editGameID = ui::EditBox::create(Size(200, 60), SpritePath + "input_outline_square.png");
	editGameID->setPosition(Vec2(winsize.width / 2-100, winsize.height / 2 - 150));
	editGameID->setFontColor(Color4B(0, 0, 0, 255));
	editGameID->setMaxLength(4);
	editGameID->setDelegate(this);

	

	this->addChild(editGameID);

	return true;
}

void LobbyScene::startMatch(const std::string& userId) 
{
	/*
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
	*/
	rapidjson::Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	rapidjson::Value value(_userId, allocator);

	d.AddMember("type", "match", allocator);
	d.AddMember("userId", value , allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	_ws->send(buffer.GetString());
}

void LobbyScene::joinMatch(const std::string& userId, const std::string& gameId) 
{
	_ws->send("gameId: " + gameId + " userId: " + userId);
}

void LobbyScene::onOpen(network::WebSocket* ws) {
	CCLOG("opend");
}

void LobbyScene::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data) 
{
	std::string msg(data.bytes, data.len);
	rapidjson::Document d;
	d.Parse(msg.c_str());

	if (d.HasParseError()) {
		CCLOG("JSON ÆÄ½Ì ½ÇÆÐ");
		return;
	}

	std::string type = d["type"].GetString();
	if (!type.compare("match")) {
		_gameId = d["gameId"].GetString();
		CCLOG(_gameId.c_str());
	}
}

void LobbyScene::onClose(network::WebSocket* ws) 
{
	CCLOG("WebSocket closed");
	delete ws;
}

void LobbyScene::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error) 
{
	CCLOG("WebSocket  error");
}

void LobbyScene::editBoxEditingDidBegin(ui::EditBox* editBox) {

}
void LobbyScene::editBoxEditingDidEnd(ui::EditBox* editBox) {

}
void LobbyScene::editBoxTextChanged(ui::EditBox* editBox, const std::string& text) {
	_gameId = text;
}
void LobbyScene::editBoxReturn(ui::EditBox* editBox) {

}

