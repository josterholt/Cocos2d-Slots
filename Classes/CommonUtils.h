#ifndef CommonUtils
#define CommonUtils
#include "cocos2d.h"
USING_NS_CC;
#define MAIN_LAYER 10000
#define WHO_REEL_MAIN 1
namespace CommonUtils
{
#ifdef UNIT_TESTING
	void SetPresentLayer(Layer *, int);
	Layer *GetPresentLayer();
	int GetWhosRunning();
#endif
}
#endif
