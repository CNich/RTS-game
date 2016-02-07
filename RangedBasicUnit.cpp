#include "RangedBasicUnit.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "RangedAttackObject.h"

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
	pSprite->initWithFile("030.png");
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	pSprite->tpf = new PathFinder<BasicUnit>(50, 50);
	pSprite->tpf->setTileX(32);
	pSprite->tpf->setTileY(32);
	pSprite->setScale(0.5);

	pSprite->scheduleUpdate();

	return pSprite;

}

RangedBasicUnit* RangedBasicUnit::create(cocos2d::Point tmp){
	RangedBasicUnit* pSprite = new RangedBasicUnit();
	pSprite->initWithFile("030.png");
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	pSprite->tpf = new PathFinder<BasicUnit>(50, 50);
	pSprite->tpf->setTileX(32);
	pSprite->tpf->setTileY(32);
	pSprite->setScale(0.5);

	pSprite->scheduleUpdate();
	pSprite->setPosition(tmp);

	//pf->block(pSprite->convertToPf(tmp).x, pSprite->convertToPf(tmp).y);
	//pf->taken(pSprite->convertToPf(tmp).x, pSprite->convertToPf(tmp).y);
	//pf->setUnit(pSprite->convertToPf(tmp).x, pSprite->convertToPf(tmp).y, pSprite);

	return pSprite;

}

void RangedBasicUnit::update(float dt) {
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
		auto rotateTo = RotateTo::create(1.5, 90);
		this->runAction(rotateTo);
	}

	//if attacking, nothing should be done
	else if (!attacking){

		//if an enemy is within striking distance and the attack cooldown has expired,
		//attack the closest enemy (loaded from the parent scene)
		if(!attacking && currentEnemy != 0 && !currentEnemy->isDead() &&
				this->enemyIsAttackable()){
			attacking = true;
			//currentEnemyMoved = false;
			CCLOG("ATTACKED");
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
			RangedAttackObject* atk = RangedAttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm', pf);
			//AttackObject* atk = AttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm', pf);
			atk->initAttack();
			lStack->reset();
		}

		/*
		else if(currentEnemy != 0 && unitToUnitDistance(this, currentEnemy) >= attackRange){
			currentEnemyIsCloseEnough = false;
			currentEnemyMoved = false;
			CCLOG("drp");
		}*/

		//else if(currentEnemyIsCloseEnough && !currentEnemyMoved && unitToUnitDistance(this, currentEnemy) < attackRange){
		else if(currentEnemy != 0 && currentEnemyMoved && unitToUnitDistance(this, currentEnemy) < attackRange){
			auto t = convertToPf(currentEnemy->getPosition());
			this->ASolve(t.x, t.y);
			currentEnemyMoved = false;
			CCLOG("doooops");
		}

		//the unit is in between movements and is not attacking
		//if the movement stack isn't empty and the unit isn't moving
		//i.e. in between moves, move to next location in stack
		else if (!lStack->empty() && !moving) {
			delayedMove();
			tempMoving = true;
			movedYet = true;
			badMove = 0;
		}

		/* if idleTrack is false then the unit should not be moving
		 * it should be waiting until the timer is up before trying
		 * to make it's next move*/

		//if the unit thinks it has nowhere to go but isn't at it's proper location
		//ie the movement stack is empty but it's not at its goal position
		//this may happen if there is no path to it's goal location at the time
		//it's pathfinder has been called

		//gets unit to find path to it's goal position
		/* A */
		else if(lStack->empty() && idle == true && idleTrack == true){
			idle = false;
			badMove = 0;
			this->ASolve(goalPosition.x, goalPosition.y);
			//CCLOG("lStack->empty(): %d   %7.7f", lStack->empty(), dt);
		}

		//causes unit to wait for 1 second until trying *A* again
		/* B */
		else if(lStack->empty() && movedYet == true && idle == false && idleTrack == true){
			badMove++;
			/* makes sure *A* is not going while*/
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

/*void RangedBasicUnit::update(float dt) {
	//check if dead
	if(!dead && health <= 0){
		dead = true;
		auto rotateTo = RotateTo::create(1.5, 90);
		this->runAction(rotateTo);
	}
	//if dead, die
	else if(dead){
		//figure out how to delete
		//delete this;
	}
	//if an enemy is within striking distance and the attack cooldown has expired,
	//attack the closest enemy (loaded from the parent scene)
	else if(!attacking && currentEnemy != 0 && !currentEnemy->isDead() &&
			this->enemyIsAttackable()){
		attacking = true;
		CCLOG("ATTACKED");
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
		RangedAttackObject* atk = RangedAttackObject::create(this, this->convertToPf(this->getCurrentEnemy()->getPosition()), 40, 'm', pf);
		CCLOG("%p THIS WAS SENT", this);
		atk->initAttack();
	} else if (!lStack->empty() && !moving) {
		delayedMove();
		tempMoving = true;
		movedYet = true;
		badMove = 0;
	} else if(lStack->empty() && idle == true && idleTrack == true){
		idle = false;
		badMove = 0;
		this->ASolve(goalPosition.x, goalPosition.y);
		//CCLOG("lStack->empty(): %d   %7.7f", lStack->empty(), dt);
	} else if(lStack->empty() && movedYet == true && idle == false && idleTrack == true){
		badMove++;
		idleTrack = false;
		auto callback = CallFunc::create([this]() {
			idle = true;
			idleTrack = true;
		});
		auto seq = Sequence::create(DelayTime::create(1), callback, nullptr);
		this->runAction(seq);
		//CCLOG("%d", badMove);
	} else if(lStack->empty() && !moving && tempMoving){
		badMove = 0;
		if(tempMoving){
			pf->block(convertToPf(this->getPosition()).x, convertToPf(this->getPosition()).y);
			tempMoving = false;
		}
	}
}*/

//Ranged
bool RangedBasicUnit::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0 && unitToUnitDistance(this, getCurrentEnemy()) <= 6){
		return true;
	}
	return false;
}
