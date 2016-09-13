#include <GameEngine/Objects/Stickman.h>
#include "Battle.h"

USING_NS_CC;

Scene *BattleScene::createScene() {
    auto scene = Scene::create();
    auto layer = BattleScene::create();

    scene->addChild(layer);
    return scene;
}

const float  BattleScene::MAX_ARROW_POWER = 50.f;
const float  BattleScene::MIN_ARROW_POWER = 5.f;
const float  BattleScene::GROUND = 120.f;
const float  BattleScene::G = -0.6f;
BattleScene *BattleScene::instance = nullptr;

bool BattleScene::init() {
    if (!LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255))) {
        return false;
    }

    Sprite *bg = Sprite::create("Background/background.png");
    bg->setScale(0.8);
    bg->setPosition(480.f, 320.f);
    this->addChild(bg);
    BattleScene::instance = this;

    _left = false;
    _right = false;
    _player = nullptr;
    _target = nullptr;

    // Load DragonBones Data.
    const auto dragonBonesData = factory.loadDragonBonesData("Animation/ArcUnlim.json");
    factory.loadTextureAtlasData("Animation/texture.json");

    if (dragonBonesData) {
        cocos2d::Director::getInstance()->getScheduler()->schedule(
                schedule_selector(BattleScene::_enterFrameHandler),
                this, 0.f, false
        );


        const auto keyboardListener = cocos2d::EventListenerKeyboard::create();
        keyboardListener->onKeyPressed = CC_CALLBACK_2(BattleScene::_keyBoardPressedHandler, this);
        keyboardListener->onKeyReleased = CC_CALLBACK_2(BattleScene::_keyBoardReleasedHandler, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);


        const auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(BattleScene::_touchHandlerBegin, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(BattleScene::_touchHandlerMove, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(BattleScene::_touchHandlerEnd, this);
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);


        _player = new Hero(50.f, BattleScene::GROUND + 300.f);
        //targets.push_back(_player);
        _target = new Stickman(500.f, BattleScene::GROUND);
        targets.push_back(_target);
    } else {
        assert(false);
    }

    const auto text = cocos2d::Label::create();
    text->setPosition(480.f, 60.f);
    text->setString(
            "Press W/A/S/D to move. Press SPACE to switch weapen. Press Q/E to upgrade weapen.\nClick to attack.");
    text->setAlignment(cocos2d::TextHAlignment::CENTER);
    this->addChild(text);

    return true;
}

void BattleScene::addArrow(Arrow *arrow) {
    _arrows.push_back(arrow);
}

void BattleScene::removeArrow(Arrow* arrow) {
    _arrows.erase(std::find(_arrows.begin(), _arrows.end(), arrow));
}

void BattleScene::_enterFrameHandler(float passedTime) {
    auto j = targets.size();
    while (j--) {
        targets[j]->update();
    }

    auto i = _arrows.size();
    while (i--) {
        const auto bullet = _arrows[i];
        if (bullet->update()) {
            removeArrow(bullet);
        }

    }
    if(_arrows.size() > 15){
        auto arrow = _arrows[0];
        removeArrow(arrow);
        arrow->remove();
    }

    dragonBones::WorldClock::clock.advanceTime(passedTime);
}

bool BattleScene::_touchHandlerBegin(const cocos2d::Touch *touch, cocos2d::Event *event) {
    return true;
}

bool BattleScene::_touchHandlerMove(const cocos2d::Touch *touch, cocos2d::Event *event) {
    this->removeChild(this->getChildByName("aim"));
    auto drawNode = DrawNode::create();

    const auto start = touch->getStartLocation();
    const auto curr = touch->getLocation();

    float x = start.x - curr.x;
    float y = start.y - curr.y;

    drawNode->drawLine(start, curr, Color4F::WHITE);
    this->addChild(drawNode, 1, "aim");

    return true;
}


bool BattleScene::_touchHandlerEnd(const cocos2d::Touch *touch, cocos2d::Event *event) {
    this->removeChild(this->getChildByName("aim"));

    const auto start = touch->getStartLocation();
    const auto curr = touch->getLocation();
    float x = start.x - curr.x;
    float y = start.y - curr.y;
    auto angle = std::atan2(y, x);
    auto power = std::sqrt(x * x + y * y);
    power = power / 10;
    power = (power > MAX_ARROW_POWER) ? MAX_ARROW_POWER : power;
    power = (power < MIN_ARROW_POWER) ? MIN_ARROW_POWER : power;
    _player->attack(angle, power);
    return true;
}

void BattleScene::_keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _left = true;
            _updateMove(-1);
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _right = true;
            _updateMove(1);
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
            _target->jump();
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            break;
    }
}

void BattleScene::_keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch (keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_A:
        case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _left = false;
            _updateMove(-1);
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_D:
        case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _right = false;
            _updateMove(1);
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_W:
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
            break;

        case cocos2d::EventKeyboard::KeyCode::KEY_S:
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            break;
    }
}

void BattleScene::_updateMove(int dir) const {
    if (_left && _right) {
        _target->move(dir);
    } else if (_left) {
        _target->move(-1);
    } else if (_right) {
        _target->move(1);
    } else {
        _target->move(0);
    }
}
