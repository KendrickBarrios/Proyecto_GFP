#include <iostream>
#include <fstream>
#include <locale.h>
#include <string>
#include <vector>
#include <regex>

void crearperfil () {
    std::string user, password;
    std::regex patronUser("^[a-zA-Z0-9_]+$"); 
    std::regex patronPassword("^[\\w\\d\\s!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~]+$");
    do {
        system("cls");
        std::cout << "\n*El nombre solo debe contener letras, numeros o guion bajo ( \"_\" ).*\n"
        << "\nIngrese el nombre de usuario: ";
        std::getline(std::cin >> std::ws, user);
        std::cout << "\nLa contrasena puede incluir simbolos.\n"
        << "\nIngrese la contrasena: ";
        std::getline(std::cin >> std::ws, password);
        if (std::regex_match(user, patronUser) && std::regex_match(password, patronPassword)) {
            break;
        }
    } while (true);
}

int main () {
    setlocale(LC_ALL, "spanish");
    crearperfil();
    return 0;
}