#ifndef __ATTACKOBJECT_H__
#define __ATTACKOBJECT_H__

#include "pathNode.h"

class AttackObject : public cocos2d::Sprite {
public:
AttackObject();
~AttackObject();
AttackObject* create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType);
int attack(cocos2d::Point location, int damage, char attackType);

private:
BasicUnit *parent;
};
#endif //__ATTACKOBJECT_H__