#include "BasicUnit.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "AttackObject.h"
#include "RangedAttackObject.h"

USING_NS_CC;

BasicUnit::BasicUnit() {
}

BasicUnit::~BasicUnit() {
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
	BasicUnit* pSprite = new BasicUnit();
	pSprite->initWithFile("029.png");
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	pSprite->tpf = new PathFinder<BasicUnit>(50, 50);
	pSprite->tpf->setTileX(32);
	pSprite->tpf->setTileY(32);
	pSprite->setScale(0.5);

	pSprite->scheduleUpdate();
	pSprite->setPosition(tmp);

	pf->block(pSprite->convertToPf(tmp).x, pSprite->convertToPf(tmp).y);
	pf->taken(pSprite->convertToPf(tmp).x, pSprite->convertToPf(tmp).y);
	pf->setUnit(pSprite->convertToPf(tmp).x, pSprite->convertToPf(tmp).y, pSprite);

	return pSprite;

}

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

	//goalPosition.x = x;
	//goalPosition.y = y;

	auto ppos = this->getPosition();

	ppos.x = (ppos.x - 16) / 32;
	ppos.y = (ppos.y - 16) / 32;

	if (x >= 0 && x < 50 && y >= 0 && y < 50) {

		tpf->setStart(ppos.x, ppos.y);
		tpf->setEnd(x, y);
		this->getMap();
		tpf->setTileX(32);
		tpf->setTileY(32);
		tpf->setOffX(16);
		tpf->setOffY(16);

		lStack = tpf->solve();


		/* MAKE A BLOCKCOUNT TYPE THING
		 *
		 *
		 * */
		/*if(lStack->empty()){
			auto callback = CallFunc::create([this, ppos]() {
				//this->ASolve(ppos.x + 1, ppos.y + 1);
				this->ASolve(goalPosition.x, goalPosition.y);
			});
			auto seq = Sequence::create(DelayTime::create(0.5), callback, nullptr);
			this->runAction(seq);
		}*/
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
		auto playerPos = this->getPosition();
		auto diff = touchLocation - playerPos;
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
		}
		if (playerPos.x < (50 * 32) && playerPos.y < (50 * 32)
				&& playerPos.y >= 0 && playerPos.x >= 0) {
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("step.mp3");

			this->setPlayerPosition(playerPos);
		}
	}
}

void BasicUnit::setPlayerPosition(Point position) {
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


		CCFiniteTimeAction* actionMove = CCMoveTo::create(0.2, position);

		auto callback = CallFunc::create([this]() {
			this->setMoving();
		});

		moving = true;

		auto seq = Sequence::create(actionMove, callback, nullptr);
		this->runAction(seq);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("step.mp3");

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
			setPF();
		}
	}
}

void BasicUnit::setPF(){
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
	//check if dead
	if(!dead && health <= 0){
		dead = true;
		auto rotateTo = RotateTo::create(1.5, 90);
		this->runAction(rotateTo);
	}
	//if dead, die
	else if(dead){
		//figure out how to delete
	}
	//if an enemy is within striking distance and the attack cooldown has expired,
	//attack the closest enemy (loaded from the parent scene)
	else if(!attacking && currentEnemy != 0 && !currentEnemy->isDead() &&
			this->enemyIsAttackable()){
		attacking = true;
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
		RangedAttackObject* atk = RangedAttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm');
		CCLOG("%p THIS WAS SENT", this);
		atk->initAttack();
	} else if (!lStack->empty() && !moving) {
		delayedMove();
		tempMoving = true;
		movedYet = true;
		badMove = 0;
	} else if(lStack->empty() && idle == true && idleTrack == true){
		idle = false;
		badMove = 0;
		this->ASolve(goalPosition.x, goalPosition.y);
		//CCLOG("lStack->empty(): %d   %7.7f", lStack->empty(), dt);
	} else if(lStack->empty() && movedYet == true && idle == false && idleTrack == true){
		badMove++;
		idleTrack = false;
		auto callback = CallFunc::create([this]() {
			idle = true;
			idleTrack = true;
		});
		auto seq = Sequence::create(DelayTime::create(1), callback, nullptr);
		this->runAction(seq);
		//CCLOG("%d", badMove);
	} else if(lStack->empty() && !moving && tempMoving){
		badMove = 0;
		if(tempMoving){
			pf->block(convertToPf(this->getPosition()).x, convertToPf(this->getPosition()).y);
			tempMoving = false;
		}
	}
}

int BasicUnit::unitToUnitDistance(BasicUnit *p1, BasicUnit *p2){
	auto loc1 = p1->convertToPf(p1->getPosition());
	auto loc2 = p2->convertToPf(p2->getPosition());
	cocos2d::Point dist;
	dist.x = abs(loc1.x - loc2.x);
	dist.y = abs(loc1.y - loc2.y);
	return dist.x + dist.y;
}
/*

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
*/

//Ranged
bool BasicUnit::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0 && unitToUnitDistance(this, getCurrentEnemy()) <= 6){
		return true;
	}
	return false;
}

//WILL BUILD ON LATER
void BasicUnit::attack(BasicUnit * attacker, int damage, char attackType){
	health = health - damage;
	CCLOG("%p WAS ATTACKEDDDD for %d damage", this, damage);
	CCLOG("%p's health: %d", this, health);
}

void BasicUnit::returnHealth(int healthTaken, char attackType){
	health += healthTaken;
}
