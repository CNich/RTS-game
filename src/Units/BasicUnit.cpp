#include "src/Units/BasicUnit.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/Utilities/GlobalVariables.h"
#include "math.h"
#include "src/AttackObjects/AttackObject.h"
#include "src/AttackObjects/RangedAttackObject.h"
#include "src/Utilities/GlobalVariables.h"

//#include <AudioEngine.h>

using namespace cocos2d::experimental;

USING_NS_CC;

BasicUnit::BasicUnit() {
}

BasicUnit::~BasicUnit() {
	CCLOG("BasicUnit WAS DELETED!!!!!!!!!!");
}

void BasicUnit::removeUnit() {
	delete tpf;
	delete lStack;
}

BasicUnit* BasicUnit::create() {
	BasicUnit* pSprite = new BasicUnit();
	//pSprite->initWithFile("029.png");
	auto *filename = __String::createWithFormat("TrollWalk00000.png");
	auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
	pSprite->initWithSpriteFrame(wframe);
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	//pSprite->setScale(0.5);
    pSprite->setScale(1.5);
	pSprite->setPfSize(2,2);
	pSprite->setAnchorPoint({0.3, 0.3});

	pSprite->scheduleUpdate();

	return pSprite;

}

BasicUnit* BasicUnit::create(cocos2d::Point tmp){
	BasicUnit* pSprite = create();
	pSprite->setPosition(tmp);
	return pSprite;
}

void BasicUnit::setPf(PathFinder<BasicUnit> *tempPf){
	tpf = new PathFinder<BasicUnit>(tempPf->getRows(), tempPf->getCols());
	tpf->setTileX(tempPf->getTileX());
	tpf->setTileY(tempPf->getTileY());

	pf = tempPf;
	auto tmp = this->getPosition();

	pf->block(convertToPf(tmp).x, convertToPf(tmp).y);
	pf->taken(convertToPf(tmp).x, convertToPf(tmp).y);
	pf->setUnit(convertToPf(tmp).x, convertToPf(tmp).y, this);

};

/*BasicUnit* BasicUnit::create(cocos2d::Point tmp) {
	BasicUnit* pSprite = new BasicUnit();

	if (pSprite->initWithSpriteFrameName("029.png")) {
		pSprite->autorelease();

		pSprite->setStartingPosCustom(tmp);

		return pSprite;

	}

	CC_SAFE_DELETE(pSprite);
	return NULL;
}*/

void BasicUnit::setBlockageMap(cocos2d::TMXLayer *mp) {
	_blockage = mp;
}

void BasicUnit::setForegroundMap(cocos2d::TMXLayer *mp) {
	_foreground = mp;
}

void BasicUnit::setTileMap(cocos2d::TMXTiledMap *mp) {
	_tileMap = mp;
}

void BasicUnit::setStartingPos() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//startingPos = Point(visibleSize.width / 4 * 3 + origin.x, visibleSize.height / 4 + origin.y + rand() % 150);
	startingPos = Point(visibleSize.width / 4 * 3 + origin.x,
			rand() % (int) visibleSize.height);
}

void BasicUnit::setStartingPosCustom(cocos2d::Point tmp) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	startingPos = tmp;
}

void BasicUnit::ASolve(int x, int y) {

	lStack->reset();

	auto ppos = this->convertToPf(this->getPosition());

	if (!pf->checkEnemySpawnArea(x, y) && x >= 0 && x < pf->getRows() && y >= 0 && y < pf->getCols()){// && !pf->checkBlock(x, y)) {

        //if(debug_pathFinder) {
        //    cPrint("------before Asolving------");
        //    tpf->print();
        //    cPrint("---------------------------");
        //}

		tpf->setStart(ppos.x, ppos.y);
		tpf->setEnd(x, y);
		tpf->setUnitSize(pfSize);
		tpf->setMapOwner(this);
		this->getMap();
        if(debug_pathFinder) {
            cPrint("------get map------");
            tpf->print();
            cPrint("---------------------------");
            //tpf->debug_pathFinder = true;
        }
		tpf->setTileX(pf->getTileX());
		tpf->setTileY(pf->getTileY());
		tpf->setOffX(pf->getOffX());
		tpf->setOffY(pf->getOffY());

		lStack = tpf->solve();

		//if(consoleTrack) CCLOG("ASolve: xy: %d, %d, goalPosition: %3.3f, %3.3f", x, y, goalPosition.x, goalPosition.y);
		consoleDebugStatement(cocos2d::__String::createWithFormat(
				"ASolve: %3d xy: %d, %d", lStack->getLength(), x, y));
		//if(consoleTrack) CCLOG("ASolve: %2d lStack->getLength()", lStack->getLength());

		/*
		PathFinder<BasicUnit> tpf2 = PathFinder<BasicUnit>(50, 50);
		tpf2.setTileX(32);
		tpf2.setTileY(32);
		tpf2.setStart(ppos.x, ppos.y);
		tpf2.setEnd(x, y);
		this->getMap(&tpf2);
		tpf2.setTileX(32);
		tpf2.setTileY(32);
		tpf2.setOffX(16);
		tpf2.setOffY(16);

		lStack = tpf2.solve();
		*/

        if(debug_pathFinder) {
            cPrint("------after Asolving------");
            tpf->print();
            cPrint("---------------------------");
        }
	}
}

