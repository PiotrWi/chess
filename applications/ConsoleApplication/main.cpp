#include <BoardEngine.hpp>

#include <cassert>
#include <cstring>
#include <iostream>
#include <HumanPlayer/HumanPlayer.hpp>
#include <ComputerPlayer/CachedPlayer.hpp>
#include <ConsoleApplication.hpp>

enum class PlayerType { Cached, Human };

struct PlayerSettings
{
    Color color;
    PlayerType playerType;
    const char* evaluatorLocation = nullptr;
    const char* evaluatorConfigurationLocation = nullptr;
};

std::unique_ptr<IPlayer> createPlayer(const PlayerSettings& playerSettings)
{
    std::unique_ptr<IPlayer> player;
    if (playerSettings.playerType == PlayerType::Human)
    {
        player = std::make_unique<HumanPlayer>();
    }
    if (playerSettings.playerType == PlayerType::Cached)
    {
        if (playerSettings.evaluatorLocation == nullptr)
        {
            player = std::make_unique<CachedPlayer>();
        }
        else
        {
            player = std::make_unique<CachedPlayer>(playerSettings.evaluatorLocation, playerSettings.evaluatorConfigurationLocation);
        }
    }
    player->init(playerSettings.color);
    return player;
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cout << "Incorrect usage. You need to specify the players." << std::endl;
        std::cout << "Syntax is: " << std::endl;
        std::cout << "./ConsolleApplication --whitePlayer playerType [--customEvaluator evaluatorLocation] [--evaluatorConfig configLocation] --blackPlayer playerType [--customEvaluator evaluatorLocation] [--evaluatorConfig configLocation]" << std::endl;
        std::cout << "where player types are:" << std::endl;
        std::cout << "  humanPlayer" << std::endl;
        std::cout << "  cachedPlayer" << std::endl;

        return 1;
    }

    // ./ConsolleApplication --whitePlayer cachedPlayer --customEvaluator libStandardEvaluator.so  --blackPlayer cachedPlayer --customEvaluator libStandardEvaluator.so

    auto app = ConsoleApplication();
    PlayerSettings settings[2];
    PlayerSettings* settingsPtr = nullptr;

    for (auto i = 1; i < argc; ++i)
    {

        if (strcmp(argv[i], "--whitePlayer") == 0)
        {
            std::cout << "Read for white" << std::endl;
            settingsPtr = &settings[0];
            settingsPtr->color = Color::white;
            continue;
        }
        if (strcmp(argv[i], "--blackPlayer") == 0)
        {
            std::cout << "Read for black" << std::endl;
            settingsPtr = &settings[1];
            settingsPtr->color = Color::black;
            continue;
        }
        if (strcmp(argv[i], "humanPlayer") == 0)
        {
            std::cout << "humanPlayer" << std::endl;
            settingsPtr->playerType = PlayerType::Human;
            continue;
        }
        if (strcmp(argv[i], "cachedPlayer") == 0)
        {
            std::cout << "cachedPlayer" << std::endl;
            settingsPtr->playerType = PlayerType::Cached;
            continue;
        }
        if (strcmp(argv[i], "--customEvaluator") == 0)
        {
            settingsPtr->evaluatorLocation = argv[++i];
            std::cout << "customEvaluator: " << settingsPtr->evaluatorLocation << std::endl;
            continue;
        }
        if (strcmp(argv[i], "--evaluatorConfig") == 0)
        {
            settingsPtr->evaluatorConfigurationLocation = argv[++i];
            std::cout << "evaluatorConfig: " << settingsPtr->evaluatorConfigurationLocation << std::endl;
            continue;
        }
    }

    app.setWhitePlayer(createPlayer(settings[0]));
    app.setBlackPlayer(createPlayer(settings[1]));
    app.start();

	return 0;
}
