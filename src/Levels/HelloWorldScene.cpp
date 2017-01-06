#include "src/Levels/HelloWorldScene.h"
#include "SimpleAudioEngine.h"
//#include <AudioEngine.h>

using namespace CocosDenshion;

USING_NS_CC;

//HelloWorldHud *HelloWorld::_hud = NULL;
HelloWorldHud * _HUD = nullptr;// = NULL;

Scene* HelloWorld::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	HelloWorldHud * hud = new HelloWorldHud();
	hud = HelloWorldHud::create();
	scene->addChild(hud);
	layer->_hudB = hud;
	_HUD = hud;
	hud->Level1 = layer;

	InfoHud * ihud = new InfoHud();
	ihud = InfoHud::create();
	scene->addChild(ihud);
	layer->_infoHud = ihud;
	ihud->Level1 = layer;

	// return the scene
	return scene;
}

bool HelloWorld::init() {
	if (!Layer::init()) {
		return false;
	}

	auto wframeCache = SpriteFrameCache::getInstance();
	wframeCache->addSpriteFramesWithFile("Units/Troll/Troll.plist");
	wframeCache->addSpriteFramesWithFile("Units/Troll/Troll - Eroded Metal.plist");
	wframeCache->addSpriteFramesWithFile("Units/Skinny Troll/attack/Skinny Troll attack.plist");
	wframeCache->addSpriteFramesWithFile("Units/Skinny Troll/die/Skinny Troll die.plist");
	wframeCache->addSpriteFramesWithFile("Units/Skinny Troll/walk/Skinny Troll walk.plist");
	wframeCache->addSpriteFramesWithFile("Units/Goblin Ranged/GoblinRangedAttack.plist");
	wframeCache->addSpriteFramesWithFile("Units/Goblin Ranged/GoblinRangedWalk.plist");
	wframeCache->addSpriteFramesWithFile("Units/Goblin Ranged/GoblinRangedDie.plist");
	wframeCache->addSpriteFramesWithFile("Units/Troll Hammer/TrollHammerAttack.plist");
	wframeCache->addSpriteFramesWithFile("Units/Troll Hammer/TrollHammerDie.plist");
	wframeCache->addSpriteFramesWithFile("Units/Troll Hammer/TrollHammerWalk.plist");
	wframeCache->addSpriteFramesWithFile("Units/Elf/Elf.plist");
	wframeCache->addSpriteFramesWithFile("Units/Wizard/Wizard.plist");
	wframeCache->addSpriteFramesWithFile("Units/Brigand/BrigandWalk.plist");
	wframeCache->addSpriteFramesWithFile("Units/Brigand/BrigandAttack.plist");
	wframeCache->addSpriteFramesWithFile("Units/Brigand/BrigandDie.plist");
	wframeCache->addSpriteFramesWithFile("Units/Goblin Melee/GoblinMeleeWalk.plist");
	wframeCache->addSpriteFramesWithFile("Units/Goblin Melee/GoblinMeleeAttack.plist");
	wframeCache->addSpriteFramesWithFile("Units/Goblin Melee/GoblinMeleeDie.plist");

	//cocos2d::experimental::AudioEngine::setMaxAudioInstance(1);

	/**********************************************************************
	//load tilemap
	CCLOG("Start tilemap");
	std::string file = "grassland/Level1FinalV2.tmx";
	CCLOG("%s",file.c_str());
	auto str =
			String::createWithContentsOfFile(
					FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
	_tileMap = TMXTiledMap::createWithXML(str->getCString(), "");

	CCLOG("made _tileMap");
	//load background layer
	//_background = _tileMap->layerNamed("Background");
	_background = _tileMap->layerNamed("bg");
	_background2 = _tileMap->layerNamed("bg2");
	_background3 = _tileMap->layerNamed("bg3");
	_background4 = _tileMap->layerNamed("bg4");
	_background4 = _tileMap->layerNamed("bg5");
	//_background = _tileMap->layerNamed("Image Layer 1");
	this->addChild(_tileMap, -1);

	//bgImg->setPosition(7 * 64, 7 * 32);

	CCLOG("added _background");

	//load blocked layer and set it to invisible
	_blockage = _tileMap->layerNamed("Meta");
	_blockage->setVisible(false);

	CCLOG("added _blockage");

	//get tilemap objects
	TMXObjectGroup *objects = _tileMap->getObjectGroup("Objects");
	CCASSERT(NULL != objects, "'Object-Player' object group not found");
	auto playerShowUpPoint = objects->getObject("PlayerShowUpPoint");
	auto enemySpawnPoint = objects->getObject("EnemySpawnPoint");
	auto enemySpawnPoint2 = objects->getObject("EnemySpawnPoint2");
	auto enemySpawnPoint3 = objects->getObject("EnemySpawnPoint3");
	auto imagePoint = objects->getObject("ImagePoint");
	CCASSERT(!playerShowUpPoint.empty(), "PlayerShowUpPoint object not found");

	CCLOG("added objects");

	//int imageX = imagePoint["x"].asInt();
	//int imageY = imagePoint["y"].asInt();
	//CCLOG("imgx: %d, imgy: %d", imageX, imageY);

	bgImg = cocos2d::Sprite::create("grassland/background cropped - small.png");
	bgImg->setAnchorPoint(Vec2(0,0));
	bgImg->setScale(2);
	this->addChild(bgImg,-2);
	bgImg->setPosition(this->convertToNodeSpace(cocos2d::Point(3.0, -6.0)));

	//auto drawNode = DrawNode::create();
	//drawNode->drawDot(cocos2d::Point(0,0), 16, Color4F::GREEN);
	//bgImg->addChild(drawNode, 1000);
	**********************************************************************/

    /**********************************************************************/
	//load tilemap
	cPrint("Start tilemap");
	std::string file = "grassland/Level 2_v1.tmx";
	cPrint("%s",file.c_str());
	auto str =
			String::createWithContentsOfFile(
					FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
	_tileMap = TMXTiledMap::createWithXML(str->getCString(), "");

	cPrint("made _tileMap");
	this->addChild(_tileMap, -1);
	cPrint("added tilemap");

	//load blocked layer and set it to invisible
	_blockage = _tileMap->layerNamed("Meta");
	_blockage->setVisible(false);

	cPrint("added _blockage");

	//get tilemap objects
	TMXObjectGroup *objects = _tileMap->getObjectGroup("Objects");
	CCASSERT(NULL != objects, "'Object-Player' object group not found");
	auto playerShowUpPoint = objects->getObject("PlayerShowUpPoint");
	auto enemySpawnPoint = objects->getObject("EnemySpawnPoint");
	auto enemySpawnPoint2 = objects->getObject("EnemySpawnPoint2");
	auto enemySpawnPoint3 = objects->getObject("EnemySpawnPoint3");
	auto imagePoint = objects->getObject("ImagePoint");
	CCASSERT(!playerShowUpPoint.empty(), "PlayerShowUpPoint object not found");

	cPrint("added objects");

	//int imageX = imagePoint["x"].asInt();
	//int imageY = imagePoint["y"].asInt();
	//CCLOG("imgx: %d, imgy: %d", imageX, imageY);

	bgImg = cocos2d::Sprite::create("grassland/Level_2/Level1_Background4.png");
	bgImg->setAnchorPoint(Vec2(0,0));
	bgImg->setScale(1);
	this->addChild(bgImg,-2);
	//bgImg->setPosition(this->convertToNodeSpace(cocos2d::Point(-905.0, -373.0)));
	//bgImg->setPosition(this->convertToNodeSpace(cocos2d::Point(-717.67, 1.33)));
	bgImg->setPosition(this->convertToNodeSpace(cocos2d::Point(0, 0)));

	//auto drawNode = DrawNode::create();
	//drawNode->drawDot(cocos2d::Point(0,0), 16, Color4F::GREEN);
	//bgImg->addChild(drawNode, 1000);
	/**********************************************************************/

	tilemapCenter = DrawNode::create();
	tilemapCenter->drawDot(cocos2d::Point(0,0), 16, Color4F::GREEN);
	this->addChild(tilemapCenter, 10);
	tilemapTopRight = DrawNode::create();
	this->addChild(tilemapTopRight, 10);
	tilemapTopRight->drawDot({0,0}, 16, Color4F::BLUE);
	tilemapTopRight->setPosition({cocos2d::Point(bgImg->getContentSize().width * bgImg->getScale(), bgImg->getContentSize().height * bgImg->getScale())});
	__String *ts2 = __String::createWithFormat("%2.0f %2.0f, %2.0f %2.0f",
			_tileMap->getMapSize().height, _tileMap->getMapSize().width,
			_tileMap->getTileSize().height, _tileMap->getTileSize().width);

	pf = new PathFinder<BasicUnit>(_tileMap->getMapSize().height,
			_tileMap->getMapSize().width);
	/*
	 * WATCH OUT FOR THIS
	 */
	pf->setTileX(_tileMap->getTileSize().width);
	pf->setTileY(_tileMap->getTileSize().height);

	pf->setOffX(pf->getTileX()/2);
	pf->setOffY(pf->getTileY()/2);

	this->setMap();

	CCLOG("SET MAP");

	int x = playerShowUpPoint["x"].asInt();
	int y = playerShowUpPoint["y"].asInt();

	int enemyX =  fixPositions('x', enemySpawnPoint["x"].asInt());
	int enemyY =  fixPositions('y', enemySpawnPoint["y"].asInt());
	int enemyX2 = fixPositions('x', enemySpawnPoint2["x"].asInt());
	int enemyY2 = fixPositions('y', enemySpawnPoint2["y"].asInt());
	int enemyX3 = fixPositions('x', enemySpawnPoint3["x"].asInt());
	int enemyY3 = fixPositions('y', enemySpawnPoint3["y"].asInt());

	//_plpos.x = x + _tileMap->getTileSize().width / 2;
	//_plpos.y = y + _tileMap->getTileSize().height / 2;

	_plpos.x = fixPositions('x', x);
	_plpos.y = fixPositions('y', y);


	//set touch events for pan/zoom
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {
		numTouch++;
		if(numTouch == 1) {
			touchBegan1 = touch->getLocation();
		}
		else if(numTouch == 2) {
			touchBegan2 = touch->getLocation();
		}

		touchBeganPoint = touch->getLocation();
		return true;
	};

	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,
			this);

	setViewPointCenter(_plpos);
	this->setScale(1);

	auto tL = objects->getObject("topLeft");
	_topLeft.x = tL["x"].asInt();
	_topLeft.y = tL["y"].asInt();

	tL = objects->getObject("topRight");
	_topRight.x = tL["x"].asInt();
	_topRight.y = tL["y"].asInt();

	tL = objects->getObject("bottomLeft");
	_bottomLeft.x = tL["x"].asInt();
	_bottomLeft.y = tL["y"].asInt();

	tL = objects->getObject("bottomRight");
	_bottomRight.x = tL["x"].asInt();
	_bottomRight.y = tL["y"].asInt();

	//useful!
	for (auto& eSpawnPoint : objects->getObjects()) {
		ValueMap& dict = eSpawnPoint.asValueMap();
		x = dict["x"].asInt();
		y = dict["y"].asInt();
		//this->addEnemyAtPos(Point(x, y));
	}

	_foreground = _tileMap->getLayer("Foreground");

	SimpleAudioEngine::getInstance()->preloadEffect("error.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("item.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("step.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("wade.mp3");
	//SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3");
	//SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1);

	this->scheduleUpdate();

	__String *tempScore = __String::createWithFormat("%i %i, h:%i t:%i", moving,
			lStack->empty(), lStack->Hempty(), lStack->Tempty());

	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto winSize = Director::getInstance()->getWinSize() * this->getScale();

	scoreLabel = Label::createWithTTF(tempScore->getCString(),
			"fonts/Marker Felt.ttf", winSize.height * SCORE_FONT_SIZE);
	//scoreLabel = Label::createWithTTF("hi", "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);
	scoreLabel->setColor(Color3B::WHITE);
	scoreLabel->setPosition(Point(winSize.width / 2, winSize.height));

	//this->addChild(scoreLabel, 10000);

	cocos2d::Label *tileAtrs = Label::createWithTTF(ts2->getCString(),
			"fonts/Marker Felt.ttf", winSize.height * SCORE_FONT_SIZE);
	tileAtrs->setColor(Color3B::BLUE);
	tileAtrs->setPosition(Point(winSize.width / 2, winSize.height * 0.75));

	int t1 = 10;
	int t2 = 0;

	createUnitGroup(2, _plpos, true);

	auto pnt = _plpos;
	pnt.x = _plpos.x + pf->getTileX() * 3;
	pnt.y = _plpos.y  + pf->getTileY();
	createUnitGroup(0, pnt, true);

	pnt.y -= pf->getTileY() * 3;
	createUnitGroup(1, pnt, true);

    pnt.y -= pf->getTileY() * 3;
	createUnitGroup(3, pnt, true);

	enemySpawnLocation1_nd.x =  (float)enemyX;
	enemySpawnLocation1_nd.y =  (float)enemyY;
	enemySpawnLocation2_nd.x =  (float)enemyX2;
	enemySpawnLocation2_nd.y =  (float)enemyY2;

	return true;

}

cocos2d::Sprite * HelloWorld::setMovementSprites(char *str, cocos2d::Point pt){
	auto spr = cocos2d::Sprite::create(str);
	spr->setPosition(pt);
	//spr->setScale(0.5);
	this->addChild(spr, 1);
	return spr;
}

void HelloWorld::goToMovementSprite(BasicUnit *i, cocos2d::Point tpos){
	i->setGoalPosition(tpos);
	//i->goalPosition.x = tpos.x;
	//i->goalPosition.y = tpos.y;


	//For debugging purposes
	//auto tt = tpos;
	//tt.x = tt.x * pf->getTileX() + pf->getOffX();
	//tt.y = tt.y * pf->getTileY() + pf->getOffY();
	//CCLOG("touch position: %3.3f %3.3f", tpos.x, tpos.y);
	//CCLOG("touch position: %3.3f %3.3f", tt.x, tt.y);
	//auto drawNode = DrawNode::create();
	//drawNode->drawDot(tt, 16, Color4F::GREEN);
	//this->addChild(drawNode, 1000);

	i->setAsovle();
}

void HelloWorld::initUnit(BasicUnit *r, char team){
	this->addChild(r, 2);
	r->setPf(pf);
	//this->setPosition(p);
	r->setBlockageMap(_blockage);
	r->setForegroundMap(_foreground);
	r->setTileMap(_tileMap);
	r->setTeam(team);
	r->initHealthBar();
	r->adjustHealthBarSize();
	//r->_infoHud = _infoHud;
	if(team == 0){
		auto pos = r->convertToPf(r->getPosition());
		//r->goalPosition.x = pos.x;
		//r->goalPosition.y = pos.y;
		r->setGoalPosition(pos);
	}
}

void HelloWorld::createUnitGroup(int option, cocos2d::Point spawnLocation, bool onLevelInit = false){
	CCLOG("onLevelInit = %b", onLevelInit);
	if(!onLevelInit){
		 if(option == 0 && _infoHud->getGoldAmount() >= 200){
			createUnitGroupHelper(option, spawnLocation);
			_infoHud->spendGold(200);
		} else if(option == 1 && _infoHud->getGoldAmount() >= 250){
			createUnitGroupHelper(option, spawnLocation);
			_infoHud->spendGold(250);
		} else if(option == 3 && _infoHud->getGoldAmount() >= 250){
			createUnitGroupHelper(option, spawnLocation);
			_infoHud->spendGold(250);
		}
		CCLOG("gold: %d, createUnitGroup %p", _infoHud->getGoldAmount(), this->_infoHud);
	} else{
		createUnitGroupHelper(option, spawnLocation);
	}
}
void HelloWorld::createUnitGroup(int option){
	createUnitGroup(option, getSpawnLocation_nd());
}

void HelloWorld::createEnemyUnit(int option, cocos2d::Point spawnLocation){
	if(option == 31){
		EnemyBasicUnit * r = EnemyBasicUnit::create(spawnLocation);
		bvec2.pushBack(r);
		initUnit(r, 1);
		r->_infoHud = _infoHud;
		r->setColor(cocos2d::Color3B::BLUE);
	} else if(option == 32){
		EnemyBasicUnitRanged * r = EnemyBasicUnitRanged::create(spawnLocation);
		rangedBasicUnitVec2.pushBack(r);
		initUnit(r, 1);
		r->_infoHud = _infoHud;
		r->setColor(cocos2d::Color3B::BLUE);
	} else if(option == 33){
		EnemyTrollErodedMetal * r = EnemyTrollErodedMetal::create(spawnLocation);
		TrollErodedMetalVec.pushBack(r);
		initUnit(r, 1);
		r->_infoHud = _infoHud;
	} else if(option == 34){
		EnemySkinnyTroll * r = EnemySkinnyTroll::create(spawnLocation);
		SkinnyTrollVec.pushBack(r);
		initUnit(r, 1);
		r->_infoHud = _infoHud;
	} else if(option == 35){
		EnemyGoblinRanged * r = EnemyGoblinRanged::create(spawnLocation);
		GoblinRangedVec.pushBack(r);
		initUnit(r, 1);
		r->_infoHud = _infoHud;
	} else if(option == 36){
		EnemyTrollHammer * r = EnemyTrollHammer::create(spawnLocation);
		TrollHammerVec.pushBack(r);
		initUnit(r, 1);
		r->_infoHud = _infoHud;
		//r->tracked = true;
	} else if(option == 37){
		EnemyGoblinMelee * r = EnemyGoblinMelee::create(spawnLocation);
		GoblinMeleeVec.pushBack(r);
		initUnit(r, 1);
		r->_infoHud = _infoHud;
		//r->tracked = true;
	}
	CCLOG("createUnitGroup %p", this->_infoHud);
}

void HelloWorld::createUnitGroupHelper(int option, cocos2d::Point spawnLocation){
	cocos2d::Vector<BasicUnit *> bv1;
	if(option == 0){
		for(int i=0; i < 2; i++){
			auto p = spawnLocation;
			p.y = spawnLocation.y  + pf->getTileY() * i * 3;
			BasicUnit * r = BasicUnit::create(p);
			bv1.pushBack(r);
			initUnit(r, 0);
			if(i == 0) r->debug_pathFinder = true;
			//r->debug_decisionTree = true;
		}
	} else if(option == 1){
		for(int i=0; i < 5; i++){
			auto p = spawnLocation;
			p.y = spawnLocation.y  + pf->getTileY() * i;
			RangedBasicUnit * r = RangedBasicUnit::create(p);
			bv1.pushBack(r);
			initUnit(r, 0);
			if(i == 4){
				r->setColor(cocos2d::Color3B::YELLOW);
			}
		}
	} else if(option == 2){
		auto ninjaPos = _plpos;
		ninja = Ninja::create(spawnLocation);
		initUnit(ninja, 0);

		//init enemy following
		auto bfsp = ninja->convertToPf(ninja->getPosition());
		ninja->setBFSmap();
		ninja->BFSInit(bfsp.x, bfsp.y);
		//ninja->tracked = true;
		//ninja->consoleTrack = true;
		bv1.pushBack(ninja);
	} else if(option == 3){
        for(int i=0; i < 5; i++){
			auto p = spawnLocation;
			p.y = spawnLocation.y  + pf->getTileY() * i;
			Brigand * r = Brigand::create(p);
			bv1.pushBack(r);
			initUnit(r, 0);
		}
	}

	goodUnitVectors.push_back(bv1);

	TMXObjectGroup *objects = _tileMap->getObjectGroup("Objects");
	CCASSERT(NULL != objects, "'Object-Player' object group not found");
	auto playerShowUpPoint = objects->getObject("PlayerShowUpPoint");
	int x = playerShowUpPoint["x"].asInt();
	int y = playerShowUpPoint["y"].asInt();

	char* wayPointPng;
	if(option == 0){
		wayPointPng = "Redsquare.png";
	} else if(option == 1){
		wayPointPng = "Cyansquare.png";
	} else if(option == 2){
		wayPointPng = "Yellowsquare.png";
	} else if(option == 3){
		wayPointPng = "GreySquare.png";
	}

	auto tSprite = setMovementSprites(wayPointPng, Point(x + 16, y + 48));
	tSprite->setOpacity(180);

	wayPointSprites.push_back(tSprite);
	tSprite->setPosition(spawnLocation.x - pf->getTileX() * 2, spawnLocation.y - pf->getTileY() * 2);

	cocos2d::Sprite * spellLocationSprite;
	if(option == 2){
		spellLocationSprite = setMovementSprites("Orangesquare.png", Point(x, y));
		//wayPointSprites.push_back(spellLocationSprite);
		spawnLocationSprite = setMovementSprites("bluesquare.png", Point(x + pf->getTileX() * 4, y));
	}
	//Create a "one by one" touch event listener (processes one touch at a time)
	auto listener = EventListenerTouchOneByOne::create();
	// When "swallow touches" is true, then returning 'true' from the onTouchBegan method will "swallow" the touch event, preventing other listeners from using it.
	listener->setSwallowTouches(true);

	// Example of using a lambda expression to implement onTouchBegan event callback function
	listener->onTouchBegan = [=](Touch* touch, Event* event) {

		// event->getCurrentTarget() returns the *listener's* sceneGraphPriority node.
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		//Get the position of the current point relative to the button
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		//Check the click area
		if (rect.containsPoint(locationInNode))
		{
			//CCLOG("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			target->setOpacity(255);
			return true;
		}
		return false;
	};

	//Trigger when moving touch
	listener->onTouchMoved = [=](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		auto tempScale = this->getScale();
		//Move the position of current button sprite
		target->setPosition(target->getPosition() +
			cocos2d::Point(touch->getDelta().x / tempScale,
			touch->getDelta().y / tempScale));
	};

	//Process the touch end event
	listener->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		//CCLOG("sprite onTouchesEnded.. ");
		target->setOpacity(180);

		auto tpos = touch->getLocation();
		tpos = convertToNodeSpace(tpos);
		auto nodeTpos = tpos;

		tpos = ninja->convertToPf(tpos);

		if (tpos.x >= 0 && tpos.x < pf->getRows() && tpos.y >= 0 && tpos.y < pf->getCols()) {
			if(option == 2){
				if(target == tSprite){
					goToMovementSprite(ninja, tpos);
					_hudB->setNinja();
				}
				else if(target == spellLocationSprite){
					ninja->setFireBallLocation(nodeTpos);
					_hudB->setNinja();
				}
			} else if(target == tSprite){
				for(BasicUnit *i : bv1){
					goToMovementSprite(i, tpos);
				}
				_hudB->setBasicUnit();
			}
		}
	};

	//Add listener
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, tSprite);
	if(option == 2){
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), spellLocationSprite);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), spawnLocationSprite);
	}
}

