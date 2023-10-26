#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <cstring>
#include <string>
#include <regex>

// funcion que valida que no se introduzcan datos que no correspondan al tipo int
void validar (int *n, int a = 0) {
	double temp;
	while (true) {
		if (std::cin >> temp && std::fmod(temp, 1) == 0 && (a == 0 || temp > 0)) {
            break;		
		} else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nEl número ingresado no debe contener decimales, letras ni otros caracteres especiales."
			<< "\nPor favor ingrese un número según lo especificado: ";
		}
	}
	*n = temp;
}

// funcion que valida que no se introduzcan datos que no correspondan al tipo double
void validar (double *p) {
	double temp;
	while (true) {
		if (std::cin >> temp) {
			break;
		} else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nEl número ingresado no debe contener letras ni otros caracteres especiales."
			<< "\nPor favor ingrese un número según lo especificado: ";
		}
	}
	*p = temp;
}

// funcion que encripta y desencripta strings
std::string encriptar (std::string str, int modo) {
    int i;
    char aux [str.length() + 1];
    std::string cripto = "";
    std::strcpy(aux, str.c_str());
    
    // cifrado
    if (modo == 1) {
        for (i = 0; i < str.length(); i++) {
            if (aux[i] >= 65 && aux[i] <= 73) {
                aux[i] -= 16;
                cripto += ' ';
                cripto += aux[i];
            } else if ((aux[i] > 73 && aux[i] < 84) || aux[i] > 90) {
                aux[i] -= 26;
                cripto += aux[i];
            } else if (aux[i] >= 84 && aux[i] <= 90) {
                aux[i] -= 35;
                cripto += ',';
                cripto += aux[i];
            } else {
                aux[i] -= 1;
                cripto += '-';
                cripto += aux[i];
            }
        }

    // descifrado
    } else if (modo == 2) {
        for (i = 0; i < str.length(); i++) {
            if (aux[i] == ' ') {
                i++;
                aux[i] += 16;
                cripto += aux[i];
            } else if (aux[i] == ',') {
                i++;
                aux[i] += 35;
                cripto += aux[i];
            } else if (aux[i] == '-') {
                i++;
                aux[i] += 1;
                cripto += 26;
            } else {
                aux[i] += 26;
                cripto += aux[i];
            }
        }
    }
    return cripto;
}

void crearperfil () {
    bool pase = false;
    int coincidencia = 0;
    std::fstream archivo;
    std::streampos inicio, fin;
    std::streamoff tamano;
    std::string user, perfil, password, password2;
    std::regex patronUser("^[a-zA-Z0-9_]+$"); 
    std::regex patronPassword("^[\\w\\d!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~]+$");
    
    // lectura y validacion de nombre de usuario y password
    do {
        system("cls");
        std::cout << "(1) Crear Perfil\n"
        << "\n*El nombre solo debe contener letras, numeros o guion bajo ( \"_\" ), no puede incluir espacios.*\n"
        << "\nSi no desea crear un perfil, ingrese -1 para volver al menu principal.\n"
        << "\nIngrese el nombre de usuario: ";
        std::getline(std::cin >> std::ws, user);
        if (user == "-1") {
            return;
        }
        
        // validacion de nombre de usuario unico y permitido
        if (std::regex_match(user, patronUser)) {
            archivo.open("perfiles.txt", std::ios::in);
            if (archivo.is_open()) {
                inicio = archivo.tellg();
                archivo.seekg(0, std::ios::end);
                fin = archivo.tellg();
                tamano = fin - inicio;
                archivo.seekg(0, std::ios::beg);
                if (tamano == 0) {
                    coincidencia = 1;
                } else {
                    while (std::getline(archivo, perfil)) {
                        perfil = encriptar(perfil, 2);
                        coincidencia = (user.compare(perfil));
                        if (coincidencia == 0) {
                            std::cout << "\nEl nombre de usuario ingresado coincide con uno ya existente.\n"
                            << "Por favor, ingrese un nombre de usuario diferente.\n\n";
                            system("pause");
                            break;
                        }
                    }
                }
                archivo.clear();
                archivo.close();
            }
        } else {
            std::cout << "\nEl nombre de usuario ingresado no cumple con los requisitos.\n\n";
            system("pause");
        }

        // validacion de password
        if (std::regex_match(user, patronUser) && coincidencia != 0) {
            std::cout << "\nLa contrasena puede incluir simbolos (no puede incluir espacios).\n"
            << "\nIngrese la contrasena: ";
            std::getline(std::cin >> std::ws, password);
            std::cout << "\nIngrese nuevamente la contrasena: ";
            std::getline(std::cin >> std::ws, password2);
            coincidencia = password.compare(password2);
            if (coincidencia != 0) {
                std::cout << "\nLas contrasenas deben ser iguales, por favor, ingrese nuevamente los datos.\n";
                system("pause");
            } else if (std::regex_match(password, patronPassword)) {
                pase = true; // actualizacion del condicional
            } else if (!std::regex_match(password, patronPassword)) {
                std::cout << "\nLa contrasena ingresada no cumple con los requisitos.\n";
                system("pause");
            } 
        }
    } while (pase == false);

    // creacion / actualizacion del archivo de perfiles
    user = encriptar(user, 1);
    password = encriptar(password, 1);
    archivo.open("perfiles.txt", std::ios::out | std::ios::app);
    if (archivo.is_open()) { 
        archivo << user << '\n' << password << '\n';
        archivo.clear();
        archivo.close();
    }
    
    // creacion del archivo csv asociado al perfil
    user += ".csv";
    archivo.open(user, std::ios::out);
    if (archivo.is_open()) {
        user.erase(user.end() - 4, user.end());
        archivo.clear();  
        archivo.close();      
    }
    
    std::cout << "\nEl perfil ha sido creado correctamente. Volviendo al menu principal.\n";
    system("pause");
}

void modificarperfil() {
    std::fstream archivo;
    archivo.open("perfiles.txt", std::ios::in);
    std::string perfil;
    int i, j, coincidencia;
    bool pase = false;
    do {
        i = 0;
        j = 1;
        archivo.seekg(0, std::ios::beg);
        system("cls");
        std::cout << "(2) Modificar perfil\n"
        << "\nPerfiles existentes:\n";
        while (std::getline(archivo, perfil)) {
            if (i % 2 == 0) {
                perfil = encriptar(perfil, 2);
                std::cout << j << "- " << perfil << '\n';
                j++;
            }
            i++;
        }
        archivo.clear();
        archivo.seekg(0, std::ios::beg);
        std::cout << "\nIngrese el numero del perfil que desea modificar: ";
        system("pause");
    } while (pase == false);
}