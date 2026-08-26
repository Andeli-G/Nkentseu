#pragma once
#include "Historique.hpp"

class Calculatrice {
private:
    Historique historique;

public:
    double addition(double a, double b);
    double soustraction(double a, double b);
    double multiplication(double a, double b);
    double division(double a, double b);
    double puissance(double a, double b);
    double racine(double a);
    double logarithme(double a);
    double exponentielle(double a);
    double sinus(double a);
    double cosinus(double a);

    void afficherHistorique() const;
    void sauvegarderHistorique() const;

    const Historique& obtenirHistorique() const { return historique; }
    void effacerHistorique() { historique.effacer(); }
};