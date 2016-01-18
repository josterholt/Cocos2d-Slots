#pragma once

#include "cocos2d.h"

class GraphicsScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual bool init();
	CREATE_FUNC(GraphicsScene);
};