#ifndef COMPUTER_NETWORKS_WORKSTATION_H
#define COMPUTER_NETWORKS_WORKSTATION_H

#include "Channel.h"

class Workstation {
public:
    Workstation() = default;

    void SetID(const int value) {
        ID = value;
    }
    void SetNodeID(const int value) {
        NodeID = value;
    }
    void SetChannel(Channel *value) {
        channel = value;
    }

    int GetNodeID() const {
        return NodeID;
    }
    int GetID() const {
        return ID;
    }
    Channel *GetChannel() const {
        return channel;
    }

    std::string Info() const {
        std::string res = "W" + std::to_string(ID) + " - ";

        if(channel){
            res += channel->Info();
        }

        return res;
    }

private:
    int ID, NodeID;
    Channel *channel;
};

#endif //COMPUTER_NETWORKS_WORKSTATION_H
