#ifndef SPY_HPP
#define SPY_HPP

#include "Player.hpp"

namespace coup {

    class Spy : public Player {
        public:
            Spy(Game& game, const std::string& name);
            
            string getRole() const override;
            void peek(Player& target) const;
            void blockArrest(Player& target);
    };

}

#endif // SPY_HPP
