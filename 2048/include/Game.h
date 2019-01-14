#pragma once

#include <random>
#include <algorithm>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "Tile.h"


class Game
{
private:
	// Window and event from SFML
	sf::RenderWindow m_Window;
	sf::Event m_Event;

	bool m_GameOver = false;

	// Random-number engine
	std::mt19937 m_Rng;

	// m_Font will be "arial"
	sf::Font m_Font;

	// Background behind tiles
	sf::RectangleShape m_TileBackground;

	// List of every active tile
	std::vector<Tile> m_Tiles;
	
	// Every tile position possible and what is left
	std::vector<sf::Vector2i> m_AllBoardPositions;
	std::vector<sf::Vector2i> m_AvailablePositions;

	// For game over, dims the screen and displays text
	sf::RectangleShape m_GameOverDim;
	sf::Text m_GameOverText;

	// Score
	unsigned int m_Score;
	unsigned int m_HighScore = 0;
	sf::Text m_ScoreText;
	// File containing the current highscore
	std::fstream m_HighscoreFile;

	// Information text
	sf::Text m_BottomInfoText;

	// Whether the game is currently animating
	bool m_InAnimation = false;

private:
	// Starts a new game
	void restart();

	// Main game-loop methods
	void handleEvents();
	void update();
	void draw();

	// Recalculates the available board positions
	void resetAvailablePositions();

	// Sets up the game-over screen
	void gameOver();

	// Generates a tile
	// The tile could have a starting value of 4
	void generateTile(bool withVal4 = false);

	// Shifts the tiles in a certain direction
	void shiftTiles(const sf::Vector2i &direction);
	// Tests if the tiles can be shifted (will not move them)
	bool testShiftTiles(const sf::Vector2i &direction);

	// Tests if a tile is available
	bool isAvailable(const sf::Vector2i &position);

	// Returns a pointer to the tile that can (potentially) be merged to
	Tile *canMerge(unsigned int value, const sf::Vector2i &position);

	// Sorts tiles depending on the direction
	void sortTiles(const sf::Vector2i &direction);
	
	// Checks if a position is in bounds
	inline bool outOfBounds(sf::Vector2i pos) { return pos.x < 0 || pos.x > 3 || pos.y < 0 || pos.y > 3; }

	// Updates the score text
	void updateScoreText();

	// Reads the highscore from file
	void readHighscore();
	// Writes the highscore to file
	void writeHighscore(unsigned int value);

public:
	Game();

	// Runs the game-loop
	void run();
};
