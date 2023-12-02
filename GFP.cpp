#include <iostream>
#include <fstream>
#include <locale.h>
#include <cstring>
#include <string>
#include <vector>
#include "gestionperfil.h"
#include "RegistrosOperaciones.h"
#include "Resultados.h"

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
        validar(&op, 0);

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
                    menuperfil(perfil); // llamada a menu perfil
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
    int op, op2;
    std::vector<double> balance;
    std::string nombrearchivo = perfil + ".csv", fecha, periodo;
    std::fstream archivo;
    bool v;
    do {
        system("cls");
        std::cout << "misBilletes - Gestor de Finanzas Personales\n"
        << "\nTe damos la bienvenida, " << encriptar(perfil, 2) << "\n\n"
        << "(1) Ingresar una operacion\n"
        << "(2) Modificar una operacion\n"
        << "(3) Eliminar una operacion\n"
        << "(4) Mostrar operaciones\n"
        << "(5) Consultar balance\n"
        << "(6) Comparar periodos\n"
        << "(7) Ver reporte mensual\n"
        << "(8) Salir del perfil\n\n"
        << "Indique la accion que desea realizar: ";
        validar(&op);

        switch (op) {
            case 1:
                registrarOperacion(perfil);
                system("pause");
                break;
            case 2:
                modificarOperacion(perfil, 1);
                break;
            case 3:
                modificarOperacion(perfil, 2);
                break;
            case 4:
            	if (Operaciones.size() > 0) {
            		Operaciones.clear();
				}
				Operaciones = cargarOperaciones(perfil);
                periodo = mostrarOperaciones();
                system("pause");
                break;
            case 5:
                v = false;
                do {
                	system("cls");
                	std::cout << "(5) Consultar balances\n\n"
                	<< "1. Consultar balance por periodo\n"
                	<< "2. Consultar balance de un periodo por categorias\n"
                	<< "\nIndique la accion que desea realizar (o -1 para volver al menu principal): ";
                	validar(&op2, 0);
                    if (op2 == -1) {
                        v = true;
                    } else if (op2 == 1) {
                        balance = calcularBalance(perfil, 1);
                        system("pause");
                        v = true;
                    } else if (op2 == 2) {
                        balance = calcularBalance(perfil, 2);
                        system("pause");
                        v = true;
                    } else {
                        std::cout << "\nEl numero ingresado no corresponde a ninguna opcion, por favor verifique.\n";
                        system("pause");
                    }
				} while (v == false);
                break;
            case 6:
                // llamada a comparar periodos
                break;
            case 7:
                system("cls");
                balance = calcularBalance(perfil, 1);
                std::cout << "\n\n"; 
                if (!balance.empty()) {
                    graficoBarras(balance);
                }
                std::cout << "\n\n";
                system("pause");
                break;
            case 8:
                return;
            default:
                std::cout << "\nPor favor, elija una opcion valida (1-5).\n";
                system("pause");
                break;
        }
    } while (true);
}
