#include "src/Units/EnemyBasicUnit.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/AttackObject.h"

USING_NS_CC;

EnemyBasicUnit::EnemyBasicUnit() {
}

EnemyBasicUnit::~EnemyBasicUnit() {
	CCLOG("EnemyBasicUnit WAS DELETED!!!!!!!!!!!");
	//delete tpf;
	//delete lStack;
	//this->getParent()->removeChild(this);
}

EnemyBasicUnit* EnemyBasicUnit::create() {
	EnemyBasicUnit* pSprite = new EnemyBasicUnit();
	auto *filename = __String::createWithFormat("TrollWalk00000.png");
	auto wframe = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename->getCString());
	pSprite->initWithSpriteFrame(wframe);
	srand((unsigned) time(NULL));
	pSprite->autorelease();
	/*pSprite->tpf = new PathFinder<BasicUnit>(50, 50);
	pSprite->tpf->setTileX(32);
	pSprite->tpf->setTileY(32);
	*/
	//pSprite->setScale(0.5);

	pSprite->scheduleUpdate();

	return pSprite;

}

EnemyBasicUnit* EnemyBasicUnit::create(cocos2d::Point tmp){
	EnemyBasicUnit* pSprite = create();
	pSprite->setPosition(tmp);
	return pSprite;

}

void EnemyBasicUnit::update(float dt) {
	cocos2d::Point currentEL;
	if(currentEnemy != 0){
		currentEL = convertToPf(currentEnemy->getPosition());
		if(currentEnemyLocation.x != currentEL.x || currentEnemyLocation.y != currentEL.y){
			currentEnemyLocation = currentEL;
			currentEnemyMoved = true;
		}
	}

	//consoleDebugStatement(cocos2d::__String::createWithFormat(
	//		"%d %d %d %d", attacking, moving, lStack->empty(), tempMoving));

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
				CCLOG("Unit Dead");
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
		this->stopAllActions();
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
				attacking = false;
			});
			lStack->reset();
			auto seq = Sequence::create(DelayTime::create(attackSpeed), callback, nullptr);
			this->runAction(seq);
			this->animationAttack();
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
		}

		//the unit is in between movements and is not attacking
		//if the movement stack isn't empty and the unit isn't moving
		//i.e. in between moves, move to next location in stack
		else if (!lStack->empty() && !moving) {
			if(tracked) CCLOG("between movements");
			delayedMove();
			tempMoving = true;
			movedYet = true;
			badMove = 0;
		}

		else if(lStack->empty() && !moving){
			auto pos = this->convertToPf(this->getPosition());
			std::string dir = pf->getBFSDir(pos.x, pos.y);
			auto bfsp = pf->getBFSParent(pos.x, pos.y);

			if(tracked) CCLOG("%d %s bfsMoving", consoleTrackNum, dir.c_str());

			auto BFSParentBlock = this->convertToPf(bfsp);
			//bool tracked = false;
			if(dir == "u"){
				if(tracked) CCLOG("u");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("u to ur");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("u to ul");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.y < pf->getBFSStart().y){
						t.y += pf->getTileY();
					} else{
						t.y -= pf->getTileY();
					}
					lStack->addFront(t);
					if(tracked) CCLOG("u to ?");
				}
			}

			else if(dir == "d"){
				if(tracked) CCLOG("d");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("d to dr");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("d to dl");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.y < pf->getBFSStart().y){
						t.y += pf->getTileY();
					} else{
						t.y -= pf->getTileY();
					}
					lStack->addFront(t);
					if(tracked) CCLOG("d to ?");
				}
			}

			else if(dir == "r"){
				if(tracked) CCLOG("r");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
					//auto t = this->getPosition();
					//t.y -= pf->getTileY();
					//lStack->addFront(t);
					if(tracked) CCLOG("r not changed");
				} else if(!pf->checkBlock(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("r to dr");
				}else if(!pf->checkBlock(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("r to ur");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.x < pf->getBFSStart().x){
						t.x += pf->getTileX();
					} else{
						t.x -= pf->getTileX();
					}
					lStack->addFront(t);
					if(tracked) CCLOG("r to ?");
				}
			}

			else if(dir == "l"){
				if(tracked) CCLOG("l");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("l to dl");
				}else if(!pf->checkBlock(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("l to ul");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.x < pf->getBFSStart().x){
						t.x += pf->getTileX();
					} else{
						t.x -= pf->getTileX();
					}
					lStack->addFront(t);
					if(tracked) CCLOG("l to ?");
				}
			}

			else if(dir == "ul"){
				if(tracked) CCLOG("ul");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("ul to l");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("ul to u");
				}
			}

			else if(dir == "ur"){
				if(tracked) CCLOG("ur");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("ur to r");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("ur to u");
				}
			}

			else if(dir == "dl"){
				if(tracked) CCLOG("dl");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("dl to l");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("dl to d");
				}
			}

			else if(dir == "dr"){
				if(tracked) CCLOG("dr");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("dr to r");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(tracked) CCLOG("dr to d");
				}
			}

			auto diff = bfsp - this->getPosition();
			if(tracked) CCLOG("%4.4f %4.4f", diff.x, diff.y);