void BasicUnit::getMap() {
	int w = 50;
	int h = 50;

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			if (pf->checkBlock(i, j)) {
				tpf->block(i, j);
			} else if(pf->checkEnemySpawnArea(i, j)){
				tpf->setEnemySpawnArea(i, j);
			}
			tpf->setUnit(i, j, pf->getUnit(i, j));
		}
	}
}

void BasicUnit::getMap(PathFinder<BasicUnit> *tpf2) {
	int w = 50;
	int h = 50;

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			if (pf->checkBlock(i, j)) {
				tpf2->block(i, j);
			}
		}
	}
}

void BasicUnit::initHealthBar(){
	greenHealth = cocos2d::Sprite::create("healthBars/GreenHealthBar.png");
	greenHealth->setAnchorPoint({0,0});
	this->addChild(greenHealth, GREEN_HEALTH_Z_ORDER);
	auto thisSize = this->getBoundingBox().size;
	greenHealth->setPosition({(thisSize.width - greenHealth->getBoundingBox().size.width) / 2,
		thisSize.height - greenHealth->getBoundingBox().size.height});

	redHealth = cocos2d::Sprite::create("healthBars/RedHealthBar.png");
	redHealth->setAnchorPoint({0,0});
	this->addChild(redHealth, RED_HEALTH_Z_ORDER);
	redHealth->setPosition({(thisSize.width - redHealth->getBoundingBox().size.width) / 2,
		thisSize.height - redHealth->getBoundingBox().size.height});

	healthBarOutline = cocos2d::Sprite::create("healthBars/Outline.png");
	healthBarOutline->setAnchorPoint({0,0});
	this->addChild(healthBarOutline, RED_HEALTH_Z_ORDER - 1);
	auto op = redHealth->getPosition();

	float dx = healthBarOutline->getContentSize().width - redHealth->getContentSize().width;
	float dy = healthBarOutline->getContentSize().height - redHealth->getContentSize().height;

	healthBarOutline->setPosition({op.x - dx / 2, op.y - dy / 2});
}

void BasicUnit::adjustHealthBarSize(){
    auto thisSize = this->getBoundingBox().size;

    greenHealth->setPosition({xHealthPos * (thisSize.width - greenHealth->getBoundingBox().size.width) / 2,
		yHealthPos * (thisSize.height - greenHealth->getBoundingBox().size.height)});

	redHealth->setPosition({xHealthPos * (thisSize.width - redHealth->getBoundingBox().size.width) / 2,
		yHealthPos * (thisSize.height - redHealth->getBoundingBox().size.height)});

	auto op = redHealth->getPosition();

	float dx = healthBarOutline->getContentSize().width - redHealth->getContentSize().width;
	float dy = healthBarOutline->getContentSize().height - redHealth->getContentSize().height;

	healthBarOutline->setPosition({op.x - dx / 2, op.y - dy / 2});
}

void BasicUnit::setHealthBar(){
	if(health > 0){
		float hpercent = (float)health / (float)initHealth;
		greenHealth->setScaleX(hpercent);
	} else{
		greenHealth->setVisible(false);
	}
}

