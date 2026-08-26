#include "Menu.hpp"
#include "Calculatrice.hpp"
#include "Historique.hpp"
#include <iostream>
#include <string>

void afficherMenu() {
    Calculatrice MaCalculatrice;
    int choix =-1;
    Historique historique;

    while (choix != 0) {

        std::cout << "\n=== Menu de la Calculatrice ===" << std::endl;
        std::cout << "1. Addition" << std::endl;
        std::cout << "2. Soustraction" << std::endl;
        std::cout << "3. Multiplication" << std::endl;
        std::cout << "4. Division" << std::endl;
        std::cout << "5. Puissance" << std::endl;
        std::cout << "6. Racine carrée" << std::endl;
        std::cout << "7. Logarithme" << std::endl;
        std::cout << "8. Exponentielle" << std::endl;
        std::cout << "9. Sinus" << std::endl;
        std::cout << "10. Cosinus" << std::endl;
        std::cout << "11. Afficher l'historique" << std::endl;
        std::cout << "0. Quitter" << std::endl;

        std::cout << "Veuillez entrer votre choix : ";
        std::cin >> choix;

        double a, b;

        switch (choix) {
            case 1:
                std::cout << "Entrez deux nombres : ";
                std::cin >> a >> b;
                std::cout << "Résultat : "
                          << MaCalculatrice.addition(a, b) << std::endl;
                
                break;

            case 2:
                std::cout << "Entrez deux nombres : ";
                std::cin >> a >> b;
                std::cout << "Résultat : "
                          << MaCalculatrice.soustraction(a, b) << std::endl;
                
                break;

            case 3:
                std::cout << "Entrez deux nombres : ";
                std::cin >> a >> b;
                std::cout << "Résultat : "
                          << MaCalculatrice.multiplication(a, b) << std::endl;
               
                break;

            case 4:
                std::cout << "Entrez deux nombres : ";
                std::cin >> a >> b;
                std::cout << "Résultat : "
                          << MaCalculatrice.division(a, b) << std::endl;
                
                break;

            case 5:
                std::cout << "Entrez le nombre et l'exposant : ";
                std::cin >> a >> b;
                std::cout << "Résultat : "
                          << MaCalculatrice.puissance(a, b) << std::endl;
                
                break;

            case 6:
                std::cout << "Entrez un nombre : ";
                std::cin >> a;
                std::cout << "Résultat : "
                          << MaCalculatrice.racine(a) << std::endl;
                
                
                break;

            case 7:
                std::cout << "Entrer le nombre dont on doit calculer le logarithme : " <<std::endl;
                std::cin >> a;
                std::cout << "Resultat :" << MaCalculatrice.logarithme(a) << std::endl;
                
                break;
            case 8: 
                std::cout << "Entrer le nombre dont on veut dalculer l'exponentielle : " << std::endl;
                std::cin >> a;
                std::cout << "Resultat :" << MaCalculatrice.exponentielle(a) <<std::endl;
                break;

            case 9:
                std::cout <<"Entrer l'angle en radiant" << std::endl;
                std::cin >> a;
                std::cout <<"Resultat :" <<MaCalculatrice.sinus(a) <<std::endl;
                break;
                
            case 10:
                std::cout <<"Entrer l'angle en radiant" << std::endl;
                std::cin >> a;
                std::cout <<"Resultat :" <<MaCalculatrice.cosinus(a) <<std::endl;
                break;
                
            case 11:
                std::cout <<"L'historique est : " <<std::endl;
                MaCalculatrice.afficherHistorique();
                break;
                
            case 0:
                std::cout << "Merci d'avoir utilisé la calculatrice. Au revoir !"
                          << std::endl;
                break;
            
            default:
                std::cout << "Choix invalide. Veuillez réessayer." << std::endl;
        }
    }
}
