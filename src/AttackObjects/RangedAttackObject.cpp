#include "src/AttackObjects/RangedAttackObject.h"
#include "src/Units/BasicUnit.h"

USING_NS_CC;


RangedAttackObject::RangedAttackObject() {
}

RangedAttackObject::~RangedAttackObject() {
}

RangedAttackObject* RangedAttackObject::create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType, PathFinder<BasicUnit> * tpf) {
	RangedAttackObject* pSprite = new RangedAttackObject();
	pSprite->initWithFile("bullet.png");
	pSprite->autorelease();
	pSprite->setScale(0.25);
	pSprite->parent = attacker;
	pSprite->location = location;
	pSprite->damage = damage;
	pSprite->pf = tpf;
	//CCLOG("%p THIS WAS RECEIVED", attacker);
	return pSprite;
}

void RangedAttackObject::initAttack(){

	this->parent->getParent()->addChild(this);
	auto eloc = parent->getPosition();
	this->setPosition(eloc.x, eloc.y);
	seq();
}

void RangedAttackObject::seqCallback(){
	this->attackArea();
	this->parent->getParent()->removeChild(this);
}

void RangedAttackObject::seq(){
	std::function<void()> callback2 = [this](){ seqCallback(); };

	CallFunc * cb2 = CallFunc::create(callback2);

	auto callback = CallFunc::create([this]() {
		this->attackArea();
		this->parent->getParent()->removeChild(this);
	});

	auto jumpTo = cocos2d::JumpTo::create(1, parent->getCurrentEnemy()->getPosition(),100, 1);
	auto seq = cocos2d::Sequence::create(jumpTo, cb2, nullptr);
	this->runAction(seq);
}

void RangedAttackObject::attackArea(){
	auto p_pf = this->parent->convertToPf(this->getPosition());
	for(int i = -1; i <= 1; i++){
		for(int j = -1; j <= 1; j++){
			cocos2d::Point p2_pf = {p_pf.x + i, p_pf.y + j};
			this->attack(p2_pf);
		}
	}
}

/*
 * Calculate Gaussian
 */
float RangedAttackObject::gausFactor(int distance){
	float pow2 = powf(distance/sigma2, 2);
	//CCLOG("pow2: %3.3f, distance: %d, sigma2: %d", pow2, distance, sigma2);
	return expf(-pow2);
}

void RangedAttackObject::attack(cocos2d::Point pos_pf){
	if(pf->checkBounds_Pf(pos_pf) && pf->getUnit(pos_pf.x, pos_pf.y) != 0 && pf->getUnit(pos_pf.x, pos_pf.y)->getTeam() != parent->getTeam()){
		auto p1 = this->getPosition();
		auto p2 = pf->getUnit(pos_pf.x, pos_pf.y)->getPosition();
		auto p3 = this->parent->getPosition();
		int distance = parent->pointToPointDistance(p1, p2);
		auto pf1 = parent->convertToPf(p1);
		auto pf2 = parent->convertToPf(p2);
		auto pf3 = parent->convertToPf(p3);

		float gf = gausFactor(distance);
		CCLOG("damage: %2.3f, distance: %d , diff e: %3.0f,%3.0f | diff p: %3.0f,%3.0f", damage * gf, distance, p1.x - p2.x, p1.y - p2.y, p1.x - p3.x, p1.y - p3.y);
		pf->getUnit(pos_pf.x, pos_pf.y)->attack(parent, damage * gf, 'm');
	}
}

