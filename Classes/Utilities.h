#ifndef Utils
#define Utils

#include "cocos2d.h"
#include <vector>
#include <array>

USING_NS_CC;

#define MAIN_LAYER 10000
#define WHO_REEL_MAIN 1

struct SlotsMatch {
	SlotsMatch(int _row, int _col) : row(_row), col(_col) {};
	int row;
	int col;
};

struct MatchSequence {
	std::vector<SlotsMatch> matches;
	int weight = 0;
};

void recursiveMatch(std::vector<std::vector<int>>& grid, std::vector<MatchSequence>& matches, MatchSequence frag_match, int row, int col, int next_row);

std::vector<MatchSequence> linearPatternSearch(std::vector<std::vector<int>> grid);
std::array<int, 3> getStopPositions();

// Other variables are outside of namespace due to incomplete factoring
namespace GameUtils
{
#ifdef UNIT_TESTING
	void SetPresentLayer(Layer *, int);
	Layer *GetPresentLayer();
	int GetWhosRunning();
#endif
}
#endif