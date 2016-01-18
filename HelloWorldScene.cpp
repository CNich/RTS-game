#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

/* MAKE SCALE VARIABLE
 *
 *
 *
 *
 *
 * */

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
	//auto str = String::createWithContentsOfFile ("C:\\CocosGames\\cocos2d-x-3.6\\tileMapTest\\Resources\\TileMap.tmx");
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
		CCLOG("=================================");
		CCLOG("%d %d %d", numTouch, numTouch, numTouch);
		if(tmcase == 0) {
			Point tmp;
			tmp.x = 750;
			tmp.y = 0;
			CCLOG("top right tmcase: %d, %4.0f, %4.0f", tmcase, tmp.x, tmp.y);
			//this->setPosition(tmp);
			tmcase = 1;
		} else if(tmcase == 1) {
			Point tmp;
			tmp.x = 1500;
			tmp.y = 0;
			CCLOG("top left  tmcase: %d, %4.0f, %4.0f", tmcase, tmp.x, tmp.y);
			//this->setPosition(tmp);
			tmcase = 2;
		} else if(tmcase == 2) {
			Point tmp;
			tmp.x = 1500;
			tmp.y = 750;
			CCLOG("bot left  tmcase: %d, %4.0f, %4.0f", tmcase, tmp.x, tmp.y);
			//this->setPosition(tmp);
			tmcase = 3;
		} else {
			Point tmp;
			tmp.x = 750;
			tmp.y = 750;
			CCLOG("bot right tmcase: %d, %4.0f, %4.0f", tmcase, tmp.x, tmp.y);
			//this->setPosition(tmp);
			tmcase = 0;
		}
		return true;
	};

	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,
			this);

	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesBegan =
			CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	touchListener->onTouchesEnded =
			CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);

	//this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	_player = Sprite::create("Player.png");
	_player->setPosition(x + _tileMap->getTileSize().width / 2,
			y + _tileMap->getTileSize().height / 2);
	_player->setScale(0.5);

	addChild(_player, 2);
	setViewPointCenter(_player->getPosition());

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
			//target->setPosition(convertToNodeSpace(touch->getLocation()));
			//CCLOG("target->getScale(): %5.5f", layerScale);
		};

	//Process the touch end event
	listener1->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		CCLOG("sprite onTouchesEnded.. ");
		//CCLOG("target->getScale(): %5.5f", layerScale);
		target->setOpacity(255);
		//Reset zOrder and the display sequence will change

			lStack->reset();
			auto ppos = _player->getPosition();
			auto tpos = touch->getLocation();
			tpos = convertToNodeSpace(tpos);

			ppos.x = (ppos.x - 16) / 32;
			ppos.y = (ppos.y - 16) / 32;
			tpos.x = (tpos.x - 16) / 32;
			tpos.y = (tpos.y - 16) / 32;

			if (tpos.x >= 0 && tpos.x < 50 && tpos.y >= 0 && tpos.y < 50) {
				//pf->setStart(ppos.x, ppos.y);
				//pf->setEnd(tpos.x, tpos.y);

				pf->setTileX(32);
				pf->setTileY(32);
				pf->setOffX(16);
				pf->setOffY(16);

				//this->setMap();
				//this->checkMap();

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

	for (auto& eSpawnPoint : objects->getObjects()) {
		ValueMap& dict = eSpawnPoint.asValueMap();
		//if(dict["Enemy"].asInt() == 1){
		x = dict["x"].asInt();
		y = dict["y"].asInt();
		this->addEnemyAtPos(Point(x, y));
		//}
	}

	_foreground = _tileMap->getLayer("Foreground");

	SimpleAudioEngine::getInstance()->preloadEffect("error.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("item.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("step.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("wade.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3");
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1);

	//this->schedule(schedule_selector(HelloWorld::testCollisions));
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

	pf = new PathFinder<int>(_tileMap->getMapSize().height,
			_tileMap->getMapSize().width);
	pf->setTileX(_tileMap->getTileSize().height);
	pf->setTileX(_tileMap->getTileSize().width);

	this->setMap();
	cocos2d::Label *tileAtrs = Label::createWithTTF(ts2->getCString(),
			"fonts/Marker Felt.ttf", winSize.height * SCORE_FONT_SIZE);
	tileAtrs->setColor(Color3B::BLUE);
	tileAtrs->setPosition(Point(winSize.width / 2, winSize.height * 0.75));
	this->addChild(tileAtrs, 10000);

	CCLOG("------------------------------------");

	for(int i=0; i< 5; i++){
		BasicUnit * r = BasicUnit::create();
		bvec.pushBack(r);
		r->setPosition(_player->getPosition().x + 32 * i, _player->getPosition().y);
		this->addChild(r, 2);
		r->setBlockageMap(_blockage);
		r->setForegroundMap(_foreground);
		r->setTileMap(_tileMap);
		r->setTeam(0);
	}

	for(int i=0; i< 5; i++){
		BasicUnit * r = BasicUnit::create();
		bvec2.pushBack(r);
		r->setPosition(_player->getPosition().x + 32, _player->getPosition().y + 32 * i);
		this->addChild(r, 2);
		r->setBlockageMap(_blockage);
		r->setForegroundMap(_foreground);
		r->setTileMap(_tileMap);
		r->setTeam(1);
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
	//auto ttouch = this->convertToNodeSpace(touch->getLocation());
	numTouch--;
	if (!firstTouch) {
		//this->setMap();
		CCLOG("first touch of the game................");
		this->firstTouch = true;
	}

	auto ttouch = touch->getLocation();
	/*if (ttouch.x < 500 && ttouch.y < 700 && ttouch.y > 500) {
	 if (_mode == false) {
	 _mode = true;
	 CCLOG("true");
	 } else {
	 _mode = false;
	 CCLOG("false");
	 }
	 } else if (ttouch.x < 500 && ttouch.y >= 700) {
	 this->setScale(this->getScale() + 0.1f);
	 CCLOG("Zoom in");
	 } else if (ttouch.x < 500 && ttouch.y <= 500) {
	 this->setScale(this->getScale() - 0.1f);
	 CCLOG("Zoom out");
	 }*/

	/*
	 CCLOG("touch getLocation: %f %f", touch->getLocation().x, touch->getLocation().y);
	 CCLOG("touch convertToNodeSpace: %f %f", this->convertToNodeSpace(touch->getLocation()).x, this->convertToNodeSpace(touch->getLocation()).y);
	 CCLOG("_topLeft: %5.1f %5.1f, _topRight: %5.1f %5.1f, _bottomLeft: %5.1f %5.1f, _bottomRight: %5.1f %5.1f", _topLeft.x, _topLeft.y,
	 _topRight.x, _topRight.y, _bottomLeft.x, _bottomLeft.y, _bottomRight.x, _bottomRight.y);

	 CCLOG("Player position: %5.1f %5.1f", _player->getPosition().x, _player->getPosition().y);
	 */
	if (_mode == false) {

		/*
		lStack->reset();
		//pf->resetMap();

		//auto ppos = tileCoordForPosition(_player->getPosition());
		//auto tpos = tileCoordForPosition(touch->getLocation());

		auto ppos = _player->getPosition();
		auto tpos = touch->getLocation();
		tpos = convertToNodeSpace(tpos);

		ppos.x = (ppos.x - 16) / 32;
		ppos.y = (ppos.y - 16) / 32;
		tpos.x = (tpos.x - 16) / 32;
		tpos.y = (tpos.y - 16) / 32;

		if (tpos.x >= 0 && tpos.x < 50 && tpos.y >= 0 && tpos.y < 50) {
			pf->setStart(ppos.x, ppos.y);
			pf->setEnd(tpos.x, tpos.y);

			pf->setTileX(32);
			pf->setTileY(32);
			pf->setOffX(16);
			pf->setOffY(16);

			this->setMap();
			this->checkMap();

			lStack = pf->solve();
		}
		*/

		/*
		 * auto temp = _player->getPosition();
		 auto drawNode = DrawNode::create();
		 drawNode->drawDot(temp, 16, Color4F::BLUE);
		 this->addChild(drawNode, 1000);
		 lStack->addBack(touch->getLocation());
		 */

		////////////
		/*
		 auto actionTo1 = RotateTo::create(0, 0, 180);
		 auto actionTo2 = RotateTo::create(0, 0, 0);
		 auto touchLocation = touch->getLocation();

		 touchLocation = this->convertToNodeSpace(touchLocation);

		 auto playerPos = _player->getPosition();
		 auto playerPos2 = _player->getPosition();
		 auto diff = touchLocation - playerPos;
		 if (abs(diff.x) > abs(diff.y)) {
		 if (diff.x > 0) {
		 playerPos.x += _tileMap->getTileSize().width / 2;
		 _player->runAction(actionTo2);
		 }
		 else {
		 playerPos.x -= _tileMap->getTileSize().width / 2;
		 _player->runAction(actionTo1);
		 }
		 }
		 else {
		 if (diff.y > 0) {
		 playerPos.y += _tileMap->getTileSize().height / 2;
		 }
		 else {
		 playerPos.y -= _tileMap->getTileSize().height / 2;
		 }
		 }

		 if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getMapSize().width) &&
		 playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getMapSize().height) &&
		 playerPos.y >= 0 &&
		 playerPos.x >= 0)
		 {
		 SimpleAudioEngine::getInstance()->playEffect("step.mp3");
		 CCLOG("player position move: %5.1f:%5.1f,   %5.1f:%5.1f", playerPos.x, playerPos2.x, playerPos.y, playerPos2.y);
		 this->addToStack(playerPos);
		 }

		 this->setViewPointCenter(_player->getPosition());
		 */
	} else {
		auto touchLocation = touch->getLocation();
		touchLocation = this->convertToNodeSpace(touchLocation);
		auto projectile = Sprite::create("bullet.png");
		projectile->setPosition(_player->getPosition());
		projectile->setScale(0.25);
		this->addChild(projectile);

		int realX;

		auto diff = touchLocation - _player->getPosition();
		if (diff.x > 0) {
			realX = (_tileMap->getMapSize().width
					* _tileMap->getTileSize().width)
					+ (projectile->getContentSize().width / 2);
		} else {
			realX = -(_tileMap->getMapSize().width
					* _tileMap->getTileSize().width)
					- (projectile->getContentSize().width / 2);
		}
		float ratio = (float) diff.y / (float) diff.x;
		int realY = ((realX - projectile->getPosition().x) * ratio)
				+ projectile->getPosition().y;
		auto realDest = Point(realX, realY);

		int offRealX = realX - projectile->getPosition().x;
		int offRealY = realY - projectile->getPosition().y;
		float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));
		float velocity = 480 / 1; // 480pixels/1sec
		float realMoveDuration = length / velocity;

		auto actionMoveDone = CallFuncN::create(
				CC_CALLBACK_1(HelloWorld::projectileMoveFinished, this));
		projectile->runAction(
				Sequence::create(MoveTo::create(realMoveDuration, realDest),
						actionMoveDone, NULL));

		_projectiles.pushBack(projectile);
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
		//auto delta = currPos - touchMovedPoint;

		int s = 1.5;
		delta.x = s * delta.x;
		delta.y = s * delta.y;

		auto newPos = currPos + delta;
		/*
		 CCLOG("---------------------------------");
		 CCLOG("%d %d %d", numTouch, numTouch, numTouch);
		 CCLOG("currPos         : %4.0f %4.0f", currPos.x, currPos.y);
		 CCLOG(
		 "touchBeganPoint : %4.0f %4.0f", touchBeganPoint.x, touchBeganPoint.y);
		 CCLOG(
		 "getStartLocation: %4.0f %4.0f", touch->getStartLocation().x, touch->getStartLocation().y);
		 CCLOG( "touch 1         : %4.0f %4.0f", touchBegan1.x, touchBegan1.y);
		 CCLOG( "touch 2         : %4.0f %4.0f", touchBegan2.x, touchBegan2.y);
		 */
		/*
		 CCLOG("touchMovedPoint: %4.0f %4.0f", touchMovedPoint.x, touchMovedPoint.y);
		 CCLOG("touchBeganPoint: %4.0f %4.0f", touchBeganPoint.x, touchBeganPoint.y);
		 CCLOG("delta          : %4.0f %4.0f", delta.x, delta.y);
		 CCLOG("currPos        : %4.0f %4.0f", currPos.x, currPos.y);
		 CCLOG("newPos         : %4.0f %4.0f", newPos.x, newPos.y);
		 */
		touchBeganPoint = touchMovedPoint;

		//this->setPosition(convertToNodeSpace(newPos));
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
			/*
			 CCLOG("=======================");
			 CCLOG("%4.0f %4.0f", touchMoved1.x, touchMoved1.y);
			 CCLOG("%4.0f %4.0f", touchMoved2.x, touchMoved2.y);
			 CCLOG("%4.0f %4.0f", moveDiff.x, moveDiff.y);
			 CCLOG("scale: %4.4f", tdiff);
			 CCLOG("--------");
			 */
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
	CCLOG("=================================");
	CCLOG("%d     %d %d %d", touches.size(), numTouch, numTouch, numTouch);

	if (numTouch == 2) {
		CCLOG(
				"%4.0f, %4.0f", touches[0]->getLocation().x, touches[0]->getLocation().y);
		CCLOG(
				"%4.0f, %4.0f", touches[1]->getLocation().x, touches[1]->getLocation().y);
		//CCLOG("%4.0f, %4.0f", touches->getLocation().x, touches->getLocation().y);
	}

	if (tmcase == 0) {
		Point tmp;
		tmp.x = 750;
		tmp.y = 0;
		CCLOG("top right tmcase: %d, %4.0f, %4.0f", tmcase, tmp.x, tmp.y);
		//this->setPosition(tmp);
		tmcase = 1;
	} else if (tmcase == 1) {
		Point tmp;
		tmp.x = 1500;
		tmp.y = 0;
		CCLOG("top left  tmcase: %d, %4.0f, %4.0f", tmcase, tmp.x, tmp.y);
		//this->setPosition(tmp);
		tmcase = 2;
	} else if (tmcase == 2) {
		Point tmp;
		tmp.x = 1500;
		tmp.y = 750;
		CCLOG("bot left  tmcase: %d, %4.0f, %4.0f", tmcase, tmp.x, tmp.y);
		//this->setPosition(tmp);
		tmcase = 3;
	} else {
		Point tmp;
		tmp.x = 750;
		tmp.y = 750;
		CCLOG("bot right tmcase: %d, %4.0f, %4.0f", tmcase, tmp.x, tmp.y);
		//this->setPosition(tmp);
		tmcase = 0;
	}
	//return true;
}

void HelloWorld::delayedMove() {
	if (!lStack->empty() && !moving) {
		auto actionTo1 = RotateTo::create(0, 0, 180);
		auto actionTo2 = RotateTo::create(0, 0, 0);

		Point touchLocation;
		if (!lStack->get(0)->empty) {
			touchLocation = lStack->get(0)->data;
		} else {
			touchLocation = lStack->get(1)->data;
			CCLOG("%p %p", lStack->get(1), lStack->getTail());
		}
		//CCLOG("===========");
		CCLOG("MU: %4.0f %4.0f", touchLocation.x, touchLocation.y);
		lStack->removeFront();
		//touchLocation = this->convertToNodeSpace(touchLocation);
		auto playerPos = _player->getPosition();
		auto playerPos2 = _player->getPosition();
		auto diff = touchLocation - playerPos;
		if (abs(diff.x) > abs(diff.y)) {
			if (diff.x > 0) {
				playerPos.x += _tileMap->getTileSize().width;
				_player->runAction(actionTo2);
			} else {
				playerPos.x -= _tileMap->getTileSize().width;
				_player->runAction(actionTo1);
			}
		} else {
			if (diff.y > 0) {
				playerPos.y += _tileMap->getTileSize().height;
			} else {
				playerPos.y -= _tileMap->getTileSize().height;
			}
		}
		if (playerPos.x
				<= (_tileMap->getMapSize().width * _tileMap->getMapSize().width)
				&& playerPos.y
						<= (_tileMap->getMapSize().height
								* _tileMap->getMapSize().height)
				&& playerPos.y >= 0 && playerPos.x >= 0) {
			SimpleAudioEngine::getInstance()->playEffect("step.mp3");

			this->setPlayerPosition(playerPos);
		}
		//this->setViewPointCenter(_player->getPosition());
	}
}

void HelloWorld::setPlayerPosition(Point position) {
	if (!moving) {
		//CCLOG("updating: moving = %d and stack empty = %d", moving, !lStack->get(0)->empty);
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

		auto seq = Sequence::create(actionMove, callback, nullptr);
		_player->runAction(seq);

	}
//_player->setPosition(position);
}

void HelloWorld::setMoving() {
	moving = false;
//CCLOG("Added to back");
//CCLOG("%d", moving);
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
//if (!lStack->get(0)->empty && !moving)
//if (!lStack->get(0)->empty && !lStack->get(1)->empty && !moving)
	if (!lStack->empty() && !moving) {
		delayedMove();
		//CCLOG("%5.0f %5.0f", lStack->get(0)->data.x, lStack->get(0)->data.y);
		//auto drawNode = DrawNode::create();
		Point tmp;
		if (!lStack->get(0)->empty) {
			tmp = lStack->get(0)->data;
		} else {
			tmp = lStack->get(1)->data;
		}

		//drawNode->drawDot(tmp, 16, Color4F::RED);
		//this->addChild(drawNode, 500);
	}
//schedule_selector(HelloWorld::testCollisions);
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

void HelloWorld::addEnemyAtPos(Point pos) {
	auto enemy = Sprite::create("030.png");
	enemy->setPosition(pos);
	enemy->setScale(0.5);
	this->animateEnemy(enemy);
	this->addChild(enemy);

	_enemies.pushBack(enemy);
}

void HelloWorld::enemyMoveFinished(Object *pSender) {
	Sprite *enemy = (Sprite *) pSender;
	this->animateEnemy(enemy);
}

void HelloWorld::animateEnemy(Sprite *enemy) {
	auto actionTo1 = RotateTo::create(0, 0, 180);
	auto actionTo2 = RotateTo::create(0, 0, 0);
	auto diff = ccpSub(_player->getPosition(), enemy->getPosition());

	if (diff.x < 0) {
		enemy->runAction(actionTo2);
	}
	if (diff.x > 0) {
		enemy->runAction(actionTo1);
	}

	float actualDuration = 0.3f;
	auto position = (_player->getPosition() - enemy->getPosition());
	position.x /= 10;
	position.y /= 10;
	auto actionMove = MoveBy::create(actualDuration, position);
	auto actionMoveDone = CallFuncN::create(
			CC_CALLBACK_1(HelloWorld::enemyMoveFinished, this));
	enemy->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
}

void HelloWorld::projectileMoveFinished(Object *pSender) {
	Sprite *sprite = (Sprite *) pSender;
	this->removeChild(sprite);
}

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
