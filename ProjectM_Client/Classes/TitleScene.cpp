#include "TitleScene.h"
#include "GameScene.h"
#include "CCEventKeyboard.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* TitleScene::createScene()
{
	return TitleScene::create();
}

bool TitleScene::init()
{
	if (!Scene::init()) {
		return false;
	}


	auto winsize = Director::getInstance()->getWinSize();

	auto plabel = Label::createWithTTF("MahjongCube", "fonts/arial.ttf", 34);
	plabel->setPosition(winsize.width / 2, winsize.height / 2 + 50);
	plabel->setTextColor(Color4B(255, 255, 255, 255));

	this->addChild(plabel);

	auto startItem = MenuItemFont::create("Start", CC_CALLBACK_1(TitleScene::doStart, this));
	auto quitItem = MenuItemFont::create("Quit", CC_CALLBACK_1(TitleScene::doQuit, this));
	startItem->setPosition(Vec2(0, -100));
	quitItem->setPosition(Vec2(0, -200));

	auto menu = Menu::create(startItem, quitItem, NULL);

	this->addChild(menu, 1);



	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(TitleScene::onKeyPressed, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TitleScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		Director::getInstance()->end();
		break;
	}
}

void TitleScene::doStart(Ref* sender) {
	auto gamescene = GameScene::createScene();
	Director::getInstance()->replaceScene(gamescene);
}

void TitleScene::doQuit(Ref* sender) {
	Director::getInstance()->end();
}
