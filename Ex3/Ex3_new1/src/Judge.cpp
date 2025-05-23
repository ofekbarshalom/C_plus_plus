#include "Judge.hpp"
#include <stdexcept>

using namespace std;

namespace coup {

    Judge::Judge(Game& game, const string& name) : Player(game, name) {}

    string Judge::getRole() const {
        return "Judge";
    }

    void Judge::undo(Player& target) {
        if (!target.isAlive()) {
            throw runtime_error("Cannot undo action of a dead player.");
        }

        if (target.getLastAction() != "bribe") {
            throw runtime_error("Judge can only undo a bribe action.");
        }

        if (target.getCoins() < 4) {
            throw runtime_error("Target does not have enough coins to undo bribe.");
        }

        target.removeCoins(4);
        setLastAction("undo_bribe", &target);
    }

}
