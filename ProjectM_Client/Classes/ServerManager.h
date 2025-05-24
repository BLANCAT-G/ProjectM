#pragma once

#include <functional>
#include <map>
#include <vector>
#include "network/WebSocket.h"

class ServerManager : public cocos2d::network::WebSocket::Delegate
{
public:
    static ServerManager& getInstance();

    void connect(const std::string& url);
    void sendMessage(const std::string& msg);

    using MessageCallback = std::function<void(const std::string&)>;
    void addMessageListener(const std::string& id, MessageCallback callback);
    void removeMessageListener(const std::string& id);

    virtual void onOpen(cocos2d::network::WebSocket* ws) override;
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) override;
    virtual void onClose(cocos2d::network::WebSocket* ws) override;
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) override;

private:
    ServerManager();

    cocos2d::network::WebSocket* _ws;
    std::map<std::string, MessageCallback> _listeners;
};