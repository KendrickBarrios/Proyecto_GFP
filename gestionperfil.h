#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <cstring>
#include <string>
#include <regex>
#include <vector>
#include <conio.h>

// expresiones regulares que especifican los caracteres permitidos en el nombre de usuario y contrasena
std::regex patronUser("^[a-zA-Z0-9_]+$"); 
std::regex patronPassword("^[\\w\\d!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~]+$");

// funcion que valida que se introduzcan datos que correspondan al tipo int
void validar (int *n, int a = 0) {
	double temp;
	while (true) {
		if (std::cin >> temp && std::fmod(temp, 1) == 0 && (a == 0 || temp > 0)) {
            break;		
		} else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nEl numero ingresado no debe contener decimales, letras ni otros caracteres especiales."
			<< "\nPor favor ingrese un numero segun lo especificado: ";
		}
	}
	*n = temp;
}

// funcion que valida que se introduzcan datos que correspondan al tipo double
void validar (double *p) {
	double temp;
	while (true) {
		if (std::cin >> temp) {
			break;
		} else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nEl numero ingresado no debe contener letras ni otros caracteres especiales."
			<< "\nPor favor ingrese un numero segun lo especificado: ";
		}
	}
	*p = temp;
}

// funcion que encripta y desencripta cadenas
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
                cripto += aux[i];
            } else {
                aux[i] += 26;
                cripto += aux[i];
            }
        }
    }

    return cripto;
}

// funcion que oculta la contraseña al momento de escribirla
void ocultarContrasena(std::string& password) {
    password = ""; // Clear the password string

    char ch;
    while ((ch = _getch()) != 13) {  // lee caracteres hasta que el usuario presione Enter
        if (ch == 8 && !password.empty()) {  // elimina el ultimo caracter si se presiona Backspace
            std::cout << "\b \b";  // retrocede el cursor, imprime un espacio y vuelve a avanzar el cursor
            password.pop_back();
        } else if (ch != 8) {
            std::cout << '*'; // imprime un * por cada caracter ingresado
            password += ch; // es añade el caracter al string password
        }
    }
    std::cout << '\n';
}

