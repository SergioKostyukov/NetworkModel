#ifndef COMPUTER_NETWORKS_COMMUNICATIONNODE_H
#define COMPUTER_NETWORKS_COMMUNICATIONNODE_H

#include "Channel.h"
#include <vector>

class CommunicationNode {
public:
    CommunicationNode() = default;

    CommunicationNode(int id);

    void SetID(int num);

    void SetChannelsCount(int num);

    void AddChannel(Channel *value);

    void SetWorkStationChannel(Channel *value){
        WorkStation = value;
    }

    int GetID() const;

    int GetChannelsCount();

    int GetVectorSize() const;

    bool IsChannelExist(int index) const;

    std::string GetNodeInfo() const;

private:
    int NodeID;
    int ChannelsCount;
    std::vector<Channel *> channels;
    Channel* WorkStation;
};

#endif //COMPUTER_NETWORKS_COMMUNICATIONNODE_H
