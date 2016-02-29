#ifndef __BASIC_UNIT_H__
#define __BASIC_UNIT_H__

#include "cocos2d.h"
#include "src/Utilities/pathNode.h"
#include "src/Utilities/PathFinder.h"
#include "src/Utilities/HeapPathFinder.h"
#include "src/Utilities/GlobalVariables.h"
#include <vector>

class BasicUnit : public cocos2d::Sprite
{
public:
	BasicUnit();
	~BasicUnit();
	void removeUnit();
	static BasicUnit* create();
	static BasicUnit* create(cocos2d::Point tmp);
	void ASolve(int x, int y);
	void setBlockageMap(cocos2d::TMXLayer *mp);
	void setForegroundMap(cocos2d::TMXLayer *mp);
	void setTileMap(cocos2d::TMXTiledMap *mp);

	cocos2d::Point goalPosition;
	void setAsovle(){ goalPositionAsolve = true; }

	cocos2d::Point convertToPf(cocos2d::Point tmp);

	void setTeam(char t){team = t;};
	char getTeam(){return team;};

	void setNewClosestEnemy(BasicUnit* b){newClosestEnemy = b;};
	void setCurrentEnemy(BasicUnit* b){currentEnemy = b;};
	BasicUnit * getCurrentEnemy(){return currentEnemy;};
	BasicUnit * getNewClosestEnemy(){return newClosestEnemy ;};

	int unitToUnitDistance(BasicUnit *p1, BasicUnit *p2);

	bool isDead(){ return dead; };

	void returnHealth(int healthTaken, char attackType);

	virtual void attack(BasicUnit *attacker, int damage, char attackType);
	int attackRange = 7;

	PathFinder<BasicUnit> *pf;
	void setPf(PathFinder<BasicUnit> *tempPf);

protected:
	cocos2d::Point startingPos;
	bool enemy = false;
	bool moving = false;
	void setStartingPos();
	void setPlayerPosition(cocos2d::Point position);
	void setStartingPosCustom(cocos2d::Point tmp);
	void getMap();
	void getMap(PathFinder<BasicUnit> *tpf2);
	void delayedMove();
	void setMoving();


	bool movedYet = false;
	bool idle = true;
	bool idleTrack = true;
	char badMove = 0;
	bool enemyIsAttackable();

	char blockedCount = 0;
	bool giveup = false;
	bool tempMoving = false;
	void setBasicUnitPF();

	char team;

    cocos2d::TMXLayer *_blockage;
    cocos2d::TMXLayer *_foreground;
    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::Point tileCoordForPosition(cocos2d::Point position);

	LinkedList<cocos2d::Point> * lStack = new LinkedList<cocos2d::Point>;
	PathFinder<BasicUnit> * tpf;

	BasicUnit *newClosestEnemy = 0;
	BasicUnit *currentEnemy = 0;
	cocos2d::Point currentEnemyLocation;

	int health = 240;
	bool attacking = false;
	bool dead = false;

	bool goalPositionAsolve = false;

	bool currentEnemyIsCloseEnough = false;
	bool currentEnemyMoved = false;

	virtual void update(float dt);
};

#endif // __BASIC_UNIT_H__
