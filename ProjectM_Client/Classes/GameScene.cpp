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


 GameScene::~GameScene() {
	 ServerManager::getInstance().removeMessageListener("GameScene");
 }



