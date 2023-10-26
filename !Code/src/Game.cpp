
#include "Game.h"
#include <iostream>
#include "TextSetup.h"

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

Game::~Game()
{
}

bool Game::init()
{
  gameState = GameState::MAINMENU;
  play_again_selected = true;
  left_key = sf::Keyboard::A;
  right_key = sf::Keyboard::D;
  score = 0;
  score_string = std::to_string(score);
  textInitialise();
  spriteInitialise();
  for (int i = 0; i < 3; i++)
  {
    lasersArray[i].visibility = false;
    for (int j = 0; j < 8; j++)
    {
      aliensArray[j + i * 8].speed = 100;
    }
  }
  for (int i = 0; i < 3; i++)
  {
    lasersArray[i].visibility = false;
  }

  return true;
}

void Game::update(float dt)
{
  if (gameState == GameState::CLASSIC || gameState == GameState::GRAVITY ||
      gameState == GameState::QUADRATIC || gameState == GameState::SINE)
  {
    background.update(dt);
    if (background.getSprite()->getPosition().y + background.getSprite()->getGlobalBounds().height > window.getSize().y + 450)
    {
      background.getSprite()->setPosition(background.getSprite()->getPosition().x, -450);
    }
    player.update(dt);
    player.speed = 750;
    // PLAYER - Movement
    if (sf::Keyboard::isKeyPressed(left_key))
    {
      player.getSprite()->move(-player.speed * dt, 0);
    }
    else if (sf::Keyboard::isKeyPressed(right_key))
    {
      player.getSprite()->move(player.speed * dt, 0);
    }
    // PLAYER - Staying in bounds
    if (player.getSprite()->getPosition().x < 0)
    {
      player.getSprite()->setPosition(0, player.getSprite()->getPosition().y);
    }
    if (
      player.getSprite()->getPosition().x >
      window.getSize().x - player.getSprite()->getGlobalBounds().width)
    {
      player.getSprite()->setPosition(
        window.getSize().x - player.getSprite()->getGlobalBounds().width,
        player.getSprite()->getPosition().y);
    }

    // ALIEN MOVEMENT
    for (int i = 0; i < 24; i++)
    {
      if (aliensArray[i].visibility)
      {
        if (aliensArray[i].getSprite()->getPosition().x < 0)
        {
          for (int j = 0; j < 24; j++)
          {
            aliensArray[j].getSprite()->setPosition(
              aliensArray[j].getSprite()->getPosition().x + 25, aliensArray[j].getSprite()->getPosition().y);
            aliensArray[j].getVector()->invertVectorX();
            if (gameState == GameState::CLASSIC)
            {
              aliensArray[j].getSprite()->setPosition(aliensArray[j].getSprite()->getPosition().x,
                                                      aliensArray[j].getSprite()->getPosition().y + 25);
            }
          }
          if (gameState == GameState::QUADRATIC || gameState == GameState::SINE)
          {
            quadsine_counter++;
          }
        }
        if (
          aliensArray[i].getSprite()->getPosition().x >
          window.getSize().x -
            aliensArray[i].getSprite()->getGlobalBounds().width)
        {
          for (int j = 0; j < 24; j++)
          {
            aliensArray[j].getSprite()->setPosition(
                aliensArray[j].getSprite()->getPosition().x - 25,
              aliensArray[j].getSprite()->getPosition().y);
            aliensArray[j].getVector()->invertVectorX();
            if (gameState == GameState::CLASSIC)
            {
              aliensArray[j].getSprite()->setPosition(aliensArray[j].getSprite()->getPosition().x,
                                                      aliensArray[j].getSprite()->getPosition().y + 25);
            }
          }
          if (gameState == GameState::QUADRATIC || gameState == GameState::SINE)
          {
            quadsine_counter++;
          }
        }
      }
    }
    for (int i = 0; i < 24; i++)
    {
     if (gameState == GameState::GRAVITY)
     {
       aliensArray[i].direction.y *= 1.002;
     }
     else if (gameState == GameState::QUADRATIC)
     {
       int y_offset;
       if (i < 8)
       {
         y_offset = 50 + (20 *quadsine_counter);
       }
       else if (i < 16)
       {
         y_offset = 125 + (20 *quadsine_counter);
       }
       else
       {
         y_offset = 200 + (20 *quadsine_counter);
       }
       aliensArray[i].getSprite()->setPosition(aliensArray[i].getSprite()->getPosition().x,
                                               0.0002f * (aliensArray[i].getSprite()->getPosition().x - window.getSize().x / 2) * (aliensArray[i].getSprite()->getPosition().x - window.getSize().x / 2) + y_offset);
     }
     else if (gameState == GameState::SINE)
     {
       aliensArray[i].speed = 180;
       int y_offset;
       if (i < 8)
       {
         y_offset = 25 + (50 *quadsine_counter);
       }
       else if (i < 16)
       {
         y_offset = 100 + (50 *quadsine_counter);
       }
       else
       {
         y_offset = 175 + (50 *quadsine_counter);
       }
       aliensArray[i].getSprite()->setPosition(aliensArray[i].getSprite()->getPosition().x,
                                               25.0f * sin(5.0f + aliensArray[i].getSprite()->getPosition().x * 2) + y_offset);
     }

      aliensArray[i].update(dt);
      if (aliensArray[i].visibility)
      {
        if (player.getSprite()->getPosition().x + player.getSprite()->getGlobalBounds().width >=
            aliensArray[i].getSprite()->getPosition().x)
        {
          if (aliensArray[i].getSprite()->getPosition().x + aliensArray[i].getSprite()->getGlobalBounds().width >=
              player.getSprite()->getPosition().x)
          {
            if (player.getSprite()->getPosition().y + player.getSprite()->getGlobalBounds().height >=
                aliensArray[i].getSprite()->getPosition().y)
            {
              if (aliensArray[i].getSprite()->getPosition().y + aliensArray[i].getSprite()->getGlobalBounds().height >=
                  player.getSprite()->getPosition().y)
              {
                gameState = GameState::DEFEAT;
              }
            }
          }
        }
        if (aliensArray[i].getSprite()->getPosition().y + aliensArray[i].getSprite()->getGlobalBounds().height > window.getSize().y)
        {
          gameState = GameState::DEFEAT;
        }
      }
    }

    // LASERS MOVEMENT
    for (int i = 0; i < 3; i++)
    {
      if (!lasersArray[i].visibility)
      {
        lasersArray[i].getSprite()->setPosition(-10,-10);
      }
      if (lasersArray[i].visibility)
      {
       lasersArray[i].update(dt);
       lasersArray[i].speed = 300;
      }

      if (lasersArray[i].getSprite()->getPosition().y < 0) // laser hitting top of screen
      {
        lasersArray[i].visibility = false;
        lasersArray[i].getSprite()->setPosition(
          player.getSprite()->getPosition().x +
             player.getSprite()->getGlobalBounds().width / 2,
          550);
      }
      for (int j = 0; j < 24; j++)
      {
        if (lasersArray[i].visibility) // laser isn't invisible to destroy an alien
        {
          if (aliensArray[j].visibility) // laser hitting an alien
          {
            if (lasersArray[i].getSprite()->getPosition().x + lasersArray[i].getSprite()->getGlobalBounds().width >=
                aliensArray[j].getSprite()->getPosition().x)
            {
              if (aliensArray[j].getSprite()->getPosition().x + aliensArray[j].getSprite()->getGlobalBounds().width >=
                  lasersArray[i].getSprite()->getPosition().x)
              {
                if (lasersArray[i].getSprite()->getPosition().y + lasersArray[i].getSprite()->getGlobalBounds().height >=
                    aliensArray[j].getSprite()->getPosition().y)
                {
                  if (aliensArray[j].getSprite()->getPosition().y + aliensArray[j].getSprite()->getGlobalBounds().height >=
                      lasersArray[i].getSprite()->getPosition().y)
                  {
                    lasersArray[i].visibility = false;
                    lasersArray[i].getSprite()->setPosition(
                      player.getSprite()->getPosition().x +
                        player.getSprite()->getGlobalBounds().width / 2,
                      550);
                    aliensArray[j].visibility = false;
                    aliensDefeated++;
                    for (int k = 0; k < 24; k++)
                    {
                      aliensArray[k].speed = aliensArray[k].speed + 10;
                    }
                    score = score + 100;
                    score_string = std::to_string(score);
                    score_display.setString("SCORE: " + score_string);
                    final_score.setString("Final score: " + score_string);
                  }
                }
              }
            }
          }
        }
      }
    }
    if (aliensDefeated == 24)
    {
      quadsine_counter = 0;
      switch (gameState)
      {
        case GameState::CLASSIC:
          gameState = GameState::VICTORYCLASSIC;
          break;
        case GameState::GRAVITY:
          gameState = GameState::VICTORYGRAVITY;
          for (int i = 0; i < 24; i++)
          {
            aliensArray[i].getVector()->aliensGravityVector();
          }
          break;
        case GameState::QUADRATIC:
          gameState = GameState::VICTORYQUAD;
          break;
        case GameState::SINE:
          gameState = GameState::VICTORYSINE;
          break;
        default:
          gameState = GameState::VICTORYCLASSIC;
          break;
      }
    }
  }
}