cocos2d::Point HelloWorld::getSpawnLocation_nd(){
	cocos2d::Point t;
	t = spawnLocationSprite->getPosition();
	t.x = (float)fixPositions('x', t.x);
	t.y = (float)fixPositions('y', t.y);
	return t;
}

void HelloWorld::setViewPointCenter(Point position) {
	auto winSize = Director::getInstance()->getWinSize() * this->getScale();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x,
			(_tileMap->getMapSize().width * this->_tileMap->getTileSize().width)
					- winSize.width / 2);
	y = MIN(y,
			(_tileMap->getMapSize().height * _tileMap->getTileSize().height)
					- winSize.height / 2);
	auto actualPosition = Point(x, y);

	auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
	auto viewPoint = centerOfView - actualPosition;
	this->setPosition(viewPoint);
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event) {

	numTouch--;
	if (!firstTouch) {
		//this->setMap();
		CCLOG("first touch of the game................");
		this->_hudB->setNinja();
		CCLOG("%p %p %p", _hudB, _hudB, _hudB);
		CCLOG("%p %p %p", _HUD, _HUD, _HUD);
		CCLOG("%p %p %p", ninja, ninja, ninja);
		CCLOG("%p %p %p", _infoHud, _infoHud, _infoHud);
		CCLOG("getTileX: %d, getTileY: %d",pf->getTileX(), pf->getTileY());
		CCLOG("getRows: %d, getCols: %d, getOffX: %d, getOffY: %d",
				pf->getRows(), pf->getCols(), pf->getOffX(), pf->getOffY());
		this->firstTouch = true;
		//checkMap();
	}

	auto ttouch = touch->getLocation();

	if (_mode == false) {
		//will fill in
	} else {
	}
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event) {
	//panning
	if (numTouch == 1) {
		moveDiff.x = 0;
		moveDiff.y = 0;
		bool move1 = false;
		bool move2 = false;
		touchMovedPoint = touch->getLocation();
		auto delta = touchMovedPoint - touchBeganPoint;
		auto currPos = this->getPosition();

		//don't slide too fast + quick fx for zoom bug that changes the pan very quick
		if(delta.x > 50){
			delta.x = 50;
		} else if(delta.x < -50){
			delta.x = -50;
		}

		if(delta.y > 50){
			delta.y = 50;
		} else if(delta.y < -50){
			delta.y = -50;
		}

		//panning factor
		int s = 1.5;

		/* delta (movement of touch)
		 * - - - - >+
		 * |
		 * |
		 * V
		 *  +
		 */
		delta.x = s * delta.x;
		delta.y = s * delta.y;

		//right side of screen (x = bgImg.width - screen width (1920 in my case)) while dragging touch left
		if(delta.x < 0 && convertToWorldSpace(tilemapCenter->getPosition()).x + delta.x < -1 * (bgImg->getContentSize().width * bgImg->getScale() * this->getScale() - Director::getInstance()->getWinSize().width)){
			delta.x = -1 * (bgImg->getContentSize().width * bgImg->getScale() * this->getScale() - Director::getInstance()->getWinSize().width) - convertToWorldSpace(tilemapCenter->getPosition()).x;
		}
		//left side of screen (x = 0) while dragging touch right
		else if(delta.x > 0 && convertToWorldSpace(tilemapCenter->getPosition()).x + delta.x > 0){
			delta.x = -convertToWorldSpace(tilemapCenter->getPosition()).x;
		}
		//top side of screen (y = bgImg.height - screen width (1080 in my case)) while dragging touch up
		if(delta.y < 0 && convertToWorldSpace(tilemapCenter->getPosition()).y + delta.y < -1 * (bgImg->getContentSize().height * bgImg->getScale() * this->getScale() - Director::getInstance()->getWinSize().height)){
			delta.y = -1 * (bgImg->getContentSize().height * bgImg->getScale() * this->getScale() - Director::getInstance()->getWinSize().height) - convertToWorldSpace(tilemapCenter->getPosition()).y;
		}
		//bottom side of screen (y = 0) while dragging touch down
		else if(delta.y > 0 && convertToWorldSpace(tilemapCenter->getPosition()).y + delta.y > 0){
			delta.y = -convertToWorldSpace(tilemapCenter->getPosition()).y;
		}

		auto newPos = currPos + delta;

		touchBeganPoint = touchMovedPoint;

		this->setPosition(newPos);
	}
	//zooming in/out
	else if (numTouch == 2) {
		cocos2d::Point moved = touch->getLocation();
		if (touch->getStartLocation() == touchBegan1) {
			touchMoved1 = moved;
			move1 = true;
		} else if (touch->getStartLocation() == touchBegan2) {
			touchMoved2 = moved;
			move2 = true;
		}
		cocos2d::Point cdiff = touchMoved1 - touchMoved2;
		if (move1 && move2) {

			float tdiff = (cdiff.x * cdiff.x + cdiff.y * cdiff.y)
					/ (moveDiff.x * moveDiff.x + moveDiff.y * moveDiff.y);
			moveDiff = cdiff;
			if (tdiff > 1.02) {
				tdiff = 1.02;
			} else if (tdiff < 0.98) {
				tdiff = 0.98;
			}

			if(bgImg->getContentSize().width > bgImg->getContentSize().height &&
					( bgImg->getContentSize().width * bgImg->getScale() * this->getScale() ) * ( this->getScale() * tdiff ) < Director::getInstance()->getWinSize().width){
				tdiff = Director::getInstance()->getWinSize().width / ( (bgImg->getContentSize().width * bgImg->getScale() * this->getScale() ) * ( this->getScale()) );
			} else if(bgImg->getContentSize().width < bgImg->getContentSize().height &&
					( bgImg->getContentSize().height * bgImg->getScale() * this->getScale() ) * ( this->getScale() * tdiff ) < Director::getInstance()->getWinSize().height){
				tdiff = Director::getInstance()->getWinSize().height / ( (bgImg->getContentSize().height * bgImg->getScale() * this->getScale() ) * ( this->getScale()) );
			}

			this->setScale(this->getScale() * tdiff);

			cocos2d::Point delta = {0, 0};

			//right side of screen (x = bgImg.width - screen width (1920 in my case))
			if(convertToWorldSpace(tilemapCenter->getPosition()).x < -1 * (bgImg->getContentSize().width * bgImg->getScale() * this->getScale() - Director::getInstance()->getWinSize().width)){
				delta.x = -1 * (bgImg->getContentSize().width * bgImg->getScale() * this->getScale() - Director::getInstance()->getWinSize().width) - convertToWorldSpace(tilemapCenter->getPosition()).x;
			}
			//left side of screen (x = 0)
			else if(convertToWorldSpace(tilemapCenter->getPosition()).x + delta.x > 0){
				delta.x = -convertToWorldSpace(tilemapCenter->getPosition()).x;
			}
			//top side of screen (y = bgImg.height - screen width (1080 in my case))
			if(convertToWorldSpace(tilemapCenter->getPosition()).y < -1 * (bgImg->getContentSize().height * bgImg->getScale() * this->getScale() - Director::getInstance()->getWinSize().height)){
				delta.y = -1 * (bgImg->getContentSize().height * bgImg->getScale() * this->getScale() - Director::getInstance()->getWinSize().height) - convertToWorldSpace(tilemapCenter->getPosition()).y;
			}
			//bottom side of screen (y = 0)
			else if(convertToWorldSpace(tilemapCenter->getPosition()).y + delta.y > 0){
				delta.y = -convertToWorldSpace(tilemapCenter->getPosition()).y;
			}

			this->setPosition(this->getPosition() + delta);

		}
		moveDiff = cdiff;
	}
}