// funcion que permite validar el nombre de usuario ingresado
int validarUser (std::string user) {
    std::fstream archivo;
    std::streampos tamano;
    std::string perfil;
    int coincidencia = 0;
    
    // validacion de nombre de usuario unico y permitido
    if (std::regex_match(user, patronUser)) {
        archivo.open("perfiles.txt", std::ios::in);
        if (archivo.is_open()) {
            archivo.seekg(0, std::ios::end);
            tamano = archivo.tellg();
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

    return coincidencia;
}

void validarPassword (std::string password, std::string password2, bool *pase) {
    int coincidencia = password.compare(password2);
    
    if (coincidencia != 0) {
        std::cout << "\nLas contrasenas deben ser iguales, por favor, ingrese nuevamente los datos.\n";
        system("pause");
    } else if (std::regex_match(password, patronPassword)) {
        *pase = true; // actualizacion del condicional
    } else if (!std::regex_match(password, patronPassword)) {
        std::cout << "\nLa contrasena ingresada no cumple con los requisitos.\n";
        system("pause");
    }
}

// funcion que permite crear un nuevo perfil con un nombre unico y contrasena
void crearperfil () {
    bool pase = false;
    std::fstream archivo;
    std::string user, perfil, password, password2; 
    int coincidencia = 0;
    
    // lectura y validacion de nombre de usuario
    do {
        system("cls");
        std::cout << "(1) Crear Perfil\n"
        << "\n*El nombre solo debe contener letras, numeros o guion bajo ( \"_\" ), no puede incluir espacios.*\n"
        << "\nSi no desea crear un perfil, ingrese -1 para volver al menu principal.\n"
        << "\nIngrese el nombre de usuario: ";
        std::getline(std::cin >> std::ws, user);
        if (user == "-1") {
            return;
        } else {
            coincidencia = validarUser (user);
        }
        
        // validacion de password permitido
        if (std::regex_match(user, patronUser) && coincidencia != 0) {
             std::cout << "\nLa contrasena puede incluir simbolos (no puede incluir espacios).\n"
            << "\nIngrese la contrasena: ";
            ocultarContrasena(password);
            std::cout << "\nIngrese nuevamente la contrasena: ";
            ocultarContrasena(password2);
            validarPassword (password, password2, &pase);
        }

    } while (pase == false);

    // actualizacion del archivo de perfiles
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
        archivo << "0000000\n";
        archivo.clear();  
        archivo.close();      
    }

    // creacion del archivo de categorias
    user += "categorias.txt";
    archivo.open(user, std::ios::out);
    if (archivo.is_open()) {
        archivo << "SueldoI\nIngresos VariosI\nProductos BasicosE\nVestido y CalzadoE\nServicios BasicosE\nSaludE\nTransporteE\nEducacionE\nOcioE\n";
        archivo.clear();
        archivo.close();
    }
    
    std::cout << "\nEl perfil ha sido creado correctamente. Volviendo al menu principal.\n";
    system("pause");
}

void modificarperfil() {
    std::fstream archivo, archivo2;
    std::string linea, user, password;
    std::vector<std::string> users;
    std::vector<std::string> passwords;
    std::streampos tamano;
    int i, j, indice, op, coincidencia;
    bool pase = false;
    
    do {
        i = 0;
        j = 1;
        archivo.open("perfiles.txt", std::ios::in);
        archivo.seekg(0, std::ios::beg);
        system("cls");
        std::cout << "(2) Modificar perfil\n";
        // obtencion de usuarios y contrasenas
        archivo.seekg(0, std::ios::end);
        tamano = archivo.tellg();
        archivo.seekg(0, std::ios::beg);
        if (tamano > 0) {
            std::cout << "\nPerfiles existentes:\n";
            while (std::getline(archivo, linea)) {
                if (!linea.empty()) {
                    if (i % 2 == 0) {
                        linea = encriptar(linea, 2);
                        users.push_back(linea);
                        std::cout << j << " - " << users[j - 1] << '\n';
                        j++;
                    } else {
                        passwords.push_back(linea);
                    }
                    i++;
                }
            }
            archivo.clear();
            archivo.close();
        } else {
            std::cout << "\nAun no se ha creado ningun perfil, volviendo al menu principal.\n";
            system("pause");
            return;
        }
        
        // seleccion de perfil a modificar
        std::cout << "\nIngrese el numero del perfil que desea modificar (ingrese -1 si no desea modificar un perfil): ";
        validar(&indice);
        if (indice > 0 && indice <= users.size()) {
            std::cout << "\nIngrese la contrasena del perfil: ";
            ocultarContrasena(linea);
            if (linea.compare(encriptar(passwords[indice - 1], 2)) == 0) {
                do {
                    system("cls");
                    std::cout << "(2) Modificar perfil\n"
                    << '\n' << indice << " - " << users[indice - 1] << '\n'
                    << "\nIndique la accion que desea realizar:\n"
                    << "(1) Modificar el nombre de usuario\n"
                    << "(2) Modificar la contrasena\n"
                    << "(-1) Volver al menu principal\n";
                    validar(&op);

                    if (op == 1) { // modificacion de nombre de usuario
                        std::cout << "\nIngrese el nuevo nombre de usuario (o -1 para volver al menu de modificacion): ";
                        std::getline(std::cin >> std::ws, user);
                        if (user == "-1") {
                            break;
                        } else {
                            coincidencia = validarUser(user);
                            if (coincidencia != 0) {
                                // actualizacion del csv del perfil
                                linea = encriptar(users[indice - 1], 1) + ".csv";
                                archivo.open(linea, std::ios::in);
                                linea = encriptar(user, 1) + ".csv";
                                archivo2.open(linea, std::ios::out);
                                archivo.seekg(0, std::ios::end);
                                tamano = archivo.tellg();
                                archivo.seekg(0, std::ios::beg);
                                if (tamano > 0) {
                                    while (std::getline(archivo, linea)) {
                                        if (!linea.empty()) {
                                            archivo2 << linea << '\n';
                                        }
                                    }
                                }
                                archivo.clear();
                                archivo2.clear();
                                archivo.close();
                                archivo2.close();
                                linea = encriptar(users[indice - 1], 1) + ".csv";
                                std::remove(linea.c_str());

                                // actualizacion del archivo de perfiles
                                archivo2.open("perfiles.txt", std::ios::out | std::ios::trunc);
                                users[indice - 1] = user;
                                for (i = 0; i < j - 1; i++) {
                                    users[i] = encriptar(users[i], 1);
                                    archivo2 << users[i] << '\n';
                                    archivo2 << passwords[i] << '\n';
                                }
                                archivo2.setstate(std::ios::goodbit);
                                archivo2.close();
                                users.clear();
                                passwords.clear();
                                std::cout << "\nNombre de usuario modificado con exito. Volviendo al menu principal\n";
                                pase = true;
                                system("pause");
                            }
                        }
                        break;
                        
                    } else if (op == 2) { // modificacion de contrasena
                        std::cout << "\nIngrese la nueva contrasena (o -1 para volver al menu de modificacion): ";
                        ocultarContrasena(password);
                        if (password == "-1") {
                            break;
                        } else {
                            std::cout << "\nConfirme la nueva contrasena: ";
                            ocultarContrasena(linea);
                            validarPassword(password, linea, &pase);
                            if (pase == true) {
                                // actualizacion del archivo de perfiles
                                archivo2.open("perfiles.txt", std::ios::out | std::ios::trunc);
                                passwords[indice - 1] = encriptar(password, 1);
                                for (i = 0; i < j - 1; i++) {
                                    users[i] = encriptar(users[i], 1);
                                    archivo2 << users[i] << '\n';
                                    archivo2 << passwords[i] << '\n';
                                }
                                archivo2.setstate(std::ios::goodbit);
                                archivo2.close();
                                users.clear();
                                passwords.clear();
                                std::cout << "\nContrasena modificada con exito. Volviendo al menu principal\n";
                                system("pause");
                                break;
                            }
                        }
                    } else if (op == -1) {
                        return;
                    } else {
                        std::cout << "\nDebe escoger una opcion valida (1, 2 o -1).\n";
                        system("pause");
                    }
                } while (true);
            } else {
                std::cout << "\nLa contrasena ingresada no es correcta.\n";
                system("pause");
            }
        } else if (indice == -1) {
            return;
        } else {
            std::cout << "\nDebe escoger una opcion valida.\n";
            system("pause");
        }
    } while (pase == false);
}

std::string accederperfil () {
    std::fstream archivo;
    std::streampos tamano;
    std::string linea;
    std::vector <std::string> users;
    std::vector <std::string> passwords;
    int i, indice;
    
    archivo.open("perfiles.txt", std::ios::in);
    archivo.seekg(0, std::ios::end);
    tamano = archivo.tellg();
    archivo.seekg(0, std::ios::beg);
    if (tamano == 0) {
            system("cls");
            std::cout << "(3) Acceder a perfil\n"
            << "\nAun no se ha creado ningun perfil. Volviendo al menu principal.\n";
            archivo.clear();
            archivo.close();
            system("pause");
            return "-1";
    } else {
        while (std::getline(archivo, linea)) {
                if (!linea.empty()) {
                    i++;
                    linea = encriptar(linea, 2);
                    users.push_back(linea);
                    std::getline(archivo, linea);
                    passwords.push_back(linea);
                }
        }
        archivo.clear();
        archivo.close();
        do {
            system("cls");
            std::cout << "(3) Acceder a perfil\n";
            std::cout << "\nPerfiles existentes:\n";
            for (i = 1; i <= users.size(); i++) {
                std::cout << i << " - " << users[i - 1] << '\n';
            }
            std::cout << "\nIngrese el numero del perfil al que desea acceder (o -1 para volver al menu principal): ";
            validar(&indice);
            if (indice == -1) {
                return "-1";
            } else if (indice > 0 && indice <= users.size()) {
                std::cout << "\nIngrese la contrasena del perfil " << users[indice - 1] << " (o -1 para volver al menu de acceso): ";
                ocultarContrasena(linea);
                if (encriptar(linea, 1) == passwords[indice - 1]) {
                    system("cls");
                    std::cout << "(3) Acceder a perfil\n\nAcceso correcto. Presentando menu de perfil.\n";
                    system("pause");
                    return encriptar(users[indice - 1], 1);
                } else if (encriptar(linea, 1) != passwords[indice - 1] && linea != "-1") {
                    std::cout << "\nLa contrasena ingresada no es correcta.\n";
                    system("pause");
                }
            } else {
                std::cout << "\nPor favor, elija un numero valido.\n";
                system("pause");
            } 
        } while (true);
    }
}

void eliminarperfil () {
    std::fstream archivo, archivo2;
    std::streampos tamano;
    std::string linea, linea2;
    std::vector <std::string> users;
    std::vector <std::string> passwords;
    char op;
    int i = 0, j, indice;

    archivo.open("perfiles.txt", std::ios::in);
    archivo.seekg(0, std::ios::end);
    tamano = archivo.tellg();
    archivo.seekg(0, std::ios::beg);
    if (tamano == 0) {
            system("cls");
            std::cout << "(4) Eliminar perfil\n"
            << "\nAun no se ha creado ningun perfil. Volviendo al menu principal.\n";
            archivo.clear();
            archivo.close();
            system("pause");
            return;
    } else {
        while (std::getline(archivo, linea)) {
                if (!linea.empty()) {
                    i++;
                    users.push_back(encriptar(linea, 2));
                    std::getline(archivo, linea);
                    passwords.push_back(linea);
                }
        }
        archivo.clear();
        archivo.close();
        do {
            system("cls");
            std::cout << "(4) Eliminar perfil\n";
            std::cout << "\nPerfiles existentes:\n";
            for (j = 1; j <= users.size(); j++) {
                std::cout << j << " - " << users[j - 1] << '\n';
            }
            std::cout << "\nIngrese el numero del perfil que desea eliminar (o -1 para volver al menu principal): ";
            validar(&indice);
            if (indice == -1) {
                return;
            } else if (indice > 0 && indice <= users.size()) {
                std::cout << "\nIngrese la contrasena del perfil " << users[indice - 1] << " (o -1 para volver al menu de eliminacion): ";
                ocultarContrasena(linea);
                if (encriptar(linea, 1) == passwords[indice - 1]) {
                    std::cout << "\nIngrese Y para confirmar la eliminacion, o N para cancelar: ";
                    std::cin >> op;
                    while (true) {
                        if (op != 'y' && op != 'Y' && op != 'N' && op != 'n') {
                            std::cout << "\nPor favor ingrese un caracter segun lo indicado: ";
                            std::cin >> op;
                        } else if (op == 'y' || op == 'Y') {
                            break;
                        } else {
                            return;
                        }
                    }
                    linea = encriptar(users[indice - 1], 1) + ".csv";
                    linea2 = encriptar(users[indice - 1], 1) + "categorias.txt";
                    std::remove(linea.c_str());
                    std::remove(linea2.c_str());
                    users.erase(users.begin() + (indice - 1));
                    passwords.erase(passwords.begin() + (indice - 1));
                    archivo2.open("perfiles.txt", std::ios::out | std::ios::trunc);
                    for (j = 0; j < users.size(); j++) {
                        users[j] = encriptar(users[j], 1);
                        archivo2 << users[j] << '\n';
                        archivo2 << passwords[j] << '\n';
                    }
                    archivo2.setstate(std::ios::goodbit);
                    archivo2.close();
                    users.clear();
                    passwords.clear();
                    std::cout << "\nSe ha eliminado correctamente el perfil. Volviendo al menu principal.\n";
                    system("pause");
                    return;
                } else if (encriptar(linea, 1) != passwords[indice - 1] && linea != "-1"){
                    std::cout << "\nLa contrasena ingresada no es correcta.\n";
                    system("pause");
                }
            } else {
                std::cout << "\nPor favor, elija un numero valido.\n";
                system("pause");
            }
        } while (true);
    }
}
