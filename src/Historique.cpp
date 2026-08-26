#include "Historique.hpp"
#include <iostream>
#include <fstream>

void Historique::ajouter(const std::string& operation) {
    operations.push_back(operation);
}

void Historique::afficher() const {
    std::cout << "\n === Historique === \n";
    if (operations.empty()) {
        std::cout << "Aucune operation. \n";
        return;
    }
    for (const auto& op : operations) {
        std::cout << op << std::endl;
    }
}

void Historique::sauvegarder(const std::string& SaveCalc) const {
    std::ofstream out(SaveCalc);
    for (const auto& op : operations) {
        out << op << std::endl;
    }
}

void Historique::effacer() {
    operations.clear();
}

const std::vector<std::string>& Historique::obtenirEntrees() const {
    return operations;
}
