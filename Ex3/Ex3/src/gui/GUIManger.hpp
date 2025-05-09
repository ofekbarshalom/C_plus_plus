#pragma once
#include <SFML/Graphics.hpp>

class GUIManager {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text startText;
    sf::RectangleShape startButton;

public:
    GUIManager();
    void run();
};
