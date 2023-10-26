//
// Created by a2-still on 06/03/2023.
//

#ifndef SPACEINVADERSSFML_TEXTSETUP_H
#define SPACEINVADERSSFML_TEXTSETUP_H

#include <SFML/Graphics.hpp>

void SFTextSetup(sf::Text& text, sf::Font& font, std::string string, float x, float y, int char_size, sf::Color color);

void SFTextSetup(sf::Text& text, sf::Font& font, std::string string, float x, float y, int char_size, sf::Color color)
{
  text.setString(string);
  text.setFont(font);
  text.setCharacterSize(char_size);
  text.setFillColor(color);
  text.setPosition(x - text.getGlobalBounds().width / 2, y -  text.getGlobalBounds().height);
}


#endif // SPACEINVADERSSFML_TEXTSETUP_H