void BasicUnit::delayedMove() {
	if (!lStack->empty() && !moving) {
		consoleDebugStatement(cocos2d::__String::createWithFormat(
				"delayedMove working %d %d", lStack->empty(), moving));
		auto actionTo1 = RotateTo::create(0, 0, 180);
		auto actionTo2 = RotateTo::create(0, 0, 0);

		Point touchLocation;
		if (!lStack->get(0)->empty) {
			touchLocation = lStack->get(0)->data;
		} else {
			touchLocation = lStack->get(1)->data;
		}
		lStack->removeFront();

		/*
		 * Very useful when debugging paths!
		 */

		//auto removeDrawNode = CallFunc::create([this](DrawNode* drawNode) {
		//	this-getParent()->removeChild(drawNode);
		//});
		/*
		auto drawNode = DrawNode::create();
		drawNode->drawDot(touchLocation, 16, Color4F::BLUE);
		this->getParent()->addChild(drawNode, 500);
		auto drawNode2 = DrawNode::create();
		drawNode2->drawDot(this->getPosition(), 16, Color4F::RED);
		this->getParent()->addChild(drawNode2, 500);
		*/
		//auto seq = Sequence::create(DelayTime::create(1.5), removeDrawNode(drawNode), nullptr);


		auto playerPos = this->getPosition();
		auto diff = touchLocation - playerPos;

		setUnitDir(getAngle(this->getPosition(), touchLocation));
		if(unitDir == 0){
			playerPos.y -= pf->getTileY();
		} else if(unitDir == 1){
			playerPos.x += pf->getTileX();
			playerPos.y -= pf->getTileY();
		} else if(unitDir == 2){
			playerPos.x += pf->getTileX();
		} else if(unitDir == 3){
			playerPos.x += pf->getTileX();
			playerPos.y += pf->getTileY();
		} else if(unitDir == 4){
			playerPos.y += pf->getTileY();
		} else if(unitDir == 5){
			playerPos.x -= pf->getTileX();
			playerPos.y += pf->getTileY();
		} else if(unitDir == 6){
			playerPos.x -= pf->getTileX();
		} else if(unitDir == 7){
			playerPos.x -= pf->getTileX();
			playerPos.y -= pf->getTileY();
		}
		//CCLOG("unitDir: %d", unitDir);
		//CCLOG("diff.x = %3.3f, diff.y = %3.3f", diff.x, diff.y);



		if (playerPos.x < (pf->getRows() * pf->getTileX())
				&& playerPos.y < (pf->getCols() * pf->getTileY())
				&& playerPos.y >= 0 && playerPos.x >= 0) {
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("step.mp3");

			/*
			 * FIX THIS TOMORROW IE NOW
			 */
			if(abs(diff.x) >= 1*pf->getTileX() || abs(diff.y) >= 1*pf->getTileY()){
				setUnitDir(getAngle(this->getPosition(), playerPos));
				this->setPlayerPosition(playerPos, abs(diff.x) > 0 && abs(diff.y) > 0);
			} else{
				setUnitDir(getAngle(this->getPosition(), touchLocation));
				this->setPlayerPosition(touchLocation, abs(diff.x) > 0 && abs(diff.y) > 0);
			}

			//this->setPlayerPosition(touchLocation, abs(diff.x) > 0 && abs(diff.y) > 0);
		}
	} else{
		consoleDebugStatement(cocos2d::__String::createWithFormat(
						"delayedMove not working %d %d", lStack->empty(), moving));
	}
}

/*
 * Get the counter clockwise angle between two points, with 0 degrees being down
 * ~~~~~Subtract 90 to normalize to x~~~~~
 *
 * remember the NODE coordinate system (tilemap has y inverted?)
 *  _
 * / \  y
 * 	|
 * 	|
 * 	 ------> x
 */
float BasicUnit::getAngle(cocos2d::Point a, cocos2d::Point b){
	cocos2d::Point centered;

	//centered.x = b.x - a.x;
	//centered.y = b.y - a.y;
	//rotate 90 degrees clockwise

	if(a.x == b.x && a.y == b.y){
		return 0;
	} else{
		//get point b wrt point a and rotate by 90 degrees
		centered.x = -(b.y - a.y);
		centered.y = b.x - a.x;

		//get the hypotenuse
		float hyp = sqrt(pow(centered.x, 2) + pow(centered.y, 2));

		//calculate the angle
		float ang = acos(centered.x / hyp) * 180 / PI;

		//if y is negative, flip the angle + 180
		if(centered.y < 0){
			ang = 360 - ang;
		}
		//CCLOG("(%3.3f,%3.3f) (%3.3f,%3.3f) angle: %f",a.x, a.y, b.x, b.y, ang);
		return ang;
	}
}

/*
 * Set the unit's direction for it's sprite based on the angle it is traveling
 */
