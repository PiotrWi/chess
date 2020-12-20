#include <BoardEngine.hpp>

#include <cstring>
#include <iostream>
/*
class HumanPlayer : public IPlayer
{
public:
    void init(NOTATION::COLOR::color c) override
    {
        playerColor_ = c;
    }

    Move act(const Move &move) override
    {
        std::cout << (c ==NOTATION::COLOR::color::white) == "Black" : "White" << " move is: " << move;
    }

    void rejectLast() override
    {
        std::cout << "IncorrectMove";
    }
private:
    NOTATION::COLOR::color playerColor_;
};*/

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cout << "Incorrect usage. You need to specify the players." << std::endl;
        std::cout << "Currently the only suported is" << std::endl;
        std::cout << "./chess --whitePlayer humanPlayer --blackPlayer humanPlayer" << std::endl;
        return 1;
    }

    for (auto i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--whitePlayer"))
        {

            continue;
        }
    }

	return 0;
}
