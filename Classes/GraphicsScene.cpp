#include "GraphicsScene.h"

USING_NS_CC;

Scene* GraphicsScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GraphicsScene::create();
	scene->addChild(layer);

	return scene;
}

bool GraphicsScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto sprite = Sprite::create("decepticon.png");
	sprite->setAnchorPoint(Vec2(0, 0));
	sprite->setPosition(0, 0);

	this->addChild(sprite, 0);

	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	//listener->onTouchBegan = CC_CALLBACK_2(GraphicsScene::onTouchBegan, this);
	
	listener->onTouchBegan = [](Touch* touch, Event* event) -> bool {
		cocos2d::log("Testing");
		return true;
	};
	

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

bool GraphicsScene::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("Testing");
	return true;
}