/*
 * Finds and sets the closest enemy of each unit
 */
void HelloWorld::enemyDistances(float dt){
	cocos2d::Vector<BasicUnit *> team1;
	cocos2d::Vector<BasicUnit *> team2;

	if(!ninja->isDead()) {
		team1.pushBack(ninja);
	}

	for(auto p : goodUnitVectors){
		for(auto u : p){
			if(u != 0 && !u->isDead()){
				team1.pushBack(u);
			}
		}
	}

	/*
	cocos2d::Vector<BasicUnit *> bvecTemp;
	for(BasicUnit * p : bvec){
		if(p != 0 && !p->isDead()){
			team1.pushBack(p);
		} else{
			bvecTemp.pushBack(p);
		}
	}
	for(BasicUnit * p : bvecTemp){
		//bvec.erase(bvec.find(p));
	}


	cocos2d::Vector<RangedBasicUnit *> rangedBasicUnitVecTemp;
	for(RangedBasicUnit * p : rangedBasicUnitVec){
		if(p != 0 && !p->isDead()){
			team1.pushBack(p);
		} else{
			rangedBasicUnitVecTemp.pushBack(p);
		}
	}
	for(RangedBasicUnit * p : rangedBasicUnitVecTemp){
		//rangedBasicUnitVec.erase(rangedBasicUnitVec.find(p));
	}
	*/

	cocos2d::Vector<EnemyBasicUnit *> bvecTemp2;
	for(EnemyBasicUnit * p : bvec2){
		if(p != 0 && !p->isDead()){
			team2.pushBack(p);
		} else{
			bvecTemp2.pushBack(p);
		}
	}
	for(EnemyBasicUnit * p : bvecTemp2){
		//bvec2.erase(bvec2.find(p));
	}

	cocos2d::Vector<EnemyBasicUnitRanged *> rangedBasicUnitVec2Temp;
	for(EnemyBasicUnitRanged * p : rangedBasicUnitVec2){
		if(p != 0 && !p->isDead()){
			team2.pushBack(p);
		} else{
			rangedBasicUnitVec2Temp.pushBack(p);
		}
	}
	for(EnemyBasicUnitRanged * p : rangedBasicUnitVec2Temp){
		//rangedBasicUnitVec2.erase(rangedBasicUnitVec2.find(p));
	}

	cocos2d::Vector<EnemyTrollErodedMetal *> temv;
	for(EnemyTrollErodedMetal * p : TrollErodedMetalVec){
		if(p != 0 && !p->isDead()){
			team2.pushBack(p);
		} else{
			temv.pushBack(p);
		}
	}
	for(EnemyBasicUnitRanged * p : rangedBasicUnitVec2Temp){
		//rangedBasicUnitVec2.erase(rangedBasicUnitVec2.find(p));
	}

	cocos2d::Vector<EnemySkinnyTroll *> temv2;
	for(EnemySkinnyTroll * p : SkinnyTrollVec){
		if(p != 0 && !p->isDead()){
			team2.pushBack(p);
		} else{
			temv2.pushBack(p);
		}
	}
	for(EnemyBasicUnitRanged * p : rangedBasicUnitVec2Temp){
		//rangedBasicUnitVec2.erase(rangedBasicUnitVec2.find(p));
	}

	for(EnemyGoblinRanged * p : GoblinRangedVec){
		if(p != 0 && !p->isDead()){
			team2.pushBack(p);
		}
	}

	for(EnemyTrollHammer * p : TrollHammerVec){
		if(p != 0 && !p->isDead()){
			team2.pushBack(p);
		}
	}

	for(EnemyGoblinMelee * p : GoblinMeleeVec){
		if(p != 0 && !p->isDead()){
			team2.pushBack(p);
		}
	}

	//reset newClosestEnemy
	if(team1.size() > 0 && team2.size() > 0){

		for(BasicUnit * p2 : team2){
			p2->setCurrentEnemy(0);
		}

		for(BasicUnit * p : team1){
			if(p != 0 && !p->isDead()){
				//reset newClosestEnemy
				p->setCurrentEnemy(0);
				//find distances between each per team
				for(BasicUnit * p2 : team2){
					//set current enemy to new if no previous current enemy exists
					if(p2 != 0 && !p2->isDead()){
						if(p->getCurrentEnemy() == 0){
							p->setCurrentEnemy(p2);
						} else{
							int dist1 = p->unitToUnitDistance(p, p2);
							int dist2 = p->unitToUnitDistance(p, p->getCurrentEnemy());
							if(dist1 < dist2){
								p->setCurrentEnemy(p2);
							}
						}

						//do the same for p2
						if(p2->getCurrentEnemy() == 0){
							p2->setCurrentEnemy(p);
						} else{
							int dist1 = p2->unitToUnitDistance(p2, p);
							int dist2 = p2->unitToUnitDistance(p2, p2->getCurrentEnemy());
							if(dist1 < dist2){
								p2->setCurrentEnemy(p);
							}
						}
					}
				}
			}
		}
	}
}

