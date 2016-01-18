#pragma once

#include "cocos2d.h"

class ReelScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void onDraw();
	void update(float delta) override;
	CREATE_FUNC(ReelScene);
private:
	cocos2d::CustomCommand _customCommand;
	//cocos2d::GLProgram* shader;
	cocos2d::Sprite* _reel1;
	cocos2d::Sprite* _reel2;
	cocos2d::Sprite* _mask;
	void ReelScene::applyMask(cocos2d::Sprite* _sprite);
};