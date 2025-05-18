#include "GameScene.h"

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