/*
 * if all units of a group are dead, hide it's waypoint sprite
 */
void HelloWorld::clearEmptyWaypoints(float dt){
	for(int i = 0; i < wayPointSprites.size(); i++){
		bool spriteVisible = false;
		for(BasicUnit * b : goodUnitVectors[i]){
			if(b != 0 && !b->isDead()){
				spriteVisible = true;
			}
		}
		if(!spriteVisible){
			wayPointSprites[i]->setVisible(false);
		}
	}
}

void HelloWorld::update(float dt) {
	elapsedTime += dt;
	testCollisions(dt);
	enemyDistances(dt);
	clearEmptyWaypoints(dt);

	enemyBasicUnitRespawn -= dt;
	enemyRangedBasicUnitRespawn -= dt;
	fireballTimer -= dt;
	ErodedMetalTrollTimer -= dt;

	if(enemyBasicUnitRespawn < 0){
		enemyBasicUnitRespawn += 30.0f;
		//createEnemyUnit(31, enemySpawnLocation1_nd);
		//createEnemyUnit(35, enemySpawnLocation1_nd);
		//createEnemyUnit(36, enemySpawnLocation1_nd);
	}

	if(enemyRangedBasicUnitRespawn < 0){
		enemyRangedBasicUnitRespawn += 5.0f;
		//createEnemyUnit(32, enemySpawnLocation2_nd);
		createEnemyUnit(36, enemySpawnLocation2_nd);
		createEnemyUnit(37, enemySpawnLocation1_nd);
	}

	if(!ninjaDead && ninja->isDead()){
        ninjaDead = true;
	}

	if(fireballTimer < 0 && !ninjaDead){
		fireballTimer += 1.4;
		//ninja->shootFireBall();
		testGlobalVariable++;
	}

	if(ErodedMetalTrollTimer < 0){
		ErodedMetalTrollTimer += 30.0f;
		auto e2_nd = enemySpawnLocation1_nd;
		e2_nd.x += 1;
		e2_nd.y -= 1;
		//createEnemyUnit(33, e2_nd);
		e2_nd.x -= 3;
		//createEnemyUnit(34, e2_nd);

		auto e1_nd = enemySpawnLocation1_nd;
		e1_nd.x += 2;
		//createEnemyUnit(35, e1_nd);
	}
}

