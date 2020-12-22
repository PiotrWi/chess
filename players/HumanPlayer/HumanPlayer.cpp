#include "HumanPlayer.hpp"
#include <iostream>

namespace
{
    const char* prefix(Color c)
    {
        return (c == Color::white) ? "[WHITE]: " : "[BLACK]";
    }
}

void HumanPlayer::init(Color c)
{
    playerColor_ = c;
}

const char* HumanPlayer::act(const char* move)
{
    std::cout << prefix(playerColor_)
        << ((playerColor_ == Color::white) ? "Black" : "White")
        << " move is: " << move << std::endl;
    std::cout << prefix(playerColor_) << "Your move: ";
    std::cin >> lastMove_;
    return lastMove_;
}

void HumanPlayer::rejectLast()
{
     std::cout << prefix(playerColor_)  << "IncorrectMove" << std::endl;
}
