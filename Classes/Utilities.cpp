#include "Utilities.h"
#include "cocos2d.h"
#include "ReelScene.h"
#include <vector>
#include <array>

USING_NS_CC;
#define MAX_SLOT_MATCHES 3

std::vector<MatchSequence> linearPatternSearch(std::vector<std::vector<int>> grid) {
	//cocos2d::log("Match check");
	//cocos2d::log("%s %s %s", std::to_string(grid[0][0]).c_str(), std::to_string(grid[0][1]).c_str(), std::to_string(grid[0][2]).c_str());
	//cocos2d::log("%s %s %s", std::to_string(grid[1][0]).c_str(), std::to_string(grid[1][1]).c_str(), std::to_string(grid[1][2]).c_str());
	//cocos2d::log("%s %s %s", std::to_string(grid[2][0]).c_str(), std::to_string(grid[2][1]).c_str(), std::to_string(grid[2][2]).c_str());

	std::vector<MatchSequence> sequences;

	cocos2d::log("Running recursive match");
	MatchSequence fragment;
	recursiveMatch(grid, sequences, fragment, 0, 0, 0);
	recursiveMatch(grid, sequences, fragment, 1, 0, 0);
	recursiveMatch(grid, sequences, fragment, 2, 0, 1);
	/*
	cocos2d::log("Recursive match ended");

	size_t grid_size = grid.size();

	for (int row = 0; row < grid_size; row++) {
		MatchSequence sequence;

		size_t grid_row_size = grid[row].size();
		for (int col = 0; col < grid_row_size; col++) {
			if (col == 0 || grid[row][col] != grid[row][col - 1]) {
				if (row != 0 && grid[row][col] == grid[row - 1][col]) {
					// Match
				}

				if (row != grid_size && grid[row][col] == grid[row + 1][col]) {
					// Match
				}

				continue; // Stop matching this row, line has been broken
			}

			// A match has started or continued
			if (col == 1) {
				// Add previous cell to start new sequence
				sequence.matches.push_back(Vec2(col - 1, row)); // Row matches, log it
			}
			sequence.matches.push_back(Vec2(col, row)); // Row matches, log it
		}

		if (sequence.matches.size() == grid_size) {
			int slot_value = grid[sequence.matches[0].x][sequence.matches[0].y];
			sequence.weight = ReelScene::payTable[slot_value];

			sequences.push_back(sequence);
		}
	}
	*/
	return sequences;
}

/**
 * Given the current position, find all matches
 * @todo possible optimization of returning object
 */
void recursiveMatch(std::vector<std::vector<int>>& grid, std::vector<MatchSequence>& matches, MatchSequence frag_match, int row, int col, int next_row) {
	int next_col = col + 1;

	// Fall out if this is an invalid row / col
	cocos2d::log("%d, %d -> %d, %d", row, col, next_row, next_col);
	//cocos2d::log("Row %d", row);
	//cocos2d::log("Grid size: %d", grid.size() - 1);
	//cocos2d::log("Col: %d", col);
	//cocos2d::log("Col grid size: %d", grid[0].size() - 1);

	if (row < 0 || (grid.size() - 1) < row || next_row < 0 || (grid.size() - 1) < next_row || col < 0 || (grid[0].size() - 1) < col || (grid[0].size() - 1) < next_col) {
		cocos2d::log("Ending recursion (a) for %d, %d, %d", row, col, next_row);
		return; // Possibly send null to signify end of line
	}

	if (next_row == row || next_row == row - 1) {
		cocos2d::log("Running recursive (b) on %d, %d, %d", row, col, next_row + 1);
		recursiveMatch(grid, matches, frag_match, row, col, next_row + 1);
	}

	// Check for all matches
	cocos2d::log("%d, %d", grid[row][col], grid[next_row][next_col]);
	if (grid[row][col] == grid[next_row][next_col]) {
		if (col == 0) {
			frag_match.matches.push_back(Vec2(col, row));
		}
		frag_match.matches.push_back(Vec2(next_col, next_row));
		// Possibly more than one node returned
		
		if (frag_match.matches.size() == MAX_SLOT_MATCHES) {
			cocos2d::log("Adding");
			matches.push_back(frag_match);
			return;
		}
		cocos2d::log("Running recursive (a) on %d, %d, %d", next_row, next_col, next_row);
		recursiveMatch(grid, matches, frag_match, next_row, next_col, next_row);
	}
	
	/*
	if (row != 0) {
		cocos2d::log("Running recursive (c) on %d, %d, %d", row, col, next_row + 2);
		recursiveMatch(grid, matches, frag_match, row, col, next_row + 2);
	}
	*/
	cocos2d::log("End of recursion (b) for %d, %d, %d", row, col, next_row);
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