#include "src/Units/RangedBasicUnit.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/RangedAttackObject.h"

USING_NS_CC;

RangedBasicUnit::RangedBasicUnit() {
}

RangedBasicUnit::~RangedBasicUnit() {
	CCLOG("THIS GUY WAS DELETED!!!!!!!!!!!");
	//delete tpf;
	//delete lStack;
	//this->getParent()->removeChild(this);
}

RangedBasicUnit* RangedBasicUnit::create() {
	RangedBasicUnit* pSprite = new RangedBasicUnit();
	//pSprite->initWithFile("030.png");
	auto *filename = __String::createWithFormat("ElfWalk00016.png");
	auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
	pSprite->initWithSpriteFrame(wframe);
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	/*pSprite->tpf = new PathFinder<BasicUnit>(50, 50);
	pSprite->tpf->setTileX(32);
	pSprite->tpf->setTileY(32);
	*/
	pSprite->setScale(0.75);

	pSprite->scheduleUpdate();

	pSprite->initHealth = 120;
	pSprite->health = pSprite->initHealth;
	pSprite->attackDamage = pSprite->initAttackDamage;
	pSprite->attackRange = pSprite->initAttackRange;
	pSprite->movementRange = pSprite->initMovementRange;
	pSprite->walkingSpeed = pSprite->initWalkingSpeed;
	pSprite->price = pSprite->initPrice;

	return pSprite;

}

RangedBasicUnit* RangedBasicUnit::create(cocos2d::Point tmp){
	RangedBasicUnit* pSprite = create();
	pSprite->setPosition(tmp);
	return pSprite;
}

void RangedBasicUnit::removeFromLevel(){
	CCLOG("RangedBasicUnit remove from level");
	this->getParent()->removeChild(this);
}

/*
 * Set attack animation
 */
void RangedBasicUnit::animationAttack(){
	RangedAttackObject* atk = RangedAttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), attackDamage, 'r', pf);
	atk->initAttack();

	Vector<SpriteFrame *> attackFrames;
	for (int i = 13; i <= 25; i++){
		auto *filename = __String::createWithFormat("ElfAttack%d00%d.png", unitDir, i);
		//CCLOG("%s", filename->getCString());
		auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
		attackFrames.pushBack(wframe);
		//CCLOG("Made troll %d", i);
	}
	auto wrunAnim = Animation::createWithSpriteFrames(attackFrames, attackDuration);
	auto animate = Animate::create(wrunAnim);
	this->runAction(animate);
}

/*
 * Set the walking animation
 */
cocos2d::Animate* RangedBasicUnit::animationWalk(){
	if(unitDir <=8){
		Vector<SpriteFrame *> walkFrames;

		//length of animation sequence
		int animationLength = 22;
		int startFrame = 16;

		for (int i = startFrame; i <= animationLength; i++){
			auto *filename = __String::createWithFormat("ElfWalk%d00%d.png", unitDir, i);
			auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
			walkFrames.pushBack(wframe);
		}
		float duration =  walkingSpeed / (animationLength - startFrame);
		if(unitDir % 2 == 1) duration *= 1.4;
		auto walkAnim = Animation::createWithSpriteFrames(walkFrames, duration);
		auto animate = Animate::create(walkAnim);
		//auto waction = RepeatForever::create(animate);
		//this->runAction(animate);
		return animate;
	} else{
		return nullptr;
	}
}


/*
 * Set dying animation
 */
void RangedBasicUnit::animationDie(){
	if(unitDir <=8){
		Vector<SpriteFrame *> dieFrames;

		//length of animation sequence
		int animationLength = 24;
		int startFrame = 13;

		for (int i = startFrame; i <= animationLength; i++){
			auto *filename = __String::createWithFormat("ElfDie%d00%d.png", unitDir, i);
			auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
			//CCLOG("%s", filename->getCString());
			dieFrames.pushBack(wframe);
		}
		auto dieAnim = Animation::createWithSpriteFrames(dieFrames, dieDuration);
		auto animate = Animate::create(dieAnim);
		//auto waction = RepeatForever::create(animate);
		this->runAction(animate);
	}
}

//Ranged
bool RangedBasicUnit::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0 && unitToUnitDistance(this, getCurrentEnemy()) <= attackRange &&
			attackTravelRange >= pointToPointDistance(this->convertToPf(this->getPosition()), goalPosition)){
		return true;
	}
	return false;
}
