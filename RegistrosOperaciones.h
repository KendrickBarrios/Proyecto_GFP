#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cctype>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <regex>
#include <locale.h>

#define ANSI_COLOR_RESET   "\033[0m"
#define ANSI_COLOR_GREEN   "\033[32m"
#define ANSI_COLOR_RED     "\033[31m"

// expresion regular que valida que el formato de la fecha ingresada sea DD-MM-AAAA
std::regex patronfecha("^(0[1-9]|[1-2][0-9]|3[0-1])-(0[1-9]|1[0-2])-(\\d{4})$");

// Vector para las categorias de operaciones
std::vector <std::string> Categorias;
std::vector<char> Tipos;

// Nuevo codigo para la ultima operacion
std::string ultimoCodigo;

// Struct para representar una operación
struct Operacion {
    std::string codigo;
    std::string fechaRegistro;
    std::string fechaTransaccion;
    std::string categoria;
    char tipo;
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
        // Se valida que la fecha tenga 10 caracteres y se obtiene el anio, mes y dia a partir del string
        while (fecha.length() != 10) {
            std::cout << "\nLa fecha ingresada no es valida, ingrese nuevamente la fecha: ";
            std::getline(std::cin >> std::ws, fecha);
        }
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
void cargarCategorias(std::string perfil, std::vector<std::string> &categorias, std::vector<char> &tipo) {
    std::fstream archivocat;
    std::string categoria;

    archivocat.open(perfil + "categorias.txt", std::ios::in);
    if (archivocat.is_open()) {
        while (std::getline(archivocat, categoria)) {
            if (!categoria.empty()) {
                tipo.push_back(categoria.back()); // se almacena el ultimo caracter de cada categoria, que determina si es ingreso o egreso
                categoria.pop_back(); // se elimina el ultimo caracter de la categoria (I / E)
                categorias.push_back(categoria); // se almacena cada categoria en su vector correspondiente
            }
        }
        archivocat.close();
    } else {
        std::cerr << "No se pudo abrir el archivo de categorias." << '\n';
    }
}

// Función para guardar las categorías en un archivo
void guardarCategorias(std::vector<std::string> &categorias, std::vector<char> &tipo, std::string perfil) {
    std::fstream archivo;
    archivo.open(perfil + "categorias.txt", std::ios::out);
    if (archivo.is_open()) {
        for (int i = 0; i < categorias.size(); i++) {
            archivo << categorias[i] << tipo[i] << "\n";
        }
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo de categorías para guardar." << std::endl;
    }
}

bool agregarCategoria(std::string perfil);

// Funciones relacionadas con las categorías
void obtenerCategoria(std::string perfil, Operacion *op) {
    if (Categorias.size() > 0) {
        Categorias.clear();
        Tipos.clear();
    }
    cargarCategorias(perfil, Categorias, Tipos);

    int i, opcion;
    bool nuevacat;
    do {
        std::cout << "\nSeleccione la categoria de la operacion:\n";
        for (i = 0; i < Categorias.size(); ++i) {
            std::cout << i + 1 << ". " << Categorias[i] << "\n";
        }
        std::cout << Categorias.size() + 1 << ". Aniadir nueva categoria\n";
        std::cout << "Opcion: ";
        validar(&opcion, 0);

        if (opcion == (Categorias.size() + 1)) {
            nuevacat = agregarCategoria(perfil);
        } else if (opcion < 1 || opcion > (Categorias.size() + 1)) {
            std::cout << "\nPor favor, seleccione una opcion valida (1-" << Categorias.size() + 1 << ").\n";
        }
    } while (opcion < 1 || opcion > (Categorias.size() + 1));

    if ((opcion > 0 && opcion <= Categorias.size()) || (opcion == (Categorias.size() + 1) && nuevacat == true) ) {
        op->categoria = Categorias[opcion - 1];
        op->tipo = Tipos[opcion - 1];
        return;
    } else if (opcion == (Categorias.size() + 1) && nuevacat == false) {
        return;
    }
}

bool agregarCategoria(std::string perfil) {
    bool pase;
    int i;
    char tipocat;
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
        std::cout << "\nIndique el tipo de la nueva categoria:\nI - Ingreso\nE - Egreso\n";
        std::cin >> tipocat;
        if (tipocat != 'I' && tipocat != 'i' && tipocat != 'E' && tipocat != 'e') {
            std::cout << "\nPor favor, seleccione un tipo valido para la categoria.\n";
            pase = false;
        } else {
            tipocat = std::toupper(static_cast<unsigned char>(tipocat));
        }
    } while (pase == false);

