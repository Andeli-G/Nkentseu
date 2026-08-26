#pragma once
#include <string>
#include <vector>

class Historique {
private:
    std::vector<std::string> operations;

public:
    void ajouter(const std::string& operation);
    void afficher() const;
    void sauvegarder(const std::string& fichier) const;
    void effacer();
    const std::vector<std::string>& obtenirEntrees() const;
};