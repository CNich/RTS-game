#include "AttackObjectNinjaStar.h"
#include "BasicUnit.h"

USING_NS_CC;


AttackObjectNinjaStar::AttackObjectNinjaStar() {
}

AttackObjectNinjaStar::~AttackObjectNinjaStar() {
}

AttackObjectNinjaStar* AttackObjectNinjaStar::create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> * tpf) {
	AttackObjectNinjaStar* pSprite = new AttackObjectNinjaStar();
	pSprite->initWithFile("star.png");
	pSprite->autorelease();
	pSprite->setScale(0.25);
	pSprite->parent = attacker;
	pSprite->location = location;
	pSprite->damage = damage;
	pSprite->pf = tpf;
	CCLOG("%p THIS WAS RECEIVED", attacker);
	//pSprite->init(location, damage, attackType);
	CCLOG("DUMMIED init");
	return pSprite;
}

void AttackObjectNinjaStar::initAttack(){
	CCLOG("DUMMIED here");
	this->parent->getParent()->addChild(this);
	auto eloc = parent->getPosition();
	this->setPosition(eloc.x, eloc.y);

	auto callback = CallFunc::create([this]() {
		this->attack();
		this->parent->getParent()->removeChild(this);
	});

	auto jumpTo = cocos2d::JumpTo::create(1, parent->getCurrentEnemy()->getPosition(),100, 1);

	auto seq = cocos2d::Sequence::create(jumpTo, callback, nullptr);
	this->runAction(seq);

	//this->parent->getParent()->removeChild(this);
}

void AttackObjectNinjaStar::attack(){
	if(pf->getUnit(location.x, location.y) != 0 && pf->getUnit(location.x, location.y)->getTeam() != parent->getTeam()){
		pf->getUnit(location.x, location.y)->attack(parent, damage, 'm');
	}
}

