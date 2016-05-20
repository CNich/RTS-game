#ifndef __NINJA_FIREBALL_ATTACK_OBJECT__
#define __NINJA_FIREBALL_ATTACK_OBJECT__

#include "src/Utilities/GlobalVariables.h"
#include "src/AttackObjects/AttackObject.h"
#include "src/Utilities/pathNode.h"
#include "cocos2d.h"

class AttackObjectNinjaFireball : public AttackObject {

public:
	AttackObjectNinjaFireball();
	~AttackObjectNinjaFireball();
	static AttackObjectNinjaFireball* create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> *tpf, cocos2d::Point tpos);
	void attack();
	void initAttack();
	cocos2d::Point targetPos;
	float attackInterval = 0.2;
	int rotationIntervalAngle = 360;
	int fbDistanceNum = 3;
};

#endif //__NINJA_FIREBALL_ATTACK_OBJECT__