void BasicUnit::setUnitDir(float ang){
	unitAngle = ang;
	consoleDebugStatement(cocos2d::__String::createWithFormat("angle: %3.3f", ang));
	float diff = 360;
	int tmp = 0;
	for(int i = 0; i < 8; i++){
		if(abs(ang - i * 45) < diff){
			diff = abs(ang - i * 45);
			tmp = i;
		}
	}
	if(tmp == unitDir){
		walkingAnimationFlag = false;
	} else{
		walkingAnimationFlag = true;
		unitDir = tmp;
	}
	//CCLOG("angle: %3.0f, unitDir: %d", ang, unitDir);
	if(ang > 360){
		unitDir = 0;
		CCLOG("How is the angle > 360?");
	}
}

/*
 * Set the walking animation
 */
cocos2d::Animate* BasicUnit::animationWalk(){
	if(unitDir <=8){
		Vector<SpriteFrame *> walkFrames;

		//length of animation sequence
		float animationLength = 6.0f;

		for (int i = 0; i <= animationLength; i++){
			auto *filename = __String::createWithFormat("TrollWalk%d000%d.png", unitDir, i);
			auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
			walkFrames.pushBack(wframe);
		}
		float duration =  walkingSpeed / animationLength;
		if(unitDir % 2 == 1) duration *= 1.4;
		auto walkAnim = Animation::createWithSpriteFrames(walkFrames, duration);
		auto animate = Animate::create(walkAnim);
		//auto waction = RepeatForever::create(animate);
		//this->runAction(animate);
		return animate;
	} else{
		return nullptr;
	}
}

/*
* Move the unit to it's new position
*/
void BasicUnit::setPlayerPosition(Point position, bool diag) {
	int x = convertToPf(position).x;
	int y = convertToPf(position).y;

	//if (!pf->checkBlock(x, y)) {
	if (!checkUnitSizeBlocked(x, y)) {
		this->setZOrder(pf->getRows() - y);
		blockedCount = 0;
		giveup = false;

		pfLocationUpdate(position);

		CCFiniteTimeAction* actionMove;

		//auto drawNode = DrawNode::create();

		if(diag){
			actionMove = CCMoveTo::create(walkingSpeed * 1.4, position);
			//drawNode->drawDot(position, 16, Color4F::RED);
			//this->getParent()->addChild(drawNode, 1000);
		} else{
			actionMove = CCMoveTo::create(walkingSpeed, position);
			//drawNode->drawDot(position, 16, Color4F::RED);
			//this->getParent()->addChild(drawNode, 1000);
		}
		auto callback = CallFunc::create([this]() {
			this->setMoving();
		});

		moving = true;

		auto seq = Sequence::create(actionMove, callback, nullptr);
		this->runAction(seq);
		this->runAction(this->animationWalk());
		auto tstr = cocos2d::__String::createWithFormat("current: %f, %f", this->getPosition().x, this->getPosition().y);
		auto tstr2 = cocos2d::__String::createWithFormat("next p : %f, %f", position.x, position.y);

		auto tstr3 = cocos2d::__String::createWithFormat("diff p : %f, %f", this->getPosition().x - position.x, this->getPosition().y - position.y);
		auto tstr4 = cocos2d::__String::createWithFormat("mod  p : %f, %f", position.x / pf->getTileX(), position.y / pf->getTileY());

		/*if(tracked) cPrint("%s", tstr->getCString());
		if(tracked) cPrint("%s", tstr2->getCString());
		if(tracked) cPrint("%s", tstr3->getCString());
		if(tracked) cPrint("%s", tstr4->getCString());
		*/
		/*if(walkingAnimationFlag){
			this->runAction(seq);
			auto waction = RepeatForever::create(this->animationWalk());
			this->runAction(waction);
		} else{
			this->runAction(seq);
		}*/



		//auto spawn = cocos2d::Spawn::create(seq, this->animationWalk(), nullptr);
		//this->runAction(spawn);



		//if(numSounds <= 10){
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("step.mp3");
			//auto id = CocosDenshion::AudioEngine::play2d(_files[index], false, 1.0f, &_audioProfile);

            //WATCH OUT FOR THIS
			numSounds++;

			//auto id = cocos2d::experimental::AudioEngineImpl::play2d("step.mp3");

			//auto soundId = cocos2d::experimental::AudioEngine::play2d("step.mp3");



			//cocos2d::experimental::AudioEngine::setFinishCallback(id, f_add_display);
			//cocos2d::experimental::AudioEngine::setFinishCallback

			//cocos2d::experimental::AudioEngine::setMaxAudioInstance(1);


			//cocos2d::experimental::AudioEngine::setFinishCallback(soundId, [&](int id, const std::string& filePath){
			//	decNumSounds();
			//});
		//}

	} else{
		blockedCount++;
		if(blockedCount < 3){
			moving = true;
			auto callback = CallFunc::create([this]() {
							this->moving = false;
						});
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("error.mp3");
			auto seq = Sequence::create(DelayTime::create(0.5), callback, nullptr);
			this->runAction(seq);
			lStack->addFront(position);
		} else if(giveup == false){
			//if(team == 0) CCLOG("giveup: 0, %d %d", moving, lStack->empty());
			//if(team == 1) CCLOG("giveup: 1, %d %d", moving, lStack->empty());
			giveup = true;
			blockedCount = 0;
			setBasicUnitPF();
		}
	}
}

