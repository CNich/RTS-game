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

	void setGoalPosition(cocos2d::Point tpos){ goalPosition = tpos; };
	void setAsovle(){ goalPositionAsolve = true; }

	cocos2d::Point convertToPf(cocos2d::Point tmp);

	void setPfSize(int x, int y);
	void setPfSize(cocos2d::Point p);
	cocos2d::Point getPfSize(){ return pfSize; };

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
	int pointToPointDistance(cocos2d::Point a, cocos2d::Point b);

	bool consoleTrack = false;
	int consoleTrackNum = 0;
	int consoleCount = 0;
	void consoleDebugStatement(cocos2d::__String * str);

	PathFinder<BasicUnit> *pf;
	void setPf(PathFinder<BasicUnit> *tempPf);

	float getAngle(cocos2d::Point a, cocos2d::Point b);
	void setUnitDir(float angle);
	int getUnitDir(){return unitDir;};
	float getUnitAngle(){return unitAngle;};
	bool getMoving(){return moving;};
	float getWalkingSpeed(){return walkingSpeed;};

	void initHealthBar();
	void setHealthBar();
	void adjustHealthBarSize();

    bool debug_pathFinder = false;
    bool debug_decisionTree = false;

    char* getUnitType(){ return unitType; };

protected:
	cocos2d::Point startingPos;
	bool enemy = false;
	bool moving = false;
	void setStartingPos();
	void setPlayerPosition(cocos2d::Point position, bool diag);
	void setStartingPosCustom(cocos2d::Point tmp);
	void getMap();
	void getMap(PathFinder<BasicUnit> *tpf2);
	void delayedMove();
	virtual void updateDelayedMove();
	void setMoving();

	cocos2d::Point pfSize = {1, 1};
	void pfLocationUpdate(cocos2d::Point position);
	bool checkUnitSizeBlocked(int x_pf, int y_pf);

	bool movedYet = false;
	bool idle = true;
	bool idleTrack = true;
	char badMove = 0;
	virtual bool enemyIsAttackable();

	char blockedCount = 0;
	bool giveup = false;
	bool tempMoving = false;
	void setBasicUnitPF();
	void aSolveHelper(int x_pf, int y_pf);

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

	bool attacking = false;
	bool dead = false;
	virtual void removeFromLevel();

	bool goalPositionAsolve = false;
	cocos2d::Point goalPosition;

	bool currentEnemyIsCloseEnough = false;
	bool currentEnemyMoved = false;

	bool removeThisUnit = false;
	bool removeFromPf = true;

	virtual int getHealth();
	int health = 1000;
	int initHealth = 1000;
	int attackDamage = 60;
	int attackRange = 7;

	//how far a unit is willing to travel to it's closest enemy
	int movementRange = 13;

	//how far a unit is willing to travel away from it's goal position to attack another unit
	int attackTravelRange = 7;
	int price = 200;
	float blockDelay = 0.25;

	float attackSpeed = 2.0f;

	/*Animation section*/
	/********************************************/
	int unitDir = 0;
	float unitAngle = 0.0f;
	virtual cocos2d::Animate* animationWalk();
	float walkingSpeed = 0.2;
	bool walkingAnimationFlag = false;

	virtual void animationDie();
	float dieDuration = 0.07;

	virtual void animationAttack();
	float attackDuration = 0.08;
	int numAttackFrames = 8;
	/********************************************/

	cocos2d::Sprite * greenHealth;
	cocos2d::Sprite * redHealth;
	cocos2d::Sprite * healthBarOutline;
	float xHealthPos = 0.5;
	float yHealthPos = 0.65;

	char * unitType = "Basic Unit";

	virtual void update(float dt);
};

#endif // __BASIC_UNIT_H__
