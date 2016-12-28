#include "src/Units/Enemies/EnemyTrollHammer.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/AttackObject.h"

USING_NS_CC;

EnemyTrollHammer::EnemyTrollHammer() {
}

EnemyTrollHammer::~EnemyTrollHammer() {
	CCLOG("EnemyTrollHammer WAS DELETED!!!!!!!!!!!");
}

EnemyTrollHammer* EnemyTrollHammer::create() {
	EnemyTrollHammer* pSprite = new EnemyTrollHammer();
	cPrint("making troll hammer");
	auto *filename = __String::createWithFormat("TrollHammerWalk00050.png");
	auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
	pSprite->initWithSpriteFrame(wframe);
	srand((unsigned) time(NULL));
	pSprite->autorelease();

	pSprite->movementRange = pSprite->initMovementRange;
	pSprite->initHealth = 1000;
	pSprite->health = pSprite->initHealth;
	pSprite->attackDamage = pSprite->initAttackDamage;
	pSprite->attackRange = pSprite->initAttackRange;
	pSprite->walkingSpeed = pSprite->initWalkingSpeed;
	pSprite->attackSpeed = pSprite->initAttackSpeed;
	pSprite->loot = pSprite->initLoot;
	pSprite->setPfSize(2,2);

	pSprite->setScale(1.5);

	//auto t = cocos2d::Sprite::create("test-4-square.png");
	//pSprite->addChild(t, 1);
	//t->setPosition(128, 128);

	pSprite->scheduleUpdate();

	return pSprite;

}

EnemyTrollHammer* EnemyTrollHammer::create(cocos2d::Point tmp){
	EnemyTrollHammer* pSprite = EnemyTrollHammer::create();
	pSprite->setPosition(tmp);
	return pSprite;
}

void EnemyTrollHammer::removeFromLevel(){
	CCLOG("EnemyTrollHammer remove from level");
	this->getParent()->removeChild(this);
}

/*
 * Set attack animation
 */
void EnemyTrollHammer::animationAttack(){
	auto attackCallback = CallFunc::create([this]() {
		AttackObject* atk = AttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), attackDamage, 'm', pf);
		atk->initAttack();
	});

	int startFrame = 110;
	int endFrame = 130;
	numAttackFrames = endFrame - startFrame + 1;
	attackDuration = (attackSpeed - 0.1) / numAttackFrames;

	auto attackSeq = Sequence::create(DelayTime::create(attackDuration * 8), attackCallback, nullptr);
	this->runAction(attackSeq);

	Vector<SpriteFrame *> trollFrames;
	for (int i = startFrame; i <= endFrame; i++){
		auto *filename = __String::createWithFormat("TrollHammerAttack%d0%d.png", unitDir, i);
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
cocos2d::Animate* EnemyTrollHammer::animationWalk(){
	if(unitDir <=8){
		Vector<SpriteFrame *> walkFrames;

		//length of animation sequence
		int startFrame = 50;
		int endFrame = 69;
		float animationLength = endFrame - startFrame + 1.0f;

		for (int i = startFrame; i <= endFrame; i++){
			auto *filename = __String::createWithFormat("TrollHammerWalk%d00%d.png", unitDir, i);
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
void EnemyTrollHammer::animationDie(){
	if(unitDir <=8){
		Vector<SpriteFrame *> dieFrames;

		//length of animation sequence
		int startFrame = 150;
		int endFrame = 165;

		dieDuration = 1.3 / (endFrame - startFrame + 1);

		for (int i = startFrame; i <= endFrame; i++){
			auto *filename = __String::createWithFormat("TrollHammerDie%d0%d.png", unitDir, i);
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


bool EnemyTrollHammer::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0){
		auto enemyLoc = convertToPf(currentEnemy->getPosition());
		auto thisLoc = convertToPf(this->getPosition());

		for(int ei = 0; ei < getCurrentEnemy()->getPfSize().x; ei++){
            for(int ej = 0; ej < getCurrentEnemy()->getPfSize().y; ej++){
                for(int i = 0; i < pfSize.x; i++){
                    for(int j = 0; j < pfSize.y; j++){
                        if(abs(enemyLoc.x + ei - thisLoc.x + i) <= pfSize.x && abs(enemyLoc.y + ej - thisLoc.y + j) <= pfSize.y){
                            return true;
                        }
                    }
                }
            }
		}
	}
	return false;
}
