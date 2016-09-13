//
// Created by igor on 08.09.16.
//

#include "Stickman.h"
#include <Scenes/Battle.h>


const float Stickman::JUMP_SPEED = 20.f;
const float Stickman::MOVE_SPEED = 2.f;
USING_NS_CC;

Stickman::Stickman(float x_pos, float y_pos) :
        _isJumping(false),
        _isAttacking(false),
        _hitCount(0),
        _faceDir(1),
        _moveDir(0),
        _speedX(0.f),
        _speedY(0.f) {

    _x_pos = x_pos;
    _y_pos = y_pos;
    _armature = BattleScene::instance->factory.buildArmature("Stickman");
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();
    _armatureDisplay->setPosition(480.f, BattleScene::GROUND);
    _armatureDisplay->setScale(0.3);
    _updateAnimation();

    dragonBones::WorldClock::clock.add(_armature);
    BattleScene::instance->addChild(_armatureDisplay);

    //new Brain("normal", this);
}

Stickman::~Stickman() {
}

void Stickman::update() {
    _updatePosition();
}

void Stickman::move(int dir) {
    if (_moveDir == dir) {
        return;
    }

    _moveDir = dir;
    if (_moveDir) {
        if (_faceDir != _moveDir) {
            _faceDir = _moveDir;
            _armatureDisplay->setScaleX(-_armatureDisplay->getScaleX());
        }
    }
    _updateAnimation();
}

void Stickman::jump() {
    if (_isJumping) {
        return;
    }

    _isJumping = true;
    _speedY = JUMP_SPEED;
    _armature->getAnimation().fadeIn("jump");
}

void Stickman::attack() {
    if (_isAttacking) {
        return;
    }

    _isAttacking = true;
    const auto animationName = "hit1";
    _armature->getAnimation().fadeIn(animationName);
}

void Stickman::_updateAnimation() {
    if (_isJumping) {
        return;
    }

    if (_moveDir == 0) {
        _speedX = 0.f;
        _armature->getAnimation().fadeIn("idle2");
    } else {
        _speedX = MOVE_SPEED * _moveDir;
        _armature->getAnimation().fadeIn("walk");
    }
}

void Stickman::_updatePosition() {
    const auto &position = _armatureDisplay->getPosition();
    if (_speedX != 0.f) {
        _armatureDisplay->setPosition(position.x + _speedX, position.y);
        if (position.x < 0.f) {
            _armatureDisplay->setPosition(0.f, position.y);
        } else if (position.x > 960.f) {
            _armatureDisplay->setPosition(960.f, position.y);
        }
    }

    if (_speedY != 0.f) {

        _speedY += BattleScene::G;

        _armatureDisplay->setPosition(position.x, position.y + _speedY);
        if (position.y < BattleScene::GROUND) {
            _armatureDisplay->setPosition(position.x, BattleScene::GROUND);
            _isJumping = false;
            _speedY = 0.f;
            _speedX = 0.f;
            _updateAnimation();
        }
    }


//    auto bones = _armatureDisplay->getChildren();
//    int i = 0;
//    for(auto bone : bones){
//        const auto transform = _armatureDisplay->getNodeToWorldTransform();
//        auto bone_rect = RectApplyTransform(bone->getBoundingBox(), transform);
//        Arrow::drawRect(bone_rect, "stickman -" + StringUtils::toString(i));
//        i++;
//    }

}