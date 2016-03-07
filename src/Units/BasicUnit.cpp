#include "src/Units/BasicUnit.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/AttackObject.h"
#include "src/AttackObjects/RangedAttackObject.h"
#include "src/Utilities/GlobalVariables.h"

#include <AudioEngine.h>

using namespace cocos2d::experimental;

USING_NS_CC;

BasicUnit::BasicUnit() {
}

BasicUnit::~BasicUnit() {
}

void BasicUnit::removeUnit() {
	delete tpf;
	delete lStack;
}

BasicUnit* BasicUnit::create() {
	BasicUnit* pSprite = new BasicUnit();
	pSprite->initWithFile("029.png");
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	pSprite->tpf = new PathFinder<BasicUnit>(50, 50);
	pSprite->tpf->setTileX(32);
	pSprite->tpf->setTileY(32);
	pSprite->setScale(0.5);

	pSprite->scheduleUpdate();

	return pSprite;

}

BasicUnit* BasicUnit::create(cocos2d::Point tmp){
	BasicUnit* pSprite = create();
	pSprite->setPosition(tmp);
	return pSprite;
}

void BasicUnit::setPf(PathFinder<BasicUnit> *tempPf){
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
	//if(!lStack->empty()){
		lStack->reset();
	//}

	//goalPosition.x = x;
	//goalPosition.y = y;

	auto ppos = this->getPosition();

	ppos.x = (ppos.x - 16) / 32;
	ppos.y = (ppos.y - 16) / 32;

	if (x >= 0 && x < 50 && y >= 0 && y < 50) {

		/*
		 * IS MAKING A NEW PATH FINDER EACH TIME BETTER
		 * OR HAVING A SET ONE AND KEEPING IT ALIVE?
		 *
		 * */


		tpf->setStart(ppos.x, ppos.y);
		tpf->setEnd(x, y);
		this->getMap();
		tpf->setTileX(32);
		tpf->setTileY(32);
		tpf->setOffX(16);
		tpf->setOffY(16);

		lStack = tpf->solve();

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
		auto actionTo1 = RotateTo::create(0, 0, 180);
		auto actionTo2 = RotateTo::create(0, 0, 0);

		Point touchLocation;
		if (!lStack->get(0)->empty) {
			touchLocation = lStack->get(0)->data;
		} else {
			touchLocation = lStack->get(1)->data;
		}
		lStack->removeFront();
		//auto drawNode = DrawNode::create();
		//drawNode->drawDot(touchLocation, 16, Color4F::BLUE);
		//this->getParent()->addChild(drawNode, 1000);

		auto playerPos = this->getPosition();
		auto diff = touchLocation - playerPos;

		//depoending on x movement, change the way the unit faces
		if (diff.x > 0) {
			this->runAction(actionTo2);
		} else {
			this->runAction(actionTo1);
		}

		//move right
		if(abs(abs(diff.x) - abs(diff.y)) > 15){
			if (abs(diff.x) > abs(diff.y)) {
				if (diff.x > 0) {
					playerPos.x += 32;
					//this->runAction(actionTo2);
				} else {
					playerPos.x -= 32;
					//this->runAction(actionTo1);
				}
			} else {
				if (diff.y > 0) {
					playerPos.y += 32;
				} else {
					playerPos.y -= 32;
				}
			}
			/*if(diff.x > 17 && diff.y < 17 && diff.y > -17){
				playerPos.x += 32;
			}
			//move left
			else if(diff.x < -17 && diff.y < 17 && diff.y > -17){
				playerPos.x -= 32;
			}
			//move down
			else if(diff.x < 17 && diff.x > -17 && diff.y > 17){
				playerPos.y += 32;
			}
			//move up
			else if(diff.x < 17 && diff.x > -17 && diff.y < - 17){
				playerPos.y -= 32;
			}*/
		}
		/***************************************/
		else{
			//move right down
			if(diff.x > 17 && diff.y > 17){
				playerPos.x += 32;
				playerPos.y += 32;
			}
			//move left down
			else if(diff.x < -17 && diff.y > 17){
				playerPos.x -= 32;
				playerPos.y += 32;
			}
			//move right up
			if(diff.x > 17 && diff.y < -17){
				playerPos.x += 32;
				playerPos.y -= 32;
			}
			//move left up
			else if(diff.x < -17 && diff.y < -17){
				playerPos.x -= 32;
				playerPos.y -= 32;
			}
		}

		/*
		if (abs(diff.x) > abs(diff.y)) {
			if (diff.x > 0) {
				playerPos.x += 32;
				this->runAction(actionTo2);
			} else {
				playerPos.x -= 32;
				this->runAction(actionTo1);
			}
		} else {
			if (diff.y > 0) {
				playerPos.y += 32;
			} else {
				playerPos.y -= 32;
			}
		}*/


		if (playerPos.x < (50 * 32) && playerPos.y < (50 * 32)
				&& playerPos.y >= 0 && playerPos.x >= 0) {
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("step.mp3");

			if(abs(abs(diff.x) + abs(diff.y)) > 64){
				this->setPlayerPosition(playerPos, abs(diff.x) > 0 && abs(diff.y) > 0);
			} else{
				this->setPlayerPosition(touchLocation, abs(diff.x) > 0 && abs(diff.y) > 0);
			}

			//this->setPlayerPosition(touchLocation, abs(diff.x) > 0 && abs(diff.y) > 0);
		}
	}
}

