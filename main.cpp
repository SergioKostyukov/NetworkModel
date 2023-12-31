#include "NetworkModel.h"

void MainMenu() {
    NetworkModel networkModel;

    int choice;
    do {
        std::cout << "1. Generate network\n"
                     "2. Traffic analysis\n"
                     "3. Exit\n";
        std::cout << "Enter your choice:";
        std::cin >> choice;

        switch (choice) {
            case 1:
                networkModel.ConfigureNetwork();
                break;
            case 2:
                networkModel.RunSimulation();
                break;
            case 3:
                std::cout << "Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Error request!" << std::endl;
                break;
        }
    } while (choice != 3);
}

int main() {
    MainMenu();

    return 0;
}