    Tipos.push_back(tipocat);
    Categorias.push_back(nuevaCategoria);
    guardarCategorias(Categorias, Tipos, perfil);
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
    std::cout << "\nIngrese una descripcion (maximo 35 caracteres): ";
    std::cin.ignore();
    std::getline(std::cin, descripcion);
    if (descripcion.length() > 35) {
        descripcion = descripcion.substr(0, 35);
    }
    return descripcion;
}

// Función para guardar una operación en un archivo CSV
void guardarOperacion(std::string perfil) {
    std::fstream archivo;
    archivo.open(perfil + ".csv", std::ios::out | std::ios::trunc);
    if (archivo.is_open()) {
        archivo << ultimoCodigo << '\n';
        if (!Operaciones.empty()) {
            for (int i = 0; i < Operaciones.size(); i++) {
                archivo << Operaciones[i].codigo << "," << Operaciones[i].fechaRegistro << "," << Operaciones[i].fechaTransaccion << ","
                << Operaciones[i].categoria << Operaciones[i].tipo << "," << Operaciones[i].monto << "," << Operaciones[i].descripcion << "\n";
            }
        }
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para guardar la operación." << std::endl;
    }
}

// Funcion para cargar las operaciones desde un archivo CSV
std::vector<Operacion> cargarOperaciones(std::string perfil) {
    std::locale::global(std::locale("C"));
    std::vector<Operacion> operaciones;
    std::fstream archivo;
    std::string linea;
    std::string codigo, fechaRegistro, fechaTransaccion, categoria, montoStr, descripcion;
    char tipo;
    double monto;
    Operacion o;
    archivo.open(perfil + ".csv", std::ios::in);
    if (archivo.is_open()) {
        std::getline(archivo, linea, '\n');
        if (!archivo.eof()) {
        	while (std::getline(archivo, codigo, ',')) {
        		if (!codigo.empty()) {
                    o.codigo = codigo;
                	std::getline(archivo, fechaRegistro, ',');
                    o.fechaRegistro = fechaRegistro;
                	std::getline(archivo, fechaTransaccion, ',');
                    o.fechaTransaccion = fechaTransaccion;
                	std::getline(archivo, categoria, ',');
                    tipo = categoria.back();
                    o.tipo = tipo;
                    categoria.pop_back();
                    o.categoria = categoria;
                	std::getline(archivo, montoStr, ',');
                	monto = std::stod(montoStr);
                    o.monto = monto;
                	std::getline(archivo, descripcion);
                    o.descripcion = descripcion;
                	operaciones.push_back(o);
            	}
        	}
		}
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para cargar las operaciones." << std::endl;
    }
    return operaciones;
}

bool compararFechas(std::string fecha1, std::string fecha2) {
    int mes1 = std::stoi(fecha1.substr(0, 2));
    int anio1 = std::stoi(fecha1.substr(3, 4));

    int mes2 = std::stoi(fecha2.substr(0, 2));
    int anio2 = std::stoi(fecha2.substr(3, 4));

    return (anio1 < anio2) || (anio1 == anio2 && mes1 < mes2);
}

