#include "PlayerSlot.h"

USING_NS_CC;

PlayerSlot* PlayerSlot::create(const std::string& filename) {
    auto ret = new (std::nothrow) PlayerSlot;
    if (ret && ret->initWithFile(filename)) {
        ret->autorelease();
        ret->initSlot();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
}

void PlayerSlot::initSlot() {

    pIdLabel = Label::createWithTTF("","fonts/arial.ttf", 20);
    pIdLabel->setPosition(Vec2(50, 30));
    pIdLabel->setTextColor(Color4B(0,0,0, 255));
    this->addChild(pIdLabel);

}

void PlayerSlot::SetPlayerInfo(const std::string& playerId) {
    pIdLabel->setString(playerId);
}
