#ifndef COMPUTER_NETWORKS_CHANNEL_H
#define COMPUTER_NETWORKS_CHANNEL_H

#include <random>
#include <vector>

enum ChannelType {
    FULL_DUPLEX,
    HALF_DUPLEX,
    SATELLITE
};

class Channel {
public:
    Channel() = default;

    Channel(int n1, int n2, ChannelType channelType) {
        node1 = n1;
        node2 = n2;
        type = channelType;
        errorProbability = 0;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 9);

        weight = possibleWeights[dis(gen)];
    }

    bool CompareChannels(int x, int y) const {
        return (node1 == x && node2 == y || node1 == y && node2 == x);
    }

    std::string GetInfo() const {
        return std::to_string(node1) + "->" + std::to_string(node2) + "(" + std::to_string(weight) + ") ";
    }

private:
    int weight;
    ChannelType type;
    int errorProbability;

    int node1;
    int node2;

    const std::vector<int> possibleWeights = {1, 2, 4, 5, 6, 7, 8, 10, 15, 21};
};

#endif //COMPUTER_NETWORKS_CHANNEL_H
