#include "GameOverScene.h"
#include "WelcomeScene.h"
#include "MainScene.h"
//#include <AudioEngine.h>

USING_NS_CC;
using namespace std;

Scene* GameOverScene::createScene()
{
    return GameOverScene::create();
}

bool GameOverScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // title
    auto label = Label::createWithTTF("Game over!", "fonts/Marker Felt.ttf", 48);
    label->setTextColor(Color4B::ORANGE);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 150));
    this->addChild(label, 1);
    
    // menu --- restart
    auto restartBtn = MenuItemImage::create("restart.png", "restart.png", [&](Ref* sender) {
        auto scene = MainScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
        });
    restartBtn->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100, origin.y + visibleSize.height / 2 + 50));
    auto restartBtnLabel = Label::createWithTTF("Try again", "fonts/Marker Felt.ttf", 32);
    restartBtnLabel->setAnchorPoint(Vec2(0.0, 0.5));
    restartBtnLabel->setPosition(Vec2(40, 16));
    restartBtn->addChild(restartBtnLabel, 1);
    
    // menu --- change settings
    auto settingsBtn = MenuItemImage::create("settings.png", "settings.png", [&](Ref* sender) {
        auto scene = WelcomeScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
        });
    settingsBtn->setPosition(Vec2(origin.x + visibleSize.width / 2 - 100, origin.y + visibleSize.height / 2 - 10));
    auto settingsBtnLabel = Label::createWithTTF("Change settings", "fonts/Marker Felt.ttf", 32);
    settingsBtnLabel->setAnchorPoint(Vec2(0.0, 0.5));
    settingsBtnLabel->setPosition(Vec2(40, 16));
    settingsBtn->addChild(settingsBtnLabel, 1);

    // create menu
    auto menu = Menu::create(restartBtn, settingsBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}
