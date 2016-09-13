//
// Created by igor on 08.09.16.
//

#ifndef DRAGONBONES_HERO_H
#define DRAGONBONES_HERO_H

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "Target.h"

class Hero : public Target {
public:
    static std::vector<std::string> WEAPON_LIST;

private:
    bool _isAttacking;
    float _attackAngle;
    float _attackPower;
    unsigned _hitCount;
    unsigned _weaponIndex;
    std::string _weaponName;
    int _faceDir;

    dragonBones::Armature* _armArmature;

public:
    Hero(float x_pos,float y_pos);
    ~Hero();

    void attack(float angle, float power);
    void switchWeapon();
    void update() override;

private:
    void _armEventHandler(cocos2d::EventCustom* event);
    void _fire(const cocos2d::Vec2& firePoint, float radian);
    void _updateAnimation() override;
};


#endif //DRAGONBONES_HERO_H
