#include "src/AttackObjects/AttackObjectNinjaStar.h"
#include "src/Units/BasicUnit.h"

USING_NS_CC;


AttackObjectNinjaStar::AttackObjectNinjaStar() {
}

AttackObjectNinjaStar::~AttackObjectNinjaStar() {
}

AttackObjectNinjaStar* AttackObjectNinjaStar::create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> * tpf) {
	AttackObjectNinjaStar* pSprite = new AttackObjectNinjaStar();
	CCLOG("NINJA STARRRR");
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

void AttackObjectNinjaStar::seq(){
	auto callback = CallFunc::create([this]() {
		this->attackArea();
		this->parent->getParent()->removeChild(this);
	});

	auto travelDistance =  (float)parent->pointToPointDistance(this->getPosition(), attackPos_nd);

	auto moveTo = cocos2d::MoveTo::create(
			travelDistance / projectileSpeed,
			attackPos_nd);
	auto rotateTo = cocos2d::RotateBy::create(
			travelDistance / projectileSpeed,
			2 * 720 * travelDistance / projectileSpeed);
	auto spawn = cocos2d::Spawn::create(moveTo, rotateTo, nullptr);
	auto seq = cocos2d::Sequence::create(spawn, callback, nullptr);
	this->runAction(seq);
}


