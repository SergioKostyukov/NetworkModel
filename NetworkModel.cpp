#include "NetworkModel.h"
#include <fstream>

int GenerateRandomNumber(int, int);

void PrintToFile(const std::string &);

void ClearLogFile();

void ClearResultFile();

NetworkModel::NetworkModel(int nodeCount, int averageChannelsToNode, ChannelType type) : CNodeCount(nodeCount),
                                                                                         CAverageChannelsToNode(
                                                                                                 averageChannelsToNode),
                                                                                         type(type) {}

// --------------------------- Configure Network ---------------------------

// меню для створення мережі
void NetworkModel::ConfigureNetwork() {
    ClearLogFile();
    ClearResultFile();

    int choice;
    int x, y;

    std::cout << "Select the channel type (1 - FullDuplex, 2 - Half-Duplex)";
    std::cin >> choice;

    if (choice == 1) type = ChannelType::FULL_DUPLEX;
    else type = ChannelType::HALF_DUPLEX;

    do {
        std::cout
                << "Select the action:\n"
                   "1. Network auto generation\n"
                   "2. Add new node\n"
                   "3. Add new channel\n"
                   "4. Delete node\n"
                   "5. Delete channel\n"
                   "6. ON node\n"
                   "7. ON channel\n"
                   "8. OFF node\n"
                   "9. OFF channel\n"
                   "10. Save configuration\n"
                   "11. Exit\n"
                   "Enter your choice:";
        std::cin >> choice;

        switch (choice) {
            case 1:
                AutoGeneration();
                break;
            case 2:
                AddNode();
                break;
            case 3:
                std::cin >> x >> y;
                AddChannel(x, y);
                break;
            case 4:
                std::cin >> choice;
                DeleteNode(choice);
                break;
            case 5:
                std::cin >> x >> y;
                DeleteChannel(x, y);
                break;
            case 6:
                std::cin >> choice;
                OnNode(choice);
                break;
            case 7:
                std::cin >> x >> y;
                OnChannel(x, y);
                break;
            case 8:
                std::cin >> choice;
                OffNode(choice);
                break;
            case 9:
                std::cin >> x >> y;
                OffChannel(x, y);
                break;
            case 10:
                SaveConfiguration();
                break;
            case 11:
                break;
            default:
                std::cout << "Error request!" << std::endl;
                break;
        }
    } while (choice != 11);
}

// автоматична генерація всієї мережі
void NetworkModel::AutoGeneration() {
    // початкова ініціалізація всіх вузлів (ID, кількість каналів зв'язку)
    for (int i = int(nodes.size()); i < CNodeCount; i++) {
        auto tempNode = new CommunicationNode(i);

        if (i < CNodeCount / 2) tempNode->SetChannelsCount(GenerateRandomNumber(3, 5));
        else tempNode->SetChannelsCount(CAverageChannelsToNode * 2 - nodes[i - CNodeCount / 2]->GetChannelsCount());

        nodes.emplace_back(tempNode);
        emptyIndex = CNodeCount;
    }

    // генерація робочих станцій та прив'язка їх до певних вузлів
    StationsGeneration();

    // генерація SIMPLEX вузлів
    SatelliteGeneration();

    // детальніше визначення вузлів та створення каналів між ними
    ChannelsGeneration();

    PrintToFile("Auto generation success\n");
    ShowNetworkModel();
}

// рахує кількість вузлів, які ще не мають всіх каналів
int NetworkModel::CountNotFullNodes() {
    int count = 0;
    for (auto &i: nodes) {
        if (i->GetChannelsCount() != i->GetVectorSize()) {
            count++;
        }
    }
    return count;
}

// автоматична генерація вузлів
void NetworkModel::ChannelsGeneration() {
    int randomNode, condition;
    std::vector<CommunicationNode *> tempNodes = nodes;

    CommunicationNode *temp;
    do {
        temp = tempNodes[0];
        condition = temp->GetChannelsCount() - temp->GetVectorSize(); // кількість ще не заповнених каналів для вузла

        for (int j = 0; j < condition; j++) {
            bool isFinalVersion; // перевірка фінальних випадків, коли канали провести вже не можливо
            do {
                randomNode = GenerateRandomNumber(1, int(tempNodes.size()));

                // вивід даних для перевірки стану зв'язків мережі
                PrintToFile(std::to_string(int(tempNodes.size())) + " " + std::to_string(tempNodes[1]->GetID()) + '\n');
                PrintToFile(std::to_string(temp->GetID()) + ". Find something else " + std::to_string(randomNode) +
                            ". Count NFN " + std::to_string(CountNotFullNodes()) + "\n" + ShowNodeStatus() + "\n");

                // можна винести за do while
                isFinalVersion = ((int(tempNodes.size()) <= 1) ||
                                  int(tempNodes.size()) == 2 && temp->IsChannelExist(tempNodes[randomNode]->GetID()));
            } while ((temp->IsChannelExist(tempNodes[randomNode]->GetID()) != nullptr ||
                      tempNodes[randomNode]->GetID() == temp->GetID()) && !isFinalVersion);

            if (!isFinalVersion) {
                auto tempChannel = new Channel(temp->GetID(), tempNodes[randomNode]->GetID(), type);
                temp->AddChannel(tempChannel);
                tempNodes[randomNode]->AddChannel(tempChannel);
                channels.push_back(tempChannel);

                if (tempNodes[randomNode]->GetVectorSize() == tempNodes[randomNode]->GetChannelsCount()) {
                    tempNodes.erase(std::remove(tempNodes.begin(), tempNodes.end(), tempNodes[randomNode]),
                                    tempNodes.end());
                    PrintToFile("Element delete\n");
                }
            }
        }

        tempNodes.erase(std::remove(tempNodes.begin(), tempNodes.end(), tempNodes[0]), tempNodes.end());
        PrintToFile("Element 0 delete\n");
    } while (!tempNodes.empty());

    PrintToFile("Channels generation success\n");
}