// Funcion para mostrar las operaciones de un usuario
std::string mostrarOperaciones(bool omitir) {
    int i, j;
    bool coincidencia;
    std::vector<std::string> periodos;
    std::string periodo_op;
    if (Operaciones.size() == 0) {
        std::cout << "\nActualmente no existen operaciones en el registro. Volviendo al menu principal.\n";
        system("pause");
        return "false";
    }
    for (i = 0; i < Operaciones.size(); i++) {
        if (i == 0) {
            periodos.push_back(Operaciones[i].fechaTransaccion.substr(3, 7));
        } else {
            coincidencia = false;
            for (j = 0; j < periodos.size(); j++) {
                if (Operaciones[i].fechaTransaccion.substr(3, 7) == periodos[j]) {
                    coincidencia = true;
                    break;
                }  
            }
            if (coincidencia == false) {
                periodos.push_back(Operaciones[i].fechaTransaccion.substr(3, 7));
            }          
        }
    }
    std::sort(periodos.begin(), periodos.end(), compararFechas);
    
    do {
        system("cls");
        std::cout << "\nPeriodos disponibles:\n";
        for (i = 0; i < periodos.size(); i++) {
            std::cout << i + 1 << " - " << periodos[i] << '\n';
        }
        std::cout << "\nIngrese el numero del periodo que desea consultar (o -1 para volver al menu anterior): ";
        validar(&j, 0);
        if (j == -1) {
            return "false";
        } else if (j < 1 || j > periodos.size()) {
            std::cout << "\nEl dato ingresado no corresponde a ningun periodo, por favor verifique.\n";
            system("pause");
        }
    } while (j < 1 || j > periodos.size());
    j--;
    system("cls");
    if (omitir == true) {
    	return periodos[j];
	}
    std::cout << "Periodo " << periodos[j] << "\n\n"
    << '+' << std::setw(150) << std::setfill('-') << std::right << '+' << std::setfill(' ') << '\n'
    << std::setw(8) << std::left << "|  Codigo "
    << std::setw(20) << std::left << " | Fecha de Registro"
    << std::setw(23) << std::left << " | Fecha de Transaccion"
    << std::setw(38) << std::left << " |              Categoria    "
    << std::setw(18) << std::left << " |      Monto     " << " |           Descripcion                 |\n"
    << '|' << std::setfill('-') << std::setw(11) << std::right << '+' << std::setw(20) << '+' << std::setw(23) << '+' << std::setw(38) << '+' << std::setw(18) << '+' << std::setw(40) << '|' << std::setfill(' ') << '\n';

    for (i = 0; i < Operaciones.size(); i++) {
        periodo_op = Operaciones[i].fechaTransaccion.substr(3, 7);
        if (periodo_op.compare(periodos[j]) == 0) {
            std::cout << "|  " << std::setw(8) << std::left << Operaciones[i].codigo
        << "| " << std::setw(17) << std::left << Operaciones[i].fechaRegistro
        << " | " << std::setw(20) << std::left << Operaciones[i].fechaTransaccion << " | ";
        if (Operaciones[i].tipo == 'I') {
            std::cout << ANSI_COLOR_GREEN;
        } else {
            std::cout << ANSI_COLOR_RED;
        }
        std::cout << std::setw(35) << std::left << Operaciones[i].categoria 
        << ANSI_COLOR_RESET << " |  ";
        if (Operaciones[i].tipo == 'I') {
            std::cout << ANSI_COLOR_GREEN;
        } else {
            std::cout << ANSI_COLOR_RED;
        }
        std::cout << std::setw(13) << std::left << std::setprecision(2) << std::fixed << std::right << Operaciones[i].monto
        << ANSI_COLOR_RESET << "  | "  << std::setw(37) << std::left << Operaciones[i].descripcion << " |\n";
        }
    }
    std::cout << '+' << std::setw(150) << std::setfill('-') << std::right << '+' << std::left << std::setfill(' ') << '\n';
    return periodos[j];
}

// Funcion que presenta el menu para crear / modificar una operacion
void mensajeOperacion (Operacion nuevaop) {
    std::cout << "                                         (1)                             (2)                        (3)                       (4)\n"
    << std::setw(8) << std::left << "|  Codigo "
    << std::setw(20) << std::left << " | Fecha de Registro"
    << std::setw(23) << std::left << " | Fecha de Transaccion"
    << std::setw(38) << std::left << " |              Categoria    "
    << std::setw(18) << std::left << " |      Monto     " << " |           Descripcion\n"
    << "|  " << std::setw(8) << std::left << nuevaop.codigo
    << "| " << std::setw(17) << std::left << nuevaop.fechaRegistro
    << " | " << std::setw(20) << std::left << nuevaop.fechaTransaccion
    << " | " << std::setw(35) << std::left << nuevaop.categoria
    << " |  " << std::setw(13) << std::right << std::setprecision(2) << std::fixed << nuevaop.monto << "  | " << std::left << nuevaop.descripcion << "\n\n"
    << "Seleccione el campo que desea ingresar (5 para completar el registro, o -1 para volver al menu principal): ";
}

