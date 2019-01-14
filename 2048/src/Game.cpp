#include "Game.h"

#include <iostream>

#include "Core.h"


Game::Game()
	: m_Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2048!"),
	  m_TileBackground(sf::Vector2f(TILE_WIDTH, TILE_WIDTH))
{
	// Loads the font
	if (!m_Font.loadFromFile("res/arial.ttf"))
	{
		std::cout << "ERROR: Could not load font." << std::endl;
	}

	// Sets text attributes
	m_BottomInfoText.setFont(m_Font);
	m_BottomInfoText.setCharacterSize(24);
	m_BottomInfoText.setFillColor(sf::Color::White);
	m_BottomInfoText.setStyle(sf::Text::Bold);
	m_BottomInfoText.setString("Use Arrow Keys to Slide Board");
	sf::FloatRect infoTextRect = m_BottomInfoText.getLocalBounds();
	m_BottomInfoText.setOrigin(infoTextRect.left + infoTextRect.width / 2.0f, infoTextRect.top + infoTextRect.height / 2.0f);
	m_BottomInfoText.setPosition(BOARD_WIDTH / 2, BOARD_HEIGHT + SCORE_TEXT_HEIGHT + BOTTOM_INFO_HEIGHT / 2);

	// Sets text attributes for the score
	m_ScoreText.setFont(m_Font);
	m_ScoreText.setCharacterSize(18);
	m_ScoreText.setFillColor(sf::Color::White);
	m_ScoreText.setStyle(sf::Text::Regular);
	updateScoreText();

#if !DEMONSTRATE_TILE_COLOURS

	// Seeds the random-number generator
	m_Rng.seed(std::random_device()());

	// Sets the colour for the background
	m_TileBackground.setFillColor(sf::Color(211, 184, 184, 255));

	// Adds every position to the vector
	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			m_AllBoardPositions.emplace_back(col, row);
		}
	}

	// Sets up the game
	restart();

#else

	// Vaiables for the position
	unsigned int row = 0;
	unsigned int col = 0;

	// Loops through each value and creates a tile
	for (unsigned int value = 2; value <= 8192; value *= 2)
	{
		m_Tiles.emplace_back(value, sf::Vector2i(col, row), m_Font);

		col += 1;

		if (col == 4)
		{
			col = 0;
			row += 1;
		}
	}

#endif // !DEMONSTRATE_TILE_COLOURS
}

void Game::restart()
{
	m_GameOver = false;
	m_InAnimation = false;

	// Resets the current game's score
	m_Score = 0;

	// Loads the last highscore from a file and updates the text
	readHighscore();
	updateScoreText();

	// Clears the tiles from the board
	m_Tiles.clear();

	// Regenerates available positions
	resetAvailablePositions();

	// Generates 2 tiles
	// One tile has a value of 2, the other could have 2 or 4
	generateTile();
	generateTile(true);
}


void Game::run()
{
	// Game loop
	while (m_Window.isOpen())
	{
		handleEvents();
		update();
		draw();
	}
}


void Game::handleEvents()
{
	while (m_Window.pollEvent(m_Event))
	{
		switch (m_Event.type)
		{
		case sf::Event::Closed:
			m_Window.close();
			break;

		case sf::Event::KeyReleased:
			switch (m_Event.key.code)
			{
			// Closes the window
			case sf::Keyboard::Escape:
				m_Window.close();
				break;

			// Restarts the game
			case sf::Keyboard::R:
				restart();
				break;

			// Restarts the game only on the game over screen
			case sf::Keyboard::Enter:
				if (m_GameOver)
				{
					restart();
				}

				break;

		#if !DEMONSTRATE_TILE_COLOURS

			// Shifts left
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
				if (!m_GameOver && !m_InAnimation)
				{
					shiftTiles(sf::Vector2i(-1,  0));
					m_InAnimation = true;

					for (Tile &tile : m_Tiles)
					{
						tile.startAnimation(AnimationType::Slide);
					}
				}
				
				break;

			// Shifts right
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
				if (!m_GameOver && !m_InAnimation)
				{
					shiftTiles(sf::Vector2i( 1,  0));
					m_InAnimation = true;

					for (Tile &tile : m_Tiles)
					{
						tile.startAnimation(AnimationType::Slide);
					}
				}

				break;

			// Shifts up
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
				if (!m_GameOver && !m_InAnimation)
				{
					shiftTiles(sf::Vector2i( 0, -1));
					m_InAnimation = true;

					for (Tile &tile : m_Tiles)
					{
						tile.startAnimation(AnimationType::Slide);
					}
				}

				break;

			// Shifts down
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
				if (!m_GameOver && !m_InAnimation)
				{
					shiftTiles(sf::Vector2i( 0,  1));
					m_InAnimation = true;

					for (Tile &tile : m_Tiles)
					{
						tile.startAnimation(AnimationType::Slide);
					}
				}

				break;

		#endif // !DEMONSTRATE_TILE_COLOURS
			}
		}
	}
}

