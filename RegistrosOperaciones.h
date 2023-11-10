#ifndef REGISTROSOPERACIONES_H
#define REGISTROSOPERACIONES_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

// Enumeracion para las categorias de operaciones
enum Categoria {
    Sueldo,
    IngresosVarios,
    ProductosBasicos,
    VestidoCalzado,
    ViviendaServiciosBasicos,
    Salud,
    Transporte,
    Educacion,
    Ocio
};

// Struct para representar una operación
struct Operacion {
    std::string codigo;
    std::string fechaRegistro;
    std::string fechaTransaccion;
    Categoria categoria;
    double monto;
    std::string descripcion;
};

// Funciones para manipular los campos del struct

std::string generarCodigo(const std::string& ultimoCodigo) {
    int codigo = std::stoi(ultimoCodigo) + 1;
    std::string nuevoCodigo = std::to_string(codigo);
    while (nuevoCodigo.length() < 6) {
        nuevoCodigo = "0" + nuevoCodigo;
    }
    return nuevoCodigo;
}

std::string obtenerFechaActual() {
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d", timeinfo);
    return std::string(buffer);
}

std::string obtenerFechaTransaccion() {
    std::string fecha;
    while (true) {
        std::cout << "Ingrese la fecha de la transacción (AAAA-MM-DD): ";
        std::cin >> fecha;

        // Verificar la longitud de la fecha y asegurarse de que sea válida
        if (fecha.length() == 10) {
            int year, month, day;
            if (sscanf(fecha.c_str(), "%d-%d-%d", &year, &month, &day) == 3) {
                if (year >= 1000 && year <= 9999 && month >= 1 && month <= 12 && day >= 1 && day <= 31) {
                    break; // Fecha válida
                }
            }
        }

        std::cout << "Formato de fecha inválido. Por favor, ingrese una fecha válida en el formato AAAA-MM-DD.\n";
    }

    return fecha;
}

// Función para cargar las categorías desde un archivo
std::vector<std::string> cargarCategorias() {
    std::vector<std::string> categorias;
    std::fstream archivo;
    archivo.open("categorias.txt", std::ios::in);
    if (archivo.is_open()) {
        std::string categoria;
        while (std::getline(archivo, categoria)) {
            categorias.push_back(categoria);
        }
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo de categorías." << std::endl;
    }
    return categorias;
}

// Función para guardar las categorías en un archivo
void guardarCategorias(const std::vector<std::string>& categorias) {
    std::fstream archivo;
    archivo.open("categorias.txt", std::ios::out);
    if (archivo.is_open()) {
        for (const std::string& categoria : categorias) {
            archivo << categoria << "\n";
        }
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo de categorías para guardar." << std::endl;
    }
}

// Funciones relacionadas con las categorías
Categoria obtenerCategoria() {
    std::vector<std::string> categorias = cargarCategorias();

    int opcion;
    do {
        std::cout << "Seleccione la categoria de la operación:\n";
        for (size_t i = 0; i < categorias.size(); ++i) {
            std::cout << i + 1 << ". " << categorias[i] << "\n";
        }
        std::cout << "Opción: ";
        std::cin >> opcion;
    } while (opcion < 1 || opcion > static_cast<int>(categorias.size()));

    return static_cast<Categoria>(opcion - 1);
}

void agregarCategoria() {
    std::vector<std::string> categorias = cargarCategorias();

    std::string nuevaCategoria;
    std::cout << "Ingrese el nombre de la nueva categoría: ";
    std::cin >> nuevaCategoria;

    categorias.push_back(nuevaCategoria);

    guardarCategorias(categorias);
}

double obtenerMonto() {
    double monto;
    do {
        std::cout << "Ingrese el monto de la transacción: ";
        std::cin >> monto;
        if (monto < 0) {
            std::cout << "El monto debe ser un valor positivo." << std::endl;
        }
    } while (monto < 0);
    return monto;
}

std::string obtenerDescripcion() {
    std::string descripcion;
    std::cout << "Ingrese una descripción (máximo 40 caracteres): ";
    std::cin.ignore();
    std::getline(std::cin, descripcion);
    if (descripcion.length() > 40) {
        descripcion = descripcion.substr(0, 40);
    }
    return descripcion;
}

// Función para guardar una operación en un archivo CSV
void guardarOperacion(const Operacion& operacion, const std::string& perfil) {
    std::fstream archivo;
    archivo.open(perfil + ".csv", std::ios::out | std::ios::app);
    if (archivo.is_open()) {
        archivo << operacion.codigo << "," << operacion.fechaRegistro << "," << operacion.fechaTransaccion << ","
                << operacion.categoria << "," << operacion.monto << "," << operacion.descripcion << "\n";
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para guardar la operación." << std::endl;
    }
}

// Funcion para cargar las operaciones desde un archivo CSV
std::vector<Operacion> cargarOperaciones(const std::string& perfil) {
    std::vector<Operacion> operaciones;
    std::fstream archivo;
    archivo.open(perfil + ".csv", std::ios::in);
    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            std::istringstream iss(linea);
            std::string codigo, fechaRegistro, fechaTransaccion, categoriaStr, montoStr, descripcion;
            Categoria categoria;
            double monto;
            std::getline(iss, codigo, ',');
            std::getline(iss, fechaRegistro, ',');
            std::getline(iss, fechaTransaccion, ',');
            std::getline(iss, categoriaStr, ',');
            std::getline(iss, montoStr, ',');
            std::getline(iss, descripcion);
            categoria = static_cast<Categoria>(std::stoi(categoriaStr));
            monto = std::stod(montoStr);
            operaciones.push_back({codigo, fechaRegistro, fechaTransaccion, categoria, monto, descripcion});
        }
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para cargar las operaciones." << std::endl;
    }
    return operaciones;
}

// Funcion para mostrar las operaciones de un usuario
void mostrarOperaciones(const std::vector<Operacion>& operaciones) {
    std::cout << std::setw(8) << std::left << "Codigo |"
              << std::setw(20) << std::left << "Fecha de Registro |"
              << std::setw(22) << std::left << "Fecha de Transacción |"
              << std::setw(15) << std::left << "Categoría |"
              << std::setw(10) << std::left << "Monto |"
              << "Descripcion\n";

    for (const Operacion& operacion : operaciones) {
        std::cout << std::setw(8) << std::left << operacion.codigo << " | "
                  << std::setw(20) << std::left << operacion.fechaRegistro << " | "
                  << std::setw(22) << std::left << operacion.fechaTransaccion << " | "
                  << std::setw(15) << std::left << operacion.categoria << " | "
                  << std::setw(10) << std::left << operacion.monto << " | "
                  << operacion.descripcion << "\n";
    }
}

#endif
