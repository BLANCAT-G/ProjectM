#include "GameScene.h"
#include "ServerManager.h"
#include "ui/CocosGUI.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "rapidjson/document.h"
#include "json/document.h";
#include "json/stringbuffer.h"
#include "json/writer.h"

USING_NS_CC;

Scene* GameScene::createScene() {
	Scene* newScene = GameScene::create();
	return newScene;
}

bool GameScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	ServerManager::getInstance().addMessageListener("GameScene", [this](const std::string& msg) {
		this->handleMessage(msg);
	});

	auto winsize = Director::getInstance()->getWinSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tilesheet/tilesheet.plist");

	auto sprite = Sprite::createWithSpriteFrameName("0m.png");
	sprite->setPosition(Vec2(winsize.width / 2, winsize.height / 2));
	this->addChild(sprite);
	

	std::string SpritePath = "kenney_ui/PNG/Extra/Double/";
	skipButton = ui::Button::create(SpritePath + "button_rectangle_depth_line.png", SpritePath + "button_rectangle_line.png");
	skipButton->setTitleText("Skip");
	skipButton->setTitleColor(Color3B::BLACK);
	skipButton->setTitleFontSize(30);
	skipButton->setPosition(Vec2(winsize.width - 150, winsize.height / 2 - 120));
	this->addChild(skipButton);

	return true;
}

void GameScene::handleMessage(const std::string& msg) {
	rapidjson::Document d;
	d.Parse(msg.c_str());

	if (d.HasParseError()) {
		CCLOG("JSON ÆÄ½Ì ½ÇÆÐ");
		return;
	}

}

void GameScene::startMyTurn() {

}

void GameScene::skipTurn() {

}


 GameScene::~GameScene() {
	 ServerManager::getInstance().removeMessageListener("GameScene");
 }



