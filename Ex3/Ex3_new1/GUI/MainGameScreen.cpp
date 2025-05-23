#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

#include "Screens.hpp"
#include "GUI.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"

using namespace sf;
using namespace std;

namespace coup {

    void launchMainGameScreen(Game& game) {
        RenderWindow window(VideoMode(800, 600), "Coup - Main Game");

        Font font;
        if (!font.loadFromFile("GUI/assets/Carlito-Regular.ttf")) {
            cerr << "Failed to load font\n";
            return;
        }

        Sprite background;
        try {
            background = loadAndScaleBackground("GUI/assets/background.jpg", window);
        } catch (const exception& e) {
            cerr << e.what() << '\n';
            return;
        }

        RectangleShape overlay(Vector2f(window.getSize()));
        overlay.setFillColor(Color(0, 0, 0, 150));

        while (window.isOpen()) {
            Event event;

            Player* current = nullptr;
            for (Player* p : game.getPlayers()) {
                if (p->getName() == game.turn()) {
                    current = p;
                    break;
                }
            }
            if (!current || !current->isAlive()) continue;

            vector<string> actions = { "Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup" };
            if (dynamic_cast<Governor*>(current)) actions.push_back("Undo Tax");
            if (dynamic_cast<Spy*>(current)) actions.push_back("Peek");
            if (dynamic_cast<Baron*>(current)) actions.push_back("Invest");

            vector<RectangleShape> buttons(actions.size());
            vector<Text> buttonTexts(actions.size());

            for (size_t i = 0; i < actions.size(); ++i) {
                Vector2f size(100, 50);
                float x = 30 + (i % 4) * 150;  // 4 buttons per row
                float y = 450 + (i / 4) * 80;
                Vector2f position(x, y);
                createButton(actions[i], font, buttons[i], buttonTexts[i], size, position);
            }

            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    for (size_t i = 0; i < buttons.size(); ++i) {
                        if (buttons[i].getGlobalBounds().contains(mousePos)) {
                            string action = actions[i];
                            try {
                                if (action == "Gather") current->gather();
                                else if (action == "Tax") current->tax();
                                else if (action == "Bribe") current->bribe();
                                else if (action == "Invest") static_cast<Baron*>(current)->invest();
                                else if (action == "Arrest" || action == "Sanction" || action == "Coup" || action == "Undo Tax" || action == "Peek") {
                                    vector<Player*> targets;
                                    for (Player* target : game.getPlayers()) {
                                        if (target != current && target->isAlive()) {
                                            targets.push_back(target);
                                        }
                                    }
                                    Player* chosen = chooseTargetPopup(font, targets);
                                    if (chosen) {
                                        if (action == "Arrest") current->arrest(*chosen);
                                        else if (action == "Sanction") current->sanction(*chosen);
                                        else if (action == "Coup") current->coup(*chosen);
                                        else if (action == "Undo Tax")  static_cast<Governor*>(current)->undo(*chosen);
                                        else if (action == "Peek")  static_cast<Spy*>(current)->peek(*chosen);
                                    }
                                }
                            } catch (const exception& e) {
                                cerr << "Action error: " << e.what() << '\n';
                            }
                            break;
                        }
                    }
                }
            }

            window.clear();
            window.draw(background);
            window.draw(overlay);

            string turnText = game.turn() + "'s Turn";
            Text title = createTitle(turnText, font, window, 30);
            window.draw(title);

            const auto& all = game.getPlayers();
            for (size_t i = 0; i < all.size(); ++i) {
                Player* p = all[i];
                stringstream ss;
                ss << p->getName() << " (" << p->getRole() << ") - "
                   << (p->isAlive() ? "Alive" : "Eliminated")
                   << " - Coins: " << p->getCoins();
                Text txt(ss.str(), font, 20);
                txt.setFillColor(p->isAlive() ? Color::White : Color(120, 120, 120));
                txt.setPosition(60.f, 160.f + i * 30);
                window.draw(txt);
            }

            for (size_t i = 0; i < buttons.size(); ++i) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                if (buttons[i].getGlobalBounds().contains(mousePos)) {
                    buttons[i].setFillColor(Color(80, 80, 80));
                } else {
                    buttons[i].setFillColor(Color(50, 50, 50));
                }
                window.draw(buttons[i]);
                window.draw(buttonTexts[i]);
            }

            try {
                string winner = game.winner();
                window.close();
                launchWinnerScreen(winner);
                return;
            } catch (...) {}

            window.display();
        }
    }

    Player* chooseTargetPopup(const Font& font, const std::vector<Player*>& targets) {
        RenderWindow popup(VideoMode(400, 300), "Choose Target");

        while (popup.isOpen()) {
            Event event;
            while (popup.pollEvent(event)) {
                if (event.type == Event::Closed)
                    popup.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f click(event.mouseButton.x, event.mouseButton.y);
                    for (size_t t = 0; t < targets.size(); ++t) {
                        FloatRect area(100, 60 + t * 50, 200, 40);
                        if (area.contains(click)) {
                            popup.close();
                            return targets[t];
                        }
                    }
                }
            }

            popup.clear(Color(30, 30, 30));
            Text title("Choose Target", font, 24);
            title.setPosition(120, 20);
            popup.draw(title);

            for (size_t t = 0; t < targets.size(); ++t) {
                RectangleShape box(Vector2f(200, 40));
                box.setPosition(100, 60 + t * 50);
                box.setFillColor(Color(60, 60, 60));

                Text txt(targets[t]->getName(), font, 20);
                txt.setPosition(110, 65 + t * 50);
                popup.draw(box);
                popup.draw(txt);
            }

            popup.display();
        }

        return nullptr;
    }

}
