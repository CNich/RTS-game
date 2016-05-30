#ifndef __NINJA_STAR_ATTACK_OBJECT__
#define __NINJA_STAR_ATTACK_OBJECT__

#include "src/Utilities/GlobalVariables.h"
#include "src/AttackObjects/RangedAttackObject.h"
#include "src/Utilities/pathNode.h"
#include "cocos2d.h"

class AttackObjectNinjaStar : public RangedAttackObject {
protected:
	virtual void seq();
public:
	AttackObjectNinjaStar();
	~AttackObjectNinjaStar();
	static AttackObjectNinjaStar* create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> *tpf);
	//void attack();
	//void initAttack();
};

#endif //__NINJA_STAR_ATTACK_OBJECT__
