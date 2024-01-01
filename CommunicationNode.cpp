#include "CommunicationNode.h"

CommunicationNode::CommunicationNode(int id) {
    NodeID = id;
    isActive = true;
    WorkStation = nullptr;
    ChannelsCount = 0;
}

void CommunicationNode::SetID(int num) {
    NodeID = num;
}

void CommunicationNode::SetChannelsCount(int num) {
    ChannelsCount = num;
}

void CommunicationNode::SetWorkStationChannel(Channel *value) {
    WorkStation = value;
}

void CommunicationNode::AddChannel(Channel *value) {
    channels.push_back(value);
}

int CommunicationNode::GetID() const {
    return NodeID;
}

int CommunicationNode::GetChannelsCount() const {
    return ChannelsCount;
}

int CommunicationNode::GetVectorSize() const {
    return int(channels.size());
}

std::vector<Channel*> CommunicationNode::GetChannels() const{
    return channels;
}

void CommunicationNode::RemoveChannel(Channel *channel) {
    channels.erase(std::remove(channels.begin(), channels.end(), channel), channels.end());
}

void CommunicationNode::DeleteWorkStation() {
    WorkStation = nullptr;
}

void CommunicationNode::DeleteChannels(){
    channels.clear();
}

const Channel* CommunicationNode::IsChannelExist(int index) const {
    for (auto &i: channels) {
        const Channel* temp = i->CompareChannels(NodeID, index);
        if (temp != nullptr) return temp;
    }
    return nullptr;
}

bool CommunicationNode::IsActive() const {
    return isActive;
}

void CommunicationNode::Activate(bool value = true){
    isActive = value;
}

std::string CommunicationNode::Info() const {
    std::string res = "N" + std::to_string(NodeID) + "(" + std::to_string(ChannelsCount) + ") - ";

    for (auto &i: channels) {
        res += i->Info();
    }

    return res;
}