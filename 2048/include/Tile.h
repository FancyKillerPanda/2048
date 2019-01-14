#pragma once

#include <array>
#include <utility>
#include <string_view>

#include <SFML/Graphics.hpp>


class Tile
{
private:
	// Colours assigned to each value
	static const std::array<std::pair<unsigned int, sf::Color>, 13> COLOURS;

	// Current value of the tile
	unsigned int m_Value;
	// Current position of the tile
	sf::Vector2i m_Position;

	// SFML drawing
	sf::RectangleShape m_Shape;
	sf::Text m_Text;

private:
	// Recalculates the colour for the tile
	void updateColour();
	// Recalculates the text for the tile
	void updateText();

	// Starts an animation
	void startAnimation(AnimationType type);

public:
	Tile(unsigned int value, sf::Vector2i position, sf::Font &font);

	// Draws the tile to the window
	void draw(sf::RenderWindow &window);

	// Moves the tile one space in the direction
	void move(sf::Vector2i direction);

	// Gets the current value
	inline unsigned int getValue() const { return m_Value; }
	// Doubles the current value (for after a merge)
	inline void doubleValue() { m_Value *= 2; updateColour(); updateText(); }
	// Gets the position
	inline const sf::Vector2i &getPosition() const { return m_Position; }

	// Animates the tile (called each frame)
	void animate();

	// Comparison
	inline bool operator==(const Tile &other) { return m_Position == other.getPosition(); }
};
