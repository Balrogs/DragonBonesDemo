//
// Created by igor on 08.09.16.
//

#ifndef DRAGONBONES_STICKMAN_H
#define DRAGONBONES_STICKMAN_H

#include <GameEngine/Brain.h>
#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "Target.h"


class Stickman : public Target {
public:
    static const float JUMP_SPEED;
    static const float MOVE_SPEED;

private:
    bool _isJumping;
    bool _isAttacking;
    unsigned _hitCount;
    int _faceDir;
    int _moveDir;
    float _speedX;
    float _speedY;

public:
    Stickman(float x_pos,float y_pos);

    ~Stickman();

    void move(int dir);

    void jump();

    void attack();

    void update() override;

private:

    void _updatePosition();
    void _updateAnimation() override;
};


#endif //DRAGONBONES_STICKMAN_H
