#include "src/Units/Ninja.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/AttackObjectNinjaStar.h"
#include "src/AttackObjects/AttackObjectNinjaFireball.h"

USING_NS_CC;

Ninja::Ninja() {
}

Ninja::~Ninja() {
	CCLOG("NINJA GUY WAS DELETED!!!!!!!!!!!");
	//delete tpf;
	//delete lStack;
	//this->getParent()->removeChild(this);
}

Ninja* Ninja::create() {
	Ninja* pSprite = new Ninja();
	pSprite->initWithFile("Units/Wizard/WizardWalk00000.png");
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	/*pSprite->tpf = new PathFinder<BasicUnit>(50, 50);
	pSprite->tpf->setTileX(32);
	pSprite->tpf->setTileY(32);
	*/
	pSprite->bfsMap = new BFS(50, 50);

	pSprite->health = pSprite->initHealth;
	pSprite->attackSpeed = pSprite->initAttackSpeed;

	pSprite->scheduleUpdate();

	return pSprite;

}

Ninja* Ninja::create(cocos2d::Point tmp){
	Ninja* pSprite = new Ninja();
	pSprite->initWithFile("Units/Wizard/WizardWalk00000.png");
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	/*pSprite->tpf = new PathFinder<BasicUnit>(50, 50);
	pSprite->tpf->setTileX(32);
	pSprite->tpf->setTileY(32);
	*/
	pSprite->bfsMap = new BFS(50, 50);

	pSprite->health = pSprite->initHealth;
	pSprite->attackSpeed = pSprite->initAttackSpeed;

	pSprite->scheduleUpdate();
	pSprite->setPosition(tmp);

	//pf->block(pSprite->convertToPf(tmp).x, pSprite->convertToPf(tmp).y);
	//pf->taken(pSprite->convertToPf(tmp).x, pSprite->convertToPf(tmp).y);
	//pf->setUnit(pSprite->convertToPf(tmp).x, pSprite->convertToPf(tmp).y, pSprite);

	return pSprite;

}

void Ninja::setBFSmap(){
	CCLOG("setBFSmap pf:%p tpf:%p", pf, tpf);
	bfsMap->setPathFinder(pf);
	CCLOG("set Path Finder");
	auto L = pf->getRows();
	auto W = pf->getCols();
	CCLOG("got rows and cols");
	for(int i = 0; i < L; i++){
		for(int j = 0; j < W; j++){
			if(pf->checkPermaBlock(i, j)){
				bfsMap->setBlocked(i, j);
			}
		}
	}
}

void Ninja::removeFromLevel(){
	CCLOG("Ninja remove from level");
	this->getParent()->removeChild(this);
}


/*
 * Set attack animation
 */
void Ninja::animationAttack(){
	AttackObjectNinjaStar *atk = AttackObjectNinjaStar::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm', pf);
	atk->initAttack();

	Vector<SpriteFrame *> attackFrames;
	for (int i = 0; i < 10; i++){
		auto *filename = __String::createWithFormat("WizardAttack%d000%d.png", unitDir, i);
		//CCLOG("%s", filename->getCString());
		auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
		attackFrames.pushBack(wframe);
		//CCLOG("Made troll %d", i);
	}

	auto *filename = __String::createWithFormat("WizardAttack%d0010.png", unitDir);
	//CCLOG("%s", filename->getCString());
	auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
	attackFrames.pushBack(wframe);

	auto wrunAnim = Animation::createWithSpriteFrames(attackFrames, attackDuration);
	auto animate = Animate::create(wrunAnim);
	this->runAction(animate);
}

/*
 * Set the walking animation
 */
cocos2d::Animate* Ninja::animationWalk(){
	if(unitDir <=8){
		Vector<SpriteFrame *> walkFrames;

		//length of animation sequence
		int animationLength = 7;
		int startFrame = 0;

		for (int i = startFrame; i <= animationLength; i++){
			auto *filename = __String::createWithFormat("WizardWalk%d000%d.png", unitDir, i);
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
void Ninja::animationDie(){
	if(unitDir <=8){
		Vector<SpriteFrame *> dieFrames;

		//length of animation sequence
		int animationLength = 8;
		int startFrame = 0;

		for (int i = startFrame; i <= animationLength; i++){
			auto *filename = __String::createWithFormat("WizardDie%d000%d.png", unitDir, i);
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

/*
 * Set fireball attack
 */
void Ninja::shootFireBall(){
	AttackObjectNinjaFireball *atk = AttackObjectNinjaFireball::create(this, fireBallLocation, 400, 'm', pf);
	atk->initAttack();
}


void Ninja::updateDelayedMove(){
	delayedMove();
	auto p = this->convertToPf(this->getPosition());
	//CCLOG("!lStack->empty() && !moving: %f %f", p.x, p.y);
	bfsMap->setStart((int)p.x, (int)p.y);
	bfsMap->solve();
	//CCLOG("BFS MAP");
	tempMoving = true;
	movedYet = true;
	badMove = 0;
}

/*void Ninja::attack(BasicUnit * attacker, int damage, char attackType){
	health -= damage;
	//CCLOG("%p Ninja WAS ATTACKEDDDD for %d damage", this, damage);
	CCLOG("%p's (Ninja) health: %d", this, health);
}*/

//Ranged
bool Ninja::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0 && unitToUnitDistance(this, getCurrentEnemy()) <= attackRange &&
			attackTravelRange >= pointToPointDistance(this->convertToPf(this->getPosition()), goalPosition)){
		return true;
	}
	return false;
}
