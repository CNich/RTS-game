#include "src/Units/Enemies/EnemySkinnyTroll.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/AttackObject.h"

USING_NS_CC;

EnemySkinnyTroll::EnemySkinnyTroll() {
}

EnemySkinnyTroll::~EnemySkinnyTroll() {
	CCLOG("EnemySkinnyTroll WAS DELETED!!!!!!!!!!!");
}

EnemySkinnyTroll* EnemySkinnyTroll::create() {
	EnemySkinnyTroll* pSprite = new EnemySkinnyTroll();
	auto *filename = __String::createWithFormat("EnemySkinnyTrollwalk00014.png");
	auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
	pSprite->initWithSpriteFrame(wframe);
	srand((unsigned) time(NULL));
	pSprite->autorelease();

	pSprite->movementRange = pSprite->initMovementRange;
	pSprite->initHealth = 400;
	pSprite->health = pSprite->initHealth;
	pSprite->attackDamage = pSprite->initAttackDamage;
	pSprite->attackRange = pSprite->initAttackRange;
	pSprite->walkingSpeed = pSprite->initWalkingSpeed;
	pSprite->loot = pSprite->initLoot;

	pSprite->scheduleUpdate();

	return pSprite;

}

EnemySkinnyTroll* EnemySkinnyTroll::create(cocos2d::Point tmp){
	EnemySkinnyTroll* pSprite = EnemySkinnyTroll::create();
	pSprite->setPosition(tmp);
	return pSprite;
}

void EnemySkinnyTroll::removeFromLevel(){
	CCLOG("EnemySkinnyTroll remove from level");
	this->getParent()->removeChild(this);
}

/*
 * Set attack animation
 */
void EnemySkinnyTroll::animationAttack(){
	auto attackCallback = CallFunc::create([this]() {
		AttackObject* atk = AttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), attackDamage, 'm', pf);
		atk->initAttack();
	});

	numAttackFrames = 17;
	attackDuration = 0.05;

	auto attackSeq = Sequence::create(DelayTime::create((float)numAttackFrames * attackDuration / 2), attackCallback, nullptr);
	this->runAction(attackSeq);

	Vector<SpriteFrame *> trollFrames;
	for (int i = 40; i < 40 + numAttackFrames; i++){
		auto *filename = __String::createWithFormat("EnemySkinnyTrollattack%d00%d.png", unitDir, i);
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
cocos2d::Animate* EnemySkinnyTroll::animationWalk(){
	if(unitDir <=8){
		Vector<SpriteFrame *> walkFrames;

		//length of animation sequence
		float animationLength = 16.0f;

		for (int i = 14; i < 14 + animationLength; i++){
			auto *filename = __String::createWithFormat("EnemySkinnyTrollwalk%d00%d.png", unitDir, i);
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
void EnemySkinnyTroll::animationDie(){
	if(unitDir <=8){
		Vector<SpriteFrame *> dieFrames;

		//length of animation sequence
		int animationLength = 20;
		int start = 56;

		dieDuration = 0.04;

		for (int i = start; i < start + animationLength; i++){
			auto *filename = __String::createWithFormat("EnemySkinnyTrolldie%d00%d.png", unitDir, i);
			auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
			CCLOG("%s", filename->getCString());
			dieFrames.pushBack(wframe);
		}
		auto dieAnim = Animation::createWithSpriteFrames(dieFrames, dieDuration);
		auto animate = Animate::create(dieAnim);
		//auto waction = RepeatForever::create(animate);
		this->runAction(animate);
	}
}

