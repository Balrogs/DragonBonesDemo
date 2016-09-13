//
// Created by igor on 08.09.16.
//

#include "Arrow.h"
#include "Scenes/Battle.h"

USING_NS_CC;

Arrow::Arrow(const std::string &armatureName, float radian, float speed, const cocos2d::Vec2 &position) {

    _speedX = std::cos(radian) * speed;
    _speedY = -std::sin(radian) * speed;
    _isActive = true;
    _armature = BattleScene::instance->factory.buildArmature(armatureName);
    _armatureDisplay = (dragonBones::CCArmatureDisplay *) _armature->getDisplay();
    _armatureDisplay->setPosition(position);
    _armatureDisplay->setRotation(radian * dragonBones::RADIAN_TO_ANGLE);
    _armature->getAnimation().play("animtion0");
    _armatureDisplay->setScale(0.3);
    debug_angle = cocos2d::Label::create();
    debug_angle->setPosition(480.f, 600.f);
    debug_angle->setString("angle: " + StringUtils::toString(radian) + " power: " + StringUtils::toString(speed));
    debug_angle->setAlignment(cocos2d::TextHAlignment::CENTER);

    BattleScene::instance->addChild(debug_angle);
    dragonBones::WorldClock::clock.add(_armature);
    BattleScene::instance->addChild(_armatureDisplay);

}

Arrow::~Arrow() {
}

bool Arrow::update() {

    if (!_isActive) {
        return false;
    }

    const auto &position = _armatureDisplay->getPosition();
    const auto &rotation = _armatureDisplay->getRotation();


    if (rotation >= 180)
        _speedY -= BattleScene::G;
    else
        _speedY += BattleScene::G;

    _armatureDisplay->setPosition(position.x + _speedX, position.y + _speedY);
    _armatureDisplay->setRotation(std::atan2(-_speedY, _speedX) * dragonBones::RADIAN_TO_ANGLE);


    if (position.x < -100.f || position.x >= 960.f + 100.f) {
        remove();
        return true;
    }

    auto random = RandomHelper::random_real(80.f, 120.f);

    if (position.y < -100.f + BattleScene::GROUND + random || position.y >= 640.f + 100.f - random) {
        disableArrow();
    }

    checkCollisions();
    return false;
}

void Arrow::remove() {
    disableArrow();
    dragonBones::WorldClock::clock.remove(_armature);
    BattleScene::instance->removeChild(_armatureDisplay);
    delete this;
}


void Arrow::disableArrow() {
    _speedX = 0;
    _speedY = 0;
    _isActive = false;
    BattleScene::instance->removeChild(debug_angle);
}

void Arrow::checkCollisions() {
    auto j = BattleScene::instance->targets.size();
    auto rect = RectApplyTransform(_armatureDisplay->getChildren().at(0)->getBoundingBox(),
                                   _armatureDisplay->getNodeToWorldTransform());
    drawRect(rect, "arrow");
    while (j--) {
        auto bones = BattleScene::instance->targets[j]->getDisplay()->getChildren();
        const auto transform = BattleScene::instance->targets[j]->getDisplay()->getNodeToWorldTransform();
        int i = 0;
        for (auto bone : bones) {
            auto bone_rect = RectApplyTransform(bone->getBoundingBox(), transform);
            Arrow::drawRect(bone_rect, StringUtils::toString(j) + "-" + StringUtils::toString(i));
            i++;
            if (bone_rect.intersectsRect(rect)) {
                BattleScene::instance->targets[j]->addChild(_armatureDisplay);
                disableArrow();
            }
        }
    }
}


void Arrow::drawRect(Rect rect, std::string tag) {
    BattleScene::instance->removeChild(BattleScene::instance->getChildByName(tag));
    auto drawNode = DrawNode::create();
    drawNode->drawRect(Vec2(rect.getMinX(), rect.getMinY()), Vec2(rect.getMinX(), rect.getMaxY()),
                       Vec2(rect.getMaxX(), rect.getMaxY()), Vec2(rect.getMaxX(), rect.getMinY()), Color4F::RED);
    BattleScene::instance->addChild(drawNode, 1, tag);
}
