#pragma once

#include "cocos2d.h"
#include "network/WebSocket.h"
#include "ui/UIEditBox/UIEditBox.h"

USING_NS_CC;
using namespace ui;

class LobbyScene : public Scene, public network::WebSocket::Delegate,public EditBoxDelegate
{
private:
	network::WebSocket* _ws = nullptr; 
	std::string _gameId = "null";
	std::string _userId = "null";
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(LobbyScene);

	virtual void onOpen(network::WebSocket* ws);
	virtual void onMessage(network::WebSocket* ws, const network::WebSocket::Data& data);
	virtual void onClose(network::WebSocket* ws);
	virtual void onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error);

	virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
	virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
	virtual void editBoxTextChanged(ui::EditBox* editBox,const std::string& text);
	virtual void editBoxReturn(ui::EditBox* editBox);

	void startMatch(const std::string& userId);

	void joinMatch(const std::string& userId, const std::string& gameId);

};