/*
* Update unit location on the map
*/
void BasicUnit::pfLocationUpdate(cocos2d::Point position){
    for(int i = 0; i < pfSize.x; i++){
        for(int j = 0; j < pfSize.y; j++){
            pf->unblock(convertToPf(this->getPosition()).x + i,	convertToPf(this->getPosition()).y + j);
            pf->block(convertToPf(position).x + i, convertToPf(position).y + j);

            pf->untaken(convertToPf(this->getPosition()).x + i,	convertToPf(this->getPosition()).y + j);
            pf->taken(convertToPf(position).x + i, convertToPf(position).y + j);

            pf->setUnitZero(convertToPf(this->getPosition()).x + i,	convertToPf(this->getPosition()).y + j);
            pf->setUnit(convertToPf(position).x + i, convertToPf(position).y + j, this);
        }
    }

    /* OLD
    pf->block(convertToPf(position).x, convertToPf(position).y);
    pf->unblock(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);

    pf->taken(convertToPf(position).x, convertToPf(position).y);
    pf->untaken(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);

    pf->setUnit(convertToPf(position).x, convertToPf(position).y, this);
    pf->setUnitZero(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);
    */
}

/*
* Check if a unit can fit in a certain pf coordinate
*/
bool BasicUnit::checkUnitSizeBlocked(int x_pf, int y_pf){
    for(int i = 0; i < pfSize.x; i++){
        for(int j = 0; j < pfSize.y; j++){
            if( (pf->getUnit(x_pf + i, y_pf + j) != nullptr && pf->getUnit(x_pf + i, y_pf + j) != this) || pf->checkPermaBlock(x_pf + i, y_pf + j) ){
                return true;
            }
        }
    }
    return false;
}

void BasicUnit::setBasicUnitPF(){
	lStack->reset();
	auto ppos = this->convertToPf(this->getPosition());

	cocos2d::Point tpos;
	//tpos = team = 0 ? this->convertToPf(goalPosition) : this->convertToPf(lStack->getTail()->data);
	if(team == 0){
		tpos = this->convertToPf(goalPosition);
	} else{
		//auto tpos = this->convertToPf(lStack->getTail()->data);
		consoleDebugStatement(cocos2d::__String::createWithFormat(
				"giveup: %d %d %d", lStack->empty(), moving, attacking));
	}

	if (tpos.x >= 0 && tpos.x < pf->getRows() && tpos.y >= 0 && tpos.y < pf->getCols() && team == 0) {
		tpf->setStart(ppos.x, ppos.y);
		tpf->setEnd(goalPosition.x, goalPosition.y);
		this->getMap();
		tpf->setTileX(pf->getTileX());
		tpf->setTileY(pf->getTileY());
		tpf->setOffX(pf->getOffX());
		tpf->setOffY(pf->getOffY());

		lStack = tpf->solve();
		if(consoleTrack) CCLOG("setBasicUnitPf!!!");
	}
}

void BasicUnit::setMoving() {
	moving = false;
}

cocos2d::Point BasicUnit::convertToPf(cocos2d::Point tmp) {
	cocos2d::Point ppos;
	ppos.x = (tmp.x - this->pf->getOffX()) / this->pf->getTileX();
	ppos.y = (tmp.y - this->pf->getOffY()) / this->pf->getTileY();
	return ppos;
}

Point BasicUnit::tileCoordForPosition(Point position) {
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height)
			- position.y) / _tileMap->getTileSize().height;
	return Point(x, y);
}

