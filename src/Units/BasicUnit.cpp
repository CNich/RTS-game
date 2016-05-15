#include "src/Units/BasicUnit.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/Utilities/GlobalVariables.h"
#include "math.h"
#include "src/AttackObjects/AttackObject.h"
#include "src/AttackObjects/RangedAttackObject.h"
#include "src/Utilities/GlobalVariables.h"

#include <AudioEngine.h>

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

	if (x >= 0 && x < pf->getRows() && y >= 0 && y < pf->getCols()){// && !pf->checkBlock(x, y)) {

		tpf->setStart(ppos.x, ppos.y);
		tpf->setEnd(x, y);
		this->getMap();
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
	}
}

void BasicUnit::getMap() {
	int w = 50;
	int h = 50;

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			if (pf->checkBlock(i, j)) {
				tpf->block(i, j);
			}
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
 */
float BasicUnit::getAngle(cocos2d::Point a, cocos2d::Point b){
	cocos2d::Point centered;

	//centered.x = b.x - a.x;
	//centered.y = b.y - a.y;
	//rotate -90 degrees

	if(a.x == b.x && a.y == b.y){
		return 361;
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
		unitDir = 9;
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

void BasicUnit::setPlayerPosition(Point position, bool diag) {
	int x = convertToPf(position).x;
	int y = convertToPf(position).y;

	if (!pf->checkBlock(x, y)) {
		this->setZOrder(pf->getRows() - y);
		blockedCount = 0;
		giveup = false;
		pf->block(convertToPf(position).x, convertToPf(position).y);
		pf->unblock(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);

		pf->taken(convertToPf(position).x, convertToPf(position).y);
		pf->untaken(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);

		pf->setUnit(convertToPf(position).x, convertToPf(position).y, this);
		pf->setUnitZero(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);

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
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("error.mp3");
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
		tpf->setEnd(tpos.x, tpos.y);
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
				pf->unblock(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);
				pf->untaken(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);
				pf->setUnitZero(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);
				//this->getParent()->removeChild(this);
				this->removeFromLevel();
			});
			auto seq = Sequence::create(DelayTime::create(4), callback, nullptr);
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
			this->ASolve(t.x, t.y);
			currentEnemyMoved = false;
			consoleDebugStatement(cocos2d::__String::createWithFormat(
					"move to enemy: %d", this->enemyIsAttackable()));
		}

		//unit's goal position has been updated
		else if(goalPositionAsolve && !moving){
			goalPositionAsolve = false;
			if(consoleTrack) {
				consoleCount++;
				CCLOG("%2d %2d goalPositionAsolve SET!", consoleCount, lStack->getLength());
			}
			this->ASolve(goalPosition.x, goalPosition.y);
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
				//consoleCount++;
			//CCLOG("lStack->empty(): %d   %7.7f", lStack->empty(), dt);
			this->ASolve(goalPosition.x, goalPosition.y);
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

//Melee
bool BasicUnit::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0 && attackTravelRange >= pointToPointDistance(this->convertToPf(this->getPosition()), goalPosition)){
		auto enemyLoc = convertToPf(currentEnemy->getPosition());
		auto thisLoc = convertToPf(this->getPosition());
		if(abs(enemyLoc.x - thisLoc.x) <= 1 && abs(enemyLoc.y - thisLoc.y) <= 1){
			return true;
		}
	}
	return false;
}

//WILL BUILD ON LATER
void BasicUnit::attack(BasicUnit * attacker, int damage, char attackType){
	//if(health > 0) CCLOG("%p's health: %d", this, health);
	health = health - damage;
	//CCLOG("%p WAS ATTACKEDDDD for %d damage", this, damage);
	//if(health >= 0) CCLOG("%p's health after attack: %d", this, health);
}

void BasicUnit::animationAttack(){
	AttackObject* atk = AttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm', pf);
	atk->initAttack();

	Vector<SpriteFrame *> trollFrames;
	for (int i = 0; i <= 8; i++){
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
	if(consoleTrack){
		CCLOG("%s",str->getCString());
	}
}
