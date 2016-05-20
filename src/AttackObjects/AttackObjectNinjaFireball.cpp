#include "src/AttackObjects/AttackObjectNinjaFireball.h"
#include "src/Units/BasicUnit.h"

USING_NS_CC;


AttackObjectNinjaFireball::AttackObjectNinjaFireball() {
}

AttackObjectNinjaFireball::~AttackObjectNinjaFireball() {
}

AttackObjectNinjaFireball* AttackObjectNinjaFireball::create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> * tpf, cocos2d::Point tpos) {
	AttackObjectNinjaFireball* pSprite = new AttackObjectNinjaFireball();
	pSprite->initWithFile("bullet.png");
	pSprite->autorelease();
	pSprite->parent = attacker;
	pSprite->location = location;
	pSprite->damage = damage;
	pSprite->pf = tpf;
	pSprite->targetPos = tpos;
	//pSprite->init(location, damage, attackType);
	return pSprite;
}

void AttackObjectNinjaFireball::initAttack(){
	this->parent->getParent()->addChild(this);
	auto eloc = parent->getPosition();
	this->setPosition(eloc.x, eloc.y);

	auto callback = CallFunc::create([this]() {
		this->attack();
		this->parent->getParent()->removeChild(this);
	});

	auto moveTo = cocos2d::MoveTo::create(attackInterval,targetPos);
	auto rotateTo = cocos2d::RotateBy::create(attackInterval, rotationIntervalAngle);
	auto spawn = cocos2d::Spawn::create(moveTo, rotateTo, nullptr);
	auto seq = cocos2d::Sequence::create(spawn, callback, nullptr);
	this->runAction(seq);
}

void AttackObjectNinjaFireball::attack(){
	if(pf->getUnit(location.x, location.y) != 0 && pf->getUnit(location.x, location.y)->getTeam() != parent->getTeam()){
		pf->getUnit(location.x, location.y)->attack(parent, damage, 'm');
	}
}

