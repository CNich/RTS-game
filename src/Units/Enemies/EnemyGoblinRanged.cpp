#include "src/Units/Enemies/EnemyGoblinRanged.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/RangedAttackObject.h"

USING_NS_CC;

EnemyGoblinRanged::EnemyGoblinRanged() {
}

EnemyGoblinRanged::~EnemyGoblinRanged() {
	CCLOG("EnemyGoblinRanged WAS DELETED!!!!!!!!!!!");
}

EnemyGoblinRanged* EnemyGoblinRanged::create() {
	EnemyGoblinRanged* pSprite = new EnemyGoblinRanged();
	auto *filename = __String::createWithFormat("GoblinRangedWalk00017.png");
	auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
	pSprite->initWithSpriteFrame(wframe);
	srand((unsigned) time(NULL));
	pSprite->autorelease();

	pSprite->movementRange = pSprite->initMovementRange;
	pSprite->initHealth = 200;
	pSprite->health = pSprite->initHealth;
	pSprite->attackDamage = pSprite->initAttackDamage;
	pSprite->attackRange = pSprite->initAttackRange;
	pSprite->walkingSpeed = pSprite->initWalkingSpeed;
	pSprite->loot = pSprite->initLoot;
	pSprite->attackSpeed = pSprite->initAttackSpeed;

	pSprite->scheduleUpdate();

	return pSprite;

}

EnemyGoblinRanged* EnemyGoblinRanged::create(cocos2d::Point tmp){
	EnemyGoblinRanged* pSprite = EnemyGoblinRanged::create();
	pSprite->setPosition(tmp);
	return pSprite;
}

void EnemyGoblinRanged::removeFromLevel(){
	CCLOG("EnemyGoblinRanged remove from level");
	this->getParent()->removeChild(this);
}

/*
 * Set attack animation
 */
void EnemyGoblinRanged::animationAttack(){
	auto attackCallback = CallFunc::create([this]() {
		RangedAttackObject* atk = RangedAttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), attackDamage, 'm', pf);
		atk->initAttack();
	});

	numAttackFrames = 22;
	attackDuration = (attackSpeed - 0.1) / numAttackFrames;

	//auto attackSeq = Sequence::create(DelayTime::create((attackSpeed - 0.1) / 2), attackCallback, nullptr);
	auto attackSeq = Sequence::create(DelayTime::create(attackDuration * 16), attackCallback, nullptr);
	this->runAction(attackSeq);

	Vector<SpriteFrame *> trollFrames;
	for (int i = 60; i < 60 + numAttackFrames; i++){
		auto *filename = __String::createWithFormat("GoblinRangedAttack%d00%d.png", unitDir, i);
		CCLOG("%s", filename->getCString());
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
cocos2d::Animate* EnemyGoblinRanged::animationWalk(){
	if(unitDir <=8){
		Vector<SpriteFrame *> walkFrames;

		//length of animation sequence
		float animationLength = 14.0f;

		for (int i = 17; i < 17 + animationLength; i++){
			auto *filename = __String::createWithFormat("GoblinRangedWalk%d00%d.png", unitDir, i);
			CCLOG("%s", filename->getCString());
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
void EnemyGoblinRanged::animationDie(){
	if(unitDir <=8){
		Vector<SpriteFrame *> dieFrames;

		//length of animation sequence
		int animationLength = 21;
		int start = 100;

		dieDuration = 0.04;

		for (int i = start; i < start + animationLength; i++){
			auto *filename = __String::createWithFormat("GoblinRangedDie%d0%d.png", unitDir, i);
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