// автоматична генерація робочих станцій
void NetworkModel::StationsGeneration() {
    int count = 0;
    std::vector<int> points{2, 4, 8, 12, 17, 22};

    for (auto &i: points) {
        auto tempWorkstation = new Workstation();
        tempWorkstation->SetID(count++);
        tempWorkstation->SetNodeID(i);

        // створення каналу до прикріпленого вузла
        auto tempChannel = new Channel(i, tempWorkstation->GetID(), type);
        nodes[i]->SetWorkStationChannel(tempChannel);
        tempWorkstation->SetChannel(tempChannel);
        channels.push_back(tempChannel);


        stations.emplace_back(tempWorkstation);
    }

    PrintToFile("Station generation success\n");
}

// генерація супутникових каналів
void NetworkModel::SatelliteGeneration() {
    int randomNode1, randomNode2;

    randomNode1 = GenerateRandomNumber(0, 23);
    do {
        randomNode2 = GenerateRandomNumber(0, 23);
    } while (nodes[randomNode1]->IsChannelExist(randomNode2) != nullptr);

    auto tempChannel = new Channel(randomNode1, randomNode2, ChannelType::SIMPLEX);
    nodes[randomNode1]->AddChannel(tempChannel);
    nodes[randomNode2]->AddChannel(tempChannel);
    channels.push_back(tempChannel);

    PrintToFile("Satellite generation success\n");
}

void NetworkModel::AddNode() {
    auto tempNode = new CommunicationNode(emptyIndex++);

    tempNode->SetChannelsCount(CAverageChannelsToNode);

    nodes.emplace_back(tempNode);

    PrintToFile("Added new node with index " + std::to_string(emptyIndex - 1) + "\n");
    ShowNetworkModel("../LogFiles/LogFile.txt");
}

void NetworkModel::AddChannel(int x, int y) {
    // перевірка на існування такого каналу
    auto tempChannel = FindChannel(x, y);
    if (tempChannel != nullptr) {
        PrintToFile("Channel " + tempChannel->Info() + "also exist.\n");
        return;
    }

    // перевірка на існування таких вузлів
    if (FindNode(x) == nullptr || FindNode(y) == nullptr) {
        PrintToFile("Nodes with this indexes does not exist.\n");
        return;
    }

    // додавання каналу та запис про нього в вузли
    tempChannel = new Channel(x, y, type);
    nodes[x]->AddChannel(tempChannel);
    nodes[y]->AddChannel(tempChannel);
    channels.push_back(tempChannel);

    PrintToFile("Added new channel " + tempChannel->Info() + "\n");
    ShowNetworkModel("../LogFiles/LogFile.txt");
}

void NetworkModel::DeleteNode(int x) {
    // перевірка на існування вузла
    if (FindNode(x) == nullptr) {
        PrintToFile("Node with this index does not exist.\n");
        return;
    }

    // видалення пов'язаних каналів
    for (auto &i: nodes[x]->GetChannels()) {
        // видалення зв'язків каналу та вузла
        nodes[i->GetNode1()]->RemoveChannel(i);
        nodes[i->GetNode2()]->RemoveChannel(i);

        // видалення каналу зі списку channels
        channels.erase(std::remove(channels.begin(), channels.end(), i), channels.end());
    }
    nodes[x]->DeleteChannels();
    PrintToFile("Channels deleted\n");

    // видалення каналу до робочої станції
    if (nodes[x]->GetWorkStationChannel()) {
        int StationID = nodes[x]->GetWorkStationChannel()->GetNode2();

        stations[StationID]->SetChannel(nullptr);
        nodes[x]->DeleteWorkStation();

        auto channel = FindChannel(x, StationID);
        channels.erase(std::remove(channels.begin(), channels.end(), channel), channels.end());

        PrintToFile("WorkStation channel deleted\n");
    }

    // видалення вузла зі списку nodes
    // nodes.erase(it);

    PrintToFile("Deleted node with index " + std::to_string(x) + "\n");
    ShowNetworkModel("../LogFiles/LogFile.txt");
}

