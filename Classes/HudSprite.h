#ifndef _HUDSPRITE_HPP_
#define _HUDSPRITE_HPP_

#pragma once

#include "cocos2d.h"

class HudSprite : public cocos2d::Layer {
public:
	HudSprite();
	~HudSprite();
	static HudSprite* create();
	virtual bool init();
	void setExperience(int experience);
	void setStatus(std::string status);
private:
	cocos2d::Label* _experience = cocos2d::Label::createWithSystemFont("0", "Helvetica", 60);
	cocos2d::Label* _status = cocos2d::Label::createWithSystemFont("", "Helvetica", 30);

};
#endif