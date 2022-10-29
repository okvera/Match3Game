#include "WelcomeScene.h"
#include "MainScene.h"
#include "ui/UIButton.h";

USING_NS_CC;

Scene* WelcomeScene::createScene()
{
    return WelcomeScene::create();
}

bool WelcomeScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // settings --- title
    auto label = Label::createWithTTF("Greetings!", "fonts/Marker Felt.ttf", 48);
    if (label == nullptr)
    {
        printf("Error while loading: 'fonts/Marker Felt.ttf'\n");
    }
    else
    {
        label->setTextColor(Color4B::ORANGE);
        label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 150));
        this->addChild(label, 1);
    }

    // settings --- field size
    char buf[20];
    snprintf(buf, sizeof(buf), "Field Size: %2d", fieldSize);

    fieldSizeLabel = Label::createWithTTF(buf, "fonts/Marker Felt.ttf", 32);
    fieldSizeLabel->setAnchorPoint(Vec2(1.0, 0.5));
    fieldSizeLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 + 70, origin.y + visibleSize.height / 2 + 50));
    this->addChild(fieldSizeLabel, 1);

    auto sizeBtnUp = ui::Button::create("up.png", "up.png", "up.png");
    sizeBtnUp->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                if (fieldSize < MAX_FIELD_SIZE)
                {
                    ++fieldSize;
                    char buf[20];
                    snprintf(buf, sizeof(buf), "Field Size: %2d", fieldSize);
                    fieldSizeLabel->setString(buf);
                }
                break;
            default:
                break;
        }
        });
    sizeBtnUp->setPosition(Vec2(fieldSizeLabel->getPositionX() + 15, fieldSizeLabel->getPositionY() + 12));
    this->addChild(sizeBtnUp);

    auto sizeBtnDown = ui::Button::create("down.png", "down.png", "down.png");
    sizeBtnDown->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                if (fieldSize > MIN_FIELD_SIZE)
                {
                    --fieldSize;
                    char buf[20];
                    snprintf(buf, sizeof(buf), "Field Size: %2d", fieldSize);
                    fieldSizeLabel->setString(buf);
                }
                break;
            default:
                break;
        }
        });
    sizeBtnDown->setPosition(Vec2(fieldSizeLabel->getPositionX() + 15, fieldSizeLabel->getPositionY() - 12));
    this->addChild(sizeBtnDown);

    // settings --- chips type amount
    snprintf(buf, sizeof(buf), "Chip types: %2d", chipTypesNum);
    chipTypesNumLabel = Label::createWithTTF(buf, "fonts/Marker Felt.ttf", 32);
    chipTypesNumLabel->setAnchorPoint(Vec2(1.0, 0.5));
    chipTypesNumLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 + 70, origin.y + visibleSize.height / 2 - 10));
    this->addChild(chipTypesNumLabel, 1);

    auto amountBtnUp = ui::Button::create("up.png", "up.png", "up.png");
    amountBtnUp->setPosition(Vec2(chipTypesNumLabel->getPositionX() + 15, chipTypesNumLabel->getPositionY() + 12));
    amountBtnUp->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                if (chipTypesNum < MAX_CHIP_AMOUNT)
                {
                    ++chipTypesNum;
                    char buf[20];
                    snprintf(buf, sizeof(buf), "Chip types: %2d", chipTypesNum);
                    chipTypesNumLabel->setString(buf);
                }
                break;
            default:
                break;
        }
        });
    this->addChild(amountBtnUp);

    auto amountBtnDown = ui::Button::create("down.png", "down.png", "down.png");
    amountBtnDown->setPosition(Vec2(chipTypesNumLabel->getPositionX() + 15, chipTypesNumLabel->getPositionY() - 12));
    amountBtnDown->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::ENDED:
                if (chipTypesNum > MIN_CHIP_AMOUNT)
                {
                    --chipTypesNum;
                    char buf[20];
                    snprintf(buf, sizeof(buf), "Chip types: %2d", chipTypesNum);
                    chipTypesNumLabel->setString(buf);
                }
                break;
            default:
                break;
            }
        });
    this->addChild(amountBtnDown);

    // settings --- start button
    auto playBtn = MenuItemImage::create("play.png", "play.png", [&](Ref* sender) {
        auto scene = MainScene::createScene(fieldSize, chipTypesNum);
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
        });
    playBtn->setScale(0.5, 0.5);
    playBtn->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
    auto playBtnLabel = Label::createWithTTF("Let\'s play!", "fonts/Marker Felt.ttf", 48);
    playBtnLabel->setTextColor(Color4B::BLACK);
    playBtnLabel->setPosition(playBtn->getContentSize().width / 2, playBtn->getContentSize().height / 2);
    playBtn->addChild(playBtnLabel, 1);

    auto menu = Menu::create(playBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}