/*
			if(dir == "u"){
				if(tracked) CCLOG("u");
				if(!pf->checkBlock(pos.x - 1, pos.y)){
					lStack->addFront(pf->getBFSParent(pos.x, pos.y));
				} else if(!pf->checkBlock(pos.x - 1, pos.y + 1)){
					pos.x -= 32;
					pos.y += 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("u to ur");
				} else{
					pos.x -= 32;
					pos.y -= 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("u to ul");
				}
			}

			else if(dir == "d"){
				if(tracked) CCLOG("d");
				if(!pf->checkBlock(pos.x + 1, pos.y)){
					lStack->addFront(pf->getBFSParent(pos.x, pos.y));
				} else if(!pf->checkBlock(pos.x + 1, pos.y + 1)){
					pos.x += 32;
					pos.y += 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("d to dr");
				} else{
					pos.x += 32;
					pos.y -= 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("d to dl");
				}
			}

			else if(dir == "r"){
				if(tracked) CCLOG("r");
				if(!pf->checkBlock(pos.x, pos.y + 1)){
					lStack->addFront(pf->getBFSParent(pos.x, pos.y));
				} else if(!pf->checkBlock(pos.x + 1, pos.y + 1)){
					pos.x += 32;
					pos.y += 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("r to dr");
				} else{
					pos.x -= 32;
					pos.y += 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("r to ur");
				}
			}

			else if(dir == "l"){
				if(tracked) CCLOG("l");
				if(!pf->checkBlock(pos.x, pos.y - 1)){
					lStack->addFront(pf->getBFSParent(pos.x, pos.y));
				} else if(!pf->checkBlock(pos.x + 1, pos.y - 1)){
					pos.x += 32;
					pos.y -= 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("l to dl");
				} else{
					pos.x -= 32;
					pos.y -= 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("l to ul");
				}
			}

			else if(dir == "ul"){
				if(tracked) CCLOG("ul");
				if(!pf->checkBlock(pos.x - 1, pos.y - 1)){
					lStack->addFront(pf->getBFSParent(pos.x, pos.y));
				} else if(!pf->checkBlock(pos.x - 1, pos.y)){
					pos.x -= 32;
					pos.y -= 0;
					lStack->addFront(pos);
					if(tracked) CCLOG("ul to u");
				} else{
					pos.x -= 0;
					pos.y -= 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("ul to l");
				}
			}

			else if(dir == "ur"){
				if(tracked) CCLOG("ur");
				if(!pf->checkBlock(pos.x - 1, pos.y + 1)){
					lStack->addFront(pf->getBFSParent(pos.x, pos.y));
				} else if(!pf->checkBlock(pos.x, pos.y + 1)){
					pos.x += 0;
					pos.y += 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("ur to r");
				} else{
					pos.x -= 32;
					pos.y -= 0;
					lStack->addFront(pos);
					if(tracked) CCLOG("ur to u");
				}
			}

			else if(dir == "dl"){
				if(tracked) CCLOG("dl");
				if(!pf->checkBlock(pos.x + 1, pos.y - 1)){
					lStack->addFront(pf->getBFSParent(pos.x, pos.y));
				} else if(!pf->checkBlock(pos.x + 1, pos.y)){
					pos.x += 32;
					pos.y += 0;
					lStack->addFront(pos);
					if(tracked) CCLOG("dl to d");
				} else{
					pos.x -= 0;
					pos.y -= 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("dl to l");
				}
			}

			else if(dir == "dr"){
				if(tracked) CCLOG("dr");
				if(!pf->checkBlock(pos.x + 1, pos.y + 1)){
					lStack->addFront(pf->getBFSParent(pos.x, pos.y));
				} else if(!pf->checkBlock(pos.x + 1, pos.y)){
					pos.x += 32;
					pos.y += 0;
					lStack->addFront(pos);
					if(tracked) CCLOG("dr to d");
				} else{
					pos.x -= 0;
					pos.y += 32;
					lStack->addFront(pos);
					if(tracked) CCLOG("dr to r");
				}
			}
			*/

		}
	}

	//basically nothing is going on
	else if(lStack->empty() && !moving && tempMoving){
		if(tracked) CCLOG("nothing going on");
		badMove = 0;
		if(tempMoving){
			pf->block(convertToPf(this->getPosition()).x, convertToPf(this->getPosition()).y);
			tempMoving = false;
		}
	}
}

void EnemyBasicUnit::removeFromLevel(){
	CCLOG("EnemyBasicUnit remove from level");
	this->getParent()->removeChild(this);
}

void EnemyBasicUnit::attack(BasicUnit * attacker, int damage, char attackType){
	health -= damage;
	//CCLOG("%p EnemyBasicUnit WAS ATTACKEDDDD for %d damage", this, damage);
	//CCLOG("%p's (EnemyBasicUnit) health: %d", this, health);
}

//Melee
bool EnemyBasicUnit::enemyIsAttackable(){
	if(this->getCurrentEnemy() != 0){
		auto enemyLoc = convertToPf(currentEnemy->getPosition());
		auto thisLoc = convertToPf(this->getPosition());
		if(abs(enemyLoc.x - thisLoc.x) <= 1 && abs(enemyLoc.y - thisLoc.y) <= 1){
			return true;
		}
	}
	return false;
}
