#ifndef __ENEMY_BASIC_UNIT__
#define __ENEMY_BASIC_UNIT__

#include "src/Units/BasicUnit.h"
//#include "src/Levels/InfoHud.h"

class EnemyBasicUnit : public BasicUnit
{
public:
	EnemyBasicUnit();
	~EnemyBasicUnit();
	static EnemyBasicUnit* create();
	static EnemyBasicUnit* create(cocos2d::Point tmp);
	
	bool tracked = false;

	//InfoHud *_infoHud;

	//void attack(BasicUnit *attacker, int damage, char attackType);
	virtual void attack(BasicUnit *attacker, int damage, char attackType);

protected:
	bool enemyIsAttackable();
	virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;
	bool movingAlongBFSMap = false;
	virtual void removeFromLevel();

	int loot = 40;

	/*initialization variables*/
	/********************************************/
	int initHealth = 240;
	int initMovementRange = 10;
	int initAttackDamage = 120;
	/********************************************/
};

#endif // __ENEMY_BASIC_UNIT__