// Funcion para registrar operaciones y actualizar el CSV
void registrarOperacion (std::string perfil) {
    std::fstream archivo;
    int i, op, op2;
    std::string linea, fechaActual = obtenerFechaActual();
    Operacion nuevaop;
    
    if (Operaciones.size() > 0) {
        Operaciones.clear();
        Tipos.clear();
    }
    Operaciones = cargarOperaciones(perfil);
    archivo.open(perfil + ".csv", std::ios::in);
    std::getline(archivo, ultimoCodigo);
    archivo.close();
    nuevaop = {ultimoCodigo, fechaActual, "xx-xx-xxxx", "xxxxxxxxxx", 'x', 0, "xxxxxxxxxxxxxxxxxxxxxxxxxx"};
    if (Categorias.size() > 0) { 
        Categorias.clear(); 
    }
    cargarCategorias(perfil, Categorias, Tipos);
    archivo.close();
    while (true) {
        system("cls");
        std::cout << std::setw(0) << std::setfill(' ');
        std::cout << "(1) Ingresar una operacion\n\n";
        mensajeOperacion(nuevaop);
        
        validar(&op, 0);
        switch (op) {
            case 1:
                nuevaop.fechaTransaccion = obtenerFechaTransaccion();
                break;
            case 2:
                obtenerCategoria(perfil, &nuevaop);
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
                system("pause");
                break;
        }
    }
}

// funcion que completa con 0s el codigo ingresado
std::string completarCodigo (std::string codigo) {
    while (codigo.length() < 7) {
        codigo = "0" + codigo;
    }
    return codigo;
}

