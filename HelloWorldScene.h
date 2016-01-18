#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "LinkedList.h"
#include "pathNode.h"
#include "PathFinder.h"
#include "HeapPathFinder.h"
#include "GlobalVariables.h"
#include "BasicUnit.h"
#include <vector>

#define SCORE_FONT_SIZE 0.1
//PathFinder<int> * pf;

class HelloWorldHud : public cocos2d::Layer
{
public:
    void numCollectedChanged(int numCollected);
    virtual bool init();
    CREATE_FUNC(HelloWorldHud);

    cocos2d::LabelTTF *label;
};

class HelloWorld : public cocos2d::Layer
{
private:
	cocos2d::TMXTiledMap *_tileMap;
	cocos2d::TMXLayer *_background;
	cocos2d::Sprite *_player;
	int _numCollected;
	static HelloWorldHud *_hud;
	cocos2d::Point _topLeft;
	cocos2d::Point _topRight;
	cocos2d::Point _bottomLeft;
	cocos2d::Point _bottomRight;
	//cocos2d::DrawNode drawNode;
	void setMoving();
	bool moving = false;
	LinkedList<cocos2d::Point> * lStack = new LinkedList<cocos2d::Point>;
	void addToStack(cocos2d::Point);
	void update(float dt);
	cocos2d::Label *scoreLabel;
	void delayedMove();
	void setPlayerPosition(cocos2d::Point position);
	void setMap();
	void checkMap();
	//PathFinder<int> * pf;
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

public:
	BasicUnit * rabbit;
	BasicUnit * rabbit2;
	BasicUnit * rabbit3;
	BasicUnit * rabbit4;
	BasicUnit * rabbit5;
	cocos2d::Vector<BasicUnit *> bvec;
	cocos2d::Vector<BasicUnit *> bvec2;
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void setViewPointCenter(cocos2d::Point position);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);

    void onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
    //void onTouchesMoved(cocos2d::Touch *touches, cocos2d::Event *unused_event);
    void onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *unused_event);

    //void setPlayerPosition(cocos2d::Point position);
    void addPlayerStack(cocos2d::Point position);

    void addEnemyAtPos(cocos2d::Point pos);
    void enemyMoveFinished(cocos2d::Object *pSender);
    void animateEnemy(cocos2d::Sprite *enemy);

    cocos2d::TMXLayer *_blockage;
    cocos2d::Point tileCoordForPosition(cocos2d::Point position);

    cocos2d::TMXLayer *_foreground;

    void projectileMoveFinished(cocos2d::Object *pSender);

    cocos2d::Vector<cocos2d::Sprite *> _enemies;
    cocos2d::Vector<cocos2d::Sprite *> _projectiles;

    bool _mode = false;

    void testCollisions(float dt);
    void enemyDistances(float dt);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
