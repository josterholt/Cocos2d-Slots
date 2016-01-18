#include "ReelScene.h"
#include <string>
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* ReelScene::createScene()
{
	auto scene = Scene::create();
	auto layer = ReelScene::create();
	scene->addChild(layer);

	return scene;
}

bool ReelScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// Load mask
	_mask = Sprite::create("mask.png");
	_mask->getTexture()->setAliasTexParameters();

	// Load reels
	_reel1 = Sprite::create("reel1.png");
	_reel1->setAnchorPoint(Vec2(0, 0));
	_reel1->setPosition(0, 0);
	this->addChild(_reel1, 0);
	applyMask(_reel1);

	_reel2 = Sprite::create("reel1.png");
	_reel2->setAnchorPoint(Vec2(0, 0));
	_reel2->setPosition(0, _reel1->getBoundingBox().size.height);
	this->addChild(_reel2, 0);
	applyMask(_reel2);

	auto overlay = Sprite::create("overlay.png");
	overlay->setAnchorPoint(Vec2(0, 0));
	overlay->setPosition(0, 0);
	this->addChild(overlay, 0);


	/**
	* Begin touch event handling
	*/
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect("start-reel.mp3");
	audio->preloadEffect("stop-reel.mp3");
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	
	listener->onTouchBegan = [this, audio](Touch* touch, Event* event) -> bool {
		this->_isSpinning = true;
		audio->playEffect("start-reel.mp3", false, 1.0f, 1.0f, 1.0f);
		
		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->schedule([this, audio](float dt) {
			this->_isSpinning = false;
			audio->playEffect("stop-reel.mp3", false, 1.0f, 1.0f, 1.0f);
		}, this, 6.0f, false, 0.0f, false, "myCallbackKey");
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	/**
	* End touch event handling
	*/

	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(ReelScene::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_customCommand);

	this->scheduleUpdate();

	return true;
}

void ReelScene::applyMask(Sprite* _sprite)
{
	// Load fragment shader
	FileUtils* fileUtils = FileUtils::getInstance();
	std::string vertexSource = ccPositionTextureColor_noMVP_vert;
	std::string fragSource = fileUtils->getStringFromFile(fileUtils->fullPathForFilename("Mask.fragmentshader"));

	GLProgram* shader = _sprite->getShaderProgram();
	shader->initWithByteArrays(vertexSource.c_str(), fragSource.c_str());
	shader->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
	shader->bindAttribLocation(cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	shader->link();

	CHECK_GL_ERROR_DEBUG();
	shader->updateUniforms();
	CHECK_GL_ERROR_DEBUG();

	int maskTexUniformLoc = shader->getUniformLocationForName("u_alphaMaskTexture");
	shader->setUniformLocationWith1i(maskTexUniformLoc, 1);

	glActiveTexture(GL_TEXTURE1); // This is presumptuous that our mask will always be TEXTURE1
	glBindTexture(GL_TEXTURE_2D, _mask->getTexture()->getName());
	glActiveTexture(GL_TEXTURE0);

	/*
	// Releasing shader causes program to crash on close
	shader->release();
	*/
}

void ReelScene::onDraw()
{
	return;
}

void ReelScene::update(float delta)
{
	if (_isSpinning)
	{
		incrementSpin(delta);
	}
}

void ReelScene::incrementSpin(float delta)
{
	cocos2d::Vec2 position1 = _reel1->getPosition(); // Returns const vec2& ... why are we modifying this even though it's a const?
	position1.y -= 1200 * delta;
	
	cocos2d::Vec2 position2 = _reel2->getPosition(); // Returns const vec2& ... why are we modifying this even though it's a const?
	position2.y -= 1200 * delta;
	
	int top_y = position1.y + _reel1->getBoundingBox().size.height;
	if (top_y < 0)
	{
		// Place sheet above secondary
		position1.y = position2.y + _reel2->getBoundingBox().size.height;
	}
	_reel1->setPosition(position1);




	top_y = position2.y + _reel2->getBoundingBox().size.height;
	if (top_y < 0)
	{
		// Place sheet above first
		position2.y = position1.y + _reel1->getBoundingBox().size.height;
	}
	_reel2->setPosition(position2);
}