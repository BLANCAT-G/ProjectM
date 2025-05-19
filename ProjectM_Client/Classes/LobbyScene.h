#pragma once

#include "cocos2d.h"
#include "network/WebSocket.h"

USING_NS_CC;

class LobbyScene : public Scene, public network::WebSocket::Delegate
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

	void startMatch(const std::string& userId);

	void joinMatch(const std::string& userId, const std::string& gameId);

};

