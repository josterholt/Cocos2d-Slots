#include "TestDelegate.h"
//#include "HelloWorldScene.h"
#include "ReelScene.h"
#include "Utilities.h"

USING_NS_CC;

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

AppDelegate::AppDelegate() {
	srand(time(NULL));
}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("MyCppGame", Rect(0, 0, 960, 640));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(960, 640, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->addSearchPath("res");

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
	auto scene = ReelScene::createScene();

    // run
    director->runWithScene(scene);

	// Unit testing
	Catch::Session session;
	session.run();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

TEST_CASE("TESTING")
{
	SECTION("RPGSLOT TEST") {
		cocos2d::log("Test 1");
		Scene *scene = ReelScene::createScene();
		Director::getInstance()->replaceScene(scene);
		ReelScene *layer = (ReelScene *)scene->getChildByTag(MAIN_LAYER);
		layer->myTestFunc();
	}

	SECTION("Match One Line") {
		std::vector<std::vector<int>> grid = { { 0, 0, 0 },{ 2, 1, 5 },{ 0, 3, 5 } };
		std::vector<MatchSequence> matches = linearPatternSearch(grid);
		// There is only one result
		assert(matches.size() == 1);

		// Match is first row
		assert(matches[0].matches[0].row == 0);
		assert(matches[0].matches[0].col == 0);

		assert(matches[0].matches[1].row == 0);
		assert(matches[0].matches[1].col == 1);

		assert(matches[0].matches[2].row == 0);
		assert(matches[0].matches[2].col == 2);
	}

	SECTION("Match Two Lines") {
		std::vector<std::vector<int>> grid = { { 0, 0, 0 },{ 1, 1, 1 },{ 0, 3, 5 } };
		std::vector<MatchSequence> matches = linearPatternSearch(grid);
		// There is only one result
		assert(matches.size() == 2);

		// Match first row
		assert(matches[0].matches[0].row == 0);
		assert(matches[0].matches[0].col == 0);

		assert(matches[0].matches[1].row == 0);
		assert(matches[0].matches[1].col == 1);

		assert(matches[0].matches[2].row == 0);
		assert(matches[0].matches[2].col == 2);

		// Match second row
		assert(matches[1].matches[0].row == 1);
		assert(matches[1].matches[0].col == 0);

		assert(matches[1].matches[1].row == 1);
		assert(matches[1].matches[1].col == 1);

		assert(matches[1].matches[2].row == 1);
		assert(matches[1].matches[2].col == 2);
	}

	SECTION("Match One Diagonal Lines") {
		std::vector<std::vector<int>> grid = { { 0, 3, 5 },{ 2, 0, 0 },{ 1, 3, 5 } };
		std::vector<MatchSequence> matches = linearPatternSearch(grid);
		// There is only one result
		assert(matches.size() == 1);

		// Match first row
		assert(matches[0].matches[0].row == 0);
		assert(matches[0].matches[0].col == 0);

		assert(matches[0].matches[1].row == 1);
		assert(matches[0].matches[1].col == 1);

		assert(matches[0].matches[2].row == 1);
		assert(matches[0].matches[2].col == 2);
	}

	SECTION("Mixed Match Lines") {
		std::vector<std::vector<int>> grid = { { 0, 0, 0 },{ 1, 3, 4 },{ 0, 1, 1 } };
		std::vector<MatchSequence> matches = linearPatternSearch(grid);
		// There is only one result
		assert(matches.size() == 2);

		// Match first row
		assert(matches[0].matches[0].row == 0);
		assert(matches[0].matches[0].col == 0);

		assert(matches[0].matches[1].row == 0);
		assert(matches[0].matches[1].col == 1);

		assert(matches[0].matches[2].row == 0);
		assert(matches[0].matches[2].col == 2);

		// Match second row
		assert(matches[1].matches[0].row == 1);
		assert(matches[1].matches[0].col == 0);

		assert(matches[1].matches[1].row == 2);
		assert(matches[1].matches[1].col == 1);

		assert(matches[1].matches[2].row == 2);
		assert(matches[1].matches[2].col == 2);
	}
}