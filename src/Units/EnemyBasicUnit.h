#ifndef __ENEMY_BASIC_UNIT__
#define __ENEMY_BASIC_UNIT__

#include "src/Units/BasicUnit.h"

class EnemyBasicUnit : public BasicUnit
{
public:
	EnemyBasicUnit();
	~EnemyBasicUnit();
	static EnemyBasicUnit* create();
	static EnemyBasicUnit* create(cocos2d::Point tmp);
	
	bool tracked = false;

	//void attack(BasicUnit *attacker, int damage, char attackType);
	int attackRange = 6;
	virtual void attack(BasicUnit *attacker, int damage, char attackType);

protected:
	bool enemyIsAttackable();
	virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;
	int health = 40;
	bool movingAlongBFSMap = false;
	virtual void removeFromLevel();
};

#endif // __ENEMY_BASIC_UNIT__
