#include "src/AttackObjects/AttackObject.h"
#include "src/Units/BasicUnit.h"

AttackObject::AttackObject() {
}

AttackObject::~AttackObject() {
}

AttackObject* AttackObject::create(BasicUnit * attacker, cocos2d::Point location_pf, int damage, char attackType, PathFinder<BasicUnit> *tpf) {
	AttackObject* pSprite = new AttackObject();
	pSprite->initWithFile("bullet.png");
	pSprite->autorelease();
	pSprite->setScale(0.5);
	pSprite->parent = attacker;
	pSprite->location_pf = location_pf;
	pSprite->damage = damage;
	pSprite->pf = tpf;
	//CCLOG("%p THIS WAS RECEIVED", attacker);
	//pSprite->init(location_pf, damage, attackType);
	return pSprite;
}

void AttackObject::initAttack(){

	this->parent->getParent()->addChild(this);
	auto eloc = parent->getCurrentEnemy()->getPosition();
	this->setPosition(eloc.x, eloc.y);
	this->attack();
	this->parent->getParent()->removeChild(this);
}

/*
*
*/
void AttackObject::attack(){
    int xarr[9] = {0,  0,  0,  1,  1,  1, -1, -1, -1};
    int yarr[9] = {0,  1, -1,  0,  1, -1,  0,  1, -1};
    for(int i = 0; i <= 8; i++){
        auto ploc = parent->convertToPf(parent->getPosition());
        auto loc = location_pf;
        loc.x += xarr[i];
        loc.y += yarr[i];

        if(pf->checkBounds_Pf(loc)){
            if(this->pf->getUnit(loc.x, loc.y) != 0 && this->pf->getUnit(loc.x, loc.y)->getTeam() != parent->getTeam()){
                this->pf->getUnit(loc.x, loc.y)->attack(parent, damage, 'm');
                cPrint("%d %d\t%d, %d | %d, %d", xarr[i], yarr[i], (int)loc.x, (int)loc.y, (int)ploc.x, (int)ploc.y);
                cPrint("parent: %s, attackee: %s", parent->getUnitType(), pf->getUnit(loc.x, loc.y)->getUnitType());
                break;
            }
        } else{
            cPrint("%d, %d, | %d, %d \tparent: %s, attackee: empty", (int)loc.x, (int)loc.y, (int)ploc.x, (int)ploc.y, parent->getUnitType());
        }
    }
}

