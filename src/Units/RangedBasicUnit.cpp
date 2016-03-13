#include "src/Units/RangedBasicUnit.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/RangedAttackObject.h"

USING_NS_CC;

RangedBasicUnit::RangedBasicUnit() {
}

RangedBasicUnit::~RangedBasicUnit() {
	CCLOG("THIS GUY WAS DELETED!!!!!!!!!!!");
	//delete tpf;
	//delete lStack;
	//this->getParent()->removeChild(this);
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

	pSprite->health = pSprite->initHealth;
	pSprite->attackRange = pSprite->initAttackRange;
	pSprite->movementRange = pSprite->initMovementRange;

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

	pSprite->health = pSprite->initHealth;
	pSprite->attackRange = pSprite->initAttackRange;
	pSprite->movementRange = pSprite->initMovementRange;
	return pSprite;

}

void RangedBasicUnit::makeAttack(){
	RangedAttackObject* atk = RangedAttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm', pf);
	atk->initAttack();
}

//Ranged
bool RangedBasicUnit::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0 && unitToUnitDistance(this, getCurrentEnemy()) <= attackRange){
		return true;
	}
	return false;
}
