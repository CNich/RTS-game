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
	this->calculateAttackPos();
	seq();
}

void RangedAttackObject::calculateAttackPos(){
	distanceToEnemy_nd = (float)parent->pointToPointDistance(this->getPosition(), parent->getCurrentEnemy()->getPosition());
	distanceToEnemy_pf = distanceToEnemy_nd / pf->getTileY();

	float diagFactor = sqrt(pow(pf->getTileX(), 2) + pow(pf->getTileY(), 2));

	attackPos_nd = parent->getCurrentEnemy()->getPosition();

	auto ep = attackPos_nd;
	auto tp = this->getPosition();

	//if(parent->getCurrentEnemy()->getMoving()){
		//walking speed of enemy
		auto ws = parent->getCurrentEnemy()->getWalkingSpeed();

		//enemy direction
		int enemyDir = parent->getCurrentEnemy()->getUnitDir();
		float th = 3.14159265359 * parent->getCurrentEnemy()->getUnitAngle() / 180.0f;

		//calculate enemy speed based on it's direction
		float enemySpeed;
		float travelFactor;
		if(enemyDir == 0 || 4){
			enemySpeed = pf->getTileY() / ws;
			travelFactor = pf->getTileY();
		} else if(enemyDir == 2 || 6){
			enemySpeed =  pf->getTileX() / ws;
			travelFactor = pf->getTileX();
		} else{
			enemySpeed = diagFactor / ws;
			travelFactor = diagFactor;
		}

		if(!parent->getCurrentEnemy()->getMoving()){
			enemySpeed = 0;
		}

		float projectileSpeed = maxRange * travelFactor / maxTravelTime;

		float a = pow(enemySpeed, 2) - pow(projectileSpeed, 2);
		//float b = 2 * (target.velocityX * (target.startX - cannon.X) + target.velocityY * (target.startY - cannon.Y))
		float b = 2 * (enemySpeed * sin(th) * (ep.x - tp.x) +
				enemySpeed * cos(th) * (ep.y - tp.y));
		float c = pow(ep.x - tp.x, 2) + pow(ep.y - tp.y, 2);

		float disc = pow(b, 2) - 4 * a * c;

		float t1 = (-b + sqrt(disc)) / (2 * a);
		float t2 = (-b - sqrt(disc)) / (2 * a);
		float t;
		if(t1 < t2 && t1 > 0){
			t = t1;
		} else{
			t = t2;
		}

		attackPos_nd.x = t * enemySpeed * sin(th) + ep.x;
		attackPos_nd.y = t * enemySpeed * cos(th) + ep.y;
		CCLOG("angle: %3.3f d, %3.3f r, pr speed: %3.3f, es: %3.3f", th * 180.0f / 3.14159265359, th,
				projectileSpeed, enemySpeed);
	//}

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

	auto jumpTo = cocos2d::JumpTo::create(
			maxTravelTime / maxRange * distanceToEnemy_pf,
			attackPos_nd,
			maxHeight / maxRange * distanceToEnemy_pf,
			1);
	auto seq = cocos2d::Sequence::create(jumpTo, cb2, nullptr);
	this->runAction(seq);
}

void RangedAttackObject::attackArea(){
	auto p_pf = this->parent->convertToPf(this->getPosition());
	for(int i = -1 * attackAreaRange; i <= attackAreaRange; i++){
		for(int j = -1 * attackAreaRange; j <= attackAreaRange; j++){
			cocos2d::Point p2_pf = {p_pf.x + i, p_pf.y + j};
			this->attack(p2_pf);
		}
	}
}

/*
 * Calculate Gaussian
 */
float RangedAttackObject::gausFactor(int distance){
	float pow2 = powf((float)distance/sigma2, 2);
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
		if(damage * gf < 35){
			//CCLOG("damage: %2.3f, distance: %d , diff e: %3.0f,%3.0f | diff p: %3.0f,%3.0f", (float)damage * gf, distance, p1.x - p2.x, p1.y - p2.y, p1.x - p3.x, p1.y - p3.y);
			//CCLOG("damage: %2.3f, distance: %d , gf: %2.3f", (float)damage * gf, distance, gf);
		}
		pf->getUnit(pos_pf.x, pos_pf.y)->attack(parent, damage * gf, 'm');
	}
}