// funcion que verifica que el codigo ingresado solo contiene numeros
bool sonDigitos (std::string str) {
    for (char c : str) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

/*  funcion para modificar o eliminar operaciones, el modo indica la accion a realizar,
    1 - modificar operacion, 2 - eliminar operacion */
void modificarOperacion (std::string perfil, int modo) {
    int i, j, op;
    char op2;
    bool valido, pase;
    std::string codigo, periodo, titulos[] = {"(2) Modificar una operacion\n\n", "(3) Eliminar una operacion\n\n"};

    do {
        pase = false;
        if (Operaciones.size() > 0) {
            Operaciones.clear();
        }
        Operaciones = cargarOperaciones(perfil);
        if (Operaciones.size() == 0) {
            std::cout << "\nAun no se ha registrado ninguna operacion, volviendo al menu principal.\n";
            system("pause");
            return;
        }
        system("cls");
        std::cout << titulos[modo - 1];
        periodo = mostrarOperaciones(false);
        if (periodo == "false") {
            break;
        }
        switch (modo) {
            case 1:
                std::cout << "\nIngrese el codigo de la operacion que desea modificar (o -1 para volver al menu principal): ";
                std::getline(std::cin >> std::ws, codigo);
                if (codigo == "-1") {
                    return;
                }
                if (codigo.length() < 7) {
                    codigo = completarCodigo(codigo);
                } else {
                    std::cout << "\nEl codigo ingresado supera la longitud maxima permitida (7 caracteres).\n";
                    system("pause");
                    break;
                }
                valido = sonDigitos(codigo);
                if (valido == true) {
                    for (i = 0; i < Operaciones.size(); i++) {
                        if (codigo == Operaciones[i].codigo) {
                            break;
                        }
                    }
                } else {
                    std::cout << "\nEl codigo ingresado no debe contener letras, decimales ni otros caracteres especiales.\n";
                    system("pause");
                    break;
                }
                if (i == Operaciones.size()) {
                    std::cout << "\nEl codigo ingresado no coincide con el de ninguna de las operaciones, por favor verifique.\n";
                    break;
                }
                while (pase == false) {
                    pase = false;
                    system("cls");
                    std::cout << std::setw(0) << std::setfill(' ');
                    std::cout << titulos[modo - 1];
                    mensajeOperacion(Operaciones[i]);
        
                    validar(&op, 0);
                    switch (op) {
                        case 1:
                            Operaciones[i].fechaTransaccion = obtenerFechaTransaccion();
                            break;
                        case 2:
                            obtenerCategoria(perfil, &Operaciones[i]);
                            break;
                        case 3:
                            Operaciones[i].monto = obtenerMonto();
                            break;
                        case 4:
                            Operaciones[i].descripcion = obtenerDescripcion();
                            break;
                        case 5:
                            ultimoCodigo = Operaciones[Operaciones.size() - 1].codigo;
                            ultimoCodigo = generarCodigo();
                            guardarOperacion(perfil);
                            std::cout << "\nOperacion modificada con exito.\n\nIngrese Y para modificar otra operacion, o N para volver al menu principal: ";
                            std::cin >> op2;
                            while (true) {
                                if (op2 != 'y' && op2 != 'Y' && op2 != 'N' && op2 != 'n') {
                                    std::cout << "\nPor favor ingrese un caracter segun lo indicado: ";
                                    std::cin >> op2;
                                } else if (op2 == 'y' || op2 == 'Y') {
                                    pase = true;
                                    break;
                                } else {
                                    return;
                                }
                            }
                            break;
                        case -1:
                            std::cout << "\nVolviendo al menu principal.\n";
                            system("pause");
                            return;
                        default:
                            std::cout << "\nPor favor, ingrese una opcion valida (1-5, -1).\n";
                            system("pause");
                            break;
                    }
                }
                break;
            case 2:
                std::cout << "\nIngrese el codigo de la operacion que desea eliminar (o -1 para volver al menu principal): ";
                std::getline(std::cin >> std::ws, codigo);
                if (codigo == "-1") {
                    return;
                }
                if (codigo.length() < 7) {
                    codigo = completarCodigo(codigo);
                } else {
                    std::cout << "\nEl codigo ingresado supera la longitud maxima permitida (7 caracteres).\n";
                    system("pause");
                    break;
                }
                valido = sonDigitos(codigo);
                if (valido == true) {
                    for (i = 0; i < Operaciones.size(); i++) {
                        if (codigo == Operaciones[i].codigo) {
                            break;
                        }
                    }
                } else {
                    std::cout << "\nEl codigo ingresado no debe contener letras, decimales ni otros caracteres especiales.\n";
                    system("pause");
                    break;
                }
                if (i == Operaciones.size()) {
                    std::cout << "\nEl codigo ingresado no coincide con el de ninguna de las operaciones, por favor verifique.\n";
                    system("pause");
                    break;
                }
                while (pase == false) {
                    std::cout << "\nIngrese Y para confirmar la eliminacion o N para cancelar: ";
                    std::cin >> op2;
                    if (op2 != 'y' && op2 != 'Y' && op2 != 'N' && op2 != 'n') {
                        std::cout << "\nPor favor ingrese un caracter segun lo indicado: ";
                        std::cin >> op2;
                    } else if (op2 == 'y' || op2 == 'Y') {
                        pase = true;
                        if (Operaciones.size() == 1) {
                            Operaciones.pop_back();
                            ultimoCodigo = "0000000";
                            guardarOperacion(perfil);
                        } else {
                            for (j = i; j < (Operaciones.size() - 1); j++) {
                                codigo = Operaciones[j].codigo;
                                Operaciones[j] = Operaciones[j + 1];
                                Operaciones[j].codigo = codigo;
                            }
                            Operaciones.pop_back();
                            std::cout << "\nEl nuevo tamaño del vector Operaciones es: " << Operaciones.size() << '\n';
                            ultimoCodigo = Operaciones[Operaciones.size() - 1].codigo;
                            ultimoCodigo = generarCodigo();
                            guardarOperacion(perfil);
                        }

                        std::cout << "\nOperacion eliminada con exito.\n\nIngrese Y para modificar otra operacion, o N para volver al menu principal: ";
                        std::cin >> op2;
                        while (true) {
                            if (op2 != 'y' && op2 != 'Y' && op2 != 'N' && op2 != 'n') {
                                std::cout << "\nPor favor ingrese un caracter segun lo indicado: ";
                                std::cin >> op2;
                            } else if (op2 == 'y' || op2 == 'Y') {
                                pase = true;
                                break;
                            } else {
                                return;
                            }
                        }
                    } else {
                        break;
                    }
                }
                break;
        }
    } while (true);
}
