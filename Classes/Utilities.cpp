#include "Utilities.h"
#include "cocos2d.h"
#include "ReelScene.h"
#include <vector>
#include <array>

USING_NS_CC;

std::vector<MatchSequence> linearPatternSearch(std::vector<std::vector<int>> grid) {
	//cocos2d::log("Match check");
	//cocos2d::log("%s %s %s", std::to_string(grid[0][0]).c_str(), std::to_string(grid[0][1]).c_str(), std::to_string(grid[0][2]).c_str());
	//cocos2d::log("%s %s %s", std::to_string(grid[1][0]).c_str(), std::to_string(grid[1][1]).c_str(), std::to_string(grid[1][2]).c_str());
	//cocos2d::log("%s %s %s", std::to_string(grid[2][0]).c_str(), std::to_string(grid[2][1]).c_str(), std::to_string(grid[2][2]).c_str());

	std::vector<MatchSequence> sequences;
	size_t grid_size = grid.size();
	for (int x = 0; x < grid_size; x++) {
		MatchSequence sequence;

		size_t grid_row_size = grid[x].size();
		for (int y = 0; y < grid_row_size; y++) {
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
			int slot_value = grid[sequence.matches[0].x][sequence.matches[0].y];

			sequence.weight = ReelScene::payTable[slot_value];
			sequences.push_back(sequence);
		}
	}

	return sequences;
}

std::array<int, 3> getStopPositions() {
	std::array<int, 3> positions;
	positions[0] = 15;
	positions[1] = 12;
	positions[2] = 11;
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