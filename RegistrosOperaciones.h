#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <regex>

// expresion regular que valida que el formato de la fecha ingresada sea DD-MM-AAAA
std::regex patronfecha("^(0[1-9]|[1-2][0-9]|3[0-1])-(0[1-9]|1[0-2])-(\\d{4})$");

// Vector para las categorias de operaciones
std::vector <std::string> Categorias;

// Nuevo codigo para la ultima operacion
std::string ultimoCodigo;

// Struct para representar una operación
struct Operacion {
    std::string codigo;
    std::string fechaRegistro;
    std::string fechaTransaccion;
    std::string categoria;
    double monto;
    std::string descripcion;
};

// Vector para almacenar las operaciones existentes
std::vector <Operacion> Operaciones;

// Funciones para manipular los campos del struct
std::string generarCodigo() {
    int codigo = std::stoi(ultimoCodigo) + 1;
    std::string nuevoCodigo = std::to_string(codigo);
    while (nuevoCodigo.length() < 7) {
        nuevoCodigo = "0" + nuevoCodigo;
    }
    return nuevoCodigo;
}

std::string obtenerFechaActual() {
    time_t actual = time(0);
    tm* timeinfo = localtime(&actual);
    char buffer[15];
    strftime(buffer, 80, "%d-%m-%Y", timeinfo);
    return std::string(buffer);
}

std::string obtenerFechaTransaccion() {
    std::string fecha;
    int anio, mes, dia;
    bool pase = false;

    while (pase == false) {
        std::cout << "\nFormato de fecha: DD-MM-AAAA, anios permitidos: 1900-2023\n"
        << "\nIngrese la fecha de la operacion (o 0 si desea usar la fecha actual): ";
        std::getline(std::cin >> std::ws, fecha);
        if (fecha == "0") { 
            fecha = obtenerFechaActual();
            return fecha;
        }
        // Se obtiene el anio, mes y dia a partir del string fecha
        dia = std::stoi(fecha.substr(0, 2));
        mes = std::stoi(fecha.substr(3, 2));
        anio = std::stoi(fecha.substr(6, 4));

        if (std::regex_match(fecha, patronfecha) && (anio >= 1900 && anio <= 2023)) {
            // Comprueba que la fecha ingresada sea valida
            if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && (dia > 0 && dia < 31)) {
                pase = true;
            } else if (mes == 2) {
                // Verifica si el dia es valido, segun si el año es bisiesto, y el mes es febrero
                if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) {
                    if (dia > 0 && dia < 30) {
                        pase = true;
                    }
                } else {
                    if (dia > 0 && dia < 29) {
                        pase = true;
                    }
                }
            } else if ((mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) && (dia > 0 && dia < 32)) {
                pase = true;
            }
            if (pase == false) {
                std::cout << "\nLa fecha ingresada no es valida, por favor verifique.\n";
            }
        } else {
            std::cout << "\nLa fecha ingresada no es valida o no cumple con las condiciones.\n";
        }
    }

    return fecha;
}

// Función para cargar las categorías desde un archivo
std::vector<std::string> cargarCategorias(std::string perfil) {
    std::vector<std::string> categorias;
    std::fstream archivocat;

    archivocat.open(perfil + "categorias.txt", std::ios::in);
    if (archivocat.is_open()) {
        std::string categoria;
        while (std::getline(archivocat, categoria)) {
            if (!categoria.empty()) {
                categorias.push_back(categoria);
            }
        }
        archivocat.close();
    } else {
        std::cerr << "No se pudo abrir el archivo de categorias." << '\n';
    }
    return categorias;
}

// Función para guardar las categorías en un archivo
void guardarCategorias(const std::vector<std::string>& categorias, std::string perfil) {
    std::fstream archivo;
    archivo.open(perfil + "categorias.txt", std::ios::out);
    if (archivo.is_open()) {
        for (int i = 0; i < categorias.size(); i++) {
            archivo << categorias[i] << "\n";
        }
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo de categorías para guardar." << std::endl;
    }
}

bool agregarCategoria(std::string perfil);

