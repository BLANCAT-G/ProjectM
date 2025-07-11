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

	ServerManager::getInstance().addMessageListener("LobbyScene", [this](const std::string& msg) {
		this->handleMessage(msg);
	});

	ServerManager::getInstance().userId= std::to_string(random(0, 100000));
	this->initServer();

	auto winsize = Director::getInstance()->getWinSize();
	std::string SpritePath = "kenney_ui/PNG/Extra/Double/";

	for (int i = 0; i < 4; ++i) {
		playerSlot[i] = PlayerSlot::create(SpritePath + "button_rectangle_line.png");
		playerSlot[i]->setPosition(Vec2(winsize.width / 2 -300 + i * 200, winsize.height / 2+ 100));
		this->addChild(playerSlot[i]);
	}

	gameIdLabel = Label::createWithTTF("", "fonts/arial.ttf",34);
	gameIdLabel->setPosition(winsize.width / 2, winsize.height / 2 + 200);
	gameIdLabel->setTextColor(Color4B(255, 255, 255, 255));
	this->addChild(gameIdLabel);
	
	createMatchButton = ui::Button::create(SpritePath + "button_rectangle_depth_line.png", SpritePath + "button_rectangle_line.png");
	createMatchButton->setTitleText("Create");
	createMatchButton->setTitleColor(Color3B::BLACK);
	createMatchButton->setTitleFontSize(30);
	createMatchButton->setPosition(Vec2(winsize.width / 2-100, winsize.height / 2 - 60));
	createMatchButton->addClickEventListener(CC_CALLBACK_0(LobbyScene::createMatch, this));
	this->addChild(createMatchButton);

	joinMatchButton= ui::Button::create(SpritePath + "button_rectangle_depth_line.png", SpritePath + "button_rectangle_line.png");
	joinMatchButton->setTitleText("Join");
	joinMatchButton->setTitleColor(Color3B::BLACK);
	joinMatchButton->setTitleFontSize(30);
	joinMatchButton->setPosition(Vec2(winsize.width / 2+100, winsize.height / 2 - 150));
	joinMatchButton->addClickEventListener(CC_CALLBACK_0(LobbyScene::joinMatch, this));
	this->addChild(joinMatchButton);

	startMatchButton = ui::Button::create(SpritePath + "button_rectangle_depth_line.png", SpritePath + "button_rectangle_line.png");
	startMatchButton->setTitleText("Start");
	startMatchButton->setTitleColor(Color3B::BLACK);
	startMatchButton->setTitleFontSize(30);
	startMatchButton->setPosition(Vec2(winsize.width / 2+100, winsize.height / 2 - 60));
	startMatchButton->addClickEventListener(CC_CALLBACK_0(LobbyScene::startMatch, this));
	this->addChild(startMatchButton);


	ui::EditBox* editGameID = ui::EditBox::create(Size(180, 64), SpritePath + "input_outline_square.png");
	editGameID->setPosition(Vec2(winsize.width / 2-100, winsize.height / 2 - 150));
	editGameID->setFontColor(Color4B(0, 0, 0, 255));
	editGameID->setMaxLength(6);
	editGameID->setDelegate(this);

	

	this->addChild(editGameID);

	return true;
}

void LobbyScene::initServer() {
	rapidjson::Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	rapidjson::Value value(ServerManager::getInstance().userId, allocator);

	d.AddMember("type", "init", allocator);
	d.AddMember("playerId", value, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	ServerManager::getInstance().sendMessage(buffer.GetString());
}

void LobbyScene::createMatch() 
{
	rapidjson::Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	rapidjson::Value value(ServerManager::getInstance().userId, allocator);

	d.AddMember("type", "match", allocator);
	d.AddMember("playerId", value , allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	ServerManager::getInstance().sendMessage(buffer.GetString());
}

void LobbyScene::joinMatch() 
{
	rapidjson::Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	rapidjson::Value uId(ServerManager::getInstance().userId, allocator);
	rapidjson::Value gId(ServerManager::getInstance().gameId, allocator);

	d.AddMember("type", "join", allocator);
	d.AddMember("gameId", gId, allocator);
	d.AddMember("playerId", uId, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	ServerManager::getInstance().sendMessage(buffer.GetString());
}

void LobbyScene::startMatch() {
	rapidjson::Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	rapidjson::Value gId(ServerManager::getInstance().gameId, allocator);

	d.AddMember("type", "start", allocator);
	d.AddMember("gameId", gId, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);

	ServerManager::getInstance().sendMessage(buffer.GetString());
}

void LobbyScene::handleMessage(const std::string& msg) {
	rapidjson::Document d;
	d.Parse(msg.c_str());

	if (d.HasParseError()) {
		CCLOG("JSON �Ľ� ����");
		return;
	}

	std::string type = d["type"].GetString();
	if (!type.compare("match")) {
		ServerManager::getInstance().gameId= d["gameId"].GetString();
	}
	else if (!type.compare("join")) {
		std::string status = d["status"].GetString();
		if (!status.compare("valid")) {
			setGameIdLabel(d["gameId"].GetString());
			const rapidjson::Value& players = d["players"];
			assert(players.IsArray());
			for (int i = 0; i < 4; ++i) {
				if (i >= players.Size()) LobbyScene::setPlayerSlot(i, "");
				else LobbyScene::setPlayerSlot(i, players[i].GetString());
			}
		}
	}
	else if (!type.compare("start")) {
		std::string status = d["status"].GetString();
		if (!status.compare("valid")) {
			LobbyScene::doStartGame();
		}
	}
}

void LobbyScene::setPlayerSlot(int num,const std::string& playerId) {
	playerSlot[num]->SetPlayerInfo(playerId);
}

void LobbyScene::setGameIdLabel(const std::string& gameId) {
	gameIdLabel->setString("gameId: "+gameId);
}

void LobbyScene::editBoxEditingDidBegin(ui::EditBox* editBox) {

}
void LobbyScene::editBoxEditingDidEnd(ui::EditBox* editBox) {

}
void LobbyScene::editBoxTextChanged(ui::EditBox* editBox, const std::string& text) {
	ServerManager::getInstance().gameId = text;
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

