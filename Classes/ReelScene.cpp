#include "ReelScene.h"
#include "ReelSprite.h"
#include "cocos2d.h"
#include <string>
#include "SimpleAudioEngine.h"

USING_NS_CC;

struct MatchSequence {
	std::vector<Vec2> matches;
	int weight = 0;
};

/*
 0, 0, 0
 0, 0, 0
 0, 0, 0
 */
std::vector<MatchSequence> linearPatternSearch(std::vector<std::vector<int>> grid) {
	cocos2d::log("Match check");
	cocos2d::log("%s %s %s", std::to_string(grid[0][0]).c_str(), std::to_string(grid[0][1]).c_str(), std::to_string(grid[0][2]).c_str());
	cocos2d::log("%s %s %s", std::to_string(grid[1][0]).c_str(), std::to_string(grid[1][1]).c_str(), std::to_string(grid[1][2]).c_str());
	cocos2d::log("%s %s %s", std::to_string(grid[2][0]).c_str(), std::to_string(grid[2][1]).c_str(), std::to_string(grid[2][2]).c_str());

	std::vector<MatchSequence> sequences;
	for (int x = 0; x < grid.size(); x++) {
		MatchSequence sequence;

		size_t grid_size = grid[x].size();
		for (int y = 0; y < grid_size; y++) {

			// Fall out if this is the first cell of a row or match sequence is broken
			if (y != 0) {
				cocos2d::log("(%d, %d) %d vs %d", x, y, grid[x][y], grid[x][y - 1]);
			}

			if (y == 0 || grid[x][y] != grid[x][y - 1]) {
				continue; // Stop matching this row, line has been broken
			}

			// A match has started or continued
			if (y == 1) {
				// Add previous cell to start new sequence
				sequence.matches.push_back(Vec2(y - 1, x)); // Row matches, log it
			}
			sequence.matches.push_back(Vec2(y, x)); // Row matches, log it
		}

		if (sequence.matches.size() == grid_size) {
			sequences.push_back(sequence);
		}
	}

	return sequences;
}

ReelScene::ReelScene() 
	: _slotGrid(3, std::vector<int>(3)),
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
		_activeRound = true;
		this->_audioMgr->playEffect("start-reel.mp3", false, 1.0f, 1.0f, 1.0f);
		this->_reel1->startSpin(rand() % 2 + 1.5f);

		auto scheduler = Director::getInstance()->getScheduler();
		scheduler->schedule([this](float dt) {
			this->_reel2->startSpin(this->_reel1->spinInterval);
		}, this, 0.3f, false, 0.0f, false, "myCallbackKey1");

		scheduler->schedule([this](float dt) {
			this->_reel3->startSpin(this->_reel2->spinInterval);
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

/*
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
}
*/

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
	for (int col = 0; col < _slotGrid.size(); ++col) {
		for (int row = 0; row < _slotGrid[col].size(); ++row) {
			int value = _reels[col]->getCellValue(row);
			_slotGrid[row][col] = value;
		}
	}
	//if (_reel1->_isSpinning) {
	//cocos2d::log("updateSlotGrid: %d", _slotGrid[0][0]);
	//cocos2d::log("updateSlotGrid: %d", _slotGrid[0][1]);
	//cocos2d::log("updateSlotGrid: %d", _slotGrid[0][2]);
	//}
	return;
}

void ReelScene::displayMatches() {
	Vec2 line_offset = Vec2(80.0f, 400.0f);
	float cell_height = (2514 / 18);
	float row_width = 267;
	float multiplier = 0.5f;
	float cell_mid_height = cell_height * 0.5f;
	float line_size = 2.0f;

	CCDrawNode* node1 = CCDrawNode::create();
	this->addChild(node1);

	std::vector<MatchSequence> sequences = linearPatternSearch(_slotGrid);
	for (MatchSequence sequence : sequences) {
		// Begging of column to center of first cell
		Vec2 column1 = sequence.matches[0];
		Vec2 column2 = sequence.matches[1];
		Vec2 column3 = sequence.matches[2];

		CCPoint line1_start = CCPoint(row_width * column1.x + line_offset.x, (cell_height * -column1.y) + cell_mid_height + line_offset.y);
		CCPoint line1_end = CCPoint(row_width * column1.x + (row_width * 0.5) + line_offset.x, (cell_height * -column1.y) + cell_mid_height + line_offset.y);
		node1->drawSegment(line1_start, line1_end, line_size, Color4F(0.0f, 0.0f, 1.0f, 1.0f));

		CCPoint line2_end = CCPoint(row_width * column2.x + (row_width * 0.5f) + line_offset.x, (cell_height * -column2.y) + cell_mid_height + line_offset.y);
		node1->drawSegment(line1_end, line2_end, line_size, Color4F(0.0f, 0.0f, 1.0f, 1.0f));

		CCPoint line3_start = CCPoint(row_width * column3.x + (row_width * 0.5f) + line_offset.x, (cell_height * -column3.y) + cell_mid_height + line_offset.y);
		node1->drawSegment(line2_end, line3_start, line_size, Color4F(0.0f, 0.0f, 1.0f, 1.0f));

		CCPoint line4_end = CCPoint(row_width * column3.x + row_width + line_offset.x, (cell_height * -column3.y) + cell_mid_height + line_offset.y);
		node1->drawSegment(line3_start, line4_end, line_size, Color4F(0.0f, 0.0f, 1.0f, 1.0f));
	}
}