#pragma once

#include "cocos2d.h"
#include "ReelSprite.h"
#include "SimpleAudioEngine.h"
#include <vector>
#include <array>

class ReelScene : public cocos2d::Layer
{
public:
	ReelScene();
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onDraw();
	void update(float delta) override;
	void myTestFunc();
	CREATE_FUNC(ReelScene);
private:
	cocos2d::CustomCommand _customCommand;
	//cocos2d::GLProgram* shader;
	ReelSprite* _reel1;
	ReelSprite* _reel2;
	ReelSprite* _reel3;
	std::array<int, 3> stopCellPositions;
	cocos2d::CCDrawNode* _matchLine1;
	std::vector<ReelSprite*> _reels;
	std::vector<std::vector<int>> _slotGrid;
	cocos2d::Sprite* _mask;
	CocosDenshion::SimpleAudioEngine* _audioMgr;
	bool _isSpinning = false;
	bool _activeRound = false;
	void applyMask(cocos2d::Sprite* _sprite);
	void updateSlotGrid();
	void displayMatches();



};