#include "Utilities.h"
#include "cocos2d.h"
#include "ReelScene.h"
#include <vector>
#include <array>

USING_NS_CC;
#define MAX_SLOT_MATCHES 3

std::vector<MatchSequence> linearPatternSearch(std::vector<std::vector<int>> grid) {
	std::vector<MatchSequence> sequences;

	MatchSequence fragment;
	recursiveMatch(grid, sequences, fragment, 0, 0, 0);
	recursiveMatch(grid, sequences, fragment, 1, 0, 0);
	recursiveMatch(grid, sequences, fragment, 2, 0, 1);

	return sequences;
}

/**
 * Given the current position, find all matches
 * @todo possible optimization of returning object
 */
void recursiveMatch(std::vector<std::vector<int>>& grid, std::vector<MatchSequence>& matches, MatchSequence frag_match, int row, int col, int next_row) {
	cocos2d::log("New recursiveMatch %d %d %d", row, col, next_row);
	int next_col = col + 1;

	// Fall out if this is an invalid row / col
	if (row < 0 || (grid.size() - 1) < row || next_row < 0 || (grid.size() - 1) < next_row || col < 0 || (grid[0].size() - 1) < col || (grid[0].size() - 1) < next_col) {
		cocos2d::log("Fall out A %d %d %d", row, col, next_row);
		return; // Possibly send null to signify end of line
	}

	if (next_row == row || next_row == row - 1) {
		cocos2d::log("recursiveMatch A (%d, %d) %d, %d, %d", row, col, row, col, next_row + 1);
		recursiveMatch(grid, matches, frag_match, row, col, next_row + 1);
	}

	// Check for all matches
	//cocos2d::log("%d, %d", grid[row][col], grid[next_row][next_col]);
	cocos2d::log("(%d, %d) -> (%d, %d)", row, col, next_row, next_col);
	if (grid[row][col] == grid[next_row][next_col]) {
		if (col == 0) {
			frag_match.matches.push_back(SlotsMatch(row, col));
		}
		frag_match.matches.push_back(SlotsMatch(next_row, next_col));
		
		if (frag_match.matches.size() == MAX_SLOT_MATCHES) {
			int slot_value = grid[frag_match.matches[0].row][frag_match.matches[0].col];
			frag_match.weight = ReelScene::payTable[slot_value];

			matches.push_back(frag_match);
			cocos2d::log("Fall out B %d %d %d", row, col, next_row);
			return;
		}
		cocos2d::log("recursiveMatch B (%d, %d) %d, %d, %d", row, col, next_row, next_col, next_row);
		recursiveMatch(grid, matches, frag_match, next_row, next_col, next_row);
		cocos2d::log("Fall out C %d %d %d", row, col, next_row);
	}
}

std::array<int, 3> getStopPositions() {
	std::array<int, 3> positions;
	positions[0] = rand() % 17;
	positions[1] = rand() % 17;
	positions[2] = rand() % 17;
	return positions;
}

namespace GameUtils {
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