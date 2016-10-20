#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "src/Utilities/LinkedList.h"
#include "src/Utilities/pathNode.h"
#include "src/Utilities/PathFinder.h"
#include "src/Utilities/HeapPathFinder.h"
#include "src/Utilities/GlobalVariables.h"
#include "src/Units/BasicUnit.h"
#include "src/Units/EnemyBasicUnit.h"
#include "src/Units/RangedBasicUnit.h"
#include "src/Units/EnemyBasicUnitRanged.h"
#include "src/Units/Ninja.h"
#include "src/Levels/HelloWorldHud.h"
#include "src/Levels/InfoHud.h"
#include <vector>
#include <memory>


#include "ui/CocosGUI.h"

#define SCORE_FONT_SIZE 0.1

class HelloWorldHud;
class InfoHud;

class HelloWorld : public cocos2d::Layer
{
private:
	cocos2d::TMXTiledMap *_tileMap;
	cocos2d::TMXLayer *_background;
	cocos2d::TMXLayer *_background2;
	cocos2d::TMXLayer *_background3;
	cocos2d::TMXLayer *_background4;
	cocos2d::TMXLayer *_background5;
	cocos2d::Sprite *_player;
	int _numCollected;
	//static HelloWorldHud *_hudB;
	HelloWorldHud *_hudB;
	//InfoHud *_infoHud;
	cocos2d::Point _topLeft;
	cocos2d::Point _topRight;
	cocos2d::Point _bottomLeft;
	cocos2d::Point _bottomRight;
	//cocos2d::DrawNode drawNode;

	bool moving = false;
	LinkedList<cocos2d::Point> * lStack = new LinkedList<cocos2d::Point>;
	void update(float dt);
	cocos2d::Label *scoreLabel;
	void delayedMove();
	void setMap();
	void checkMap();
	bool firstTouch = false;
	bool checked = false;
	float elapsedTime = 0;

	float touchBeganTime = 0;
	float touchMovedTime = 0;
	cocos2d::Point touchBeganPoint;
	cocos2d::Point touchMovedPoint;
	cocos2d::Point touchBegan1;
	cocos2d::Point touchBegan2;
	cocos2d::Point touchMoved1;
	cocos2d::Point touchMoved2;
	cocos2d::Point moveDiff;
	bool move1 = false;
	bool move2 = false;
	int tmcase = 0;
	int numTouch = 0;
	cocos2d::DrawNode * tilemapCenter;
	cocos2d::DrawNode * tilemapTopRight;

	int numSoundsPlaying = 0;

	void drawBFSMap();

	cocos2d::Sprite* setMovementSprites(char* str, cocos2d::Point pt);
	void goToMovementSprite(BasicUnit *i, cocos2d::Point tpos);

	cocos2d::EventListenerTouchOneByOne * movementSpriteListener;

	void initUnit(BasicUnit *r, char team);

	int fixPositions(char dim, int val);

	cocos2d::Sprite * spawnLocationSprite;
	cocos2d::Point enemySpawnLocation1_nd;
	cocos2d::Point enemySpawnLocation2_nd;

	float enemyBasicUnitRespawn = 5.0f;
	float enemyRangedBasicUnitRespawn = 7.0f;

public:
	InfoHud *_infoHud;
	PathFinder<BasicUnit> * pf;
	Ninja *ninja;
	cocos2d::Vector<BasicUnit *> bvec;
	cocos2d::Vector<EnemyBasicUnit *> bvec2;
	cocos2d::Vector<RangedBasicUnit *> rangedBasicUnitVec;
	cocos2d::Vector<EnemyBasicUnitRanged *> rangedBasicUnitVec2;

	std::vector<cocos2d::Sprite *> wayPointSprites;
	std::vector<cocos2d::Vector<BasicUnit *>> goodUnitVectors;
	std::vector<cocos2d::Vector<BasicUnit *>> badUnitVectors;

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void setViewPointCenter(cocos2d::Point position);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);

    //void setPlayerPosition(cocos2d::Point position);
    void addPlayerStack(cocos2d::Point position);

    void addEnemyAtPos(cocos2d::Point pos);
    void enemyMoveFinished(cocos2d::Object *pSender);
    void animateEnemy(cocos2d::Sprite *enemy);

    cocos2d::TMXLayer *_blockage;
    cocos2d::Point tileCoordForPosition(cocos2d::Point position);

    cocos2d::TMXLayer *_foreground;

    cocos2d::Sprite* bgImg;

    void projectileMoveFinished(cocos2d::Object *pSender);

    cocos2d::Vector<cocos2d::Sprite *> _enemies;
    cocos2d::Vector<cocos2d::Sprite *> _projectiles;

    bool _mode = false;

    void testCollisions(float dt);
    void enemyDistances(float dt);

    int getNumSoundsPlaying() {return numSoundsPlaying;};
    void incrementNumSoundsPlaying() { numSoundsPlaying++; };
    void decrementNumSoundsPlaying() { numSoundsPlaying++; };

    void createEnemyUnit(int option, cocos2d::Point spawnLocation);
    void createUnitGroup(int option, cocos2d::Point spawnLocation);
    void createUnitGroup(int option);
    cocos2d::Point getSpawnLocation_nd();

    cocos2d::Point _plpos;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