/*
* Set how large the unit is on the map
*/
void BasicUnit::setPfSize(int x, int y){
    pfSize.x = x;
    pfSize.y = y;
}

/*
* Set how large the unit is on the map
*/
void BasicUnit::setPfSize(cocos2d::Point p){
    pfSize = p;
}

void BasicUnit::update(float dt) {
	cocos2d::Point currentEL;

	//check if closest enemy either moved or has changed
	if(currentEnemy != 0){
		currentEL = convertToPf(currentEnemy->getPosition());
		if(currentEnemyLocation.x != currentEL.x || currentEnemyLocation.y != currentEL.y){
			currentEnemyLocation = currentEL;
			currentEnemyMoved = true;
		}
	}

	//check if dead
	if(!dead && this->getHealth() <= 0){
		dead = true;
		this->stopAllActions();
		this->animationDie();
	}

	//if dead, die
	if(dead){
		//figure out how to delete
		//delete this;

		//delete tpf;
		//delete lStack;
		if(removeFromPf){
			auto callback = CallFunc::create([this]() {
				delete tpf;
				delete lStack;

				auto die_pf = convertToPf(this->getPosition());
				cocos2d::Point delete_pf;
				for(int i = -1; i <= 1; i++){
					for(int j = -1; j <= 1; j++){
						cocos2d::Point p2_pf = {die_pf.x + i, die_pf.y + j};
						if(pf->checkBounds_Pf(p2_pf) && pf->getUnit(p2_pf.x, p2_pf.y) == this){
							delete_pf = p2_pf;
						}
					}
				}

				for(int i = -pfSize.x; i < pfSize.x; i++){
                    for(int j = -pfSize.y; j < pfSize.y; j++){
                        if(pf->getUnit(delete_pf.x + i, delete_pf.y + j) == this){
                            pf->unblock(delete_pf.x + i, delete_pf.y + j);
                            pf->untaken(delete_pf.x + i, delete_pf.y + j);
                            pf->setUnitZero(delete_pf.x + i, delete_pf.y + j);
                        }
                    }
				}

				//this->getParent()->removeChild(this);
				this->removeFromLevel();
			});
			auto seq = Sequence::create(DelayTime::create(5), callback, nullptr);
			this->runAction(seq);
			removeFromPf = false;
		}
	}

	//if attacking, nothing should be done
	else if (!attacking){

		//if an enemy is within striking distance and the attack cooldown has expired,
		//attack the closest enemy (loaded from the parent scene)
		if(!attacking && currentEnemy != 0 && !currentEnemy->isDead() &&
				this->enemyIsAttackable()){
			attacking = true;
			//currentEnemyMoved = false;

			auto callback = CallFunc::create([this]() {
				attacking = false;
			});
			lStack->reset();
			auto seq = Sequence::create(DelayTime::create(attackSpeed), callback, nullptr);
			this->runAction(seq);

            setUnitDir(getAngle(this->getPosition(), currentEnemy->getPosition()));
			this->animationAttack();
		}

		/*
		else if(currentEnemy != 0 && unitToUnitDistance(this, currentEnemy) >= attackRange){
			currentEnemyIsCloseEnough = false;
			currentEnemyMoved = false;
			CCLOG("drp");
		}*/

		//else if(currentEnemyIsCloseEnough && !currentEnemyMoved && unitToUnitDistance(this, currentEnemy) < attackRange){
		else if(currentEnemy != 0 && currentEnemyMoved && unitToUnitDistance(this, currentEnemy) < movementRange &&
				attackTravelRange >= pointToPointDistance(this->convertToPf(this->getPosition()), goalPosition)){
			auto t = convertToPf(currentEnemy->getPosition());
			auto hack_move = pfSize;
			pfSize = {1, 1};
			this->ASolve(t.x, t.y);
			pfSize = hack_move;
			currentEnemyMoved = false;
			consoleDebugStatement(cocos2d::__String::createWithFormat(
					"move to enemy: %d", this->enemyIsAttackable()));
		}

		//unit's goal position has been updated
		else if(goalPositionAsolve && !moving){
            consoleDebugStatement(cocos2d::__String::createWithFormat("updated goal position"));
			goalPositionAsolve = false;
			if(consoleTrack) {
				consoleCount++;
			}
			//this->ASolve(goalPosition.x, goalPosition.y);
            aSolveHelper(goalPosition.x, goalPosition.y);
		}

		//the unit is in between movements and is not attacking
		//if the movement stack isn't empty and the unit isn't moving
		//i.e. in between moves, move to next location in stack
		else if (!lStack->empty() && !moving) {
			updateDelayedMove();
		}

		/* if idleTrack is false then the unit should not be moving
		 * it should be waiting until the timer is up before trying
		 * to make it's next move*/

		//if the unit thinks it has nowhere to go but isn't at it's proper location
		//ie the movement stack is empty but it's not at its goal position
		//this may happen if there is no path to it's goal location at the time
		//it's pathfinder has been called

		//gets unit to find path to it's goal position
		/* A */
		else if(lStack->empty() && idle == true && idleTrack == true && !moving){
			idle = false;
			badMove = 0;
			consoleDebugStatement(__String::createWithFormat(
					"%2d %2d idle", consoleCount, lStack->getLength()));
			//this->ASolve(goalPosition.x, goalPosition.y);
			//auto hack_move = pfSize;
			//pfSize = {1, 1};
			//aSolveHelper(goalPosition.x, goalPosition.y);
			//pfSize = hack_move;
			goalPositionAsolve = true;

		}

		//movedYet, tempMoving

		//causes unit to wait for x second until trying *A* again
		/* B */
		else if(lStack->empty() && movedYet == true && idle == false && idleTrack == true && badMove < 3){
			badMove++;
			/* makes sure *A* is not going while*/
			idleTrack = false;
			auto callback = CallFunc::create([this]() {
				idle = true;
				idleTrack = true;
			});
			auto seq = Sequence::create(DelayTime::create(blockDelay), callback, nullptr);
			this->runAction(seq);
			consoleDebugStatement(__String::createWithFormat(
					"blocked"));
			//CCLOG("%d", badMove);
			//DelayTime * action = new (std::nothrow) DelayTime();
			//action->initWithDuration(2);
		}
	}

	//basically nothing is going on
	else if(lStack->empty() && !moving && tempMoving){
		badMove = 0;
		if(tempMoving){
			pf->block(convertToPf(this->getPosition()).x, convertToPf(this->getPosition()).y);
			tempMoving = false;
		}
	}
}

