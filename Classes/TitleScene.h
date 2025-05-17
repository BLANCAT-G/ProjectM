#pragma once

#include "cocos2d.h"

class TitleScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	
	virtual bool init();

	CREATE_FUNC(TitleScene);
};

