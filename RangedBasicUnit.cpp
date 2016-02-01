#include "RangedBasicUnit.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "RangedAttackObject.h"

USING_NS_CC;

RangedBasicUnit::RangedBasicUnit() {
}

RangedBasicUnit::~RangedBasicUnit() {
}

RangedBasicUnit* RangedBasicUnit::create() {
	RangedBasicUnit* pSprite = new RangedBasicUnit();
	pSprite->initWithFile("030.png");
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	pSprite->tpf = new PathFinder<BasicUnit>(50, 50);
	pSprite->tpf->setTileX(32);
	pSprite->tpf->setTileY(32);
	pSprite->setScale(0.5);

	pSprite->scheduleUpdate();

	return pSprite;

}

RangedBasicUnit* RangedBasicUnit::create(cocos2d::Point tmp){
	RangedBasicUnit* pSprite = new RangedBasicUnit();
	pSprite->initWithFile("030.png");
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

void RangedBasicUnit::update(float dt) {
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

//Ranged
bool RangedBasicUnit::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0 && unitToUnitDistance(this, getCurrentEnemy()) <= 6){
		return true;
	}
	return false;
}
