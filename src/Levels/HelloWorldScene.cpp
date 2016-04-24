#include "src/Levels/HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <AudioEngine.h>

using namespace CocosDenshion;

USING_NS_CC;

HelloWorldHud *HelloWorld::_hud = NULL;

Scene* HelloWorld::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	auto hud = HelloWorldHud::create();
	_hud = hud;

	scene->addChild(hud);

	// return the scene
	return scene;
}

bool HelloWorld::init() {
	if (!Layer::init()) {
		return false;
	}

	auto wframeCache = SpriteFrameCache::getInstance();
	wframeCache->addSpriteFramesWithFile("Units/Troll/Troll.plist");
	wframeCache->addSpriteFramesWithFile("Units/Elf/Elf.plist");
	wframeCache->addSpriteFramesWithFile("Units/Wizard/Wizard.plist");

	//cocos2d::experimental::AudioEngine::setMaxAudioInstance(1);

	/**********************************************************************/
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

	bgImg = cocos2d::Sprite::create("grassland/background cropped.png");
	bgImg->setAnchorPoint(Vec2(0,0));
	this->addChild(bgImg,-2);
	bgImg->setPosition(this->convertToNodeSpace(cocos2d::Point(3.0, -6.0)));

	//auto drawNode = DrawNode::create();
	//drawNode->drawDot(cocos2d::Point(0,0), 16, Color4F::GREEN);
	//bgImg->addChild(drawNode, 1000);
	/**********************************************************************/

	auto tilemapCenter = DrawNode::create();
	tilemapCenter->drawDot(cocos2d::Point(0,0), 16, Color4F::GREEN);
	this->addChild(tilemapCenter, 10);
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

	int enemyX = enemySpawnPoint["x"].asInt();
	int enemyY = enemySpawnPoint["y"].asInt();
	int enemyX2 = enemySpawnPoint2["x"].asInt();
	int enemyY2 = enemySpawnPoint2["y"].asInt();
	int enemyX3 = enemySpawnPoint3["x"].asInt();
	int enemyY3 = enemySpawnPoint3["y"].asInt();

	_plpos.x = x + _tileMap->getTileSize().width / 2;
	_plpos.y = y + _tileMap->getTileSize().height / 2;

	CCLOG("GET PLAYER SHOW UP POINT: %3.3f %3.3f", _plpos.x, _plpos.y);

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
	this->setScale(0.75);

	//create and set up sprites for controlling unit movement
	auto sprite1 = setMovementSprites("Cyansquare.png", Point(x + 16, y + 16));
	auto sprite2 = setMovementSprites("Redsquare.png", Point(x + 16, y + 48));
	auto sprite3 = setMovementSprites("Yellowsquare.png", Point(x + 48, y + 48));

	/*********************************************************************************************/
	//Event Listener: http://www.cocos2d-x.org/wiki/EventDispatcher_Mechanism
	/*********************************************************************************************/

	//Create a "one by one" touch event listener (processes one touch at a time)
	auto listener1 = EventListenerTouchOneByOne::create();
	// When "swallow touches" is true, then returning 'true' from the onTouchBegan method will "swallow" the touch event, preventing other listeners from using it.
	listener1->setSwallowTouches(true);

	// Example of using a lambda expression to implement onTouchBegan event callback function
	listener1->onTouchBegan = [=](Touch* touch, Event* event) {

		// event->getCurrentTarget() returns the *listener's* sceneGraphPriority node.
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		//Get the position of the current point relative to the button
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		//Check the click area
		if (rect.containsPoint(locationInNode))
		{
			CCLOG("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			target->setOpacity(180);
			return true;
		}
		return false;
	};

	//Trigger when moving touch
	listener1->onTouchMoved = [=](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		auto tempScale = this->getScale();
		//Move the position of current button sprite
			target->setPosition(target->getPosition() +
					cocos2d::Point(touch->getDelta().x / tempScale,
					touch->getDelta().y / tempScale));
		};

	//Process the touch end event
	listener1->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		CCLOG("sprite onTouchesEnded.. ");
		target->setOpacity(255);

			lStack->reset();
			auto ppos = _plpos;
			auto tpos = touch->getLocation();
			tpos = convertToNodeSpace(tpos);

			//ppos.x = (ppos.x - pf->getOffX()) / pf->getTileX();
			//ppos.y = (ppos.y - pf->getOffY()) / pf->getTileY();
			//tpos.x = (tpos.x - pf->getOffX()) / pf->getTileX();
			//tpos.y = (tpos.y - pf->getOffY()) / pf->getTileY();

			ppos = ninja->convertToPf(ppos);
			tpos = ninja->convertToPf(tpos);

			if (tpos.x >= 0 && tpos.x < pf->getRows() && tpos.y >= 0 && tpos.y < pf->getCols()) {

				if(target == sprite1){
					for(BasicUnit *i : bvec){
						goToMovementSprite(i, tpos);
					}
				} else if(target == sprite2){
					for(RangedBasicUnit *i : rangedBasicUnitVec){
						goToMovementSprite(i, tpos);
					}
				}
				else{
					goToMovementSprite(ninja, tpos);
				}
			}
		};

	//Add listener
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1,	sprite1);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), sprite2);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), sprite3);

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
	//this->addChild(tileAtrs, 10000);

	auto ninjaPos = _plpos;
	//ninjaPos.x = _plpos.x + pf->getTileX() * 10;
	//ninjaPos.y = _plpos.y  - pf->getTileY();
	ninja = Ninja::create(ninjaPos);
	initUnit(ninja, 0);

	//init enemy following
	auto bfsp = ninja->convertToPf(ninja->getPosition());
	ninja->setBFSmap();
	ninja->BFSInit(bfsp.x, bfsp.y);


	int t1 = 12;
	int t2 = 9;

	for(int i=0; i < t1; i+=2){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX() * 3;
		p.y = _plpos.y  + pf->getTileY() * (i - 13);
		BasicUnit * r = BasicUnit::create(p);
		bvec.pushBack(r);
		initUnit(r, 0);
	}

	for(int i=0; i < t1; i+=2){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX();
		p.y = _plpos.y  + pf->getTileY() * (i - 13);
		RangedBasicUnit * r = RangedBasicUnit::create(p);
		rangedBasicUnitVec.pushBack(r);
		initUnit(r, 0);
	}


	for(int i=0; i < t2; i+=1){
		cocos2d::Point p;
		p.x = enemyX + pf->getTileX() * i;
		p.y = enemyY;
		EnemyBasicUnit * r = EnemyBasicUnit::create(p);
		r->setColor(Color3B::RED);
		bvec2.pushBack(r);
		initUnit(r, 1);
	}


	for(int i=0; i < t2; i+=2){
		cocos2d::Point p;
		p.x = enemyX + pf->getTileX() * i;
		p.y = enemyY - pf->getTileY();
		EnemyBasicUnitRanged * r = EnemyBasicUnitRanged::create(p);
		rangedBasicUnitVec2.pushBack(r);
		r->setColor(Color3B::RED);
		initUnit(r, 1);
	}

	for(int i=0; i < t2; i+=2){
		cocos2d::Point p;
		p.x = enemyX2;
		p.y = enemyY2 + pf->getTileY() * i;
		EnemyBasicUnit * r = EnemyBasicUnit::create(p);
		r->setColor(Color3B::GREEN);
		bvec2.pushBack(r);
		initUnit(r, 1);
	}


	for(int i=0; i < t2; i+=2){
		cocos2d::Point p;
		p.x = enemyX2 + pf->getTileX();
		p.y = enemyY2 + pf->getTileY() * i;
		EnemyBasicUnitRanged * r = EnemyBasicUnitRanged::create(p);
		rangedBasicUnitVec2.pushBack(r);
		r->setColor(Color3B::GREEN);
		initUnit(r, 1);
	}

	for(int i=0; i < t2; i+=2){
		cocos2d::Point p;
		p.x = enemyX3;
		p.y = enemyY3 + pf->getTileY() * i;
		EnemyBasicUnit * r = EnemyBasicUnit::create(p);
		r->setColor(Color3B::GREEN);
		bvec2.pushBack(r);
		initUnit(r, 1);
	}


	for(int i=0; i < t2; i+=2){
		cocos2d::Point p;
		p.x = enemyX3 + pf->getTileX();
		p.y = enemyY3 + pf->getTileY() * i;
		EnemyBasicUnitRanged * r = EnemyBasicUnitRanged::create(p);
		rangedBasicUnitVec2.pushBack(r);
		r->setColor(Color3B::GREEN);
		initUnit(r, 1);
	}

