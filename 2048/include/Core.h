#pragma once

// If 0, game runs normally
// When this is 1, one of every tile is created
#define DEMONSTRATE_TILE_COLOURS 0


// Width of the square tile
constexpr unsigned int TILE_WIDTH = 100;
// Gap between each tile
constexpr unsigned int TILE_GAP = 3;

// Dimensions of text areas
constexpr unsigned int BOTTOM_INFO_HEIGHT = 40;
constexpr unsigned int SCORE_TEXT_HEIGHT = 30;

// Board dimensions
constexpr unsigned int BOARD_WIDTH  = TILE_WIDTH * 4 + TILE_GAP * 3;
constexpr unsigned int BOARD_HEIGHT = TILE_WIDTH * 4 + TILE_GAP * 3;

// Window dimensions
constexpr unsigned int WINDOW_WIDTH  = BOARD_WIDTH;
constexpr unsigned int WINDOW_HEIGHT = BOARD_HEIGHT + BOTTOM_INFO_HEIGHT + SCORE_TEXT_HEIGHT;