// Funciones relacionadas con las categorías
std::string obtenerCategoria(std::string perfil) {
    Categorias.clear();
    Categorias = cargarCategorias(perfil);

    int i, opcion;
    bool nuevacat;
    do {
        std::cout << "\nSeleccione la categoria de la operacion:\n";
        for (i = 0; i < Categorias.size(); ++i) {
            std::cout << i + 1 << ". " << Categorias[i] << "\n";
        }
        std::cout << Categorias.size() + 1 << ". Aniadir nueva categoria\n";
        std::cout << "Opcion: ";
        validar(&opcion);

        if (opcion == (Categorias.size() + 1)) {
            nuevacat = agregarCategoria(perfil);
        } else if (opcion < 1 || opcion > (Categorias.size() + 1)) {
            std::cout << "\nPor favor, seleccione una opcion valida (1-" << Categorias.size() + 1 << ").\n";
        }
    } while (opcion < 1 || opcion > (Categorias.size() + 1));

    if ((opcion > 0 && opcion <= Categorias.size()) || (opcion == (Categorias.size() + 1) && nuevacat == true) ) {
        return Categorias[opcion - 1];
    } else if (opcion == (Categorias.size() + 1) && nuevacat == false) {
        return "xxxxxxxxxx";
    }
}

bool agregarCategoria(std::string perfil) {
    bool pase;
    int i;
    std::string nuevaCategoria;
    do {
        pase = true;
        std::cout << "\nIngrese el nombre de la nueva categoria (o -1 para regresar al menu anterior): ";
        std::getline(std::cin >> std::ws, nuevaCategoria);
        for (i = 0; i < Categorias.size(); i++) {
            if (nuevaCategoria == Categorias[i]) {
                pase = false;
                std::cout << "La categoria " << nuevaCategoria << " ya existe, por favor, verifique.\n";
            }
        }
        if (nuevaCategoria == "-1") {
            return false;
        }
    } while (pase == false);

    Categorias.push_back(nuevaCategoria);
    guardarCategorias(Categorias, perfil);
    return true;
}

double obtenerMonto() {
    double monto;
    do {
        std::cout << "\nIngrese el monto de la operacion: ";
        validar(&monto);
        if (monto < 0) {
            std::cout << "\nEl monto debe ser un valor positivo." << std::endl;
        }
    } while (monto < 0);
    return monto;
}

std::string obtenerDescripcion() {
    std::string descripcion;
    std::cout << "Ingrese una descripcion (maximo 40 caracteres): ";
    std::cin.ignore();
    std::getline(std::cin, descripcion);
    if (descripcion.length() > 40) {
        descripcion = descripcion.substr(0, 40);
    }
    return descripcion;
}

// Función para guardar una operación en un archivo CSV
void guardarOperacion(std::string perfil) {
    std::fstream archivo;
    archivo.open(perfil + ".csv", std::ios::out);
    if (archivo.is_open()) {
        archivo << ultimoCodigo << '\n';
        for (int i = 0; i < Operaciones.size(); i++) {
            archivo << Operaciones[i].codigo << "," << Operaciones[i].fechaRegistro << "," << Operaciones[i].fechaTransaccion << ","
                << Operaciones[i].categoria << "," << Operaciones[i].monto << "," << Operaciones[i].descripcion << "\n";
        }
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para guardar la operación." << std::endl;
    }
}

// Funcion para cargar las operaciones desde un archivo CSV
std::vector<Operacion> cargarOperaciones(std::string perfil) {
    std::vector<Operacion> operaciones;
    std::fstream archivo;
    std::string linea;
    std::string codigo, fechaRegistro, fechaTransaccion, categoria, montoStr, descripcion;
    double monto;
    archivo.open(perfil + ".csv", std::ios::in);
    if (archivo.is_open()) {
        std::getline(archivo, linea, '\n');
        if (!archivo.eof()) {
        	while (std::getline(archivo, codigo, ',')) {
        		if (!linea.empty()) {
                	std::getline(archivo, fechaRegistro, ',');
                	std::getline(archivo, fechaTransaccion, ',');
                	std::getline(archivo, categoria, ',');
                	std::getline(archivo, montoStr, ',');
                	std::getline(archivo, descripcion);
                	monto = std::stod(montoStr);
                	operaciones.push_back({codigo, fechaRegistro, fechaTransaccion, categoria, monto, descripcion});
            	}
        	}
		}
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para cargar las operaciones." << std::endl;
    }
    return operaciones;
}

