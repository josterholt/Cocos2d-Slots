#ifndef _REELSPRITE_HPP_
#define _REELSPRITE_HPP_


#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <string>

class ReelSprite : public cocos2d::Sprite {
public:
	ReelSprite();
	//using cocos2d::Sprite::Sprite;
	//ReelSprite::ReelSprite(const std::string& filename);
	~ReelSprite();
	static ReelSprite* create(const std::string& filename);
	virtual bool init(const std::string& filename);
	void ReelSprite::startSpin();
	void ReelSprite::stopSpin();
	void ReelSprite::incrementSpin(float delta);
	bool _isSpinning = false;
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
};
#endif