#ifndef COMPUTER_NETWORKS_COMMUNICATIONNODE_H
#define COMPUTER_NETWORKS_COMMUNICATIONNODE_H

#include "Channel.h"
#include <vector>
#include <iostream>

class CommunicationNode {
public:
    CommunicationNode() = default;

    explicit CommunicationNode(int);

    void SetID(int);
    void SetChannelsCount(int);
    void SetWorkStationChannel(Channel *);

    void AddChannel(Channel *);

    int GetID() const;
    int GetChannelsCount() const;
    int GetVectorSize() const;
    std::vector<Channel*> GetChannels() const;
    Channel* GetWorkStationChannel() const {
        return WorkStation;
    }

    void RemoveChannel(Channel *);
    void DeleteWorkStation();
    void DeleteChannels();

    const Channel *IsChannelExist(int) const;

    bool IsActive() const;

    void Activate(bool);

    bool operator==(int x) const {
        return NodeID == x;
    }

    std::string Info() const;

private:
    std::vector<Channel *> channels;
    Channel *WorkStation;
    int NodeID;
    int ChannelsCount;
    bool isActive;
};

#endif //COMPUTER_NETWORKS_COMMUNICATIONNODE_H
