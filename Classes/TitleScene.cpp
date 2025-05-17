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
	Scene* gamescene;

	auto winsize = Director::getInstance()->getWinSize();

	auto plabel = Label::createWithTTF("MahjongCube", "fonts/arial.ttf",34);
	plabel->setPosition(winsize.width / 2, winsize.height/2+50);
	plabel->setTextColor(Color4B(255, 255, 255, 255));

	this->addChild(plabel);

	auto button = ui::Button::create("Sprites/UI/clickbox.png", "Sprites/UI/clickbox.png", "Sprites/UI/clickbox.png");
	button->setPosition(Vec2(winsize.width / 2, winsize.height / 2 -100));
	button->setScale(2, 2);

	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			gamescene = GameScene::createScene();
			Director::getInstance()->replaceScene(gamescene);
			break;
		default:
			break;
		}
	});

	this->addChild(button);
	

	
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