/*
	for(int i=0; i < t1/4; i++){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX() * i;
		p.y = _plpos.y  - pf->getTileY();
		BasicUnit * r = BasicUnit::create(p);
		bvec.pushBack(r);
		initUnit(r, 0);
	}
	for(int i=0; i < t1/4; i++){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX() * i;
		p.y = _plpos.y  - pf->getTileY() * 2;
		RangedBasicUnit * r = RangedBasicUnit::create(p);
		rangedBasicUnitVec.pushBack(r);
		initUnit(r, 0);
	}


	for(int i=0; i < t1; i++){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX() * i;
		p.y = _plpos.y  - pf->getTileY() * 11;
		BasicUnit * r = BasicUnit::create(p);
		bvec.pushBack(r);
		initUnit(r, 0);
	}

	for(int i=0; i < t1; i++){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX() * i;
		p.y = _plpos.y  - pf->getTileY() * 9;
		RangedBasicUnit * r = RangedBasicUnit::create(p);
		rangedBasicUnitVec.pushBack(r);
		initUnit(r, 0);
	}

	for(int i=0; i < t1; i++){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX() * i;
		p.y = _plpos.y  - pf->getTileY() * 8;
		RangedBasicUnit * r = RangedBasicUnit::create(p);
		rangedBasicUnitVec.pushBack(r);
		initUnit(r, 0);
	}

	for(int i=0; i < t2; i+=2){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX() * (i - 10);
		p.y = _plpos.y  - pf->getTileY() * 13;
		EnemyBasicUnit * r = EnemyBasicUnit::create(p);
		r->setColor(Color3B::BLUE);
		bvec2.pushBack(r);
		initUnit(r, 1);
	}

	for(int i=0; i < t2; i+=2){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX() * (i - 10);
		p.y = _plpos.y  - pf->getTileY() * 15;
		EnemyBasicUnit * r = EnemyBasicUnit::create(p);
		r->setColor(Color3B::BLUE);
		bvec2.pushBack(r);
		initUnit(r, 1);
	}


	for(int i=0; i < t2; i+=2){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX() * (i - 10);
		p.y = _plpos.y  - pf->getTileY() * 17;
		EnemyBasicUnitRanged * r = EnemyBasicUnitRanged::create(p);
		rangedBasicUnitVec2.pushBack(r);
		r->setColor(Color3B::BLUE);
		initUnit(r, 1);
	}

	for(int i=0; i < t2; i+=2){
		auto p = _plpos;
		p.x = _plpos.x + pf->getTileX() * (i - 10);
		p.y = _plpos.y  - pf->getTileY() * 19;
		EnemyBasicUnitRanged * r = EnemyBasicUnitRanged::create(p);
		rangedBasicUnitVec2.pushBack(r);
		r->setColor(Color3B::BLUE);
		initUnit(r, 1);
	}
	*/

	/*
	auto p = _plpos;
	p.x = pf->getTileX();
	p.y = _plpos.y  - pf->getTileY() * 18;
	EnemyBasicUnitRanged * r = EnemyBasicUnitRanged::create(p);
	rangedBasicUnitVec2.pushBack(r);
	r->setColor(Color3B::GREEN);
	initUnit(r, 1);
	//r->tracked = true;

	 */
	//this->drawBFSMap();

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
	i->goalPosition.x = tpos.x;
	i->goalPosition.y = tpos.y;

	auto tt = tpos;
	tt.x = tt.x * pf->getTileX() + pf->getOffX();
	tt.y = tt.y * pf->getTileY() + pf->getOffY();

	//For debugging purposes
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
	if(team == 0){
		auto pos = r->convertToPf(r->getPosition());
		r->goalPosition.x = pos.x;
		r->goalPosition.y = pos.y;
	}
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
	if (numTouch == 1) {
		moveDiff.x = 0;
		moveDiff.y = 0;
		bool move1 = false;
		bool move2 = false;
		touchMovedPoint = touch->getLocation();
		auto delta = touchMovedPoint - touchBeganPoint;
		auto currPos = this->getPosition();

		int s = 1.5;
		delta.x = s * delta.x;
		delta.y = s * delta.y;

		auto newPos = currPos + delta;

		touchBeganPoint = touchMovedPoint;

		this->setPosition(newPos);
	} else if (numTouch == 2) {
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
			this->setScale(this->getScale() * tdiff);

		}
		moveDiff = cdiff;
	}
}

