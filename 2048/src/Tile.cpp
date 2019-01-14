#include "Tile.h"

#include "Core.h"


const std::array<std::pair<unsigned int, sf::Color>, 13> Tile::COLOURS = {		
		std::pair<unsigned int, sf::Color>(   2, sf::Color(255, 205, 205, 255)),
		std::pair<unsigned int, sf::Color>(   4, sf::Color(255, 164, 164, 255)),
		std::pair<unsigned int, sf::Color>(   8, sf::Color(255, 123, 123, 255)),
		std::pair<unsigned int, sf::Color>(  16, sf::Color(255,  82,  82, 255)),
		std::pair<unsigned int, sf::Color>(  32, sf::Color(255,  41,  41, 255)),
		std::pair<unsigned int, sf::Color>(  64, sf::Color(255,   0,   0, 255)),
		std::pair<unsigned int, sf::Color>( 128, sf::Color(255, 195,  80, 255)),
		std::pair<unsigned int, sf::Color>( 256, sf::Color(255, 210,  60, 255)),
		std::pair<unsigned int, sf::Color>( 512, sf::Color(255, 225,  40, 255)),
		std::pair<unsigned int, sf::Color>(1024, sf::Color(255, 240,  20, 255)),
		std::pair<unsigned int, sf::Color>(2048, sf::Color(255, 255,   0, 255)),
		std::pair<unsigned int, sf::Color>(4096, sf::Color(  0,  80,   0, 255)),
		std::pair<unsigned int, sf::Color>(8192, sf::Color(  0,  20,   0, 255)),
};


Tile::Tile(unsigned int value, sf::Vector2i position, sf::Font &font)
	: m_Value(value), m_Position(position), m_Shape(sf::Vector2f((float) TILE_WIDTH, (float) TILE_WIDTH))
{
	// Sets the position
	m_Shape.setPosition(
		(float) (position.x * TILE_WIDTH + position.x * TILE_GAP),
		(float) (position.y * TILE_WIDTH + position.y * TILE_GAP + SCORE_TEXT_HEIGHT)
	);

	// Sets text attributes
	m_Text.setFont(font);
	m_Text.setCharacterSize(36);
	m_Text.setStyle(sf::Text::Bold);
	updateText();
	
	// Sets the colour
	updateColour();
}


void Tile::draw(sf::RenderWindow &window)
{
	window.draw(m_Shape);
	window.draw(m_Text);
}

void Tile::updateColour()
{
	// Sets the text colour
	if (m_Value == 2 || m_Value == 4)
	{
		m_Text.setFillColor(sf::Color::Black);
	}

	else
	{
		m_Text.setFillColor(sf::Color::White);
	}

	for (std::pair<unsigned int, sf::Color> pair : COLOURS)
	{
		// If the values match
		if (pair.first == m_Value)
		{
			// Fill with the colour associated with the value
			m_Shape.setFillColor(pair.second);
			return;
		}
	}

	// Unknown value
	m_Shape.setFillColor(sf::Color::Black);
}

void Tile::updateText()
{
	// Sets text attributes
	m_Text.setString(std::to_string(m_Value));
	sf::FloatRect textRect = m_Text.getLocalBounds();
	m_Text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_Text.setPosition(m_Shape.getPosition().x + (TILE_WIDTH / 2), m_Shape.getPosition().y + (TILE_WIDTH / 2));
}

void Tile::move(sf::Vector2i direction) {
	m_Position += direction;
	m_Shape.move(direction.x * (float) TILE_WIDTH + direction.x * (float) TILE_GAP, direction.y * (float) TILE_WIDTH + direction.y * (float) TILE_GAP);
	
	// Updates text so it it not left behind
	updateText();
}
