#include "ReelScene.h"
#include "ReelSprite.h"
#include "cocos2d.h"
#include <string>
#include "SimpleAudioEngine.h"
#include "Utilities.h"


USING_NS_CC;

ReelScene::ReelScene() 
	: _slotGrid(3, std::vector<int>(3)),
	cocos2d::Layer()
{

}

Scene* ReelScene::createScene()
{
	auto scene = Scene::create();
	auto layer = ReelScene::create();
	layer->setTag(MAIN_LAYER);
#ifdef UNIT_TESTING
	GameUtils::SetPresentLayer(layer, WHO_REEL_MAIN);
#endif
	scene->addChild(layer);

	return scene;
}

bool ReelScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	float x_offset = 81.0f;
	float _y_offset = 111.0f;

	_reel1 = ReelSprite::create("reel1-debug.png", {
		ReelSprite::slotNames::SEVEN,
		ReelSprite::slotNames::PLUM,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::PLUM,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::PLUM,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::PLUM,
		ReelSprite::slotNames::SEVEN,
		ReelSprite::slotNames::CHERRY,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::BELL,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::STRAWBERRY,
		ReelSprite::slotNames::SEVEN,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::PARTY,
		ReelSprite::slotNames::PLUM
	});
	_reel1->setPosition(Vec2(x_offset,_y_offset));
	//_reel1->yOffset = 0;
	this->addChild(_reel1, 0);

	_reel2 = ReelSprite::create("reel2.png", {
		ReelSprite::slotNames::SEVEN,
		ReelSprite::slotNames::BELL,
		ReelSprite::slotNames::PARTY,
		ReelSprite::slotNames::BELL,
		ReelSprite::slotNames::CHERRY,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::PLUM,
		ReelSprite::slotNames::CHERRY,
		ReelSprite::slotNames::BELL,
		ReelSprite::slotNames::PARTY,
		ReelSprite::slotNames::BELL,
		ReelSprite::slotNames::STRAWBERRY,
		ReelSprite::slotNames::PLUM,
		ReelSprite::slotNames::SEVEN,
		ReelSprite::slotNames::BELL,
		ReelSprite::slotNames::PARTY,
		ReelSprite::slotNames::BELL,
		ReelSprite::slotNames::STRAWBERRY
	});
	_reel2->setPosition(267 + x_offset, _y_offset);
	//_reel2->yOffset = 0;
	this->addChild(_reel2, 0);


	_reel3 = ReelSprite::create("reel3.png", {
		ReelSprite::slotNames::SEVEN,
		ReelSprite::slotNames::CHERRY,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::PARTY,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::LEMON,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::PLUM,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::PLUM,
		ReelSprite::slotNames::ORANGE,
		ReelSprite::slotNames::PLUM,
		ReelSprite::slotNames::BELL,
		ReelSprite::slotNames::STRAWBERRY,
		ReelSprite::slotNames::SEVEN,
		ReelSprite::slotNames::BELL,
		ReelSprite::slotNames::CHERRY,
		ReelSprite::slotNames::LEMON
	});
	_reel3->setPosition(534 + x_offset, _y_offset);
	//_reel3->yOffset = 0;
	this->addChild(_reel3, 0);
	
	_reels.push_back(_reel1);
	_reels.push_back(_reel2);
	_reels.push_back(_reel3);

	// Adding matching lines
	_matchLines.push_back(CCDrawNode::create());
	_matchLines.push_back(CCDrawNode::create());
	_matchLines.push_back(CCDrawNode::create());

	this->addChild(_matchLines[0]);
	this->addChild(_matchLines[1]);
	this->addChild(_matchLines[2]);


	// Add overlay
	auto overlay = Sprite::create("overlay.png");
	overlay->setAnchorPoint(Vec2(0, 0));
	overlay->setPosition(0, 0);
	this->addChild(overlay, 0);

	auto topGradient = Sprite::create("interface_gradient.png");
	topGradient->setPosition(0, 0);
	topGradient->setPosition(481, 523);
	topGradient->setRotation(180.0f);
	this->addChild(topGradient, 0);

	auto bottomGradient = Sprite::create("interface_gradient.png");
	bottomGradient->setPosition(481, 118);
	this->addChild(bottomGradient, 0);

	auto uiBottomPane = Sprite::create("ui_bottom_pane.png");
	uiBottomPane->setPosition(0, 0);
	uiBottomPane->setAnchorPoint(Vec2(0, 0));
	this->addChild(uiBottomPane, 1);

	auto uiSpinButton = Sprite::create("ui_spin_button.png");
	uiSpinButton->setPosition(785, 0);
	uiSpinButton->setAnchorPoint(Vec2(0, 0));
	this->addChild(uiSpinButton, 1);

	/**
	* Begin touch event handling
	*/
	_audioMgr = CocosDenshion::SimpleAudioEngine::getInstance();
	_audioMgr->preloadEffect("start-reel.mp3");
	_audioMgr->preloadEffect("stop-reel.mp3");

	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this, uiSpinButton](Touch* touch, Event* event) -> bool {
		Vec2 p = touch->getLocation();
		Rect rect = uiSpinButton->getBoundingBox();

		if (!rect.containsPoint(p)) {
			return false;
		}

		/**
		 * Pre-round routines
		 */
		for (CCDrawNode* node : _matchLines) {
			node->clear();
		}


		/**
		 * Start new round
		 */
		this->_activeRound = true;

		std::array<int, 3> stop_positions = getStopPositions(); // Would this be optimized by passing in allocated var?

		this->_audioMgr->playEffect("start-reel.mp3", false, 1.0f, 1.0f, 1.0f);
		this->_reel1->startSpin(rand() % 2 + 1.5f, stop_positions[0]);

		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->schedule([this, stop_positions](float dt) {
			this->_reel2->startSpin(this->_reel1->spinInterval, stop_positions[1]);
		}, this, 0.3f, false, 0.0f, false, "myCallbackKey1");

		scheduler->schedule([this, stop_positions](float dt) {
			this->_reel3->startSpin(this->_reel2->spinInterval, stop_positions[2]);
		}, this, 0.5f, false, 0.0f, false, "myCallbackKey2");

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

void ReelScene::onDraw()
{
	return;
}

void ReelScene::update(float delta)
{
	bool currently_spinning = false;

	if (_reel1->_isSpinning)
	{
		_reel1->incrementSpin(delta);
		currently_spinning = true;
	}

	if (_reel2->_isSpinning)
	{
		_reel2->incrementSpin(delta);
		currently_spinning = true;
	}

	if (_reel3->_isSpinning)
	{
		_reel3->incrementSpin(delta);
		currently_spinning = true;
	}

	updateSlotGrid();

	if (!currently_spinning && _activeRound) {
		displayMatches();
		_activeRound = false;
	}
}

void ReelScene::updateSlotGrid()
{
	size_t grid_size = _slotGrid.size();
	for (int col = 0; col < grid_size; ++col) {
		for (int row = 0; row < _slotGrid[col].size(); ++row) {
			int value = _reels[col]->getCellValue(row);
			_slotGrid[row][col] = value;
		}
	}
	return;
}

void ReelScene::displayMatches() {
	Vec2 line_offset = Vec2(80.0f, 111.0f);
	float cell_height = (2514 / 18);
	float row_width = 267;
	float cell_mid_height = cell_height * 0.5f;
	float cell_mid_width = row_width * 0.5f;
	float line_size = 2.0f;

	std::vector<cocos2d::CCDrawNode*>::iterator it = _matchLines.begin();

	std::vector<MatchSequence> sequences = linearPatternSearch(_slotGrid);
	for (MatchSequence sequence : sequences) {
		// Begging of column to center of first cell
		Vec2 column1 = sequence.matches[0];
		Vec2 column2 = sequence.matches[1];
		Vec2 column3 = sequence.matches[2];

		CCPoint line1_start = CCPoint(row_width * column1.x + line_offset.x, (cell_height * column1.y) + cell_mid_height + line_offset.y);
		CCPoint line1_end = CCPoint(row_width * column1.x + cell_mid_width + line_offset.x, (cell_height * column1.y) + cell_mid_height + line_offset.y);
		(*it)->drawSegment(line1_start, line1_end, line_size, Color4F(0.0f, 0.0f, 1.0f, 1.0f));

		CCPoint line2_end = CCPoint(row_width * column2.x + cell_mid_width + line_offset.x, (cell_height * column2.y) + cell_mid_height + line_offset.y);
		(*it)->drawSegment(line1_end, line2_end, line_size, Color4F(0.0f, 0.0f, 1.0f, 1.0f));

		CCPoint line3_start = CCPoint(row_width * column3.x + cell_mid_width + line_offset.x, (cell_height * column3.y) + cell_mid_height + line_offset.y);
		(*it)->drawSegment(line2_end, line3_start, line_size, Color4F(0.0f, 0.0f, 1.0f, 1.0f));

		CCPoint line4_end = CCPoint(row_width * column3.x + row_width + line_offset.x, (cell_height * column3.y) + cell_mid_height + line_offset.y);
		(*it)->drawSegment(line3_start, line4_end, line_size, Color4F(0.0f, 0.0f, 1.0f, 1.0f));

		if (it == std::end(_matchLines)) {
			continue;
		}
		std::next(it);
	}
}

void ReelScene::myTestFunc() {
	cocos2d::log("Func invoked!");
}