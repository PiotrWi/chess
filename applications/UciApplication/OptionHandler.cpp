#include "OptionHandler.hpp"
#include "Events.hpp"
#include "EventsPropagator.hpp"
#include <common/evaluators/PositionEvaluator.hpp>
#include <utils/DebugWrapper.hpp>

OptionHandler optionHandler;

void handleOption(SET_OPTION& option)
{
    debug.logInDebug("handle option: " + option.key + " " + option.value);
    if (option.key == "resetEvaluators")
    {
        evaluationSettings.switchOfAll();
    }
    if (option.key == "materialEvaluation")
    {
        evaluationSettings.materialEvaluation = option.value == "on";
    }
}

void OptionHandler::registerHandler()
{
    eventPropagator.registerToEvent(handleOption);
}
