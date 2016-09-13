//
// Created by igor on 09.09.16.
//

#include "Target.h"


Target::Target(){

}

dragonBones::CCArmatureDisplay* Target::getDisplay() {
    return _armatureDisplay;
}

void Target::addChild(cocos2d::Node* child) {
    return this->_armatureDisplay->addChild(child);
}