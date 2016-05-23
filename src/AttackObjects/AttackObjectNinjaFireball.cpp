#include "src/AttackObjects/AttackObjectNinjaFireball.h"
#include "src/Units/BasicUnit.h"

USING_NS_CC;


AttackObjectNinjaFireball::AttackObjectNinjaFireball() {
}

AttackObjectNinjaFireball::~AttackObjectNinjaFireball() {
}

AttackObjectNinjaFireball* AttackObjectNinjaFireball::create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> * tpf) {
	AttackObjectNinjaFireball* pSprite = new AttackObjectNinjaFireball();
	pSprite->initWithFile("bullet.png");
	pSprite->autorelease();
	pSprite->parent = attacker;
	pSprite->location = location;
	pSprite->damage = damage;
	pSprite->pf = tpf;
	pSprite->targetPos = location;
	pSprite->scheduleUpdate();
	//pSprite->init(location, damage, attackType);
	return pSprite;
}

void AttackObjectNinjaFireball::initAttack(){
	this->parent->getParent()->addChild(this);
	this->setPosition(parent->getPosition());
	movementAngle = parent->getAngle(this->getPosition(), targetPos);
	init = true;
}

/*
 * Attack the unit at the pathFinder location
 */
void AttackObjectNinjaFireball::attack(){
	if(pf->getUnit(location.x, location.y) != 0 && pf->getUnit(location.x, location.y)->getTeam() != parent->getTeam()){
		pf->getUnit(location.x, location.y)->attack(parent, damage, 'm');
	}
}

/*
 * Calculate the next position of the attack object
 */
cocos2d::Point AttackObjectNinjaFireball::intMove(float angle, cocos2d::Point curPos){
	cocos2d::Point nextPoint;
	nextPoint.x = curPos.x + pf->getTileX() * sinf(angle);
	nextPoint.y = curPos.y + pf->getTileY() * cosf(angle);
	return nextPoint;
}

/*
 * 1. Attack current location
 * 2. Calculate next location and move there
 * 3. Repeat 1-2 until fbDistanceNum == 0
 * 4. Delete this object
 */
void AttackObjectNinjaFireball::update(float dt){
	if(init){
		if(!moving && fbDistanceNum > 0){
			fbDistanceNum--;
			attack();
			setMoving(true);

			auto callback = CallFunc::create([this]() {
				setMoving(false);
			});

			//create actions + run sequence
			auto targetPosition = intMove(movementAngle, this->getPosition());
			auto moveTo = cocos2d::MoveTo::create(attackInterval,targetPosition);
			auto rotateTo = cocos2d::RotateBy::create(attackInterval, rotationIntervalAngle);
			auto spawn = cocos2d::Spawn::create(moveTo, rotateTo, nullptr);
			auto seq = cocos2d::Sequence::create(spawn, callback, nullptr);
			this->runAction(seq);
			CCLOG("movementAngle: %3.3f fbDistanceNum: %d", movementAngle, fbDistanceNum);
		} else if(!moving && fbDistanceNum == 0){
			this->parent->getParent()->removeChild(this);
		}
	}
}

