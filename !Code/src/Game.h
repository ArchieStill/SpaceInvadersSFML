
#ifndef SPACEINVADERS_GAME_H
#define SPACEINVADERS_GAME_H

#include <SFML/Graphics.hpp>
#include "Vector2.h"
#include "GameObject.h"
#include <math.h>

enum class GameState
{
  MAINMENU,
  MODESELECT,
  CLASSIC,
  GRAVITY,
  QUADRATIC,
  SINE,
  DEFEAT,
  VICTORYCLASSIC,
  VICTORYGRAVITY,
  VICTORYQUAD,
  VICTORYSINE
};

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void renderArrays();
  void keyPressed(sf::Event event);
  GameState gameState;
  bool textInitialise();
  bool modesInitialise();
  bool spriteInitialise();

 private:
  sf::Keyboard::Key left_key;
  sf::Keyboard::Key right_key;

  sf::Color MainColor = sf::Color(87, 67, 234, 255);
  int menu_integer = 0;

  sf::Font font;
  sf::Text title_text;
  sf::Text play_option;

  sf::Text pick_a_mode;
  sf::Text normal_option;
  sf::Text normal_text;
  sf::Text gravity_option;
  sf::Text gravity_text;
  sf::Text quad_option;
  sf::Text quad_text;
  sf::Text curve_option;
  sf::Text curve_text;

  sf::Text defeat_text;
  sf::Text return_to_menu;
  sf::Text final_score;
  sf::Text victory_text;
  sf::Text play_again;
  sf::Text pa_yes;
  sf::Text pa_no;
  bool play_again_selected;

  int score;
  sf::Text score_display;
  std::string score_string;
  int aliensDefeated = 0;
  int quadsine_counter = 0;

  GameObject player;
  sf::Texture player_texture;
  GameObject background;
  sf::Texture background_texture;
  GameObject *aliensArray;
  sf::Texture aliens1_texture;
  sf::Texture aliens2_texture;
  sf::Texture aliens3_texture;
  GameObject *lasersArray;
  sf::Texture laser_texture;

  sf::RenderWindow& window;
};

#endif // SPACEINVADERS_GAME_H
