#include "IsQuietHandler.hpp"
#include "Events.hpp"
#include "EventsPropagator.hpp"

#include <notations/fen.h>
#include <common/searchingAlgorithms/FullSearchingImplementation.hpp>
#include <utils/DebugWrapper.hpp>

#include <iostream>

IsQuietHandler isQuietHandler;

void handleIsQuiet(IS_QUIET& event)
{
    debug.logInDebug("handle isQuiet: " + event.fenString);
    isQuietHandler.handle(event.fenString);
}

void IsQuietHandler::registerHandler()
{
    eventPropagator.registerToEvent(handleIsQuiet);
}

void IsQuietHandler::handle(const std::string& fenString)
{
    be = notations::fen::initByFen(fenString);
    auto staticEvaluation = cachedEngine.getEvaluationValue(be, be.generateValidMoveCount());
    auto quiescenceEvaluationValue = full_search::quiescenceEvaluation(be, cachedEngine);
    eventPropagator.enqueue(QUIET_RESP(staticEvaluation == quiescenceEvaluationValue));
}