Point HelloWorld::tileCoordForPosition(Point position) {
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height)
			- position.y) / _tileMap->getTileSize().height;
	//int y = position.y / _tileMap->getTileSize().height;
	return Point(x, y);
}

//Useful function
/*void HelloWorld::addEnemyAtPos(Point pos) {
	auto enemy = Sprite::create("030.png");
	enemy->setPosition(pos);
	enemy->setScale(0.5);
	//this->animateEnemy(enemy);
	this->addChild(enemy);

	_enemies.pushBack(enemy);
	//deleted functions that mess around with these enemies but they weren't too special
}*/

//may be useful to know
void HelloWorld::testCollisions(float dt) {
	Vector<Sprite*> projectilesToDelete;

	for (Sprite *projectile : _projectiles) {
		auto projectileRect = Rect(
				projectile->getPositionX()
						- projectile->getContentSize().width / 2,
				projectile->getPositionY()
						- projectile->getContentSize().height / 2,
				projectile->getContentSize().width,
				projectile->getContentSize().height);

		Vector<Sprite*> targetsToDelete;

		for (Sprite *target : _enemies) {
			auto targetRect = Rect(
					target->getPositionX() - target->getContentSize().width / 2,
					target->getPositionY()
							- target->getContentSize().height / 2,
					target->getContentSize().width,
					target->getContentSize().height);

			if (projectileRect.intersectsRect(targetRect)) {
				targetsToDelete.pushBack(target);
			}
		}

		for (Sprite *target : targetsToDelete) {
			_enemies.eraseObject(target);
			this->removeChild(target);
		}

		if (targetsToDelete.size() > 0) {
			// add the projectile to the list of ones to remove
			projectilesToDelete.pushBack(projectile);
		}
		targetsToDelete.clear();
	}

	for (Sprite *projectile : projectilesToDelete) {
		_projectiles.eraseObject(projectile);
		this->removeChild(projectile);
	}
	projectilesToDelete.clear();
}

