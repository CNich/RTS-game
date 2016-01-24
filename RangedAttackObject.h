#ifndef __RANGEDRangedAttackObject_H__
#define __RANGEDRangedAttackObject_H__

#include "GlobalVariables.h"
#include "pathNode.h"
#include "cocos2d.h"

class RangedAttackObject : public cocos2d::Sprite {

public:
	RangedAttackObject();
	~RangedAttackObject();
	static RangedAttackObject* create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType);
	void attack();
	void initAttack();

private:
	BasicUnit *parent;
	cocos2d::Point location;
	int damage;
	char attackType;
};

#endif //__RANGEDRangedAttackObject_H__
