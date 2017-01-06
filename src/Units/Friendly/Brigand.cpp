#include "src/Units/Friendly/Brigand.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/AttackObject.h"

USING_NS_CC;

Brigand::Brigand() {
}

Brigand::~Brigand() {
	CCLOG("Brigand WAS DELETED!!!!!!!!!!!");
}

Brigand* Brigand::create() {
	Brigand* pSprite = new Brigand();
	cPrint("making Brigand");
	auto *filename = __String::createWithFormat("Brigand1Walk00064.png");
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
	pSprite->setScale(0.75);
	pSprite->xHealthPos = 1.5;
	pSprite->yHealthPos = 1;

	pSprite->unitType = "Brigand";

	pSprite->scheduleUpdate();

	cPrint("Brigand Made");

	return pSprite;

}

Brigand* Brigand::create(cocos2d::Point tmp){
	Brigand* pSprite = Brigand::create();
	pSprite->setPosition(tmp);
	return pSprite;
}

void Brigand::removeFromLevel(){
	CCLOG("Brigand remove from level");
	this->getParent()->removeChild(this);
}

/*
 * Set attack animation
 */
void Brigand::animationAttack(){
	auto attackCallback = CallFunc::create([this]() {
		AttackObject* atk = AttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), attackDamage, 'm', pf);
		atk->initAttack();
	});

	int startFrame = 100;
	int endFrame = 117;
	numAttackFrames = endFrame - startFrame + 1;
	attackDuration = (attackSpeed - 0.1) / numAttackFrames;

	auto attackSeq = Sequence::create(DelayTime::create(attackDuration * 8), attackCallback, nullptr);
	this->runAction(attackSeq);

	Vector<SpriteFrame *> trollFrames;
	for (int i = startFrame; i <= endFrame; i++){
		auto *filename = __String::createWithFormat("Brigand1Attack%d0%d.png", unitDir, i);
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
cocos2d::Animate* Brigand::animationWalk(){
	if(unitDir <=8){
		Vector<SpriteFrame *> walkFrames;

		//length of animation sequence
		int startFrame = 64;
		int endFrame = 84;
		float animationLength = endFrame - startFrame + 1.0f;

		for (int i = startFrame; i <= endFrame; i++){
			auto *filename = __String::createWithFormat("Brigand1Walk%d00%d.png", unitDir, i);
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
void Brigand::animationDie(){
	if(unitDir <=8){
		Vector<SpriteFrame *> dieFrames;

		//length of animation sequence
		int startFrame = 666;
		int endFrame = 686;

		dieDuration = 1.3 / (endFrame - startFrame + 1);

		for (int i = startFrame; i <= endFrame; i++){
			auto *filename = __String::createWithFormat("Brigand1Die%d0%d.png", unitDir, i);
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
