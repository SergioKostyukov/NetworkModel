#ifndef COMPUTER_NETWORKS_NETWORKMODEL_H
#define COMPUTER_NETWORKS_NETWORKMODEL_H

#include "CommunicationNode.h"
#include "Workstation.h"
#include <iostream>

class NetworkModel {
public:
    NetworkModel() = default;

    void ConfigureNetwork();

    void RunSimulation();

private:
    std::vector<CommunicationNode> nodes;
    std::vector<Workstation> stations;
    const int CNodeCount = 24;
    const int CAverageChannelsToNode = 4;
    const int CSatelliteCount = 2;

    void ShowNetworkModel();

    void AutoGeneration();

    void StationsGeneration();

    int AddNode();

    int AddChannel();

    int DeleteNode();

    int DeleteChannel();

    int OnNode();

    int OnChannel();

    int OffNode();

    int OffChannel();

    int SaveConfiguration(); // save to .csv file

    int CountNotFullNodes();

    std::string ShowStatus();
};

#endif //COMPUTER_NETWORKS_NETWORKMODEL_H
