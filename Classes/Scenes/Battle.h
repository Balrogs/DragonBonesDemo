#ifndef __KNIGHT_H__
#define __KNIGHT_H__

#include <GameEngine/Objects/Arrow.h>
#include <GameEngine/Objects/Hero.h>
#include <GameEngine/Objects/Stickman.h>
#include <GameEngine/Objects/Aim.h>
#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"


class BattleScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(BattleScene);


public:
    static const float MAX_ARROW_POWER;
    static const float MIN_ARROW_POWER;
    static const float GROUND;
    static const float G;
    static BattleScene* instance;

public:
    dragonBones::CCFactory factory;
    std::vector<Target*> targets;
    void addArrow(Arrow* arrow);
    void removeArrow(Arrow* arrow);

private:
    bool _left;
    bool _right;
    Hero* _player;
    Stickman* _target;
    std::vector<Arrow*> _arrows;

    void _enterFrameHandler(float passedTime);
    void _keyBoardPressedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void _keyBoardReleasedHandler(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    bool _touchHandlerBegin(const cocos2d::Touch* touch, cocos2d::Event* event);
    bool _touchHandlerMove(const cocos2d::Touch* touch, cocos2d::Event* event);
    bool _touchHandlerEnd(const cocos2d::Touch* touch, cocos2d::Event* event);
    void _updateMove(int dir) const;
};

#endif // __KNIGHT_H__