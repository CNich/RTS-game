#ifndef __RANGEDRangedAttackObject_H__
#define __RANGEDRangedAttackObject_H__

#include "src/Utilities/GlobalVariables.h"
#include "src/AttackObjects/AttackObject.h"
#include "src/Utilities/pathNode.h"
#include "cocos2d.h"

class RangedAttackObject : public AttackObject {

public:
	RangedAttackObject();
	~RangedAttackObject();
	static RangedAttackObject* create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> *tpf);
	void attack();
	void initAttack();
};

#endif //__RANGEDRangedAttackObject_H__