void NetworkModel::DeleteChannel(int x, int y) {
    Channel *temp = FindChannel(x, y);
    if (temp == nullptr) {
        PrintToFile("No such channel " + std::to_string(x) + "->" + std::to_string(y) + "\n");
    }

    nodes[temp->GetNode1()]->RemoveChannel(temp);
    nodes[temp->GetNode2()]->RemoveChannel(temp);

    channels.erase(std::remove(channels.begin(), channels.end(), temp), channels.end());

    PrintToFile("Deleted channel " + std::to_string(x) + "->" + std::to_string(y) +
                "\nNew list:\n");
    ShowNetworkModel("../LogFiles/LogFile.txt");
}

void NetworkModel::OnNode(int x) {
    CommunicationNode *temp = FindNode(x);

    if (temp != nullptr) {
        temp->Activate(true);
        PrintToFile("Node №" + std::to_string(x) + " activate\n");
    } else {
        std::cerr << "Error find node" << std::endl;
        PrintToFile("Error find node\n");
    }
}

void NetworkModel::OffNode(int x) {
    CommunicationNode *temp = FindNode(x);

    if (temp != nullptr) {
        temp->Activate(false);
        PrintToFile("Node №" + std::to_string(x) + " deactivate");
    } else {
        std::cerr << "Error find node" << std::endl;
        PrintToFile("Error find node\n");
    }
}

void NetworkModel::OnChannel(int x, int y) {
    Channel *temp = FindChannel(x, y);

    if (temp != nullptr) {
        temp->Activate(true);
        PrintToFile("Channel (" + std::to_string(x) + ", " + std::to_string(y) + ") activate\n");
    } else {
        std::cerr << "Error find channel" << std::endl;
        PrintToFile("Error find channel\n");
    }
}

void NetworkModel::OffChannel(int x, int y) {
    Channel *temp = FindChannel(x, y);

    if (temp != nullptr) {
        temp->Activate(false);
        PrintToFile("Channel (" + std::to_string(x) + ", " + std::to_string(y) + ") deactivate\n");
    } else {
        std::cerr << "Error find channel" << std::endl;
        PrintToFile("Error find channel\n");
    }
}

// знаходження вузла в списку nodes
CommunicationNode *NetworkModel::FindNode(int x) {
    auto it = std::find_if(nodes.begin(), nodes.end(),
                           [x](const CommunicationNode *node) { return node->GetID() == x; });

    if (it == nodes.end()) return nullptr;
    return *it;
}

// знаходження каналу в списку channels
Channel *NetworkModel::FindChannel(int x, int y) {
    auto it = std::find_if(channels.begin(), channels.end(),
                           [x, y](const Channel *node) { return node->CompareChannels(x, y) != nullptr; });

    if (it == channels.end()) return nullptr;
    return *it;
}

// ----------------------------- Out messages ---------------------------------

// повертає інформацію про вузли, які ще не мають всіх каналів
std::string NetworkModel::ShowNodeStatus() {
    std::string res;
    for (auto &i: nodes) {
        if (i->GetChannelsCount() != i->GetVectorSize()) {
            res += i->Info() + "\n";
        }
    }
    return res;
}

// записує у NetworkModel.txt повну інформацію про мережу
void NetworkModel::ShowNetworkModel(const std::string &fileName) {
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open()) std::cerr << "Error open file" << std::endl;

    for (auto &i: nodes) {
        file << i->Info() << std::endl;
    }

    for (auto &i: stations) {
        file << i->Info() << std::endl;
    }

    file << "\nNot Full Node Connections\n" << ShowNodeStatus() << std::endl;

    file.close();
}

// зберігає у file .csv інформацію для графічного зображення
void NetworkModel::SaveConfiguration() {
    std::ofstream file("../View/matrix.csv");

    if (!file.is_open()) {
        std::cerr << "Error open file" << std::endl;
        return;
    }

    // створює таблицю [CNodeCount]x[CNodeCount]
    for (auto &i: nodes) {
        for (int j = 0; j < CNodeCount; j++) {
            const Channel *temp = i->IsChannelExist(j);
            if (temp) file << temp->GetWeight() << ";";
            else file << "0;";
        }
        file << std::endl;
    }

    file.close();
}

// очищення LogFile.txt
void ClearLogFile() {
    std::ofstream file("../LogFiles/LogFile.txt");
    if (!file.is_open()) {
        std::cerr << "Error open file" << std::endl;
        return;
    }

    file.close();
}

void ClearResultFile() {
    std::ofstream file("../LogFiles/NetworkModel.txt");
    if (!file.is_open()) {
        std::cerr << "Error open file" << std::endl;
        return;
    }

    file.close();
}

// запис повідомлення у LogFile.txt
void PrintToFile(const std::string &text) {
    std::ofstream file("../LogFiles/LogFile.txt", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error open file" << std::endl;
        return;
    }

    file << text;

    file.close();
}

// записує у LogFile.txt наявні канали
void NetworkModel::PrintToFileChannels() {
    for (auto &i: channels) {
        PrintToFile(i->Info() + "\n");
    }
}

// ------------------------------- Messages -----------------------------------

void NetworkModel::RunSimulation() {

}

// --------------------------- Additional functions ---------------------------

// генерація числа між x та y
int GenerateRandomNumber(int x, int y) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(x, y);

    return dis(gen);
}
