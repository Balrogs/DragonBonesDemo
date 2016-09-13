//
// Created by igor on 08.09.16.
//

#ifndef ARCUNLIM_BRAIN_H
#define ARCUNLIM_BRAIN_H

#include <GameEngine/Objects/Stickman.h>
#include "cocos2d.h"

class Brain {

private:
   // Stickman* _body;

public:
    Brain(std::string type);

    ~Brain();

private:

    void _sendCommand();
    void _update();

};


#endif //ARCUNLIM_BRAIN_H
