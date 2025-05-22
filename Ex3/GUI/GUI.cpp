#include <SFML/Graphics.hpp>
#include <iostream>
#include "GUI.hpp"

using namespace sf;
using namespace std;

namespace coup {

    void launchOpeningScreen() {
        RenderWindow window(VideoMode(800, 600), "Coup");
        Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("GUI/assets/background.jpg")) {
            cerr << "Failed to load background image\n";
            return;
        }

        Sprite background(backgroundTexture);
        background.setScale(
            window.getSize().x / background.getLocalBounds().width,
            window.getSize().y / background.getLocalBounds().height
        );


        Font font;
        if (!font.loadFromFile("GUI/assets/Carlito-Regular.ttf")) {
            cerr << "Failed to load Carlito-Regular.ttf\n";
            return;
        }

        RectangleShape overlay(Vector2f(window.getSize()));
        overlay.setFillColor(Color(0, 0, 0, 150)); // black with 150 alpha

        Text title("Welcome to Coup!", font, 48);
        FloatRect titleBounds = title.getLocalBounds();
        title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        title.setPosition(window.getSize().x / 2.f, 100);
        title.setFillColor(Color::White);

        RectangleShape startButton(Vector2f(200, 60));
        startButton.setPosition(window.getSize().x / 2.f - 100, 300);
        startButton.setFillColor(Color::Blue);

        Text buttonText("Start Game", font, 24);
        FloatRect buttonTextBounds = buttonText.getLocalBounds();
        buttonText.setOrigin(
            buttonTextBounds.width / 2,
            buttonTextBounds.height / 2 + buttonTextBounds.top
        );
        buttonText.setPosition(
            startButton.getPosition().x + startButton.getSize().x / 2,
            startButton.getPosition().y + startButton.getSize().y / 2
        );

        buttonText.setFillColor(Color::White);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (startButton.getGlobalBounds().contains(mousePos)) {
                        cout << "Game Starting...\n";
                        window.close(); // Next: transition to the main game screen
                    }
                }
            }
            
            // Hover effect
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            if (startButton.getGlobalBounds().contains(mousePos)) {
                startButton.setFillColor(Color(0, 100, 255)); // hover color
            } else {
                startButton.setFillColor(Color::Blue); // default
            }

            window.clear(Color::Black);
            window.draw(background);
            window.draw(overlay);
            window.draw(title);
            window.draw(startButton);
            window.draw(buttonText);
            window.display();
        }
    }

}
