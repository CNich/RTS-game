#ifndef __NINJA_FIREBALL_ATTACK_OBJECT__
#define __NINJA_FIREBALL_ATTACK_OBJECT__

#include "src/Utilities/GlobalVariables.h"
#include "src/AttackObjects/RangedAttackObject.h"
#include "src/Utilities/pathNode.h"
#include "cocos2d.h"

class AttackObjectNinjaFireball : public RangedAttackObject {

protected:
	cocos2d::Point intMove(float angle, cocos2d::Point curPos);
	virtual void update(float dt);
	bool init = false;
	bool moving = false;
	void setMoving(bool tf){moving = tf;};
	cocos2d::Point dxy;

public:
	AttackObjectNinjaFireball();
	~AttackObjectNinjaFireball();
	static AttackObjectNinjaFireball* create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> *tpf);
	//void attack();
	void initAttack();
	cocos2d::Point targetPos;
	float attackInterval = 0.2;
	int rotationIntervalAngle = 360;
	int fbDistanceNum = 20;
	float movementAngle;
};

#endif //__NINJA_FIREBALL_ATTACK_OBJECT__
