#include "Baron.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace coup {

    Baron::Baron(Game& game, const string& name) : Player(game, name) {}

    void Baron::invest() {
        requireMyTurnAndAlive();
        if (coins >= 10) {
            throw runtime_error("You must coup when holding 10 or more coins.");
        }

        removeCoins(3);     // Pay 3 coins
        addCoins(6);        // Gain 6 coins
        setLastAction("invest");
        game->advanceTurn();
    }

}
