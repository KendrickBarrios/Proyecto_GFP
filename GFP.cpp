#include <iostream>
#include <fstream>
#include <locale.h>
#include <cstring>
#include <string>
#include <vector>
#include "gestionperfil.h"

void menuperfil(std::string perfil);
void menuprincipal();

int main () {
    setlocale(LC_ALL, "spanish");
    menuprincipal();
    return 0;
}

void menuprincipal () {
    int op;
    std::string perfil;
    do {
        system("cls");
        std::cout << "misBilletes - Gestor de Finanzas Personales\n"
        << "\nMenu Principal\n\n"
        << "(1) Crear un nuevo perfil\n"
        << "(2) Modificar un perfil\n"
        << "(3) Acceder a un perfil\n"
        << "(4) Eliminar un perfil\n"
        << "(5) Salir de la aplicacion\n\n"
        << "Indique la accion que desea realizar: ";
        validar(&op);

        switch (op) {
            case 1:
                crearperfil();
                break;
            case 2:
                modificarperfil();
                break;
            case 3:
                perfil = accederperfil();
                if (perfil != "-1") {
                    menuperfil(perfil);
                    // llamada a menu perfil
                }
                break;
            case 4:
                eliminarperfil();
                break;
            case 5:
                std::cout << "\nGracias por usar el programa.\n";
                system("pause");
                break;
            default:
                std::cout << "\nPor favor, elija una opcion valida (1-5).\n";
                system("pause");
                break;
        }
    } while (op != 5);
}

void menuperfil (std::string perfil) {
    int op;
    std::string nombrearchivo = perfil + ".csv";
    std::fstream archivo;
    do {
        system("cls");
        std::cout << "misBilletes - Gestor de Finanzas Personales\n"
        << "\nTe damos la bienvenida, " << encriptar(perfil, 2) << "\n\n"
        << "(1) Ingresar una operacion\n"
        << "(2) Ver ultimas operaciones\n"
        << "(3) Consultar balance\n"
        << "(4) Comparar periodos\n"
        << "(5) Ver reporte mensual\n"
        << "(6) Salir del perfil\n\n"
        << "Indique la accion que desea realizar: ";
        validar(&op);

        switch (op) {
            case 1:
                // llamada a funcion de registro de operacion
                break;
            case 2:
                // llamada a funcion de ver ultimas operaciones
                break;
            case 3:
                // llamada a consultar balance
                break;
            case 4:
                break;
            case 5:
                // llamada a comparar periodos
                system("pause");
                break;
            case 6:
                return;
            default:
                std::cout << "\nPor favor, elija una opcion valida (1-5).\n";
                system("pause");
                break;
        }
    } while (true);
}