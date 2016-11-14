#ifndef _REELSCENE_HPP_
#define _REELSCENE_HPP_

#pragma once

#include "cocos2d.h"
#include "ReelSprite.h"
#include "HudSprite.h"
#include "SimpleAudioEngine.h"
#include <vector>
#include <array>
#include <map>

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
	typedef std::map<int, int> PayTableMap;
	static PayTableMap payTable; // Based on a sequence of single matching number
	//static PayTableMap payTable2 = { { 0, 1 } };


	static PayTableMap initPayTable() {
		// Initialize pay table
		PayTableMap tmp_map;
		tmp_map[ReelSprite::slotNames::CHERRY] = 1000;
		tmp_map[ReelSprite::slotNames::ORANGE] = 50;
		tmp_map[ReelSprite::slotNames::PARTY] = 300;
		tmp_map[ReelSprite::slotNames::PLUM] = 50;
		tmp_map[ReelSprite::slotNames::SEVEN] = 500;
		tmp_map[ReelSprite::slotNames::STRAWBERRY] = 300;
		tmp_map[ReelSprite::slotNames::BELL] = 200;
		return tmp_map;

	}


	//static int getpayTableValue(int slotValue);
		
private:
	cocos2d::CustomCommand _customCommand;
	ReelSprite* _reel1;
	ReelSprite* _reel2;
	ReelSprite* _reel3;
	std::array<int, 3> stopCellPositions;
	std::vector<cocos2d::CCDrawNode*> _matchLines;
	
	std::vector<ReelSprite*> _reels;
	std::vector<std::vector<int>> _slotGrid;
	cocos2d::Sprite* _mask;
	CocosDenshion::SimpleAudioEngine* _audioMgr;
	bool _isSpinning = false;
	bool _activeRound = false;
	cocos2d::ValueMap _profile;

	HudSprite* _hudSprite;


	void applyMask(cocos2d::Sprite* _sprite);
	void updateSlotGrid();
	void displayMatches();
	void _loadPlayerData();
	void _updateExperience(int xp);
};
#endif