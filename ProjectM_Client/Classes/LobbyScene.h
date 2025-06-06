#pragma once
#include "ui/CocosGUI.h"
#include "PlayerSlot.h";
#include "cocos2d.h"
#include "network/WebSocket.h"
#include "ui/UIEditBox/UIEditBox.h"

USING_NS_CC;
using namespace ui;

class LobbyScene : public Scene,public EditBoxDelegate
{
private:
	~LobbyScene();

	Label* gameIdLabel;
	Button* createMatchButton,*startMatchButton,* joinMatchButton;
	PlayerSlot* playerSlot[4];
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(LobbyScene);

	virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
	virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
	virtual void editBoxTextChanged(ui::EditBox* editBox,const std::string& text);
	virtual void editBoxReturn(ui::EditBox* editBox);

	void setPlayerSlot(int num, const std::string& playerId);

	void setGameIdLabel(const std::string& playerId);

	void createMatch();

	void joinMatch();

	void startMatch();
	
	void initServer();

	void doStartGame();

	void handleMessage(const std::string& msg);

};

