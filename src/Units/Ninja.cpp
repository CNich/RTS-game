#include "src/Units/Ninja.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/AttackObjectNinjaStar.h"

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

/*
void Ninja::update(float dt) {
	cocos2d::Point currentEL;
	if(currentEnemy != 0){
		currentEL = convertToPf(currentEnemy->getPosition());
		if(currentEnemyLocation.x != currentEL.x || currentEnemyLocation.y != currentEL.y){
			currentEnemyLocation = currentEL;
			currentEnemyMoved = true;
		}
	}

	//if dead, die
	if(dead){
		//figure out how to delete
		//delete this;

		//delete tpf;
		//delete lStack;
		if(removeFromPf){
			auto callback = CallFunc::create([this]() {
				delete tpf;
				delete lStack;
				pf->unblock(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);
				pf->untaken(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);
				pf->setUnitZero(convertToPf(this->getPosition()).x,	convertToPf(this->getPosition()).y);
				this->getParent()->removeChild(this);
			});

			auto seq = Sequence::create(DelayTime::create(3), callback, nullptr);
			this->runAction(seq);
			removeFromPf = false;
		}
		//this->getParent()->removeChild(this);
	}

	//check if dead
	else if(!dead && health <= 0){
		dead = true;
		//auto rotateTo = RotateTo::create(1.5, 90);
		//this->runAction(rotateTo);
		this->animationDie();
	}

	//if attacking, nothing should be done
	else if (!attacking){

		//if an enemy is within striking distance and the attack cooldown has expired,
		//attack the closest enemy (loaded from the parent scene)
		if(!attacking && currentEnemy != 0 && !currentEnemy->isDead() &&
				this->enemyIsAttackable()){
			attacking = true;
			//currentEnemyMoved = false;

			auto callback = CallFunc::create([this]() {
				//if(currentEnemy != 0 && !currentEnemy->isDead()){
				//	currentEnemy->attack(this, 40, 'a');
				//}
				attacking = false;
			});
			auto rotateTo = RotateTo::create(0.1, 0, 20.0f);
			auto rotateBack = RotateTo::create(0.1, 0, 0);
			auto seq = Sequence::create(DelayTime::create(2), callback,
					rotateTo, rotateBack, nullptr);
			this->runAction(seq);
			AttackObjectNinjaStar *atk = AttackObjectNinjaStar::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm', pf);
			//AttackObject* atk = AttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm', pf);
			atk->initAttack();
			lStack->reset();
		}

		/*
		else if(currentEnemy != 0 && unitToUnitDistance(this, currentEnemy) >= attackRange){
			currentEnemyIsCloseEnough = false;
			currentEnemyMoved = false;
			CCLOG("drp");
		}* /

		//else if(currentEnemyIsCloseEnough && !currentEnemyMoved && unitToUnitDistance(this, currentEnemy) < attackRange){
		else if(currentEnemy != 0 && currentEnemyMoved && unitToUnitDistance(this, currentEnemy) < attackRange){
			auto t = convertToPf(currentEnemy->getPosition());
			this->ASolve(t.x, t.y);
			currentEnemyMoved = false;
		}

		//the unit is in between movements and is not attacking
		//if the movement stack isn't empty and the unit isn't moving
		//i.e. in between moves, move to next location in stack
		else if (!lStack->empty() && !moving) {
			delayedMove();
			auto p = this->convertToPf(this->getPosition());
			//CCLOG("!lStack->empty() && !moving: %f %f", p.x, p.y);
			bfsMap->setStart((int)p.x, (int)p.y);
			bfsMap->solve();
			CCLOG("BFS MAP");
			tempMoving = true;
			movedYet = true;
			badMove = 0;
		}

		else if(goalPositionAsolve){
			goalPositionAsolve = false;
			this->ASolve(goalPosition.x, goalPosition.y);
		}

		/* if idleTrack is false then the unit should not be moving
		 * it should be waiting until the timer is up before trying
		 * to make it's next move* /

		//if the unit thinks it has nowhere to go but isn't at it's proper location
		//ie the movement stack is empty but it's not at its goal position
		//this may happen if there is no path to it's goal location at the time
		//it's pathfinder has been called

		//gets unit to find path to it's goal position
		/* A * /
		else if(lStack->empty() && idle == true && idleTrack == true){
			idle = false;
			badMove = 0;
			this->ASolve(goalPosition.x, goalPosition.y);
			//CCLOG("lStack->empty(): %d   %7.7f", lStack->empty(), dt);
		}

		//causes unit to wait for 1 second until trying *A* again
		/* B * /
		else if(lStack->empty() && movedYet == true && idle == false && idleTrack == true){
			badMove++;
			/* makes sure *A* is not going while* /
			idleTrack = false;
			auto callback = CallFunc::create([this]() {
				idle = true;
				idleTrack = true;
			});
			auto seq = Sequence::create(DelayTime::create(1), callback, nullptr);
			this->runAction(seq);
			//CCLOG("%d", badMove);
		}
	}

	//basically nothing is going on
	else if(lStack->empty() && !moving && tempMoving){
		badMove = 0;
		if(tempMoving){
			pf->block(convertToPf(this->getPosition()).x, convertToPf(this->getPosition()).y);
			tempMoving = false;
		}
	}
}
*/

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
