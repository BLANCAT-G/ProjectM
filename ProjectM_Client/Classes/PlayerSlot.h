#pragma once

#include "ui/CocosGUI.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace ui;

class PlayerSlot : public Sprite
{
private:
	Label* pIdLabel;

public:
	void initSlot();

	static PlayerSlot* create(const std::string& filename);

	void SetPlayerInfo(const std::string& playerId);
};

