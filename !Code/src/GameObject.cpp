
#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
  visibility = true;
}

GameObject::~GameObject()
{
  delete sprite;
  sprite = nullptr;
}

bool GameObject::initialiseSprite(sf::Texture& texture, std::string filename)
{
  sprite = new sf::Sprite;
  speed = 100;
  if (!texture.loadFromFile(filename))
  {
    std::cout << "texture did not load \n";
    return false;
  }
  sprite->setTexture(texture);
  return true;
}

sf::Sprite* GameObject::getSprite()
{
  return sprite;
}
Vector2* GameObject::getVector()
{
  return &direction;
}

void GameObject::update(float dt)
{
  if (sprite != nullptr)
  {
    sprite->move(direction.x * speed * dt, direction.y * speed * dt);
    if (visibility)
    {
      sprite->getTexture();
    }
  }
}