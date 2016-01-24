#ifndef __ATTACKOBJECT_H__
#define __ATTACKOBJECT_H__

#include "GlobalVariables.h"
#include "pathNode.h"
#include "cocos2d.h"

class AttackObject : public cocos2d::Sprite {

public:
	AttackObject();
	~AttackObject();
	static AttackObject* create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType);
	void attack();
	void initAttack();

private:
	BasicUnit *parent;
	cocos2d::Point location;
	int damage;
	char attackType;
};

#endif //__ATTACKOBJECT_H__
