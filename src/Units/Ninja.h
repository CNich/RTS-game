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
	int attackRange = 6;
	//virtual void attack(BasicUnit *attacker, int damage, char attackType);

protected:
	bool enemyIsAttackable();
	//virtual void update(float dt);
	bool removeThisUnit = false;
	bool removeFromPf = true;
	int initHealth = 200;

	float initAttackSpeed = 1.2;

	/*Animation section*/
	/********************************************/
	virtual cocos2d::Animate* animationWalk();
	float initWalkingSpeed = 0.25;
	bool walkingAnimationFlag = false;

	virtual void animationDie();

	virtual void animationAttack();
	/********************************************/

private:
	BFS *bfsMap;

};

#endif // __NINJA__
