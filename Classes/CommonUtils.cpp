#include "CommonUtils.h"

namespace CommonUtils {
	static Layer *present_layer = NULL;
	static int whos_running = 0;
	void SetPresentLayer(Layer *layer, int who)
	{
		whos_running = who;
		present_layer = layer;
	}
	Layer *GetPresentLayer()
	{
		return present_layer;
	}
	int GetWhosRunning()
	{
		return whos_running;
	}
}