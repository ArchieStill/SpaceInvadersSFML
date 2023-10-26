
#ifndef SPACEINVADERS_GAMEOBJECT_H
#define SPACEINVADERS_GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "Vector2.h"

class GameObject
{
 public:
  GameObject();
  ~GameObject();
  void update(float dt);

  bool initialiseSprite(sf::Texture &texture, std::string filename);
  sf::Sprite* getSprite();
  Vector2* getVector();
  bool visibility;
  int speed;
  Vector2 direction {0,0};

 private:
  sf::Sprite* sprite = nullptr;
};


#endif // SPACEINVADERS_GAMEOBJECT_H