/*
 * Initializes the Meta Tile map (blocked coordinates/Enemy spawn areas)
 */
void HelloWorld::setMap() {
	int tw = _tileMap->getTileSize().width;
	int th = _tileMap->getTileSize().height;
	for (int i = tw / 2; i < _tileMap->getMapSize().width * tw; i += tw) {
		for (int j = th / 2; j < _tileMap->getMapSize().height * th; j += th) {
			Point temp;
			temp.x = i;
			temp.y = j;
			Point tileCoord = this->tileCoordForPosition(temp);
			int tileGid = _blockage->getTileGIDAt(tileCoord);

			if (tileGid) {
				auto properties = _tileMap->getPropertiesForGID(tileGid).asValueMap();
				if (!properties.empty()) {
					auto collision = properties["Collidable"].asString();
					auto enemySpawnArea = properties["EnemySpawnArea"].asString();
					if ("True" == collision) {
						//pf->block(tileCoord.x - 1, -1 * (tileCoord.y - 25) + 25 - 1);
						//pf->permaBlock(tileCoord.x - 1, -1 * (tileCoord.y - 25) + 25 - 1);

						pf->block(tileCoord.x, -1 * (tileCoord.y - 25) + 25 - 1);
						pf->permaBlock(tileCoord.x, -1 * (tileCoord.y - 25) + 25 - 1);

						//pf->block(tileCoord.x, tileCoord.y);
						//pf->permaBlock(tileCoord.x, tileCoord.y);
					} else if("True" == enemySpawnArea){
						pf->setEnemySpawnArea(tileCoord.x, -1 * (tileCoord.y - 25) + 25 - 1);
					}
				}
			}
		}
	}
}