void Game::render()
{
  if (gameState == GameState::MAINMENU)
  {
    window.draw(title_text);
    window.draw(play_option);
  }
  else if (gameState == GameState::MODESELECT)
  {
    window.draw(pick_a_mode);
    window.draw(normal_option);
    window.draw(gravity_option);
    window.draw(quad_option);
    window.draw(curve_option);
  }
  else if (gameState == GameState::CLASSIC || gameState == GameState::GRAVITY ||
           gameState == GameState::QUADRATIC || gameState == GameState::SINE)
  {
    window.draw(*background.getSprite());
    window.draw(*player.getSprite());
    renderArrays();
    window.draw(score_display);
    if (gameState == GameState::CLASSIC)
    {
      window.draw(normal_text);
    }
    else if (gameState == GameState::GRAVITY)
    {
      window.draw(gravity_text);
    }
    else if (gameState == GameState::QUADRATIC)
    {
      window.draw(quad_text);
    }
    else if (gameState == GameState::SINE)
    {
      window.draw(curve_text);
    }
  }
  else if (gameState == GameState::DEFEAT)
  {
    window.draw(defeat_text);
    window.draw(final_score);
    window.draw(return_to_menu);
  }
  else
  {
    window.draw(victory_text);
    window.draw(play_again);
    window.draw(pa_yes);
    window.draw(pa_no);
  }
}
void Game::renderArrays()
{
  for(int i = 0; i < 24; i++)
  {
    if (aliensArray[i].getSprite() != nullptr)
    {
      if (aliensArray[i].visibility)
      {
        window.draw(*aliensArray[i].getSprite());
      }
    }
  }
  for (int i = 0; i < 3; i++)
  {
    if (lasersArray[i].getSprite() != nullptr)
    {
      if (lasersArray[i].visibility)
      {
        window.draw(*lasersArray[i].getSprite());
      }
    }
  }
}

