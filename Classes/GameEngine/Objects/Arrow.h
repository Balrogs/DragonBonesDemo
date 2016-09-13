//
// Created by igor on 08.09.16.
//

#ifndef DRAGONBONES_ARROW_H
#define DRAGONBONES_ARROW_H

#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "cocos2d.h"

class Arrow {
private:
    float _speedX;
    float _speedY;
    bool _isActive;

    dragonBones::Armature *_armature;
    dragonBones::CCArmatureDisplay *_armatureDisplay;

    cocos2d::Vec2* box;
    cocos2d::Label *debug_angle;

public:
    Arrow(const std::string &armatureName, float radian, float speed, const cocos2d::Vec2 &position);
    ~Arrow();

    bool update();
    void remove();

    static void drawRect(cocos2d::Rect rect, std::string tag);

private:
    void checkCollisions();
    void disableArrow();
};


#endif //DRAGONBONES_ARROW_H
