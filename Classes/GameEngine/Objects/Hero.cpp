//
// Created by igor on 08.09.16.
//

#include "Hero.h"
#include "Scenes/Battle.h"

std::vector<std::string> Hero::WEAPON_LIST;

Hero::Hero(float x_pos, float y_pos) :
        _isAttacking(false),
        _hitCount(0),
        _weaponIndex(0),
        _weaponName(""),
        _faceDir(1),
        _armArmature(nullptr)
{

    _x_pos = x_pos;
    _y_pos = y_pos;

    WEAPON_LIST.push_back("standart");
    WEAPON_LIST.push_back("tripple");
    WEAPON_LIST.push_back("fire");
    WEAPON_LIST.push_back("frost");
    WEAPON_LIST.push_back("bomb");

    _weaponName = WEAPON_LIST[_weaponIndex];

    _armature =  BattleScene::instance->factory.buildArmature("Stickman");
    _armatureDisplay = (dragonBones::CCArmatureDisplay*)_armature->getDisplay();
    _armatureDisplay->setPosition(_x_pos, _y_pos);
    _armatureDisplay->setScale(0.3);

    _armArmature = BattleScene::instance->factory.buildArmature("Bow");
    const auto armArmatureDisplay = (dragonBones::CCArmatureDisplay*)_armArmature->getDisplay();
    armArmatureDisplay->getEventDispatcher()->setEnabled(true);
    armArmatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::COMPLETE, std::bind(&Hero::_armEventHandler, this, std::placeholders::_1));
    armArmatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::FRAME_EVENT, std::bind(&Hero::_armEventHandler, this, std::placeholders::_1));

    _updateAnimation();

    dragonBones::WorldClock::clock.add(_armature);
    BattleScene::instance->addChild(_armatureDisplay);
}

Hero::~Hero()
{
}

void Hero::update()
{
    _updateAnimation();
}

void Hero::attack(float angle, float power)
{
    if (_isAttacking)
    {
        return;
    }
    _attackAngle = angle;
    _attackPower = power;
    _isAttacking = true;
    const auto animationName = "shot"; //+ _weaponName + "_" + dragonBones::to_string(_hitCount + 1);
    const auto firePointBone = _armature->getBone("bow_handler");
    const auto transform = _armatureDisplay->getNodeToWorldTransform();
    cocos2d::Vec3 localPoint(firePointBone->global.x, -firePointBone->global.y, 0.f);
    cocos2d::Vec2 globalPoint;
    transform.transformPoint(&localPoint);
    globalPoint.set(localPoint.x, localPoint.y);

    auto radian = -_attackAngle;

    switch (_weaponIndex)
    {
        case 0:
            _fire(globalPoint, radian);
            break;

        case 1:
            _fire(globalPoint, radian + 3.f * dragonBones::ANGLE_TO_RADIAN);
            _fire(globalPoint, radian - 3.f * dragonBones::ANGLE_TO_RADIAN);
            break;

        case 2:
            _fire(globalPoint, radian + 6.f * dragonBones::ANGLE_TO_RADIAN);
            _fire(globalPoint, radian);
            _fire(globalPoint, radian - 6.f * dragonBones::ANGLE_TO_RADIAN);
            break;
    }
    _armature->getAnimation().fadeIn(animationName,0.f,1);
    _isAttacking = false;
}

void Hero::switchWeapon()
{
    _isAttacking = false;
    _hitCount = 0;

    _weaponIndex++;
    if (_weaponIndex >= WEAPON_LIST.size())
    {
        _weaponIndex = 0;
    }

    _armArmature->getAnimation().fadeIn("idle");
}

void Hero::_armEventHandler(cocos2d::EventCustom* event)
{
    const auto eventObject = (dragonBones::EventObject*)event->getUserData();

    if (eventObject->type == dragonBones::EventObject::COMPLETE)
    {
        _isAttacking = false;
        _hitCount = 0;
        _armArmature->getAnimation().fadeIn("idle");
    }
    else if (eventObject->type == dragonBones::EventObject::FRAME_EVENT)
    {
        if (eventObject->name == "idle")
        {
            _isAttacking = false;
            _hitCount++;
        }
        else if (eventObject->name == "shot")
        {
            const auto display = (dragonBones::CCArmatureDisplay*)(eventObject->armature->getDisplay());
            const auto firePointBone = eventObject->armature->getBone("bow_handler");
            const auto transform = display->getNodeToWorldTransform();
            cocos2d::Vec3 localPoint(firePointBone->global.x, -firePointBone->global.y, 0.f);
            cocos2d::Vec2 globalPoint;
            transform.transformPoint(&localPoint);
            globalPoint.set(localPoint.x, localPoint.y);

            auto radian = 0.f;
            if (_faceDir > 0)
            {
                radian = firePointBone->global.getRotation() + display->getRotation() * dragonBones::ANGLE_TO_RADIAN;
            }
            else
            {
                radian = dragonBones::PI - (firePointBone->global.getRotation() + display->getRotation() * dragonBones::ANGLE_TO_RADIAN);
            }


            radian = -_attackAngle;

            switch (_weaponIndex)
            {
                case 0:
                    _fire(globalPoint, radian);
                    break;

                case 1:
                    _fire(globalPoint, radian + 3.f * dragonBones::ANGLE_TO_RADIAN);
                    _fire(globalPoint, radian - 3.f * dragonBones::ANGLE_TO_RADIAN);
                    break;

                case 2:
                    _fire(globalPoint, radian + 6.f * dragonBones::ANGLE_TO_RADIAN);
                    _fire(globalPoint, radian);
                    _fire(globalPoint, radian - 6.f * dragonBones::ANGLE_TO_RADIAN);
                    break;
            }
        }
    }
}

void Hero::_fire(const cocos2d::Vec2& firePoint, float radian)
{
    const auto arrow = new Arrow("Arrow2", radian, _attackPower, firePoint);
    BattleScene::instance->addArrow(arrow);
}

void Hero::_updateAnimation() {

    if(!_isAttacking && !_armature->getAnimation().isPlaying()){
        _armature->getAnimation().fadeIn("idle");
    }

}