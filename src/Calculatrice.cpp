#include  "Calculatrice.hpp"
#include <iostream>
#include "Historique.hpp"
#include <math.h>

double Calculatrice::addition(double a, double b) {
    historique.ajouter(
                std::to_string(a) + " + " +
                std::to_string(b) + " = " +
                std::to_string(a + b)
                );
    return a + b;
}
double Calculatrice::soustraction(double a, double b) {
    historique.ajouter(std::to_string(a) + " - " + std::to_string(b) + " = " + std::to_string(a-b));
    return a - b;
}
double Calculatrice::multiplication(double a, double b) {
    historique.ajouter(std::to_string(a) + " * " + std::to_string(b) + " = " + std::to_string(a * b));
    return a * b;
}
double Calculatrice::division(double a, double b) {
    if (b == 0) {
        std::cerr << "Erreur : division par zéro." << std::endl;
        return NAN; // Retourne NaN pour indiquer une erreur
    }
    historique.ajouter(std::to_string(a) + " / " + std::to_string(b) + " = " + std::to_string(a/b));
    return a / b;
}
double Calculatrice::puissance(double a, double b) {
    historique.ajouter(std::to_string(a) + " ^ " + std::to_string(b) + " = " + std::to_string(pow(a, b)));
    return pow(a, b);
    
}
double Calculatrice::racine(double a) {
    if (a < 0) {
        std::cerr << "Erreur : racine carrée d'un nombre négatif." << std::endl;
        return NAN; // Retourne NaN pour indiquer une erreur
    }
    historique.ajouter("racine (" + std::to_string(a) + ") = " + std::to_string(sqrt(a)));
    return sqrt(a);
}
double Calculatrice::logarithme(double a) {
    if (a <= 0) {
        std::cerr << "Erreur : logarithme d'un nombre non positif." << std::endl;
        return NAN; // Retourne NaN pour indiquer une erreur
    }
    historique.ajouter("logarithme (" + std::to_string(a) +  ") = " + std::to_string(log(a)));
    return log(a);
}
double Calculatrice::exponentielle(double a) {
    historique.ajouter("exp (" + std::to_string(a) +  ") = " + std::to_string(exp(a)));
    return exp(a);
           
}
double Calculatrice::sinus(double a) {
    historique.ajouter("sin (" + (std::to_string(a)) + ") = " + std::to_string(sin(a)));
    return sin(a);
}
double Calculatrice::cosinus(double a) {
    historique.ajouter("cos (" + std::to_string(a) + ") = " + std::to_string(cos(a)));
    return cos(a);

}
void Calculatrice::afficherHistorique() const {
    historique.afficher();
}
void Calculatrice::sauvegarderHistorique() const{
    historique.sauvegarder("SaveCalc.txt");
}