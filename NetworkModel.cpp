#include "NetworkModel.h"
#include <fstream>

int GenerateRandomNumber(int, int);

void PrintToFile(const std::string &);

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

// повертає інформацію про вузли, які ще не мають всіх каналів
std::string NetworkModel::ShowStatus() {
    std::string res;
    for (auto &i: nodes) {
        if (i.GetChannelsCount() != i.GetVectorSize()) {
            res += i.GetNodeInfo() + "\n";
        }
    }
    return res;
}

// меню для створення мережі
void NetworkModel::ConfigureNetwork() {
    int choice;
    do {
        std::cout
                << "1. Network auto generation\n"
                   "2. Add new node\n"
                   "3. Add new channel\n"
                   "4. Delete node\n"
                   "5. Delete channel\n"
                   "6. Off node\n"
                   "7. Off channel\n"
                   "8. Save configuration\n"
                   "9. Exit\n";
        std::cout << "Enter your choice:";
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
                AddChannel();
                break;
            case 4:
                DeleteNode();
                break;
            case 5:
                DeleteChannel();
                break;
            case 6:
                OffNode();
                break;
            case 7:
                OffChannel();
                break;
            case 8:
                SaveConfiguration();
                break;
            case 9:
                std::cout << "Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Error request!" << std::endl;
                break;
        }
    } while (choice != 9);
}

// автоматична генерація всієї мережі
void NetworkModel::AutoGeneration() {
    CommunicationNode tempNode{};

    // початкова ініціалізація всіх вузлів (ID, кількість каналів зв'язку)
    for (int i = 0; i < CNodeCount; i++) {
        tempNode.SetID(i);

        if (i < CNodeCount / 2) tempNode.SetChannelsCount(GenerateRandomNumber(3, 5));
        else tempNode.SetChannelsCount(CAverageChannelsToNode * 2 - nodes[i - CNodeCount / 2].GetChannelsCount());

        nodes.emplace_back(tempNode);
    }

    // детальніше визначення каналів
    int randomNode;
    for (auto &i: nodes) {
        int condition = i.GetChannelsCount() - i.GetVectorSize(); // кількість ще не заповнених каналів
        for (int j = 0; j < condition; j++) {
            randomNode = GenerateRandomNumber(0, 24);

            bool isFinalVersion; // перевірка фінальних випадків, коли канали провести вже не можливо
            /* Уникнення випадків: дуги з самим собою
                                   додавання до вже заповненого вузла
                                   повторення каналів між вузлами
            */
            while (randomNode == i.GetID() ||
                   nodes[randomNode].GetVectorSize() == nodes[randomNode].GetChannelsCount() ||
                   i.IsChannelExist(randomNode)) {
                randomNode = GenerateRandomNumber(0, 24);

                // вивід даних для перевірки стану зв'язків мережі
                PrintToFile("Find something else " + std::to_string(i.GetID()) + ". Count NFN " +
                            std::to_string(CountNotFullNodes()) + "\n" + ShowStatus() + "\n");

                // !!! бажано замінити функцію 'i.IsChannelExist(randomNode)' на конкретне порівняння двох вузлів, що залишились і їх зв'язки !!!
                isFinalVersion =
                        (CountNotFullNodes() <= 1) || (CountNotFullNodes() == 2 && i.IsChannelExist(randomNode));
                if (isFinalVersion) {
                    StationsGeneration();
                    return;
                }
            }

            auto tempChannel = new Channel(i.GetID(), randomNode, ChannelType::FULL_DUPLEX);
            i.AddChannel(tempChannel);
            nodes[randomNode].AddChannel(tempChannel);
        }
    }
}

// автоматична генерація робочих станцій
void NetworkModel::StationsGeneration() {
    Workstation tempWorkstation{};
    int count = CNodeCount;
    std::vector<int> points{2, 4, 8, 12, 17, 22};

    for (auto &i: points) {
        tempWorkstation.SetNodeID(count++);

        auto tempChannel = new Channel(nodes[i].GetID(), tempWorkstation.GetNodeID(), ChannelType::FULL_DUPLEX);
        nodes[i].SetWorkStationChannel(tempChannel);
        tempWorkstation.SetChannel(tempChannel);

        stations.emplace_back(tempWorkstation);
    }
}

int NetworkModel::AddNode() {

    return 0;
}

int NetworkModel::AddChannel() {

    return 0;
}

int NetworkModel::DeleteNode() {

    return 0;
}

int NetworkModel::DeleteChannel() {

    return 0;
}

int NetworkModel::OnNode() {

    return 0;
}

int NetworkModel::OffNode() {

    return 0;
}

int NetworkModel::OnChannel() {

    return 0;
}

int NetworkModel::OffChannel() {

    return 0;
}

// записує у file повну інформацію про мережу
void NetworkModel::ShowNetworkModel() {
    std::ofstream file("../NetworkModel.txt");
    if (!file.is_open()) std::cerr << "Error open file" << std::endl;

    for (auto &i: nodes) {
        //std::cout << i.GetNodeInfo() << std::endl;
        file << i.GetNodeInfo() << std::endl;
    }

    for (auto &i: stations) {
        file << i.GetNodeInfo() << std::endl;
    }

    file << "\nNot Full Node Connections\n" << ShowStatus() << std::endl;

    file.close();
}

// зберігає у file .csv інформацію для графічного зображення
int NetworkModel::SaveConfiguration() {
    // у .csv file має занестись інформація:
    // вузли - id
    // робочі станції - id вузла до якого під'єднана
    // канали - тип(дуплекс, напівдуплекс, супутниковий); вага; вузли які поєднує.

    return 0;
}

// ------------------------------- Messages -----------------------------------

void NetworkModel::RunSimulation() {

}

// --------------------------- Additional functions ---------------------------
int GenerateRandomNumber(int x, int y) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(x, y);

    return dis(gen);
}

void PrintToFile(const std::string &text) {
    std::ofstream file("../LogFile.txt", std::ios::app);
    if (!file.is_open()) std::cerr << "Error open file" << std::endl;

    file << text;

    file.close();
}