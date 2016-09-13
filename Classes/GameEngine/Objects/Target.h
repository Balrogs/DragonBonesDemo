//
// Created by igor on 09.09.16.
//

#ifndef ARCUNLIM_TARGET_H
#define ARCUNLIM_TARGET_H

#include <dragonBones/cocos2dx/CCArmatureDisplay.h>
#include "cocos2d.h"

class Target {
public:
    dragonBones::CCArmatureDisplay* getDisplay();
    void addChild(cocos2d::Node* child);
    virtual void update()= 0;
    Target();

protected:
    dragonBones::Armature *_armature;
    dragonBones::CCArmatureDisplay *_armatureDisplay;
    float _x_pos;
    float _y_pos;
    virtual void _updateAnimation()= 0;
};

#endif //ARCUNLIM_TARGET_H
