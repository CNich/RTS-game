#ifndef __RANGEDRangedAttackObject_H__
#define __RANGEDRangedAttackObject_H__

#include "src/Utilities/GlobalVariables.h"
#include "src/AttackObjects/AttackObject.h"
#include "src/Utilities/pathNode.h"
#include "cocos2d.h"

class RangedAttackObject : public AttackObject {

protected:
	void seqCallback();
	virtual void seq();
	int sigma2 = 20; //sigma^2
	int attackAreaRange = 1;
	cocos2d::Point attackPos_nd;
	void calculateAttackPos();
	float diagonalFactor = 1.4;

	float maxRange = 8.0f;
	float maxHeight = 100.0f;
	float maxTravelTime = 1.0f;
	float distanceToEnemy_nd;
	float distanceToEnemy_pf;

public:
	RangedAttackObject();
	~RangedAttackObject();
	static RangedAttackObject* create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> *tpf);
	void attack(cocos2d::Point pos_pf);
	float gausFactor(int distance);
	void attackArea();
	void initAttack();
};

#endif //__RANGEDRangedAttackObject_H__
