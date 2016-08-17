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

	
	CCDrawNode* node = CCDrawNode::create();
	this->addChild(node, -1);
	node->drawSegment(CCPoint(0, 0), CCPoint(600, 600), 5.0f, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
	return true;
}

bool GraphicsScene::onTouchBegan(Touch* touch, Event* event)
{
	cocos2d::log("Testing");
	return true;
}