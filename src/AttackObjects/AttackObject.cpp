#include "src/AttackObjects/AttackObject.h"
#include "src/Units/BasicUnit.h"


AttackObject::AttackObject() {
}

AttackObject::~AttackObject() {
}

AttackObject* AttackObject::create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> *tpf) {
	AttackObject* pSprite = new AttackObject();
	pSprite->initWithFile("bullet.png");
	pSprite->autorelease();
	pSprite->setScale(0.5);
	pSprite->parent = attacker;
	pSprite->location = location;
	pSprite->damage = damage;
	pSprite->pf = tpf;
	//CCLOG("%p THIS WAS RECEIVED", attacker);
	//pSprite->init(location, damage, attackType);
	return pSprite;
}

void AttackObject::initAttack(){

	this->parent->getParent()->addChild(this);
	auto eloc = parent->getCurrentEnemy()->getPosition();
	this->setPosition(eloc.x, eloc.y);
	this->attack();
	this->parent->getParent()->removeChild(this);
}

void AttackObject::attack(){
	if(this->pf->getUnit(location.x, location.y) != 0 && this->pf->getUnit(location.x, location.y)->getTeam() != parent->getTeam()){
		this->pf->getUnit(location.x, location.y)->attack(parent, damage, 'm');
	}
}

