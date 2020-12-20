#include "HumanPlayer.hpp"
#include <iostream>

void HumanPlayer::init(Color c)
{
    playerColor_ = c;
}

const char* HumanPlayer::act(const char* move)
{
    std::cout << ((playerColor_ == Color::white) ? "Black" : "White") << " move is: " << move;
    return "";
}

void HumanPlayer::rejectLast()
{
     std::cout << "IncorrectMove";
}
