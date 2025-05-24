#define RAPIDJSON_HAS_STDSTRING 1

#include "LobbyScene.h"
#include "ServerManager.h"
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

Scene* LobbyScene::createScene()
{
	return LobbyScene::create();
}

bool LobbyScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	ServerManager::getInstance().connect("ws://localhost:8080");
	ServerManager::getInstance().addMessageListener("LobbyScene", [this](const std::string& msg) {
		this->handleMessage(msg);
	});
	
	_userId = std::to_string(random(0, 100000));

	auto winsize = Director::getInstance()->getWinSize();

	playerNum = Label::createWithTTF("", "fonts/arial.ttf",34);
	playerNum->setPosition(winsize.width / 2, winsize.height / 2 + 100);
	playerNum->setTextColor(Color4B(255, 255, 255, 255));
	playerNum->setVisible(false);
	this->addChild(playerNum);

	std::string SpritePath = "kenney_ui/PNG/Extra/Double/";
	createMatchButton = ui::Button::create(SpritePath + "button_rectangle_depth_line.png", SpritePath + "button_rectangle_line.png");
	createMatchButton->setTitleText("Create");
	createMatchButton->setTitleColor(Color3B::BLACK);
	createMatchButton->setTitleFontSize(30);
	createMatchButton->setPosition(Vec2(winsize.width / 2-100, winsize.height / 2 - 60));
	createMatchButton->addClickEventListener(CC_CALLBACK_0(LobbyScene::createMatch, this, _userId));
	this->addChild(createMatchButton);

	joinMatchButton= ui::Button::create(SpritePath + "button_rectangle_depth_line.png", SpritePath + "button_rectangle_line.png");
	joinMatchButton->setTitleText("Join");
	joinMatchButton->setTitleColor(Color3B::BLACK);
	joinMatchButton->setTitleFontSize(30);
	joinMatchButton->setPosition(Vec2(winsize.width / 2+100, winsize.height / 2 - 150));
	joinMatchButton->addClickEventListener(CC_CALLBACK_0(LobbyScene::joinMatch, this, _userId, _gameId));
	this->addChild(joinMatchButton);

	startMatchButton = ui::Button::create(SpritePath + "button_rectangle_depth_line.png", SpritePath + "button_rectangle_line.png");
	startMatchButton->setTitleText("Start");
	startMatchButton->setTitleColor(Color3B::BLACK);
	startMatchButton->setTitleFontSize(30);
	startMatchButton->setPosition(Vec2(winsize.width / 2+100, winsize.height / 2 - 60));
	startMatchButton->addClickEventListener(CC_CALLBACK_0(LobbyScene::startMatch, this, _userId));
	this->addChild(startMatchButton);


	ui::EditBox* editGameID = ui::EditBox::create(Size(180, 64), SpritePath + "input_outline_square.png");
	editGameID->setPosition(Vec2(winsize.width / 2-100, winsize.height / 2 - 150));
	editGameID->setFontColor(Color4B(0, 0, 0, 255));
	editGameID->setMaxLength(6);
	editGameID->setDelegate(this);

	

	this->addChild(editGameID);

	return true;
}

void LobbyScene::createMatch(const std::string& userId) 
{
	rapidjson::Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	rapidjson::Value value(_userId, allocator);

	d.AddMember("type", "match", allocator);
	d.AddMember("userId", value , allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	ServerManager::getInstance().sendMessage(buffer.GetString());
}

void LobbyScene::joinMatch(const std::string& userId, const std::string& gameId) 
{
	rapidjson::Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	rapidjson::Value uId(_userId, allocator);
	rapidjson::Value gId(_gameId, allocator);

	d.AddMember("type", "join", allocator);
	d.AddMember("gameId", gId, allocator);
	d.AddMember("userId", uId, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	ServerManager::getInstance().sendMessage(buffer.GetString());
}

void LobbyScene::startMatch(const std::string& gameId) {
	rapidjson::Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	rapidjson::Value gId(_gameId, allocator);

	d.AddMember("type", "start", allocator);
	d.AddMember("gameId", gId, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	ServerManager::getInstance().sendMessage(buffer.GetString());
}

/*
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
		LobbyScene::setPlayerNum(1);
	}
	else if (!type.compare("join")) {
		std::string status = d["status"].GetString();
		if (!status.compare("accepted")) {
			LobbyScene::setPlayerNum(d["playerNum"].GetInt());	
		}
	}
	else if (!type.compare("start")) {
		std::string status = d["status"].GetString();
		if (!status.compare("valid")) {
			LobbyScene::doStartGame();
		}
	}
}
*/

void LobbyScene::handleMessage(const std::string& msg) {
	rapidjson::Document d;
	d.Parse(msg.c_str());

	if (d.HasParseError()) {
		CCLOG("JSON ÆÄ½Ì ½ÇÆÐ");
		return;
	}

	std::string type = d["type"].GetString();
	if (!type.compare("match")) {
		_gameId = d["gameId"].GetString();
		LobbyScene::setPlayerNum(1);
	}
	else if (!type.compare("join")) {
		std::string status = d["status"].GetString();
		if (!status.compare("accepted")) {
			LobbyScene::setPlayerNum(d["playerNum"].GetInt());
		}
	}
	else if (!type.compare("start")) {
		std::string status = d["status"].GetString();
		if (!status.compare("valid")) {
			LobbyScene::doStartGame();
		}
	}
}

void LobbyScene::setPlayerNum(int num) {
	playerNum->setVisible(true);
	playerNum->setString("gameId: "+_gameId+" / "+"player: " + std::to_string(num) + " / 4");
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

void LobbyScene::doStartGame() {
	auto gamescene = GameScene::createScene();
	Director::getInstance()->replaceScene(gamescene);
}

LobbyScene::~LobbyScene() {
	ServerManager::getInstance().removeMessageListener("LobbyScene");
}