void Game::keyPressed(sf::Event event)
{
  if (event.key.code == sf::Keyboard::Enter)
  {
    if (gameState == GameState::MAINMENU)
    {
      gameState = GameState::MODESELECT;
    }
    else if (gameState == GameState::MODESELECT)
    {
      if (menu_integer == 0)
      {
        gameState = GameState::CLASSIC;
      }
      else if (menu_integer == 1)
      {
        gameState = GameState::GRAVITY;
        for (int i = 0; i < 24; i++)
        {
          aliensArray[i].getVector()->aliensGravityVector();
        }
      }
      else if (menu_integer == 2)
      {
        gameState = GameState::QUADRATIC;
      }
      else if (menu_integer == 3)
      {
        gameState = GameState::SINE;
      }
    }
    else if (gameState == GameState::DEFEAT)
    {
      aliensDefeated = 0;
      score = 0;
      score_string = std::to_string(score);
      score_display.setString("SCORE: " + score_string);
      gameState = GameState::MAINMENU;
      for (int i = 0; i < 3; i++)
      {
        lasersArray[i].visibility = false;
        for (int j = 0; j < 8; j++)
        {
          aliensArray[j + i * 8].visibility = true;
          aliensArray[j + i * 8].direction.y = 1;
          aliensArray[j + i * 8].getSprite()->setPosition(195 + j * 90, 50 + 75 * i);
          aliensArray[j + i * 8].speed = 100;
          aliensArray[j + i * 8].getVector()->aliensVector();
        }
      }
    }
    else if (gameState == GameState::VICTORYCLASSIC || gameState == GameState::VICTORYGRAVITY ||
             gameState == GameState::VICTORYQUAD || gameState == GameState::VICTORYSINE)
    {
      if (play_again_selected)
      {
        aliensDefeated = 0;
        score_string = std::to_string(score);
        score_display.setString("SCORE: " + score_string);
        switch (gameState)
        {
          case GameState::VICTORYCLASSIC:
            gameState = GameState::CLASSIC;
            break;
          case GameState::VICTORYGRAVITY:
            gameState = GameState::GRAVITY;
            break;
          case GameState::VICTORYQUAD:
            gameState = GameState::QUADRATIC;
            break;
          case GameState::VICTORYSINE:
            gameState = GameState::SINE;
            break;
          default:
            gameState = GameState::MAINMENU;
            break;
        }
        for (int i = 0; i < 3; i++)
        {
          lasersArray[i].visibility = false;
          for (int j = 0; j < 8; j++)
          {
            aliensArray[j + i * 8].visibility = true;
            aliensArray[j + i * 8].getSprite()->setPosition(195 + j * 90, 50 + 75 * i);
            if (gameState == GameState::VICTORYCLASSIC || gameState == GameState::VICTORYQUAD ||
                gameState == GameState::VICTORYSINE)
            {
              aliensArray[j + i * 8].getVector()->aliensVector();
            }
            else if (gameState == GameState::VICTORYGRAVITY)
            {
              aliensArray[j + i * 8].getVector()->aliensGravityVector();
            }
          }
        }
      }
      else
      {
        aliensDefeated = 0;
        score = 0;
        score_string = std::to_string(score);
        score_display.setString("SCORE: " + score_string);
        gameState = GameState::MAINMENU;
        for (int i = 0; i < 3; i++)
        {
          lasersArray[i].visibility = false;
          for (int j = 0; j < 8; j++)
          {
            aliensArray[j + i * 8].visibility = true;
            aliensArray[j + i * 8].getSprite()->setPosition(195 + j * 90, 50 + 75 * i);
            aliensArray[j + i * 8].speed = 100;
            aliensArray[j + i * 8].getVector()->aliensVector();
          }
        }
      }
    }
  }
  else if (event.key.code == sf::Keyboard::Space)
  {
    if (gameState == GameState::CLASSIC || gameState == GameState::GRAVITY ||
        gameState == GameState::QUADRATIC || gameState == GameState::SINE)
    {
      for (int i = 0; i < 3; i++)
      {
        if (!lasersArray[i].visibility)
        {
          lasersArray[i].visibility = true;
          lasersArray[i].getSprite()->setPosition(
            player.getSprite()->getPosition().x +
              player.getSprite()->getGlobalBounds().width / 2,
            550);
          break;
        }
      }
    }
  }
  else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
  {
    if (event.key.code == sf::Keyboard::A)
    {
      if (gameState == GameState::MODESELECT)
      {
        menu_integer--;
        modesInitialise();
        if (menu_integer == -1)
        {
          menu_integer = 3;
        }
      }
    }
    else if (event.key.code == sf::Keyboard::D)
    {
      if (gameState == GameState::MODESELECT)
      {
        menu_integer++;
        modesInitialise();
        if (menu_integer == 4)
        {
          menu_integer = 0;
        }
      }
    }
    if (gameState == GameState::VICTORYCLASSIC || gameState == GameState::VICTORYGRAVITY ||
        gameState == GameState::VICTORYQUAD || gameState == GameState::VICTORYSINE)
    {
      play_again_selected = !play_again_selected;
      if (play_again_selected)
      {
        SFTextSetup(pa_yes, font, "> YES <", window.getSize().x / 3,
                    window.getSize().y / 1.25, 50, MainColor);
        SFTextSetup(pa_no, font, "NO", window.getSize().x / 1.5,
                    window.getSize().y / 1.25, 50, sf::Color::White);
      }
      else
      {
        SFTextSetup(pa_yes, font, "YES", window.getSize().x / 3,
                    window.getSize().y / 1.25, 50, sf::Color::White);
        SFTextSetup(pa_no, font, "> NO <", window.getSize().x / 1.5,
                    window.getSize().y / 1.25, 50, MainColor);
      }
    }
  }
  else if (event.key.code == sf::Keyboard::Escape)
  {
    window.close();
  }
}

