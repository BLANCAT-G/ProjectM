#include "ServerManager.h"
#include "cocos2d.h"

USING_NS_CC;

ServerManager& ServerManager::getInstance()
{
    static ServerManager instance;
    return instance;
}

ServerManager::ServerManager() : _ws(nullptr)
{
}

void ServerManager::connect(const std::string& url)
{
    if (_ws) delete _ws;

    _ws = new cocos2d::network::WebSocket();
    _ws->init(*this, url);
}

void ServerManager::sendMessage(const std::string& msg)
{
    if (_ws && _ws->getReadyState() == cocos2d::network::WebSocket::State::OPEN)
    {
        _ws->send(msg);
    }
}

void ServerManager::addMessageListener(const std::string& id, MessageCallback callback)
{
    _listeners[id] = callback;
}

void ServerManager::removeMessageListener(const std::string& id)
{
    _listeners.erase(id);
}

void ServerManager::onOpen(cocos2d::network::WebSocket* ws)
{
    CCLOG("WebSocket opened");
}

void ServerManager::onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)
{
    std::string msg(data.bytes, data.len);
    CCLOG("WebSocket message: %s", msg.c_str());

    for (auto& pair : _listeners)
    {
        pair.second(msg);
    }
}

void ServerManager::onClose(cocos2d::network::WebSocket* ws)
{
    CCLOG("WebSocket closed");
}

void ServerManager::onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)
{
    CCLOG("WebSocket error");
}