void BasicUnit::setPlayerPosition(Point position, bool diag) {
	int x = convertToPf(position).x;
	int y = convertToPf(position).y;

	if (!pf->checkBlock(x, y)) {

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
			actionMove = CCMoveTo::create(0.2 * 1.4, position);
			//drawNode->drawDot(position, 16, Color4F::RED);
			//this->getParent()->addChild(drawNode, 1000);
		} else{
			actionMove = CCMoveTo::create(0.2, position);
			//drawNode->drawDot(position, 16, Color4F::RED);
			//this->getParent()->addChild(drawNode, 1000);
		}
		auto callback = CallFunc::create([this]() {
			this->setMoving();
		});

		moving = true;

		auto seq = Sequence::create(actionMove, callback, nullptr);
		this->runAction(seq);
		//if(numSounds <= 10){
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("step.mp3");
			//auto id = CocosDenshion::AudioEngine::play2d(_files[index], false, 1.0f, &_audioProfile);

			numSounds++;
			//auto id = cocos2d::experimental::AudioEngineImpl::play2d("step.mp3");
			auto soundId = cocos2d::experimental::AudioEngine::play2d("step.mp3");



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
			CCLOG("HERE HERE HERE");
			giveup = true;
			blockedCount = 0;
			setBasicUnitPF();
		}
	}
}

void BasicUnit::setBasicUnitPF(){
	lStack->reset();
	auto ppos = this->getPosition();
	auto tpos = lStack->getTail()->data;

	ppos.x = (ppos.x - 16) / 32;
	ppos.y = (ppos.y - 16) / 32;
	tpos.x = (tpos.x - 16) / 32;
	tpos.y = (tpos.y - 16) / 32;

	if (tpos.x >= 0 && tpos.x < 50 && tpos.y >= 0 && tpos.y < 50) {
		tpf->setStart(ppos.x, ppos.y);
		tpf->setEnd(tpos.x, tpos.y);
		this->getMap();
		tpf->setTileX(32);
		tpf->setTileY(32);
		tpf->setOffX(16);
		tpf->setOffY(16);

		lStack = tpf->solve();
	}
}

void BasicUnit::setMoving() {
	moving = false;
}

