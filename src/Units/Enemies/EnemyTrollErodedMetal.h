#ifndef __ENEMY_TROLL_ERODED_METAL__
#define __ENEMY_TROLL_ERODED_METAL__

#include "src/Units/EnemyBasicUnit.h"

class EnemyTrollErodedMetal : public EnemyBasicUnit
{
public:
	EnemyTrollErodedMetal();
	~EnemyTrollErodedMetal();
	static EnemyTrollErodedMetal* create();
	static EnemyTrollErodedMetal* create(cocos2d::Point tmp);
	

	//void attack(BasicUnit *attacker, int damage, char attackType);
	//virtual void attack(BasicUnit *attacker, int damage, char attackType);

	bool tracked = false;

protected:
	//virtual bool enemyIsAttackable();
	//virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;
	bool movingAlongBFSMap = false;
	virtual void animationAttack();
	virtual cocos2d::Animate* animationWalk();
	virtual void animationDie();
	virtual void removeFromLevel();

	/*initialization variables*/
	/********************************************/
	int initAttackRange = 12;
	int initMovementRange = 16;
	//int initHealth = 120;
	int initAttackDamage = 100;
	float initWalkingSpeed = 0.45;
	int initLoot = 300;
	/********************************************/
};

#endif // __ENEMY_TROLL_ERODED_METAL__
