#include "src/Units/Enemies/EnemyTrollErodedMetal.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/AttackObject.h"

USING_NS_CC;

EnemyTrollErodedMetal::EnemyTrollErodedMetal() {
}

EnemyTrollErodedMetal::~EnemyTrollErodedMetal() {
	CCLOG("EnemyTrollErodedMetal WAS DELETED!!!!!!!!!!!");
}

EnemyTrollErodedMetal* EnemyTrollErodedMetal::create() {
	EnemyTrollErodedMetal* pSprite = new EnemyTrollErodedMetal();
	auto *filename = __String::createWithFormat("TrollErodedMetalWalk00000.png");
	auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
	pSprite->initWithSpriteFrame(wframe);
	srand((unsigned) time(NULL));
	pSprite->autorelease();

	pSprite->movementRange = pSprite->initMovementRange;
	pSprite->initHealth = 700;
	pSprite->health = pSprite->initHealth;
	pSprite->attackDamage = pSprite->initAttackDamage;
	pSprite->attackRange = pSprite->initAttackRange;
	pSprite->walkingSpeed = pSprite->initWalkingSpeed;
	pSprite->loot = pSprite->initLoot;

	pSprite->scheduleUpdate();

	return pSprite;

}

EnemyTrollErodedMetal* EnemyTrollErodedMetal::create(cocos2d::Point tmp){
	EnemyTrollErodedMetal* pSprite = EnemyTrollErodedMetal::create();
	pSprite->setPosition(tmp);
	return pSprite;
}

void EnemyTrollErodedMetal::removeFromLevel(){
	CCLOG("EnemyTrollErodedMetal remove from level");
	this->getParent()->removeChild(this);
}

/*
 * Set attack animation
 */
void EnemyTrollErodedMetal::animationAttack(){
	auto attackCallback = CallFunc::create([this]() {
		AttackObject* atk = AttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), attackDamage, 'm', pf);
		atk->initAttack();
	});
	auto attackSeq = Sequence::create(DelayTime::create(numAttackFrames * attackDuration / 2), attackCallback, nullptr);
	this->runAction(attackSeq);

	Vector<SpriteFrame *> trollFrames;
	for (int i = 0; i <= numAttackFrames; i++){
		auto *filename = __String::createWithFormat("TrollErodedMetalAttack%d001%d.png", unitDir, i);
		//CCLOG("%s", filename->getCString());
		auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
		trollFrames.pushBack(wframe);
	}
	auto wrunAnim = Animation::createWithSpriteFrames(trollFrames, attackDuration);
	auto animate = Animate::create(wrunAnim);

	this->runAction(animate);
}

/*
 * Set the walking animation
 */
cocos2d::Animate* EnemyTrollErodedMetal::animationWalk(){
	if(unitDir <=8){
		Vector<SpriteFrame *> walkFrames;

		//length of animation sequence
		float animationLength = 6.0f;

		for (int i = 0; i <= animationLength; i++){
			auto *filename = __String::createWithFormat("TrollErodedMetalWalk%d000%d.png", unitDir, i);
			auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
			walkFrames.pushBack(wframe);
		}
		float duration =  walkingSpeed / animationLength;
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
void EnemyTrollErodedMetal::animationDie(){
	if(unitDir <=8){
		Vector<SpriteFrame *> dieFrames;

		//length of animation sequence
		int animationLength = 30;
		int start = 20;

		for (int i = start; i <= animationLength; i++){
			auto *filename = __String::createWithFormat("TrollErodedMetalDie%d00%d.png", unitDir, i);
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

