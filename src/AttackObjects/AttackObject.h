#ifndef __ATTACKOBJECT_H__
#define __ATTACKOBJECT_H__

#include "src/Utilities/GlobalVariables.h"
#include "src/Utilities/pathNode.h"
#include "cocos2d.h"

class AttackObject : public cocos2d::Sprite {

public:
	AttackObject();
	~AttackObject();
	static AttackObject* create(BasicUnit * attacker, cocos2d::Point location_pf, int damage, char attackType, PathFinder<BasicUnit> *tpf);
	void attack();
	void initAttack();
	PathFinder<BasicUnit> * pf;

protected:
	BasicUnit *parent;
	cocos2d::Point location_pf;
	int damage;
	char attackType;
};

#endif //__ATTACKOBJECT_H__