bool Game::textInitialise()
{
  if (!font.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout << "font did not load \n";
  }
  SFTextSetup(title_text, font, ">SPACE INVADERS<", window.getSize().x / 2,
              window.getSize().y / 3, 110, MainColor);
  SFTextSetup(play_option, font, "Press Enter to start the game!", window.getSize().x / 2,
              window.getSize().y / 1.5, 50, sf::Color::White);
  SFTextSetup(score_display, font, "SCORE: " + score_string, 110,
              50, 50, sf::Color::Red);

  SFTextSetup(defeat_text, font, "You lose...", window.getSize().x / 2,
              window.getSize().y / 4, 125, sf::Color::Red);
  SFTextSetup(return_to_menu, font, "Press Enter to return to the main menu", window.getSize().x / 2,
              window.getSize().y / 1.25, 50, sf::Color::White);
  SFTextSetup(final_score, font, "Final score: " + score_string, window.getSize().x / 2.5,
              window.getSize().y / 2, 50, MainColor);

  SFTextSetup(victory_text, font, "YOU WIN!!!", window.getSize().x / 2,
              window.getSize().y / 4, 125, sf::Color::Green);
  SFTextSetup(play_again, font, "Would you like to play again?", window.getSize().x / 2,
              window.getSize().y / 1.75, 50, sf::Color::White);
  SFTextSetup(pa_yes, font, "> YES <", window.getSize().x / 3,
              window.getSize().y / 1.25, 50, MainColor);
  SFTextSetup(pa_no, font, "NO", window.getSize().x / 1.5,
              window.getSize().y / 1.25, 50, sf::Color::White);

  SFTextSetup(pick_a_mode, font, "Choose a mode", window.getSize().x / 2,
              window.getSize().y / 10, 75, sf::Color::White);
  SFTextSetup(normal_option, font, "> Classic <", window.getSize().x / 3,
              window.getSize().y / 2.5, 50, MainColor);
  SFTextSetup(gravity_option, font, "Gravity", window.getSize().x / 1.5,
              window.getSize().y / 2.5, 50, sf::Color::White);
  SFTextSetup(quad_option, font, "Quadratic", window.getSize().x / 3,
              window.getSize().y / 1.4, 50, sf::Color::White);
  SFTextSetup(curve_option, font, "Sine", window.getSize().x / 1.5,
              window.getSize().y / 1.4, 50, sf::Color::White);

  SFTextSetup(normal_text, font, "CLASSIC", window.getSize().x - 110,
              50, 50, sf::Color::Black);
  SFTextSetup(gravity_text, font, "GRAVITY", window.getSize().x - 115,
              50, 50, sf::Color::Black);
  SFTextSetup(quad_text, font, "QUADRATIC", window.getSize().x - 155,
              50, 50, sf::Color::Black);
  SFTextSetup(curve_text, font, "SINE", window.getSize().x - 85,
              50, 50, sf::Color::Black);

  return false;
}
bool Game::modesInitialise()
{
  if (menu_integer == 0 || menu_integer == 4)
  {
    SFTextSetup(normal_option, font, "> Classic <", window.getSize().x / 3,
                window.getSize().y / 2.5, 50, MainColor);
    SFTextSetup(gravity_option, font, "Gravity", window.getSize().x / 1.5,
                window.getSize().y / 2.5, 50, sf::Color::White);
    SFTextSetup(quad_option, font, "Quadratic", window.getSize().x / 3,
                window.getSize().y / 1.4, 50, sf::Color::White);
    SFTextSetup(curve_option, font, "Sine", window.getSize().x / 1.5,
                window.getSize().y / 1.4, 50, sf::Color::White);
  }
  if (menu_integer == 1)
  {
    SFTextSetup(normal_option, font, "Classic", window.getSize().x / 3,
                window.getSize().y / 2.5, 50, sf::Color::White);
    SFTextSetup(gravity_option, font, "> Gravity <", window.getSize().x / 1.5,
                window.getSize().y / 2.5, 50, MainColor);
    SFTextSetup(quad_option, font, "Quadratic", window.getSize().x / 3,
                window.getSize().y / 1.4, 50, sf::Color::White);
    SFTextSetup(curve_option, font, "Sine", window.getSize().x / 1.5,
                window.getSize().y / 1.4, 50, sf::Color::White);
  }
  if (menu_integer == 2)
  {
    SFTextSetup(normal_option, font, "Classic", window.getSize().x / 3,
                window.getSize().y / 2.5, 50, sf::Color::White);
    SFTextSetup(gravity_option, font, "Gravity", window.getSize().x / 1.5,
                window.getSize().y / 2.5, 50, sf::Color::White);
    SFTextSetup(quad_option, font, "> Quadratic <", window.getSize().x / 3,
                window.getSize().y / 1.4, 50, MainColor);
    SFTextSetup(curve_option, font, "Sine", window.getSize().x / 1.5,
                window.getSize().y / 1.4, 50, sf::Color::White);
  }
  if (menu_integer == 3 || menu_integer == -1)
  {
    SFTextSetup(normal_option, font, "Classic", window.getSize().x / 3,
                window.getSize().y / 2.5, 50, sf::Color::White);
    SFTextSetup(gravity_option, font, "Gravity", window.getSize().x / 1.5,
                window.getSize().y / 2.5, 50, sf::Color::White);
    SFTextSetup(quad_option, font, "Quadratic", window.getSize().x / 3,
                window.getSize().y / 1.4, 50, sf::Color::White);
    SFTextSetup(curve_option, font, "> Sine <", window.getSize().x / 1.5,
                window.getSize().y / 1.4, 50, MainColor);
  }
  return false;
}
bool Game::spriteInitialise()
{
  player.initialiseSprite(player_texture, "Data/Images/SpaceShooterRedux/PNG/playerShip2_red.png");
  player.getSprite()->setPosition(505, 625);
  player.getSprite()->setScale(0.7,0.7);

  background.initialiseSprite(background_texture, "Data/Images/SpaceShooterRedux/Backgrounds/blue.png");
  background.getSprite()->setPosition(0,-450);
  background.getSprite()->setScale(4.225,5);
  background.getVector()->backgroundVector();

  aliensArray = new GameObject[24];
  if (aliensDefeated == 0)
  {
    for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        switch (i)
        {
          case 0:
            aliensArray[j + i * 8].initialiseSprite(aliens1_texture, "Data/Images/SpaceShooterRedux/PNG/Enemies/enemyRed4.png");
            break;
          case 1:
            aliensArray[j + i * 8].initialiseSprite(aliens2_texture, "Data/Images/SpaceShooterRedux/PNG/Enemies/enemyGreen1.png");
            break;
          case 2:
            aliensArray[j + i * 8].initialiseSprite(aliens3_texture, "Data/Images/SpaceShooterRedux/PNG/Enemies/enemyBlue3.png");
            break;
          default:
            aliensArray[j + i * 8].initialiseSprite(aliens1_texture, "Data/Images/SpaceShooterRedux/PNG/Enemies/enemyBlue3.png");
            break;
        }
        aliensArray[j + i * 8].getSprite()->setPosition(195 + j * 90, 50 + 75 * i);
        aliensArray[j + i * 8].getSprite()->setScale(0.7f, 0.7f);
        switch (gameState)
        {
          case GameState::CLASSIC:
            aliensArray[j + i * 8].getVector()->aliensVector();
            break;
          case GameState::GRAVITY:
            aliensArray[j + i * 8].getVector()->aliensGravityVector();
            break;
          case GameState::QUADRATIC:
            aliensArray[j + i * 8].getVector()->aliensVector();
            break;
          case GameState::SINE:
            aliensArray[j + i * 8].getVector()->aliensVector();
            break;
          default:
            aliensArray[j + i * 8].getVector()->aliensVector();
            break;
        }
      }
    }
  }
  for (int i = 0; i < 3; i++)
  {
    lasersArray[i].initialiseSprite(laser_texture, "Data/Images/SpaceShooterRedux/PNG/Lasers/laserRed13.png");
    lasersArray[i].getVector()->lasersVector();
  }
  return false;
}