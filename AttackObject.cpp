#include "AttackObject.h"

AttackObject::AttackObject() {
}

AttackObject::~AttackObject() {
}

AttackObject* AttackObject::create(BasicUnit * attacker, cocos2d::Point location, int damage, char attackType) {
	AttackObject* pSprite = new AttackObject();
	pSprite->initWithFile("bullet.png");
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	pSprite->setScale(0.5);
	pSprite->parent = attacker;
	return pSprite;
}

void AttackObject::attack(cocos2d::Point location, int damage, char attackType){
	if(pf->map[location.x][location.y]->getTeam() != 0 && pf->map[location.x][location.y]->getUnit() != 0 && pf->map[location.x][location.y]->team != parent->getTeam()){
		int healthTaken = pf->map[location.x][location.y]->unit->attack(this, damage, 'm');
		parent->returnHealth(healthTaken, attackType);
	}
}

