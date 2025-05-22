#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "Player.hpp"

namespace coup {

class General : public Player {
    public:
        General(Game& game, const string& name);

        void undo(Player& target) override; // Pay 5 coins to block a coup on target
};

}

#endif // GENERAL_HPP
