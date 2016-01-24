#include "AttackObject.h"
#include "BasicUnit.h"


AttackObject::AttackObject() {
}

AttackObject::~AttackObject() {
}

AttackObject* AttackObject::create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType) {
	AttackObject* pSprite = new AttackObject();
	pSprite->initWithFile("bullet.png");
	pSprite->autorelease();
	pSprite->setScale(0.5);
	pSprite->parent = attacker;
	pSprite->location = location;
	pSprite->damage = damage;
	CCLOG("%p THIS WAS RECEIVED", attacker);
	//pSprite->init(location, damage, attackType);
	CCLOG("DUMMIED init");
	return pSprite;
}

void AttackObject::initAttack(){
	CCLOG("DUMMIED here");
	this->parent->getParent()->addChild(this);
	auto eloc = parent->getCurrentEnemy()->getPosition();
	this->setPosition(eloc.x, eloc.y);
	this->attack();
	this->parent->getParent()->removeChild(this);
}

void AttackObject::attack(){
	if(pf->getUnit(location.x, location.y) != 0 && pf->getUnit(location.x, location.y)->getTeam() != parent->getTeam()){
		pf->getUnit(location.x, location.y)->attack(parent, damage, 'm');
	}
}

