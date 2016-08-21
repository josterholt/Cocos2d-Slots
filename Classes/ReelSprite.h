#ifndef _REELSPRITE_HPP_
#define _REELSPRITE_HPP_

#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <string>
#include <functional>

class ReelSprite : public cocos2d::Layer {
public:
	ReelSprite();
	//using cocos2d::Sprite::Sprite;
	//ReelSprite::ReelSprite(const std::string& filename);
	~ReelSprite();
	static ReelSprite* create(const std::string& filename, std::vector<int> _cells);
	virtual bool init(const std::string& filename);
	void ReelSprite::startSpin(int interval_buffer, int stop_position);
	void ReelSprite::stopSpin();
	void ReelSprite::incrementSpin(float delta);
	float spinInterval = 0;
	bool _isSpinning = false;
	bool _isStopping = false;
	float yOffset = 111.0f;
	static enum slotNames { SEVEN, PLUM, ORANGE, CHERRY, BELL, STRAWBERRY, PARTY, LEMON };
	std::vector<int> cells;
	
	cocos2d::Label* debugSlotName1 = cocos2d::Label::createWithSystemFont("Undefined", "Helvetica", 60);
	cocos2d::Label* debugSlotName2 = cocos2d::Label::createWithSystemFont("Undefined", "Helvetica", 60);
	cocos2d::Label* debugSlotName3 = cocos2d::Label::createWithSystemFont("Undefined", "Helvetica", 60);

	int getCellValue(int num);
	float getCellHeight();
	void setSlotPosition(int position);

private:
	cocos2d::Sprite* _reel1;
	cocos2d::Sprite* _reel2;
	int speed = 1600;
	CocosDenshion::SimpleAudioEngine* _audioMgr;	
	int _numCells = 18; // Hardcoding for now
	int _stripHeight = 2514;
	float _cellHeight = 2514/18; // Hardcoding for now
	cocos2d::Sequence* _stopSequence;
};
#endif