void BasicUnit::aSolveHelper(int x_pf, int y_pf){
    auto this_pf = convertToPf(this->getPosition());
    if(pf->getUnit(this_pf.x, this_pf.y) != this && pf->checkBlock(goalPosition.x, goalPosition.y)){
        bool asolveNeighboorFound = false;
        if(debug_decisionTree) cPrint("==============================");
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                if(!pf->checkBlock(goalPosition.x + i, goalPosition.y + j)){
                    if(debug_decisionTree) cPrint("asolve neighboor %d, %d", i, j);
                    this->ASolve(goalPosition.x + i, goalPosition.y + j);
                    asolveNeighboorFound = true;
                    break;
                } else if(!pf->checkBlock(goalPosition.x - i, goalPosition.y - j)){
                    if(debug_decisionTree) cPrint("asolve neighboor %d, %d", -i, -j);
                    this->ASolve(goalPosition.x - i, goalPosition.y - j);
                    asolveNeighboorFound = true;
                    break;
                } else if(!pf->checkBlock(goalPosition.x + i, goalPosition.y - j)){
                    if(debug_decisionTree) cPrint("asolve neighboor %d, %d", i, -j);
                    this->ASolve(goalPosition.x + i, goalPosition.y - j);
                    asolveNeighboorFound = true;
                    break;
                } else if(!pf->checkBlock(goalPosition.x - i, goalPosition.y + j)){
                    if(debug_decisionTree) cPrint("asolve neighboor %d, %d", -i, j);
                    this->ASolve(goalPosition.x - i, goalPosition.y + j);
                    asolveNeighboorFound = true;
                    break;
                }
            }
            if(asolveNeighboorFound) break;
        }
        if(debug_decisionTree) cPrint("==============================");

    } else{
        if(debug_decisionTree) cPrint("asolve neighboor original");
        this->ASolve(goalPosition.x, goalPosition.y);
    }
}

void BasicUnit::removeFromLevel(){
	CCLOG("BasicUnit remove from level");
	this->getParent()->removeChild(this);
}

/*
 * Set dying animation
 */
