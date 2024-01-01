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

// рахує кількість вузлів, які ще не мають всіх каналів
int NetworkModel::CountNotFullNodes() {
    int count = 0;
    for (auto &i: nodes) {
        if (i.GetChannelsCount() != i.GetVectorSize()) {
            count++;
        }
    }
    return count;
}

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
                ShowNetworkModel();
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
    CommunicationNode tempNode{};

    // початкова ініціалізація всіх вузлів (ID, кількість каналів зв'язку)
    for (int i = int(nodes.size()); i < CNodeCount; i++) {
        tempNode.SetID(i);

        if (i < CNodeCount / 2) tempNode.SetChannelsCount(GenerateRandomNumber(3, 5));
        else tempNode.SetChannelsCount(CAverageChannelsToNode * 2 - nodes[i - CNodeCount / 2].GetChannelsCount());

        nodes.emplace_back(tempNode);
        emptyIndex = CNodeCount;
    }

    // генерація робочих станцій та прив'язка їх до певних вузлів
    StationsGeneration();
    // emptyIndex = CNodeCount + int(stations.size());

    // детальніше визначення вузлів та створення каналів між ними
    ChannelsGeneration();

    PrintToFile("Auto generation success\n");
}

// автоматична генерація вузлів
void NetworkModel::ChannelsGeneration() {
    int randomNode, condition;
    for (auto &i: nodes) {
        condition = i.GetChannelsCount() - i.GetVectorSize(); // кількість ще не заповнених каналів для вузла
        for (int j = 0; j < condition; j++) {
            bool isFinalVersion = false; // перевірка фінальних випадків, коли канали провести вже не можливо

            do {
                randomNode = GenerateRandomNumber(i.GetID() + 1, 23);

                // вивід даних для перевірки стану зв'язків мережі
                PrintToFile(std::to_string(i.GetID()) + ". Find something else " + std::to_string(randomNode) +
                            ". Count NFN " + std::to_string(CountNotFullNodes()) + "\n" + ShowNodeStatus() + "\n");

                // !!! бажано замінити функцію 'i.IsChannelExist(randomNode)' на конкретне порівняння двох/трьох/чотирьох вузлів, що залишились і їх зв'язки !!!
                isFinalVersion =
                        (CountNotFullNodes() <= 1) || (CountNotFullNodes() == 2 && i.IsChannelExist(randomNode) !=
                                                                                   nullptr);
                PrintToFile(std::to_string(isFinalVersion) + '\n');
            } while ((nodes[randomNode].GetVectorSize() == nodes[randomNode].GetChannelsCount() ||
                      i.IsChannelExist(randomNode)) &&
                     !isFinalVersion);
            /* Уникнення випадків: додавання до вже заповненого вузла
                                   повторення каналів між вузлами,
                                   канали провести вже не можливо
            */

            if (!isFinalVersion) {
                auto tempChannel = new Channel(i.GetID(), randomNode, type);
                i.AddChannel(tempChannel);
                nodes[randomNode].AddChannel(tempChannel);
                channels.push_back(tempChannel);
            }
        }
    }

    PrintToFile("Channels generation success\n");
}

// автоматична генерація робочих станцій
void NetworkModel::StationsGeneration() {
    Workstation tempWorkstation{};
    int count = 0;
    std::vector<int> points{2, 4, 8, 12, 17, 22};

    for (auto &i: points) {
        tempWorkstation.SetID(count++);
        tempWorkstation.SetNodeID(i);

        // створення каналу до прикріпленого вузла
        auto tempChannel = new Channel(i, tempWorkstation.GetID(), type);
        nodes[i].SetWorkStationChannel(tempChannel);
        tempWorkstation.SetChannel(tempChannel);
        channels.push_back(tempChannel);

        stations.emplace_back(tempWorkstation);
    }

    PrintToFile("Station generation success\n");
}