cocos2d::Point BasicUnit::convertToPf(cocos2d::Point tmp) {
	cocos2d::Point ppos;
	ppos.x = (tmp.x - 16) / 32;
	ppos.y = (tmp.y - 16) / 32;
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
	if(currentEnemy != 0){
		currentEL = convertToPf(currentEnemy->getPosition());
		if(currentEnemyLocation.x != currentEL.x || currentEnemyLocation.y != currentEL.y){
			currentEnemyLocation = currentEL;
			currentEnemyMoved = true;
		}
	}

	//check if dead
	if(!dead && health <= 0){
		dead = true;
		auto rotateTo = RotateTo::create(1.5, 90);
		this->runAction(rotateTo);
	}

	//if dead, die
	else if(dead){
		//figure out how to delete
		//delete this;

		delete tpf;
		delete lStack;
		pf->unblock(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);
		pf->untaken(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);
		pf->setUnitZero(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);
		this->getParent()->removeChild(this);
	}

	//if attacking, nothing should be done
	else if (!attacking){

		//if an enemy is within striking distance and the attack cooldown has expired,
		//attack the closest enemy (loaded from the parent scene)
		if(!attacking && currentEnemy != 0 && !currentEnemy->isDead() &&
				this->enemyIsAttackable()){
			attacking = true;
			//currentEnemyMoved = false;
			CCLOG("ATTACKED");
			auto callback = CallFunc::create([this]() {
				//if(currentEnemy != 0 && !currentEnemy->isDead()){
				//	currentEnemy->attack(this, 40, 'a');
				//}
				attacking = false;
			});
			auto rotateTo = RotateTo::create(0.1, 0, 20.0f);
			auto rotateBack = RotateTo::create(0.1, 0, 0);
			auto seq = Sequence::create(DelayTime::create(2), callback,
					rotateTo, rotateBack, nullptr);
			this->runAction(seq);
			//RangedAttackObject* atk = RangedAttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm');
			AttackObject* atk = AttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm', pf);
			atk->initAttack();
			lStack->reset();
		}

		/*
		else if(currentEnemy != 0 && unitToUnitDistance(this, currentEnemy) >= attackRange){
			currentEnemyIsCloseEnough = false;
			currentEnemyMoved = false;
			CCLOG("drp");
		}*/

		//else if(currentEnemyIsCloseEnough && !currentEnemyMoved && unitToUnitDistance(this, currentEnemy) < attackRange){
		else if(currentEnemy != 0 && currentEnemyMoved && unitToUnitDistance(this, currentEnemy) < attackRange){
			auto t = convertToPf(currentEnemy->getPosition());
			this->ASolve(t.x, t.y);
			currentEnemyMoved = false;
			CCLOG("doooops");
		}

		//the unit is in between movements and is not attacking
		//if the movement stack isn't empty and the unit isn't moving
		//i.e. in between moves, move to next location in stack
		else if (!lStack->empty() && !moving) {
			delayedMove();
			tempMoving = true;
			movedYet = true;
			badMove = 0;
		}

		else if(goalPositionAsolve){
			goalPositionAsolve = false;
			this->ASolve(goalPosition.x, goalPosition.y);
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
		else if(lStack->empty() && idle == true && idleTrack == true){
			idle = false;
			badMove = 0;
			this->ASolve(goalPosition.x, goalPosition.y);
			//CCLOG("lStack->empty(): %d   %7.7f", lStack->empty(), dt);
		}

		//causes unit to wait for 1 second until trying *A* again
		/* B */
		else if(lStack->empty() && movedYet == true && idle == false && idleTrack == true){
			badMove++;
			/* makes sure *A* is not going while*/
			idleTrack = false;
			auto callback = CallFunc::create([this]() {
				idle = true;
				idleTrack = true;
			});
			auto seq = Sequence::create(DelayTime::create(1), callback, nullptr);
			this->runAction(seq);
			//CCLOG("%d", badMove);
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

int BasicUnit::unitToUnitDistance(BasicUnit *p1, BasicUnit *p2){
	if(p1 != 0 && p2 != 0){
		auto loc1 = p1->convertToPf(p1->getPosition());
		auto loc2 = p2->convertToPf(p2->getPosition());
		cocos2d::Point dist;
		dist.x = abs(loc1.x - loc2.x);
		dist.y = abs(loc1.y - loc2.y);
		return dist.x + dist.y;
	} else{
		return 100;
	}
}


//Melee
bool BasicUnit::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0){
		auto enemyLoc = convertToPf(currentEnemy->getPosition());
		auto thisLoc = convertToPf(this->getPosition());
		if(abs(enemyLoc.x - thisLoc.x) <= 1 && abs(enemyLoc.y - thisLoc.y) <= 1){
			return true;
		}
	}
	return false;
}

/*
//Ranged
bool BasicUnit::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0 && unitToUnitDistance(this, getCurrentEnemy()) <= 6){
		return true;
	}
	return false;
}
*/
//WILL BUILD ON LATER
void BasicUnit::attack(BasicUnit * attacker, int damage, char attackType){
	health = health - damage;
	CCLOG("%p WAS ATTACKEDDDD for %d damage", this, damage);
	CCLOG("%p's health: %d", this, health);
}

void BasicUnit::returnHealth(int healthTaken, char attackType){
	health += healthTaken;
}
