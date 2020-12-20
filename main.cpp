#include <BoardEngine.hpp>

#include <cstring>
#include <iostream>

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
