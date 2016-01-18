#ifndef __BASIC_UNIT_H__
#define __BASIC_UNIT_H__

#include "cocos2d.h"
#include "pathNode.h"
#include "PathFinder.h"
#include "HeapPathFinder.h"
#include "GlobalVariables.h"
#include <vector>

class BasicUnit : public cocos2d::Sprite
{
public:
	BasicUnit();
	~BasicUnit();
	static BasicUnit* create();
	static BasicUnit* create(cocos2d::Point tmp);
	void ASolve(int x, int y);
	void setBlockageMap(cocos2d::TMXLayer *mp);
	void setForegroundMap(cocos2d::TMXLayer *mp);
	void setTileMap(cocos2d::TMXTiledMap *mp);
	cocos2d::Point goalPosition;

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

	void attack(BasicUnit *attacker, int damage, char attackType);
	int attackRange = 2;

private:
	cocos2d::Point startingPos;
	bool enemy = false;
	bool moving = false;
	void setStartingPos();
	void setPlayerPosition(cocos2d::Point position);
	void setStartingPosCustom(cocos2d::Point tmp);
	void getMap();
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
	void setPF();

	char team;

    cocos2d::TMXLayer *_blockage;
    cocos2d::TMXLayer *_foreground;
    cocos2d::TMXTiledMap *_tileMap;
    cocos2d::Point tileCoordForPosition(cocos2d::Point position);

	LinkedList<cocos2d::Point> * lStack = new LinkedList<cocos2d::Point>;
	PathFinder<int> * tpf;

	BasicUnit *newClosestEnemy = 0;
	BasicUnit *currentEnemy = 0;

	int health = 100;
	bool attacking = false;
	bool dead = false;

	void update(float dt);
};

#endif // __BASIC_UNIT_H__
