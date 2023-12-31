#include "CommunicationNode.h"

CommunicationNode::CommunicationNode(int id) {
    NodeID = id;
}

void CommunicationNode::SetID(int num) {
    NodeID = num;
}

void CommunicationNode::SetChannelsCount(int num) {
    ChannelsCount = num;
}

void CommunicationNode::AddChannel(Channel *value) {
    channels.push_back(value);
}

int CommunicationNode::GetID() const {
    return NodeID;
}

int CommunicationNode::GetChannelsCount() {
    return ChannelsCount;
}

int CommunicationNode::GetVectorSize() const {
    return int(channels.size());
}

bool CommunicationNode::IsChannelExist(int index) const {
    for (auto &i: channels) {
        if (i->CompareChannels(NodeID, index)) return true;
    }

    return false;
}

std::string CommunicationNode::GetNodeInfo() const {
    std::string res = "N" + std::to_string(NodeID) + "(" + std::to_string(ChannelsCount) + ") - ";

    for (auto &i: channels) {
        res += i->GetInfo();
    }

    return res;
}