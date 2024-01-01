#ifndef COMPUTER_NETWORKS_NETWORKMODEL_H
#define COMPUTER_NETWORKS_NETWORKMODEL_H

#include "CommunicationNode.h"
#include "Workstation.h"
#include <iostream>

/*
 *
 * */

class NetworkModel {
public:
    explicit NetworkModel(int = 24, int = 4, ChannelType = ChannelType::FULL_DUPLEX);

    // меню для створення мережі
    void ConfigureNetwork();

    //
    void RunSimulation();

private:
    std::vector<Channel *> channels;
    std::vector<CommunicationNode> nodes;
    std::vector<Workstation> stations;
    const int CNodeCount = 24;
    const int CAverageChannelsToNode = 4;
    const int CSatelliteCount = 2;
    int emptyIndex = 0;
    ChannelType type = ChannelType::FULL_DUPLEX;

    // автоматична генерація всієї мережі
    void AutoGeneration();

    // автоматична генерація вузлів
    void ChannelsGeneration();

    // автоматична генерація робочих станцій
    void StationsGeneration();

    // генерація супутникових каналів
    void SatelliteGenration();

    // додавання/видалення каналів/вузлів
    void AddNode();

    void AddChannel(int, int);

    void DeleteNode(int);

    void DeleteChannel(int, int);

    // On/Off channel/node
    void OnNode(int);

    void OnChannel(int, int);

    void OffNode(int);

    void OffChannel(int, int);

    // знаходження вузла/каналу в списку
    CommunicationNode *FindNode(int);

    Channel *FindChannel(int, int);

    // зберігає у file .csv інформацію для графічного зображення
    void SaveConfiguration();

    // рахує кількість вузлів, які ще не мають всіх каналів
    int CountNotFullNodes();

    // повертає інформацію про вузли, які ще не мають всіх каналів
    std::string ShowNodeStatus();

    // записує у file наявні канали
    void PrintToFileChannels();

    // записує у file повну інформацію про мережу
    void ShowNetworkModel(const std::string &fileName = "../LogFiles/NetworkModel.txt");
};

#endif //COMPUTER_NETWORKS_NETWORKMODEL_H
