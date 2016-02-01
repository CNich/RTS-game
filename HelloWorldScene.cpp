#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

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
	std::string file = "TileMap.tmx";
	auto str =
			String::createWithContentsOfFile(
					FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
	_tileMap = TMXTiledMap::createWithXML(str->getCString(), "");
	_background = _tileMap->layerNamed("Background");

	addChild(_tileMap, -1);

	_blockage = _tileMap->layerNamed("Meta");
	_blockage->setVisible(false);

	TMXObjectGroup *objects = _tileMap->getObjectGroup("Objects");
	CCASSERT(NULL != objects, "'Object-Player' object group not found");
	auto playerShowUpPoint = objects->getObject("PlayerShowUpPoint");
	CCASSERT(!playerShowUpPoint.empty(), "PlayerShowUpPoint object not found");

	int x = playerShowUpPoint["x"].asInt();
	int y = playerShowUpPoint["y"].asInt();

	_plpos.x = x + _tileMap->getTileSize().width / 2;
	_plpos.y = y + _tileMap->getTileSize().height / 2;

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

	/*********************************************************************************************/
	//Event Listener: http://www.cocos2d-x.org/wiki/EventDispatcher_Mechanism
	/*********************************************************************************************/
	auto sprite1 = Sprite::create("Cyansquare.png");
	sprite1->setPosition(Point(x + 16, y + 16));
	sprite1->setScale(0.5);
	addChild(sprite1, 1);

	auto sprite2 = Sprite::create("Redsquare.png");
	sprite2->setPosition(Point(x + 48, y + 16));
	sprite2->setScale(0.5);
	addChild(sprite2, 1);

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

			ppos.x = (ppos.x - 16) / 32;
			ppos.y = (ppos.y - 16) / 32;
			tpos.x = (tpos.x - 16) / 32;
			tpos.y = (tpos.y - 16) / 32;

			if (tpos.x >= 0 && tpos.x < 50 && tpos.y >= 0 && tpos.y < 50) {

				pf->setTileX(32);
				pf->setTileY(32);
				pf->setOffX(16);
				pf->setOffY(16);

				lStack = pf->solve();

				if(target == sprite1){
					for(BasicUnit *i : bvec){
						i->ASolve(tpos.x, tpos.y);
						i->goalPosition.x = tpos.x;
						i->goalPosition.y = tpos.y;
					}
				} else{
					for(BasicUnit *i : bvec2){
						i->ASolve(tpos.x, tpos.y);
						i->goalPosition.x = tpos.x;
						i->goalPosition.y = tpos.y;
					}
				}
			}
		};

	//Add listener
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1,	sprite1);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), sprite2);

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
	SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3");
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1);

	this->scheduleUpdate();

	__String *tempScore = __String::createWithFormat("%i %i, h:%i t:%i", moving,
			lStack->empty(), lStack->Hempty(), lStack->Tempty());

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getWinSize() * this->getScale();
	scoreLabel = Label::createWithTTF(tempScore->getCString(),
			"fonts/Marker Felt.ttf", winSize.height * SCORE_FONT_SIZE);
	//scoreLabel = Label::createWithTTF("hi", "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE);
	scoreLabel->setColor(Color3B::WHITE);
	scoreLabel->setPosition(Point(winSize.width / 2, winSize.height));

	this->addChild(scoreLabel, 10000);

	__String *ts2 = __String::createWithFormat("%2.0f %2.0f, %2.0f %2.0f",
			_tileMap->getMapSize().height, _tileMap->getMapSize().width,
			_tileMap->getTileSize().height, _tileMap->getTileSize().width);

	pf = new PathFinder<BasicUnit>(_tileMap->getMapSize().height,
			_tileMap->getMapSize().width);
	pf->setTileX(_tileMap->getTileSize().height);
	pf->setTileX(_tileMap->getTileSize().width);

	this->setMap();
	cocos2d::Label *tileAtrs = Label::createWithTTF(ts2->getCString(),
			"fonts/Marker Felt.ttf", winSize.height * SCORE_FONT_SIZE);
	tileAtrs->setColor(Color3B::BLUE);
	tileAtrs->setPosition(Point(winSize.width / 2, winSize.height * 0.75));
	this->addChild(tileAtrs, 10000);

	for(int i=0; i< 5; i++){
		auto p = _plpos;
		p.x = _plpos.x + 32 * i;
		p.y = _plpos.y  - 32 * 11;
		BasicUnit * r = BasicUnit::create(p);
		bvec.pushBack(r);
		//r->setPosition(_plpos.x + 32 * i, _plpos.y  - 32 * 15 );
		this->addChild(r, 2);
		r->setBlockageMap(_blockage);
		r->setForegroundMap(_foreground);
		r->setTileMap(_tileMap);
		r->setTeam(0);
		r->setPf(pf);
	}

	for(int i=0; i< 5; i++){
		auto p = _plpos;
		p.x = _plpos.x + 32 * i;
		p.y = _plpos.y  - 32 * 10;
		RangedBasicUnit * r = RangedBasicUnit::create(p);
		bvec.pushBack(r);
		//r->setPosition(_plpos.x + 32 * i, _plpos.y  - 32 * 15 );
		this->addChild(r, 2);
		r->setBlockageMap(_blockage);
		r->setForegroundMap(_foreground);
		r->setTileMap(_tileMap);
		r->setTeam(0);
		r->setPf(pf);
	}

	for(int i=0; i< 5; i++){
		auto p = _plpos;
		p.x = _plpos.x + 32 * i;
		p.y = _plpos.y  - 32 * 15;
		BasicUnit * r = BasicUnit::create(p);
		bvec2.pushBack(r);
		//r->setPosition(_plpos.x + 32 * i, _plpos.y  - 32 * 15 - 32 * 4);
		this->addChild(r, 2);
		r->setBlockageMap(_blockage);
		r->setForegroundMap(_foreground);
		r->setTileMap(_tileMap);
		r->setTeam(1);
		r->setPf(pf);
	}

	for(int i=0; i< 5; i++){
		auto p = _plpos;
		p.x = _plpos.x + 32 * i;
		p.y = _plpos.y  - 32 * 16;
		RangedBasicUnit * r = RangedBasicUnit::create(p);
		bvec2.pushBack(r);
		//r->setPosition(_plpos.x + 32 * i, _plpos.y  - 32 * 15 - 32 * 4);
		this->addChild(r, 2);
		r->setBlockageMap(_blockage);
		r->setForegroundMap(_foreground);
		r->setTileMap(_tileMap);
		r->setTeam(1);
		r->setPf(pf);
	}

	return true;

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
		this->firstTouch = true;
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

