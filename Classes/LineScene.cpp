#include "LineScene.h"
#include "cocos2d.h"
#include <string>
#include "SimpleAudioEngine.h"

USING_NS_CC;

LineScene::LineScene()
{

}

Scene* LineScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LineScene::create();
	scene->addChild(layer);

	return scene;
}

void LineScene::onDraw()
{
	return;
}