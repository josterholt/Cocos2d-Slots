#include "ReelScene.h"
#include "ReelSprite.h"
#include "cocos2d.h"
#include <string>
#include "SimpleAudioEngine.h"

USING_NS_CC;

ReelScene::ReelScene() 
	: _slotGrid(1, std::vector<int>(3)),
	cocos2d::Layer()
{

}

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
	//_mask = Sprite::create("mask.png");
	//_mask->getTexture()->setAliasTexParameters();

	// Load reels
	/*
	_reel1 = Sprite::create("reel1a.png");
	_reel1->setAnchorPoint(Vec2(0, 0));
	_reel1->setPosition(0, 0);
	this->addChild(_reel1, 0);

	_reel2 = Sprite::create("reel1a.png");
	_reel2->setAnchorPoint(Vec2(0, 0));
	_reel2->setPosition(0, _reel1->getBoundingBox().size.height);
	this->addChild(_reel2, 0);
	*/
	_reel1 = ReelSprite::create("reel1.png");
	_reel1->setPosition(0, 0);
	this->addChild(_reel1, 0);

	_reel1->setSlotPosition(35);

	/*
	_reel2 = ReelSprite::create("reel2.png");
	_reel2->setPosition(267, 0);
	this->addChild(_reel2, 0);

	_reel3 = ReelSprite::create("reel3.png");
	_reel3->setPosition(534, 0);
	this->addChild(_reel3, 0);
	*/

	_reels.push_back(_reel1);
	/*
	_reels.push_back(_reel2);
	_reels.push_back(_reel3);
	*/


	auto overlay = Sprite::create("overlay.png");
	overlay->setAnchorPoint(Vec2(0, 0));
	overlay->setPosition(0, 0);
	this->addChild(overlay, 0);


	/**
	* Begin touch event handling
	*/
	_audioMgr = CocosDenshion::SimpleAudioEngine::getInstance();
	_audioMgr->preloadEffect("start-reel.mp3");
	_audioMgr->preloadEffect("stop-reel.mp3");

	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
		this->_audioMgr->playEffect("start-reel.mp3", false, 1.0f, 1.0f, 1.0f);
		this->_reel1->startSpin();
		/*
		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->schedule([this](float dt) {
			this->_reel2->startSpin();
		}, this, 0.3f, false, 0.0f, false, "myCallbackKey1");

		scheduler->schedule([this](float dt) {
			this->_reel3->startSpin();
		}, this, 0.5f, false, 0.0f, false, "myCallbackKey2");
		*/
		/*
		this->_isSpinning = true;
		audio->playEffect("start-reel.mp3", false, 1.0f, 1.0f, 1.0f);
		
		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->schedule([this, audio](float dt) {
			this->_isSpinning = false;
			audio->playEffect("stop-reel.mp3", false, 1.0f, 1.0f, 1.0f);
		}, this, 6.0f, false, 0.0f, false, "myCallbackKey");
		*/
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
	if (_reel1->_isSpinning)
	{
		_reel1->incrementSpin(delta);
	}
	/*
	if (_reel2->_isSpinning)
	{
		_reel2->incrementSpin(delta);
	}
	if (_reel3->_isSpinning)
	{
		_reel3->incrementSpin(delta);
	}
	*/

	updateSlotGrid();
}

void ReelScene::updateSlotGrid()
{
	int cell_num;
	for (int i = 0; i < _slotGrid.size(); ++i) {
		for (int h = 0; h < _slotGrid[i].size(); ++h) {
			int value = _reels[i]->getCellValue(h);
			_slotGrid[i][h] = value;
		}
	}
	//if (_reel1->_isSpinning) {
	cocos2d::log("updateSlotGrid: %d", _slotGrid[0][0]);
	//}
	return;
}