// генерація супутникових каналів
void NetworkModel::SatelliteGenration() {


    PrintToFile("Satellite generation success\n");
}

void NetworkModel::AddNode() {
    CommunicationNode tempNode{};

    tempNode.SetID(emptyIndex++);
    tempNode.SetChannelsCount(CAverageChannelsToNode);

    nodes.emplace_back(tempNode);

    PrintToFile("Added new node with index " + std::to_string(emptyIndex - 1) + "\n");
    ShowNetworkModel("../LogFiles/LogFile.txt");
}

void NetworkModel::AddChannel(int x, int y) {
    // перевірка на існування такого каналу
    for (auto &i: channels) {
        if (i->CompareChannels(x, y) != nullptr) {
            PrintToFile("Channel " + i->Info() + "also exist.\n");
            return;
        }
    }

    // перевірка на існування таких вузлів
    if (std::find_if(nodes.begin(), nodes.end(), [x](const CommunicationNode &node) { return node.GetID() == x; }) ==
        nodes.end() ||
        std::find_if(nodes.begin(), nodes.end(), [y](const CommunicationNode &node) {
            return node.GetID() == y;
        }) == nodes.end()) {
        PrintToFile("Nodes with this indexes does not exist.\n");
        return;
    }

    // додавання каналу та запис про нього в вузли
    auto tempChannel = new Channel(x, y, type);
    nodes[x].AddChannel(tempChannel);
    nodes[y].AddChannel(tempChannel);
    channels.push_back(tempChannel);

    PrintToFile("Added new channel " + tempChannel->Info() + "\n");
    ShowNetworkModel("../LogFiles/LogFile.txt");
}

void NetworkModel::DeleteNode(int x) {
    // перевірка на існування вузла
    auto it = std::find(nodes.begin(), nodes.end(), x);
    if (it == nodes.end()) {
        PrintToFile("Node with this index does not exist.\n");
        return;
    }

    // видалення пов'язаних каналів
    for (auto &i: nodes[x].GetChannels()) {
        // видалення зв'язків каналу та вузла
        nodes[i->GetNode1()].RemoveChannel(i);
        nodes[i->GetNode2()].RemoveChannel(i);

        // видалення каналу зі списку channels
        channels.erase(std::remove(channels.begin(), channels.end(), i), channels.end());
    }
    nodes[x].DeleteChannels();
    PrintToFile("Channels deleted\n");

    // видалення каналу до робочої станції
    if (nodes[x].GetWorkStationChannel()) {
        int StationID = nodes[x].GetWorkStationChannel()->GetNode2();

        stations[StationID].SetChannel(nullptr);
        nodes[x].DeleteWorkStation();

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

    nodes[temp->GetNode1()].RemoveChannel(temp);
    nodes[temp->GetNode2()].RemoveChannel(temp);

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
    for (auto &i: nodes) {
        if (i.GetID() == x) return &i;
    }
    return nullptr;
}

// знаходження каналу в списку channels
Channel *NetworkModel::FindChannel(int x, int y) {
    for (auto &i: channels) {
        if (i->CompareChannels(x, y)) return i;
    }
    return nullptr;
}

// ----------------------------- Out messages ---------------------------------

// повертає інформацію про вузли, які ще не мають всіх каналів
std::string NetworkModel::ShowNodeStatus() {
    std::string res;
    for (auto &i: nodes) {
        if (i.GetChannelsCount() != i.GetVectorSize()) {
            res += i.Info() + "\n";
        }
    }
    return res;
}

// записує у NetworkModel.txt повну інформацію про мережу
void NetworkModel::ShowNetworkModel(const std::string &fileName) {
    std::ofstream file(fileName, std::ios::app);
    if (!file.is_open()) std::cerr << "Error open file" << std::endl;

    for (auto &i: nodes) {
        file << i.Info() << std::endl;
    }

    for (auto &i: stations) {
        file << i.Info() << std::endl;
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
            const Channel *temp = i.IsChannelExist(j);
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
