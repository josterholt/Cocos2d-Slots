#include "HudSprite.h"

using namespace cocos2d;

HudSprite::HudSprite() {}
HudSprite::~HudSprite() {}

HudSprite* HudSprite::create() {
	HudSprite* hudSprite = new HudSprite();
	hudSprite->init();
	hudSprite->autorelease();
	return hudSprite;
}

bool HudSprite::init() {
	_status->setPosition(Vec2(280, 15));
	_status->setAnchorPoint(Vec2(0, 0));
	_status->setColor(cocos2d::Color3B(0, 0, 0));
	this->addChild(_status);

	_experience->setPosition(Vec2(20, 0));
	_experience->setAnchorPoint(Vec2(0, 0));
	_experience->setColor(cocos2d::Color3B(0, 0, 0));
	this->addChild(_experience);

	return true;
}

void HudSprite::setStatus(std::string status) {
	_status->setString(status);
}

void HudSprite::setExperience(int experience) {
	_experience->setString(std::to_string(experience));
}