#ifndef SCREENS_HPP
#define SCREENS_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Game.hpp"
#include "Player.hpp"

using namespace sf;
using namespace std;

namespace coup {

    bool launchOpeningScreen(); // returns true if user starts game
    void launchPlayerSetupScreen(Game& game);
    void launchNameEntryScreen(Game& game, int playerCount);
    void launchRevealRolesScreen(Game& game);
    Player* chooseTargetPopup(const Font& font, const vector<Player*>& targets);
    void launchMainGameScreen(Game& game);
    void launchWinnerScreen(const string& winnerName);
}

#endif // SCREENS_HPP
