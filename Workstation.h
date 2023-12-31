#ifndef COMPUTER_NETWORKS_WORKSTATION_H
#define COMPUTER_NETWORKS_WORKSTATION_H

#include "Channel.h"

class Workstation {
public:
    Workstation() = default;

    void SetNodeID(const int value) {
        NodeID = value;
    }

    int GetNodeID() const {
        return NodeID;
    }

    void SetChannel(Channel *value) {
        channel = value;
    }

    std::string GetNodeInfo() const {
        std::string res = "W" + std::to_string(NodeID) + " - ";

        res += channel->GetInfo();

        return res;
    }

private:
    int NodeID;
    Channel *channel;
};

#endif //COMPUTER_NETWORKS_WORKSTATION_H
