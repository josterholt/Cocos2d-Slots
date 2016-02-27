#include "ReelSprite.h"
#include <math.h>

using namespace cocos2d;

ReelSprite::ReelSprite() {}

/*
ReelSprite::ReelSprite(const std::string& filename)
{
	
}
*/

ReelSprite::~ReelSprite() {}

ReelSprite* ReelSprite::create(const std::string& filename)
{
	ReelSprite* mainSprite = new ReelSprite();
	mainSprite->init(filename);

	return mainSprite;
}

bool ReelSprite::init(const std::string& filename)
{
	cocos2d::log(filename.c_str());
	this->_reel1 = Sprite::create(filename);
	this->_reel1->setAnchorPoint(Vec2(0, 0));
	this->_reel1->setPosition(0, 0);
	this->addChild(_reel1);

	this->_reel2 = Sprite::create(filename);
	this->_reel2->setAnchorPoint(Vec2(0, 0));
	this->_reel2->setPosition(0, _reel1->getBoundingBox().size.height);
	this->addChild(_reel2);


	audioMgr = CocosDenshion::SimpleAudioEngine::getInstance();
	audioMgr->preloadEffect("start-reel.mp3");
	audioMgr->preloadEffect("stop-reel.mp3");

	return true;
}

void ReelSprite::startSpin()
{
	this->_isSpinning = true;

	auto scheduler = Director::getInstance()->getScheduler();
	scheduler->schedule([this](float dt) {
		Vec2 position = this->_reel1->getPosition();
		float new_y = this->_cellHeight * floorf(position.y / this->_cellHeight);
		if (position.y != new_y)
		{
			//position.y = new_y;
			float y_diff = new_y - position.y;
			float velocity = abs(y_diff) / this->speed;
			MoveBy* moveBy1 = MoveBy::create(velocity, Vec2(0, y_diff));
			MoveBy* moveBy2 = MoveBy::create(velocity, Vec2(0, y_diff));
			//Sequence* sequence = Sequence::create(moveBy);
			//this->_reel1->runAction(sequence);
			this->_reel1->runAction(moveBy1);
			this->_reel2->runAction(moveBy2);
			this->stopSpin();
		}
		else {
			this->stopSpin();
		}
	}, this, 6.0f, false, 0.0f, false, "myCallbackKey");
}

void ReelSprite::stopSpin()
{
	this->_isSpinning = false;
	this->audioMgr->playEffect("stop-reel.mp3", false, 1.0f, 1.0f, 1.0f);

}

void ReelSprite::incrementSpin(float delta)
{
	Vec2 position1 = this->_reel1->getPosition(); // Returns const vec2& ... why are we modifying this even though it's a const?
	position1.y -= this->speed * delta;

	Vec2 position2 = this->_reel2->getPosition(); // Returns const vec2& ... why are we modifying this even though it's a const?
	position2.y -= this->speed * delta;

	int top_y = position1.y + this->_reel1->getBoundingBox().size.height;
	if (top_y < 0)
	{
		// Place sheet above secondary
		position1.y = position2.y + this->_reel2->getBoundingBox().size.height;
	}
	this->_reel1->setPosition(position1);




	top_y = position2.y + this->_reel2->getBoundingBox().size.height;
	if (top_y < 0)
	{
		// Place sheet above first
		position2.y = position1.y + this->_reel1->getBoundingBox().size.height;
	}
	this->_reel2->setPosition(position2);
}