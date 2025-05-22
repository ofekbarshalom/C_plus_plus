#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "Player.hpp"

namespace coup {

class Judge : public Player {
    public:
        Judge(Game& game, const string& name);

        void undo(Player& target) override;  // Cancels target's bribe
};

}

#endif // JUDGE_HPP
