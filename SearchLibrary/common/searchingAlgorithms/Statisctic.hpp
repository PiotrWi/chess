#pragma once

#include <atomic>
#include <chrono>
#include <iostream>

struct SinglePerrioadRaiiWrapper;

struct Statistic
{
    float getNodesPerSecondLastCalculation() const;
    float getNodesPerSecondTotal() const;

friend SinglePerrioadRaiiWrapper;
private:
    void publishMeasurements(unsigned long long nodes);
    void startMeassurement();
    void stopMeasurement();


    std::atomic<long long> nodesLastCalc_;
    std::atomic<long long> nodesTolal_;
    decltype(std::chrono::high_resolution_clock::now()) startPoint;
    decltype(std::chrono::high_resolution_clock::now()) endPoint;
    std::chrono::nanoseconds lastMeasurement;
    std::chrono::nanoseconds allMeasurements;
    // totalTime;
};

inline float Statistic::getNodesPerSecondLastCalculation() const
{
    return nodesLastCalc_.load(std::memory_order_acquire) / std::chrono::duration<float, std::chrono::seconds::period>(lastMeasurement).count();
}

inline float Statistic::getNodesPerSecondTotal() const
{
    return nodesLastCalc_.load(std::memory_order_acquire) / std::chrono::duration<float, std::chrono::seconds::period>(nodesTolal_).count();
}

inline void Statistic::publishMeasurements(unsigned long long nodes)
{
    nodesLastCalc_.fetch_add(nodes, std::memory_order_release);
    nodesTolal_.fetch_add(nodes, std::memory_order_release);
}

inline void Statistic::startMeassurement()
{
    startPoint = std::chrono::high_resolution_clock::now();
}

inline void Statistic::stopMeasurement()
{
    endPoint = std::chrono::high_resolution_clock::now();
    lastMeasurement = std::chrono::nanoseconds(endPoint - startPoint);
    allMeasurements += lastMeasurement;
}

struct SinglePerrioadRaiiWrapper
{
    Statistic& instace;
    unsigned long long& nodes;

    SinglePerrioadRaiiWrapper(Statistic& referencedInstance, unsigned long long& nodesReference);
    void operator++();
    ~SinglePerrioadRaiiWrapper();
};

inline SinglePerrioadRaiiWrapper::SinglePerrioadRaiiWrapper(Statistic& referencedInstance, unsigned long long& nodesReference)
    : instace(referencedInstance)
    , nodes(nodesReference)
{
    nodes = 0;
    instace.startMeassurement();
}

inline void SinglePerrioadRaiiWrapper::operator++()
{
    ++nodes;
}

inline SinglePerrioadRaiiWrapper::~SinglePerrioadRaiiWrapper()
{
    instace.stopMeasurement();
    instace.publishMeasurements(nodes);
}
