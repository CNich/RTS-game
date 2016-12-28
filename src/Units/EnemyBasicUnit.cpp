#include "src/Units/EnemyBasicUnit.h"
#include "SimpleAudioEngine.h"
#include "stdlib.h"
#include "src/AttackObjects/AttackObject.h"
#include "src/Levels/InfoHud.h"

USING_NS_CC;

EnemyBasicUnit::EnemyBasicUnit() {
}

EnemyBasicUnit::~EnemyBasicUnit() {
	cPrint("EnemyBasicUnit WAS DELETED!!!!!!!!!!!");
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

	pSprite->movementRange = pSprite->initMovementRange;
	pSprite->initHealth = 320;
	pSprite->health = pSprite->initHealth;
	pSprite->attackDamage = pSprite->initAttackDamage;

	pSprite->scheduleUpdate();

	return pSprite;

}

EnemyBasicUnit* EnemyBasicUnit::create(cocos2d::Point tmp){
	EnemyBasicUnit* pSprite = create();
	pSprite->setPosition(tmp);
	return pSprite;

}

void EnemyBasicUnit::update(float dt) {
    //debug_decisionTree = true;

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
		if(removeFromPf){
			auto callback = CallFunc::create([this]() {
				delete tpf;
				delete lStack;

				auto die_pf = convertToPf(this->getPosition());
				cocos2d::Point delete_pf;
				for(int i = -1; i <= 1; i++){
					for(int j = -1; j <= 1; j++){
						cocos2d::Point p2_pf = {die_pf.x + i, die_pf.y + j};
						if(pf->checkBounds_Pf(p2_pf) && pf->getUnit(p2_pf.x, p2_pf.y) == this){
							delete_pf = p2_pf;
						}
					}
				}

				for(int i = -pfSize.x; i < pfSize.x; i++){
                    for(int j = -pfSize.y; j < pfSize.y; j++){
                        if(pf->getUnit(delete_pf.x + i, delete_pf.y + j) == this){
                            pf->unblock(delete_pf.x + i, delete_pf.y + j);
                            pf->untaken(delete_pf.x + i, delete_pf.y + j);
                            pf->setUnitZero(delete_pf.x + i, delete_pf.y + j);
                        }
                    }
				}

				cPrint("Unit Dead");
				this->getParent()->removeChild(this);
			});

			auto seq = Sequence::create(DelayTime::create(5), callback, nullptr);
			this->runAction(seq);
			removeFromPf = false;
			this->_infoHud->addGold(loot);
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
			cPrint("drp");
		}*/

		//else if(currentEnemyIsCloseEnough && !currentEnemyMoved && unitToUnitDistance(this, currentEnemy) < attackRange){
		else if(currentEnemy != 0 && currentEnemyMoved && unitToUnitDistance(this, currentEnemy) < movementRange){
			auto t = convertToPf(currentEnemy->getPosition());
			this->ASolve(t.x, t.y);
			currentEnemyMoved = false;
		}

		//the unit is in between movements and is not attacking
		//if the movement stack isn't empty and the unit isn't moving
		//i.e. in between moves, move to next location in stack
		else if (!lStack->empty() && !moving) {
			if(debug_decisionTree) cPrint("between movements");
			delayedMove();
			tempMoving = true;
			movedYet = true;
			badMove = 0;
		}

		else if(lStack->empty() && !moving){
			auto pos = this->convertToPf(this->getPosition());
			std::string dir = pf->getBFSDir(pos.x, pos.y);
			auto bfsp = pf->getBFSParent(pos.x, pos.y);

			if(debug_decisionTree) cPrint("%d %s bfsMoving", consoleTrackNum, dir.c_str());

			auto BFSParentBlock = this->convertToPf(bfsp);
			//bool debug_decisionTree = false;
			if(dir == "u"){
				if(debug_decisionTree) cPrint("u");
				if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("u to ur");
				}else if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("u to ul");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.y < pf->getBFSStart().y){
						t.y += pf->getTileY();
					} else{
						t.y -= pf->getTileY();
					}
					lStack->addFront(t);
					if(debug_decisionTree) cPrint("u to ?");
				}
			}

			else if(dir == "d"){
				if(debug_decisionTree) cPrint("d");
				if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("d to dr");
				}else if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("d to dl");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.y < pf->getBFSStart().y){
						t.y += pf->getTileY();
					} else{
						t.y -= pf->getTileY();
					}
					lStack->addFront(t);
					if(debug_decisionTree) cPrint("d to ?");
				}
			}

			else if(dir == "r"){
				if(debug_decisionTree) cPrint("r");
				if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
					//auto t = this->getPosition();
					//t.y -= pf->getTileY();
					//lStack->addFront(t);
					if(debug_decisionTree) cPrint("r not changed");
				} else if(!checkUnitSizeBlocked(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("r to dr");
				}else if(!checkUnitSizeBlocked(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("r to ur");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.x < pf->getBFSStart().x){
						t.x += pf->getTileX();
					} else{
						t.x -= pf->getTileX();
					}
					lStack->addFront(t);
					if(debug_decisionTree) cPrint("r to ?");
				}
			}

			else if(dir == "l"){
				if(debug_decisionTree) cPrint("l");
				if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!checkUnitSizeBlocked(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("l to dl");
				}else if(!checkUnitSizeBlocked(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("l to ul");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.x < pf->getBFSStart().x){
						t.x += pf->getTileX();
					} else{
						t.x -= pf->getTileX();
					}
					lStack->addFront(t);
					if(debug_decisionTree) cPrint("l to ?");
				}
			}

			else if(dir == "ul"){
				if(debug_decisionTree) cPrint("ul");
				if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!checkUnitSizeBlocked(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("ul to l");
				}else if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("ul to u");
				}
			}

			else if(dir == "ur"){
				if(debug_decisionTree) cPrint("ur");
				if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!checkUnitSizeBlocked(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("ur to r");
				}else if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("ur to u");
				}
			}

			else if(dir == "dl"){
				if(debug_decisionTree) cPrint("dl");
				if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!checkUnitSizeBlocked(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("dl to l");
				}else if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("dl to d");
				}
			}

			else if(dir == "dr"){
				if(debug_decisionTree) cPrint("dr");
				if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!checkUnitSizeBlocked(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("dr to r");
				}else if(!checkUnitSizeBlocked(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("dr to d");
				}
			}
        }

        /*
		else if(lStack->empty() && !moving){
			auto pos = this->convertToPf(this->getPosition());
			std::string dir = pf->getBFSDir(pos.x, pos.y);
			auto bfsp = pf->getBFSParent(pos.x, pos.y);

			if(debug_decisionTree) cPrint("%d %s bfsMoving", consoleTrackNum, dir.c_str());

			auto BFSParentBlock = this->convertToPf(bfsp);
			//bool debug_decisionTree = false;
			if(dir == "u"){
				if(debug_decisionTree) cPrint("u");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("u to ur");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("u to ul");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.y < pf->getBFSStart().y){
						t.y += pf->getTileY();
					} else{
						t.y -= pf->getTileY();
					}
					lStack->addFront(t);
					if(debug_decisionTree) cPrint("u to ?");
				}
			}

			else if(dir == "d"){
				if(debug_decisionTree) cPrint("d");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("d to dr");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("d to dl");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.y < pf->getBFSStart().y){
						t.y += pf->getTileY();
					} else{
						t.y -= pf->getTileY();
					}
					lStack->addFront(t);
					if(debug_decisionTree) cPrint("d to ?");
				}
			}

			else if(dir == "r"){
				if(debug_decisionTree) cPrint("r");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
					//auto t = this->getPosition();
					//t.y -= pf->getTileY();
					//lStack->addFront(t);
					if(debug_decisionTree) cPrint("r not changed");
				} else if(!pf->checkBlock(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("r to dr");
				}else if(!pf->checkBlock(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("r to ur");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.x < pf->getBFSStart().x){
						t.x += pf->getTileX();
					} else{
						t.x -= pf->getTileX();
					}
					lStack->addFront(t);
					if(debug_decisionTree) cPrint("r to ?");
				}
			}

			else if(dir == "l"){
				if(debug_decisionTree) cPrint("l");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("l to dl");
				}else if(!pf->checkBlock(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("l to ul");
				}else{
					auto t = this->getPosition();
					auto thisPos = this->convertToPf(this->getPosition());
					if(thisPos.x < pf->getBFSStart().x){
						t.x += pf->getTileX();
					} else{
						t.x -= pf->getTileX();
					}
					lStack->addFront(t);
					if(debug_decisionTree) cPrint("l to ?");
				}
			}

			else if(dir == "ul"){
				if(debug_decisionTree) cPrint("ul");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("ul to l");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("ul to u");
				}
			}

			else if(dir == "ur"){
				if(debug_decisionTree) cPrint("ur");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x - 1, BFSParentBlock.y)){
					bfsp.x -= pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("ur to r");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("ur to u");
				}
			}

			else if(dir == "dl"){
				if(debug_decisionTree) cPrint("dl");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("dl to l");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y - 1)){
					bfsp.y -= pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("dl to d");
				}
			}

			else if(dir == "dr"){
				if(debug_decisionTree) cPrint("dr");
				if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y)){
					lStack->addFront(bfsp);
				} else if(!pf->checkBlock(BFSParentBlock.x + 1, BFSParentBlock.y)){
					bfsp.x += pf->getTileX();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("dr to r");
				}else if(!pf->checkBlock(BFSParentBlock.x, BFSParentBlock.y + 1)){
					bfsp.y += pf->getTileY();
					lStack->addFront(bfsp);
					if(debug_decisionTree) cPrint("dr to d");
				}
			}

			auto diff = bfsp - this->getPosition();
			if(debug_decisionTree) cPrint("%4.4f %4.4f", diff.x, diff.y);

		}
		*/
	}

	//basically nothing is going on
	else if(lStack->empty() && !moving && tempMoving){
		if(debug_decisionTree) cPrint("nothing going on");
		badMove = 0;
		if(tempMoving){
			pf->block(convertToPf(this->getPosition()).x, convertToPf(this->getPosition()).y);
			tempMoving = false;
		}
	}
}

void EnemyBasicUnit::removeFromLevel(){
	cPrint("EnemyBasicUnit remove from level");
	this->getParent()->removeChild(this);
}

/*
void EnemyBasicUnit::attack(BasicUnit * attacker, int damage, char attackType){
	health -= damage;
	//cPrint("%p EnemyBasicUnit WAS ATTACKEDDDD for %d damage", this, damage);
	//cPrint("%p's (EnemyBasicUnit) health: %d", this, health);
}
*/

/*
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
*/
bool EnemyBasicUnit::enemyIsAttackable(){
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
