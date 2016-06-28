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


	_audioMgr = CocosDenshion::SimpleAudioEngine::getInstance();
	_audioMgr->preloadEffect("start-reel.mp3");
	_audioMgr->preloadEffect("stop-reel.mp3");

	return true;
}

/**
 * Returns 0 based index value of slot cell from strip based
 * on vertical location of reel on machine
 */
int ReelSprite::getCellValue(int num)
{
	// Check where first reel exists
	Vec2 reel_position1 = this->_reel1->getPosition();
	Vec2 reel_position2 = this->_reel2->getPosition();
	Vec2 container_position = this->getPosition();
	int slot_top_y = this->_cellHeight * 3;
	
	int scan_line_y = slot_top_y - reel_position1.y;

	if (scan_line_y <= 0) {
		scan_line_y = slot_top_y - reel_position2.y;
	}

	float location = floor((static_cast<float>(scan_line_y) / this->_cellHeight));
	/*
	if (scan_line_y > (location * this->_cellHeight)) {
		location -= 1;
	}
	*/

	if (_isSpinning) {
		//cocos2d::log("Container Position: %d", container_position.y); // This should always be 0
		cocos2d::log("Reel 1 Position: %d", reel_position1.y);
		cocos2d::log("Reel 2 Position: %d", reel_position2.y);
		cocos2d::log("Cell Height: %d", this->_cellHeight);
		cocos2d::log("Location Result: %d", 0.00 + (scan_line_y / this->_cellHeight));
		cocos2d::log("Location: %d", location);
	}
	
	return (int) this->_numCells - (location); // hard coded value to represent slot offset
}

float ReelSprite::getCellHeight()
{
	return _cellHeight;
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
	this->_audioMgr->playEffect("stop-reel.mp3", false, 1.0f, 1.0f, 1.0f);

	Vec2 position1 = this->_reel1->getPosition();
	Vec2 position2 = this->_reel2->getPosition();
	cocos2d::log("### STOP SPINNING ###");
	cocos2d::log("Reel 1 Position: %d", position1.y);
	cocos2d::log("Reel 2 Position: %d", position2.y);
	cocos2d::log("Cell Height: %d", this->_cellHeight);
}

void ReelSprite::incrementSpin(float delta)
{
	Vec2 position1 = this->_reel1->getPosition(); // Returns const vec2& ... why are we modifying this even though it's a const?
	position1.y -= this->speed * delta;

	Vec2 position2 = this->_reel2->getPosition(); // Returns const vec2& ... why are we modifying this even though it's a const?
	position2.y -= this->speed * delta;

	float height;

	height = this->_reel1->getBoundingBox().size.height;
	int top_y = position1.y + height;
	if (top_y < 0)
	{
		// Place sheet above secondary
		position1.y = position2.y + height;
	}
	this->_reel1->setPosition(position1);



	height = this->_reel2->getBoundingBox().size.height;
	top_y = position2.y + height;
	if (top_y < 0)
	{
		// Place sheet above first
		position2.y = position1.y + height;
	}
	this->_reel2->setPosition(position2);
}

/**
 Sets position based off of top row
*/
void ReelSprite::setSlotPosition(int position) {
	Vec2 position1 = this->_reel1->getPosition();
	cocos2d::log("Reel height: %d", this->_reel1->getBoundingBox().size.height);
	cocos2d::log("NumCells: %d", this->_numCells);

	int calc = (this->_numCells - (position + 3)) * this->_cellHeight;
	int reel_height = this->_reel1->getBoundingBox().size.height;
	position1.y = -calc;

	this->_reel1->setPosition(position1);

	Vec2 position2 = this->_reel2->getPosition();
	position2.y = position1.y - this->_reel1->getBoundingBox().size.height;
	this->_reel2->setPosition(position2);
}