void HelloWorld::enemyDistances(float dt){
	cocos2d::Vector<BasicUnit *> team1;
	cocos2d::Vector<BasicUnit *> team2;

	if(!ninja->isDead()) {
		team1.pushBack(ninja);
	}

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

void HelloWorld::update(float dt) {
	elapsedTime += dt;
	testCollisions(dt);
	enemyDistances(dt);
}

Point HelloWorld::tileCoordForPosition(Point position) {
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height)
			- position.y) / _tileMap->getTileSize().height;
	//int y = position.y / _tileMap->getTileSize().height;
	return Point(x, y);
}

bool HelloWorldHud::init() {
	if (!Layer::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	label = LabelTTF::create("0", "fonts/Marker Felt.ttf", 18.0f, Size(50, 20),
			TextHAlignment::RIGHT);
	label->setColor(Color3B(0, 0, 0));
	int margin = 10;
	label->setPosition(
			visibleSize.width - (label->getDimensions().width / 2) - margin,
			label->getDimensions().height / 2 + margin);
	this->addChild(label);

	return true;
}

void HelloWorldHud::numCollectedChanged(int numCollected) {
	char showStr[20];
	sprintf(showStr, "%d", numCollected);
	label->setString(showStr);
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
					if ("True" == collision) {
						//pf->block(tileCoord.x - 1, -1 * (tileCoord.y - 25) + 25 - 1);
						//pf->permaBlock(tileCoord.x - 1, -1 * (tileCoord.y - 25) + 25 - 1);

						pf->block(tileCoord.x, -1 * (tileCoord.y - 25) + 25 - 1);
						pf->permaBlock(tileCoord.x, -1 * (tileCoord.y - 25) + 25 - 1);

						//pf->block(tileCoord.x, tileCoord.y);
						//pf->permaBlock(tileCoord.x, tileCoord.y);
					}
				}
			}
		}
	}
}

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

void HelloWorld::menuCloseCallback(Ref* pSender) {
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
