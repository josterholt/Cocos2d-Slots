#pragma once

#include "cocos2d.h"
#include "ReelSprite.h"
#include "SimpleAudioEngine.h"
#include <vector>

class LineScene : public cocos2d::Layer
{
public:
	LineScene();
	static cocos2d::Scene* createScene();
	virtual void onDraw();
	CREATE_FUNC(LineScene);
};