void Game::update()
{
	if (m_InAnimation)
	{
		for (Tile &tile : m_Tiles)
		{
			tile.animate();
		}
	}

	// Checks if the game is over
	if (m_AvailablePositions.size() == 0 &&
		!testShiftTiles(sf::Vector2i(-1,  0)) &&
		!testShiftTiles(sf::Vector2i( 1,  0)) &&
		!testShiftTiles(sf::Vector2i( 0, -1)) &&
		!testShiftTiles(sf::Vector2i( 0,  1))
		)
	{
		gameOver();
	}
}

void Game::draw()
{
	// Clears the screen to black
	m_Window.clear(sf::Color::Black);

	// Draws the background
	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			m_TileBackground.setPosition(
				(float) (col * TILE_WIDTH + col * TILE_GAP),
				(float) (row * TILE_WIDTH + row * TILE_GAP) + SCORE_TEXT_HEIGHT
			);
			m_Window.draw(m_TileBackground);
		}
	}

	// Draws each tile
	for (Tile &tile : m_Tiles)
	{
		tile.draw(m_Window);
	}

	// Draws information and score text
	m_Window.draw(m_BottomInfoText);
	m_Window.draw(m_ScoreText);

	// Dims the screen and draws text when game over
	if (m_GameOver)
	{
		m_Window.draw(m_GameOverDim);
		m_Window.draw(m_GameOverText);
	}

	// Swaps the buffers
	m_Window.display();
}

void Game::generateTile(bool withValue4)
{
	// No open spaces left
	if (m_AvailablePositions.size() == 0)
	{
		std::cout << "WARNING: Tried to generate tile with no space." << std::endl;
		return;
	}

	// Value could be 4
	if (withValue4)
	{
		// 1 in 10 chance
		std::uniform_int_distribution<std::mt19937::result_type> dist(1, 11);
		
		if (dist(m_Rng) != 1)
		{
			m_Tiles.emplace_back(2, m_AvailablePositions.back(), m_Font);
		}

		else
		{
			m_Tiles.emplace_back(4, m_AvailablePositions.back(), m_Font);
		}

	}

	else
	{
		m_Tiles.emplace_back(2, m_AvailablePositions.back(), m_Font);
	}

	// Pops the position that was just used
	m_AvailablePositions.pop_back();
}

void Game::shiftTiles(const sf::Vector2i &direction)
{
	// Sorts the tiles depending on the direction
	sortTiles(direction);

	for (Tile &tile : m_Tiles)
	{
		sf::Vector2i newPos = tile.getPosition() + direction;
		
		// Moves the tile while it can
		while (!outOfBounds(newPos) && isAvailable(newPos))
		{
			tile.move(direction);
			newPos += direction;
		}

		// Gets a Tile* to merge with, or nullptr
		Tile *merger = canMerge(tile.getValue(), newPos);

		if (merger != nullptr)
		{
			// Removes the current tile
			m_Tiles.erase(std::remove(m_Tiles.begin(), m_Tiles.end(), tile), m_Tiles.end());

			// Doubles the other tile
			merger->doubleValue();

			// Adds to the current score and updates the text
			m_Score += merger->getValue();
			updateScoreText();
		}
	}

	// Recalculates available positions
	resetAvailablePositions();

	// Generates a new tile on the board
	generateTile();
}

bool Game::testShiftTiles(const sf::Vector2i &direction)
{
	for (Tile &tile : m_Tiles)
	{
		sf::Vector2i newPos = tile.getPosition() + direction;

		// If the position is in bounds and can be moved to,
		// then the game is not over yet
		if (!outOfBounds(newPos) && (isAvailable(newPos) || canMerge(tile.getValue(), newPos) != nullptr))
		{
			return true;
		}
	}

	return false;
}

