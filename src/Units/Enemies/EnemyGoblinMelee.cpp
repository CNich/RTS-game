#include "src/Units/Enemies/EnemyGoblinMelee.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/AttackObject.h"

USING_NS_CC;

EnemyGoblinMelee::EnemyGoblinMelee() {
}

EnemyGoblinMelee::~EnemyGoblinMelee() {
	CCLOG("EnemyGoblinMelee WAS DELETED!!!!!!!!!!!");
}

EnemyGoblinMelee* EnemyGoblinMelee::create() {
	EnemyGoblinMelee* pSprite = new EnemyGoblinMelee();
	cPrint("making EnemyGoblinMelee");
	auto *filename = __String::createWithFormat("GoblinMeleeWalk00014.png");
	auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
	pSprite->initWithSpriteFrame(wframe);
	srand((unsigned) time(NULL));
	pSprite->autorelease();

	pSprite->movementRange = pSprite->initMovementRange;
	pSprite->initHealth = pSprite->initMaxHealth;
	pSprite->health = pSprite->initMaxHealth;
	pSprite->attackDamage = pSprite->initAttackDamage;
	pSprite->attackRange = pSprite->initAttackRange;
	pSprite->walkingSpeed = pSprite->initWalkingSpeed;
	pSprite->attackSpeed = pSprite->initAttackSpeed;
	//pSprite->setScale(0.75);
	//pSprite->xHealthPos = 1.5;
	//pSprite->yHealthPos = 1;

	pSprite->unitType = "EnemyGoblinMelee";

	pSprite->scheduleUpdate();

	cPrint("EnemyGoblinMelee Made");

	return pSprite;

}

EnemyGoblinMelee* EnemyGoblinMelee::create(cocos2d::Point tmp){
	EnemyGoblinMelee* pSprite = EnemyGoblinMelee::create();
	pSprite->setPosition(tmp);
	return pSprite;
}

void EnemyGoblinMelee::removeFromLevel(){
	CCLOG("EnemyGoblinMelee remove from level");
	this->getParent()->removeChild(this);
}

/*
 * Set attack animation
 */
void EnemyGoblinMelee::animationAttack(){
	auto attackCallback = CallFunc::create([this]() {
		AttackObject* atk = AttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), attackDamage, 'm', pf);
		atk->initAttack();
	});

	int startFrame = 40;
	int endFrame = 57;
	numAttackFrames = endFrame - startFrame + 1;
	attackDuration = (attackSpeed - 0.1) / numAttackFrames;

	auto attackSeq = Sequence::create(DelayTime::create(attackDuration * 8), attackCallback, nullptr);
	this->runAction(attackSeq);

	Vector<SpriteFrame *> trollFrames;
	for (int i = startFrame; i <= endFrame; i++){
		auto *filename = __String::createWithFormat("GoblinMeleeAttack%d00%d.png", unitDir, i);
		//cPrint("%s", filename->getCString());
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
cocos2d::Animate* EnemyGoblinMelee::animationWalk(){
	if(unitDir <=8){
		Vector<SpriteFrame *> walkFrames;

		//length of animation sequence
		int startFrame = 14;
		int endFrame = 30;
		float animationLength = endFrame - startFrame + 1.0f;

		for (int i = startFrame; i <= endFrame; i++){
			auto *filename = __String::createWithFormat("GoblinMeleeWalk%d00%d.png", unitDir, i);
			//cPrint("%s", filename->getCString());
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
void EnemyGoblinMelee::animationDie(){
	if(unitDir <=8){
		Vector<SpriteFrame *> dieFrames;

		//length of animation sequence
		int startFrame = 75;
		int endFrame = 95;

		dieDuration = 1.3 / (endFrame - startFrame + 1);

		for (int i = startFrame; i <= endFrame; i++){
			auto *filename = __String::createWithFormat("GoblinMeleeDie%d00%d.png", unitDir, i);
			auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
			//cPrint("%s", filename->getCString());
			dieFrames.pushBack(wframe);
		}
		auto dieAnim = Animation::createWithSpriteFrames(dieFrames, dieDuration);
		auto animate = Animate::create(dieAnim);
		//auto waction = RepeatForever::create(animate);
		this->runAction(animate);
	}
}