void BasicUnit::animationDie(){
	if(unitDir <=8){
		Vector<SpriteFrame *> dieFrames;

		//length of animation sequence
		int animationLength = 30;
		int start = 20;

		for (int i = start; i <= animationLength; i++){
			auto *filename = __String::createWithFormat("TrollDie%d00%d.png", unitDir, i);
			auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
			//CCLOG("%s", filename->getCString());
			dieFrames.pushBack(wframe);
		}
		auto dieAnim = Animation::createWithSpriteFrames(dieFrames, dieDuration);
		auto animate = Animate::create(dieAnim);
		//auto waction = RepeatForever::create(animate);
		this->runAction(animate);
	}
}

int BasicUnit::unitToUnitDistance(BasicUnit *p1, BasicUnit *p2){
	if(p1 != 0 && p2 != 0){
		//CCLOG("will this crash? %p %p", p1, p2);
		auto loc1 = p1->convertToPf(p1->getPosition());
		auto loc2 = p2->convertToPf(p2->getPosition());
		cocos2d::Point dist;
		dist.x = abs(loc1.x - loc2.x);
		dist.y = abs(loc1.y - loc2.y);
		//CCLOG("loc1: (%3.3f,%3.3f) loc2: (%3.3f,%3.3f)", loc1.x, loc1.y, loc2.x, loc2.y);
		//CCLOG("not this time    %p %p", p1, p2);
		return dist.x + dist.y;
	} else{
		return 100;
	}
}

void BasicUnit::updateDelayedMove(){
	delayedMove();
	tempMoving = true;
	movedYet = true;
	badMove = 0;
}

int BasicUnit::pointToPointDistance(cocos2d::Point a, cocos2d::Point b){
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

/*
//Melee
bool BasicUnit::enemyIsAttackable(){
	//if(this->getCurrentEnemy() != 0 && attackTravelRange >= pointToPointDistance(this->convertToPf(this->getPosition()), goalPosition)){
	if(this->getCurrentEnemy() != 0){
		auto enemyLoc = convertToPf(currentEnemy->getPosition());
		auto thisLoc = convertToPf(this->getPosition());
		if(abs(enemyLoc.x - thisLoc.x) <= 1 && abs(enemyLoc.y - thisLoc.y) <= 1){
			return true;
		}
	}
	return false;
}
*/

bool BasicUnit::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0 && attackTravelRange >= pointToPointDistance(this->convertToPf(this->getPosition()), goalPosition)){
		auto enemyLoc = convertToPf(currentEnemy->getPosition());
		auto thisLoc = convertToPf(this->getPosition());

		for(int ei = 0; ei < getCurrentEnemy()->getPfSize().x; ei++){
            for(int ej = 0; ej < getCurrentEnemy()->getPfSize().y; ej++){
                for(int i = 0; i < pfSize.x; i++){
                    for(int j = 0; j < pfSize.y; j++){
                        if(abs(enemyLoc.x + ei - thisLoc.x + i) <= pfSize.x && abs(enemyLoc.y + ej - thisLoc.y + j) <= pfSize.y){
                            return true;
                        }
                    }
                }
            }
		}
	}
	return false;
}

//WILL BUILD ON LATER
void BasicUnit::attack(BasicUnit * attacker, int damage, char attackType){
	//if(health > 0) CCLOG("%p's health: %d", this, health);
	health = health - damage;
	setHealthBar();
	//CCLOG("%p WAS ATTACKEDDDD for %d damage", this, damage);
	//if(health >= 0) CCLOG("%p's health after attack: %d", this, health);
}

void BasicUnit::animationAttack(){

	auto attackCallback = CallFunc::create([this]() {
		AttackObject* atk = AttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), attackDamage, 'm', pf);
		atk->initAttack();
	});
	auto attackSeq = Sequence::create(DelayTime::create(numAttackFrames * attackDuration / 2), attackCallback, nullptr);
	this->runAction(attackSeq);

	Vector<SpriteFrame *> trollFrames;
	for (int i = 0; i <= numAttackFrames; i++){
		auto *filename = __String::createWithFormat("TrollAttack%d001%d.png", unitDir, i);
		//CCLOG("%s", filename->getCString());
		auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
		trollFrames.pushBack(wframe);
	}
	auto wrunAnim = Animation::createWithSpriteFrames(trollFrames, attackDuration);
	auto animate = Animate::create(wrunAnim);

	this->runAction(animate);
}

void BasicUnit::returnHealth(int healthTaken, char attackType){
	health += healthTaken;
}

int BasicUnit::getHealth(){
	return this->health;
}


void BasicUnit::consoleDebugStatement(cocos2d::__String * str){
	if(debug_decisionTree){
		cPrint("%s",str->getCString());
	}
}