void HelloWorld::onTouchesEnded(const std::vector<Touch *> &touches,
		Event *unused_event) {
	numTouch--;
}

void HelloWorld::onTouchesBegan(const std::vector<Touch *> &touches,
		Event *unused_event) {
	numTouch++;
	touchBeganPoint = touches[0]->getLocation();
}


void HelloWorld::setPlayerPosition(Point position) {
	if (!moving) {
		Point tileCoord = this->tileCoordForPosition(position);
		int tileGid = _blockage->getTileGIDAt(tileCoord);

		if (tileGid) {
			auto properties =
					_tileMap->getPropertiesForGID(tileGid).asValueMap();
			if (!properties.empty()) {
				auto collision = properties["Collidable"].asString();
				if ("True" == collision) {
					SimpleAudioEngine::getInstance()->playEffect("error.mp3");
					return;
				}
				auto collectable = properties["Collectable"].asString();
				if ("True" == collectable) {
					_blockage->removeTileAt(tileCoord);
					_foreground->removeTileAt(tileCoord);
					//_player->setPosition(position);
					this->_numCollected++;
					this->_hud->numCollectedChanged(_numCollected);
					SimpleAudioEngine::getInstance()->playEffect("item.mp3");
				}
			}
		}

		CCFiniteTimeAction* actionMove = CCMoveTo::create(0.2, position);

		auto callback = CallFunc::create([this]() {
			this->setMoving();
		});

		moving = true;

		//auto seq = Sequence::create(actionMove, callback, nullptr);
		//_player->runAction(seq);

	}
//_player->setPosition(position);
}

void HelloWorld::setMoving() {
	moving = false;
}

void HelloWorld::enemyDistances(float dt){
	//reset newClosestEnemy
	if(bvec.size() > 0 && bvec2.size() > 0){

		for(BasicUnit * p2 : bvec2){
			p2->setCurrentEnemy(0);
		}

		for(BasicUnit * p : bvec){
			//reset newClosestEnemy
			p->setCurrentEnemy(0);

			if(!p->isDead()){
			//find distances between each per team
				for(BasicUnit * p2 : bvec2){
					//set current enemy to new if no previous current enemy exists
					if(!p2->isDead()){
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
	if (!lStack->empty() && !moving) {
		//delayedMove();
		Point tmp;
		if (!lStack->get(0)->empty) {
			tmp = lStack->get(0)->data;
		} else {
			tmp = lStack->get(1)->data;
		}
	}
	testCollisions(dt);
	enemyDistances(dt);
	__String *tempScore = __String::createWithFormat("%i %i, h:%i t:%i", moving,
			lStack->empty(), lStack->Hempty(), lStack->Tempty());
	scoreLabel->setString(tempScore->getCString());
}

void HelloWorld::addToStack(Point position) {
	lStack->addBack(position);
}

Point HelloWorld::tileCoordForPosition(Point position) {
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height)
			- position.y) / _tileMap->getTileSize().height;
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
	for (int i = th / 2; i < _tileMap->getMapSize().height * th; i += th) {
		for (int j = tw / 2; j < _tileMap->getMapSize().width * th; j += tw) {
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
						pf->block(tileCoord.x - 1, -1 * (tileCoord.y - 25) + 25 - 1);
						pf->permaBlock(tileCoord.x - 1, -1 * (tileCoord.y - 25) + 25 - 1);
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
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				if (pf->checkBlock(i, j)) {
					//auto drawNode = DrawNode::create();
					//drawNode->drawDot(Vec2(16 + i * 32, 16 + j * 32), 16, Color4F::GREEN);
					//this->addChild(drawNode, 1000);
				}
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