/*
 * Draws the Meta tile map - For Debugging purposes
 */
void HelloWorld::checkMap() {
	int tw = _tileMap->getTileSize().width;
	int th = _tileMap->getTileSize().height;
	if (!checked) {
		checked = true;
		for (int i = 0; i < pf->getRows(); i++) {
			for (int j = 0; j < pf->getCols(); j++) {
				if (pf->checkBlock(i, j)) {
					auto drawNode = DrawNode::create();
					drawNode->drawDot(Vec2(pf->getOffX() + i * pf->getTileX(),
							pf->getOffY() + j * pf->getTileY()), 16, Color4F::GREEN);
					this->addChild(drawNode, 1000);
				}
			}
		}
	}
}

/*
 * Draws the BFS Map (for debugging purposes)
 */
void HelloWorld::drawBFSMap() {
	int tw = _tileMap->getTileSize().width;
	int th = _tileMap->getTileSize().height;

	for (int i = 0; i < pf->getRows(); i++) {
		for (int j = 0; j < pf->getCols(); j++) {
			auto t = pf->getBFSParent(i, j);
			//auto t = pf->getBFSDir(i, j);
			cocos2d::Point p;
			p.x = 16 + 32 * i;
			p.y = 16 + 32 * j;

			//CCLOG("%f %f, %f %f", p.x, p.y, t.x, t.y);

			//up
			if(t.x < p.x){
				auto drawNode = DrawNode::create();
				drawNode->drawDot(Vec2(16 + i * 32, 16 + j * 32), 16, Color4F::BLUE);
				this->addChild(drawNode, 1000);
			}
			//down
			else if(t.x > p.x){
				auto drawNode = DrawNode::create();
				drawNode->drawDot(Vec2(16 + i * 32, 16 + j * 32), 16, Color4F::GRAY);
				this->addChild(drawNode, 1000);
			}
			//left
			else if(t.y < p.y){
				auto drawNode = DrawNode::create();
				drawNode->drawDot(Vec2(16 + i * 32, 16 + j * 32), 16, Color4F::GREEN);
				this->addChild(drawNode, 1000);
			}
			//right
			else if(t.y > p.y){
				auto drawNode = DrawNode::create();
				drawNode->drawDot(Vec2(16 + i * 32, 16 + j * 32), 16, Color4F::RED);
				this->addChild(drawNode, 1000);
			}
		}
	}

}