// Funcion para mostrar las operaciones de un usuario
void mostrarOperaciones() {
    int i, j = 0;
    std::vector<std::string> periodos;
    // for (i = 0; i < Operaciones.size(); i++) {
    //     if (i == 0) {
    //         periodos.push_back(Operaciones[i].fechaTransaccion.substr(3, 7));
    //         j++;
    //     } else {
    //         if (Operaciones[i].fechaTransaccion.substr(3, 7) != periodos[j - 1]) {
    //             periodos.push_back(Operaciones[i].fechaTransaccion.substr(3, 7));
    //             j++;
    //         }            
    //     }
    // }
    std::cout << std::setw(8) << std::left << "|  Codigo "
    << std::setw(20) << std::left << " | Fecha de Registro"
    << std::setw(23) << std::left << " | Fecha de Transaccion"
    << std::setw(29) << std::left << " |          Categoria    "
    << std::setw(18) << std::left << " |      Monto     " << " | Descripcion\n";

    for (Operacion& operacion : Operaciones) {
        std::cout << "|  " << std::setw(8) << std::left << operacion.codigo
        << "| " << std::setw(17) << std::left << operacion.fechaRegistro
        << " | " << std::setw(20) << std::left << operacion.fechaTransaccion
        << " | " << std::setw(26) << std::left << operacion.categoria
        << " |  " << std::setw(13) << std::left << std::setprecision(2) << std::fixed << operacion.monto 
        << "  | " << operacion.descripcion << "\n";
    }
}

void registrarOperacion (std::string perfil, int modo) {
    int i, op, op2;
    std::string titulos[] = {"(1) Ingresar una operacion\n\n", "(2) Modificar una operacion\n\n", "(3) Eliminar una operacion"};
    if (Operaciones.size() > 0) {
        Operaciones.clear();
    }
    Operaciones = cargarOperaciones(perfil);
    std::string linea, fechaActual = obtenerFechaActual();
    std::fstream archivo;
    archivo.open(perfil + ".csv", std::ios::in);
    std::getline(archivo, ultimoCodigo);
    archivo.close();
    if (Categorias.size() > 0) { Categorias.clear(); }
    Categorias = cargarCategorias(perfil);
    archivo.close();
    Operacion nuevaop;
    if (modo == 1) {
        nuevaop = {ultimoCodigo, fechaActual, "xx-xx-xxxx", "xxxxxxxxxx", 0, "xxxxxxxxxxxxxxxxxxxxxxxxxx"};
    } else if (modo == 2 || modo == 3) {
        if (Operaciones.size() > 0) {
            mostrarOperaciones();
        }
    } 
    while (modo == 1 || modo == 2) {
        system("cls");
        std::cout << titulos[modo]
        << "                                         (1)                       (2)                      (3)         (4)\n"
        << std::setw(8) << std::left << "|  Codigo "
        << std::setw(20) << std::left << " | Fecha de Registro"
        << std::setw(23) << std::left << " | Fecha de Transaccion"
        << std::setw(29) << std::left << " |          Categoria    "
        << std::setw(18) << std::left << " |      Monto     " << " | Descripcion\n"
        << "|  " << std::setw(8) << std::left << nuevaop.codigo
        << "| " << std::setw(17) << std::left << nuevaop.fechaRegistro
        << " | " << std::setw(20) << std::left << nuevaop.fechaTransaccion
        << " | " << std::setw(26) << std::left << nuevaop.categoria
        << " |  " << std::setw(13) << std::left << std::setprecision(2) << std::fixed << nuevaop.monto << "  | " << nuevaop.descripcion << "\n\n"
        << "Seleccione el campo que desea modificar (5 para completar el registro, o -1 para volver al menu principal): ";
        
        validar(&op);
        switch (op) {
            case 1:
                nuevaop.fechaTransaccion = obtenerFechaTransaccion();
                break;
            case 2:
                nuevaop.categoria = obtenerCategoria(perfil);
                break;
            case 3:
                nuevaop.monto = obtenerMonto();
                break;
            case 4:
                nuevaop.descripcion = obtenerDescripcion();
                break;
            case 5:
                if (nuevaop.fechaTransaccion == "xx-xx-xxxx" || nuevaop.categoria == "xxxxxxxxxx" || nuevaop.monto == 0) {
                    std::cout << "\nLos campos de fecha de transaccion, categoria y monto son obligatorios para registrar la operacion, por favor, verifique.\n";
                    system("pause");
                    break;
                } else {
                    Operaciones.push_back(nuevaop);
                    ultimoCodigo = generarCodigo();
                    guardarOperacion(perfil);
                    std::cout << "\nOperacion registrada con exito, volviendo al menu principal.\n";
                    return;
                }
            case -1:
                std::cout << "\nVolviendo al menu principal.\n";
                return;
            default:
                std::cout << "\nPor favor, ingrese una opcion valida (1-5, -1).\n";
        }
    }
}