bool Game::isAvailable(const sf::Vector2i &position)
{
	// True if nothing in the tiles vector matches that position
	return std::none_of(m_Tiles.begin(), m_Tiles.end(), [&position](Tile &tile) { return tile.getPosition() == position; });
}

Tile *Game::canMerge(unsigned int value, const sf::Vector2i &position)
{
	for (Tile &tile : m_Tiles)
	{
		// If a tile exists on the position with the same value
		if (tile.getPosition() == position && tile.getValue() == value)
		{
			return &tile;
		}
	}

	return nullptr;
}

void Game::sortTiles(const sf::Vector2i &direction)
{
	// Left (sorted by 'x' value in ascending order)
	if (direction.x == -1)
	{
		std::sort(m_Tiles.begin(), m_Tiles.end(), [](Tile &t1, Tile &t2) { return t1.getPosition().x < t2.getPosition().x; });
	}

	// Right (sorted by 'x' value in descending order)
	else if (direction.x == 1)
	{
		std::sort(m_Tiles.begin(), m_Tiles.end(), [](Tile &t1, Tile &t2) { return t1.getPosition().x > t2.getPosition().x; });
	}

	// Up (sorted by 'y' value in ascending order)
	else if (direction.y == -1)
	{
		std::sort(m_Tiles.begin(), m_Tiles.end(), [](Tile &t1, Tile &t2) { return t1.getPosition().y < t2.getPosition().y; });
	}

	// Down (sorted by 'y' value in descending order)
	else if (direction.y == 1)
	{
		std::sort(m_Tiles.begin(), m_Tiles.end(), [](Tile &t1, Tile &t2) { return t1.getPosition().y > t2.getPosition().y; });
	}
}

void Game::updateScoreText()
{
	// Sets the text
	std::string msg = "Score: ";
	msg += std::to_string(m_Score) + " | Highscore: " + std::to_string(m_HighScore);
	m_ScoreText.setString(msg);
	
	// Sets the position
	sf::FloatRect scoreTextRect = m_ScoreText.getLocalBounds();
	m_ScoreText.setOrigin(scoreTextRect.left + scoreTextRect.width / 2.0f, scoreTextRect.top + scoreTextRect.height / 2.0f);
	m_ScoreText.setPosition(BOARD_WIDTH / 2, SCORE_TEXT_HEIGHT / 2);
}

void Game::resetAvailablePositions()
{
	// Every position is available so far
	m_AvailablePositions = m_AllBoardPositions;

	// Removes positions from the vector if they are already occupied
	for (Tile &tile : m_Tiles)
	{
		m_AvailablePositions.erase(std::remove(m_AvailablePositions.begin(), m_AvailablePositions.end(), tile.getPosition()), m_AvailablePositions.end());
	}

	// Randomises the order of the positions
	std::shuffle(m_AvailablePositions.begin(), m_AvailablePositions.end(), m_Rng);
}

void Game::gameOver()
{
	m_GameOver = true;

	// Sets the highscore
	if (m_Score > m_HighScore)
	{
		writeHighscore(m_Score);
	}

	// Fills a rectangle the size of the window
	m_GameOverDim = sf::RectangleShape(sf::Vector2f((float) WINDOW_WIDTH, (float) WINDOW_HEIGHT));
	m_GameOverDim.setFillColor(sf::Color(80, 80, 80, 160));

	// Sets text attributes
	m_GameOverText.setFont(m_Font);
	m_GameOverText.setCharacterSize(50);
	m_GameOverText.setFillColor(sf::Color::White);
	m_GameOverText.setStyle(sf::Text::Bold);
	m_GameOverText.setString("GAME OVER!");
	sf::FloatRect textRect = m_GameOverText.getLocalBounds();
	m_GameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_GameOverText.setPosition(BOARD_WIDTH / 2, SCORE_TEXT_HEIGHT + BOARD_HEIGHT / 2);
}

void Game::readHighscore()
{
	m_HighscoreFile.open("res/highscore.txt", std::fstream::in);
	m_HighscoreFile >> m_HighScore;
	m_HighscoreFile.close();
}

void Game::writeHighscore(unsigned int value)
{
	m_HighscoreFile.open("res/highscore.txt", std::fstream::out | std::fstream::trunc);
	m_HighscoreFile << value << std::endl;;
	m_HighscoreFile.close();
}
