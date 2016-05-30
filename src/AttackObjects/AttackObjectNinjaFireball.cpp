#include "src/AttackObjects/AttackObjectNinjaFireball.h"
#include "src/Units/BasicUnit.h"

USING_NS_CC;


AttackObjectNinjaFireball::AttackObjectNinjaFireball() {
}

AttackObjectNinjaFireball::~AttackObjectNinjaFireball() {
}

AttackObjectNinjaFireball* AttackObjectNinjaFireball::create(BasicUnit * attacker, cocos2d::Point enemyPfLocation, int damage, char attackType, PathFinder<BasicUnit> * tpf) {
	AttackObjectNinjaFireball* pSprite = new AttackObjectNinjaFireball();
	pSprite->initWithFile("bullet.png");
	pSprite->autorelease();
	pSprite->parent = attacker;
	pSprite->location = enemyPfLocation;
	pSprite->damage = damage;
	pSprite->pf = tpf;
	pSprite->targetPos = enemyPfLocation;
	pSprite->scheduleUpdate();
	//pSprite->init(location, damage, attackType);
	return pSprite;
}

void AttackObjectNinjaFireball::initAttack(){
	this->parent->getParent()->addChild(this);
	this->setPosition(parent->getPosition());
	movementAngle = parent->getAngle(this->getPosition(), targetPos) - 90;
	//movementAngle = cocos2d::RandomHelper::random_int((int)movementAngle - 10, (int)movementAngle + 10);
	init = true;
	//CCLOG("angle: %3.3f, this position: %3.3f %3.3f, targetPos: %3.3f, %3.3f", movementAngle, this->getPosition().x, this->getPosition().y,
	//		targetPos.x, targetPos.y);

	auto centered = targetPos - this->getPosition();
	float hyp = sqrt(pow(centered.x, 2) + pow(centered.y, 2));


	dxy.x = centered.x * pf->getTileX() / hyp;
	dxy.y = centered.y * pf->getTileX() / hyp;

	sigma2 = 20;

	//CCLOG("dxy: %3.3f %3.3f", dxy.x, dxy.y);

}

/*
 * Attack the unit at the pathFinder location
 */
/*
void AttackObjectNinjaFireball::attack(){
	auto thisPos = this->parent->convertToPf(this->getPosition());
	//CCLOG("attack %3.3f %3.3f, unit at position: %p", thisPos.x, thisPos.y, pf->getUnit(thisPos.x, thisPos.y));
	if(pf->getUnit(thisPos.x, thisPos.y) != 0 && pf->getUnit(thisPos.x, thisPos.y)->getTeam() != parent->getTeam()){
		pf->getUnit(thisPos.x, thisPos.y)->attack(parent, damage, 'm');
		CCLOG("fireball attack");
	}
}
*/

/*
 * Calculate the next position of the attack object
 */
cocos2d::Point AttackObjectNinjaFireball::intMove(float angle, cocos2d::Point curPos){
	cocos2d::Point nextPoint;
	//auto p = this->getPosition();
	//nextPoint.x = p.x + dxy.x;
	//nextPoint.y = p.y + dxy.y;

	float rads;
	rads = angle / 180 * PI;

	auto p = this->getPosition();
	nextPoint.x = p.x + pf->getTileX() * cos(rads);
	nextPoint.y = p.y + pf->getTileX() * sin(rads);

	auto diff = p - nextPoint;

	//CCLOG("angle of intMove: %3.3f, input angle: %3.3f", parent->getAngle(p, nextPoint) - 90, angle);
	//CCLOG("diff: %3.3f %3.3f", diff.x, diff.y);

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
			attackArea();
			setMoving(true);

			auto callback = CallFunc::create([this]() {
				setMoving(false);
				//CCLOG("Fireball Attack");
			});

			//create actions + run sequence
			auto targetPosition = intMove(movementAngle, this->getPosition());
			auto moveTo = cocos2d::MoveTo::create(attackInterval,targetPosition);
			auto rotateTo = cocos2d::RotateBy::create(attackInterval, rotationIntervalAngle);
			auto spawn = cocos2d::Spawn::create(moveTo, rotateTo, nullptr);
			auto seq = cocos2d::Sequence::create(spawn, callback, nullptr);
			this->runAction(seq);
			//CCLOG("movementAngle: %3.3f fbDistanceNum: %d", movementAngle, fbDistanceNum);
		} else if(!moving && fbDistanceNum == 0){
			this->parent->getParent()->removeChild(this);
		}
	}
}

