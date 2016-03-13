#include "src/AttackObjects/AttackObjectNinjaStar.h"
#include "src/Units/BasicUnit.h"

USING_NS_CC;


AttackObjectNinjaStar::AttackObjectNinjaStar() {
}

AttackObjectNinjaStar::~AttackObjectNinjaStar() {
}

AttackObjectNinjaStar* AttackObjectNinjaStar::create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> * tpf) {
	AttackObjectNinjaStar* pSprite = new AttackObjectNinjaStar();
	pSprite->initWithFile("star.png");
	pSprite->autorelease();
	pSprite->setScale(0.75);
	pSprite->parent = attacker;
	pSprite->location = location;
	pSprite->damage = damage;
	pSprite->pf = tpf;
	//pSprite->init(location, damage, attackType);
	return pSprite;
}

void AttackObjectNinjaStar::initAttack(){
	this->parent->getParent()->addChild(this);
	auto eloc = parent->getPosition();
	this->setPosition(eloc.x, eloc.y);

	auto callback = CallFunc::create([this]() {
		this->attack();
		this->parent->getParent()->removeChild(this);
	});

	auto moveTo = cocos2d::MoveTo::create(0.5,parent->getCurrentEnemy()->getPosition());
	auto rotateTo = cocos2d::RotateBy::create(0.5, 720);
	auto spawn = cocos2d::Spawn::create(moveTo, rotateTo, nullptr);
	auto seq = cocos2d::Sequence::create(spawn, callback, nullptr);
	this->runAction(seq);
}

void AttackObjectNinjaStar::attack(){
	if(pf->getUnit(location.x, location.y) != 0 && pf->getUnit(location.x, location.y)->getTeam() != parent->getTeam()){
		pf->getUnit(location.x, location.y)->attack(parent, damage, 'm');
	}
}

