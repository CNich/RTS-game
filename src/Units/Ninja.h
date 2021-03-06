#ifndef __NINJA__
#define __NINJA__

#include "src/Units/BasicUnit.h"
#include "src/Utilities/BFS.h"

class Ninja : public BasicUnit
{
public:
	Ninja();
	~Ninja();
	static Ninja* create();
	static Ninja* create(cocos2d::Point tmp);

	virtual void setBFSmap();
	virtual void BFSInit(int x, int y){ bfsMap->setStart(x, y); bfsMap->solve();};

	//void attack(BasicUnit *attacker, int damage, char attackType);
	//int attackRange = 26;
	//virtual void attack(BasicUnit *attacker, int damage, char attackType);
	virtual void updateDelayedMove();

	void setFireBallLocation(cocos2d::Point tmp) { fireBallLocation = tmp; };

	void shootFireBall();

protected:
	bool enemyIsAttackable();
	//virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;

	/*Animation section*/
	/********************************************/
	virtual cocos2d::Animate* animationWalk();
	bool walkingAnimationFlag = false;
	virtual void animationDie();
	virtual void animationAttack();
	/********************************************/

	/*initialization variables*/
	/********************************************/
	//int initHealth = 5000;
	int initAttackRange = 10;
	int initMovementRange = 13;
	int initAttackDamage = 60;
	float initAttackSpeed = 1.2;
	float initWalkingSpeed = 0.25;
	/********************************************/

	cocos2d::Point fireBallLocation;

	virtual void removeFromLevel();

private:
	BFS *bfsMap;

};

#endif // __NINJA__
