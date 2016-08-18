#include "ReelSprite.h"
#include <math.h>
#include <functional>

using namespace cocos2d;

ReelSprite::ReelSprite() {}
ReelSprite::~ReelSprite() {}

ReelSprite* ReelSprite::create(const std::string& filename, std::vector<int> _cells)
{
	ReelSprite* mainSprite = new ReelSprite();

	mainSprite->init(filename);
	mainSprite->autorelease();
	mainSprite->cells = _cells; // @todo Check on how to properly handle this

	return mainSprite;
}

bool ReelSprite::init(const std::string& filename)
{
	this->_reel1 = Sprite::create(filename);
	this->_reel1->setAnchorPoint(Vec2(0, 0));
	this->_reel1->setPosition(0, 0);
	this->addChild(_reel1);

	this->_reel2 = Sprite::create(filename);
	this->_reel2->setAnchorPoint(Vec2(0, 0));
	this->_reel2->setPosition(0, _reel1->getBoundingBox().size.height);
	this->addChild(_reel2);
	
	Vec2 tmp = this->getPosition();
	debugSlotName1->setPosition(tmp.x + 100, tmp.y + (_cellHeight * 2.5));
	debugSlotName1->setColor(cocos2d::Color3B(0, 0, 0));
	this->addChild(debugSlotName1);

	debugSlotName2->setPosition(tmp.x + 100, tmp.y + (_cellHeight * 1.5));
	debugSlotName2->setColor(cocos2d::Color3B(0, 0, 0));
	this->addChild(debugSlotName2);

	debugSlotName3->setPosition(tmp.x + 100, tmp.y + _cellHeight * 0.5);
	debugSlotName3->setColor(cocos2d::Color3B(0, 0, 0));
	this->addChild(debugSlotName3);

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
	int slot_top_y = (this->_cellHeight * (2 - num));
	
	int scan_line_y = slot_top_y - reel_position1.y;

	if (scan_line_y <= 0 || scan_line_y > this->_stripHeight) {
		scan_line_y = slot_top_y - reel_position2.y;
	}

	int location = this->_numCells - ((static_cast<float>(scan_line_y) / this->_cellHeight));
	cocos2d::log("Location %d", location);
	return cells[location]; // hard coded value to represent slot offset
}

float ReelSprite::getCellHeight()
{
	return _cellHeight;
}

void ReelSprite::startSpin(int interval_buffer)
{
	this->spinInterval = (rand() % 2) + interval_buffer;
	//cocos2d::log("Spin interval %d", this->spinInterval);
	this->_isSpinning = true;
	auto scheduler = Director::getInstance()->getScheduler();

	scheduler->schedule([this](float dt) {
		Vec2 position = this->_reel1->getPosition();
		int slot = floorf((position.y - this->yOffset) / this->_cellHeight);
		float new_y = (this->_cellHeight * slot);
		new_y +=  this->yOffset;

		//cocos2d::log("new y: %s", std::to_string(new_y).c_str());
		if (position.y != new_y)
		{
			float y_diff = position.y - new_y;
			float velocity = abs(y_diff) / this->speed;

			MoveBy* moveBy1 = MoveBy::create(velocity, Vec2(0, -y_diff));		
			MoveBy* moveBy2 = MoveBy::create(velocity, Vec2(0, -y_diff));

			//cocos2d::log("### Sprite before move %s", std::to_string(this->_reel1->getPosition().y).c_str());
			//cocos2d::log("### Move by: %s", std::to_string(y_diff).c_str());

			_isStopping = true;
			CallFunc* callback = CallFunc::create(this, callfunc_selector(ReelSprite::stopSpin));
			_stopSequence = Sequence::create(Spawn::create(TargetedAction::create(this->_reel1, moveBy1), TargetedAction::create(this->_reel2, moveBy2)), callback, NULL);
			this->runAction(_stopSequence);
		}
		else {
			this->stopSpin();
		}
	}, this, this->spinInterval, false, 0.0f, false, "myCallbackKey");
}

void ReelSprite::stopSpin()
{
	if (_stopSequence->isDone()) {
		cocos2d::log("Sprite after move %s", std::to_string(this->_reel1->getPosition().y).c_str());
		this->_isSpinning = false;
		_isStopping = false;
		this->_audioMgr->playEffect("stop-reel.mp3", false, 1.0f, 1.0f, 1.0f);

		debugSlotName1->setString(std::to_string(getCellValue(0)));
		debugSlotName2->setString(std::to_string(getCellValue(1)));
		debugSlotName3->setString(std::to_string(getCellValue(2)));
	}
}

void ReelSprite::incrementSpin(float delta)
{
	if (_isSpinning && !_isStopping) {
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
}

/**
 Sets position based off of top row
*/
void ReelSprite::setSlotPosition(int position) {
	Vec2 position1 = this->_reel1->getPosition();

	int calc = (this->_numCells - (position + 3)) * this->_cellHeight;
	int reel_height = this->_reel1->getBoundingBox().size.height;
	position1.y = -calc;

	this->_reel1->setPosition(position1);

	Vec2 position2 = this->_reel2->getPosition();
	position2.y = position1.y - this->_reel1->getBoundingBox().size.height;
	this->_reel2->setPosition(position2);
}