/*
 * Fixes the positions of a coordinate so they align with the tilemap
 * We want things to lie in the center of a tile
 * Eg. If we have a 64 x 32 tile, we want to place a unit at the center of (0,0), meaning (32, 18)
 * Thus, if we are given (46, 29), we round it down to (32, 18)
 */
int HelloWorld::fixPositions(char dim, int val){
	if(dim == 'x'){
		return (pf->getTileX() * ((int) (val / pf->getTileX())) + pf->getOffX());
	} else{
		return (pf->getTileY() * ((int) (val / pf->getTileY())) + pf->getOffY());
	}
}


/*
bool HelloWorldHud::init() {
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	label = LabelTTF::create("0", "fonts/Marker Felt.ttf", 38.0f, Size(50, 50),
			TextHAlignment::RIGHT);
	label->setColor(Color3B(255, 255, 0));
	int margin = 10;
	label->setPosition(
			visibleSize.width / 2 - (label->getDimensions().width / 2) - margin,
			label->getDimensions().height / 2 + margin);
	this->addChild(label);

	return true;
}

void HelloWorldHud::setNinja(){
	cocos2d::ui::Button * button = cocos2d::ui::Button::create("buttons/Button_Normal.png", "buttons/Button_Press.png", "buttons/Button_Disable.png");
	button->setTitleText("Button Text");
	button->setScale(4);

	CCLOG("HERERERERERERE");

	CCLOG("WHAST IS THIS THIS?");
	CCLOG("WHAST IS THIS THIS %p", this);
	CCLOG("WHAST IS THIS THIS %p", button);
	//CCLOG("WHAST IS THIS THIS %p", Level1);

	button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
		switch (type)
		{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				CCLOG("Shots fired");
				this->Level1->ninja->shootFireBall();
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				CCLOG("button clicked");
				break;
			default:
				break;
		}
	});


	this->addChild(button, 10000);
}

void HelloWorldHud::numCollectedChanged(int numCollected) {
	char showStr[20];
	sprintf(showStr, "%d", numCollected);
	label->setString(showStr);
}

*/


void HelloWorld::menuCloseCallback(Ref* pSender) {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
