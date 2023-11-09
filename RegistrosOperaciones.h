#ifndef REGISTROSOPERACIONES_H
#define REGISTROSOPERACIONES_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <string>

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
        if (fecha.length() == 10) {
            return fecha;
        } else {
            std::cout << "Formato de fecha incorrecto. Debe ser AAAA-MM-DD." << std::endl;
        }
    }
}

Categoria obtenerCategoria() {
    int opcion;
    do {
        std::cout << "Seleccione la categoria de la operación:\n";
        std::cout << "1. Sueldo\n2. Ingresos Varios\n3. Productos Basicos\n4. Vestido y Calzado\n5. Vivienda y Servicios Basicos\n";
        std::cout << "6. Salud\n7. Transporte\n8. Educación\n9. Ocio\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;
    } while (opcion < 1 || opcion > 9);

    return static_cast<Categoria>(opcion - 1);
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
    std::cout << "Codigo | Fecha de Registro | Fecha de Transacción | Categoría | Monto | Descripcion\n";
    for (const Operacion& operacion : operaciones) {
        std::cout << operacion.codigo << " | " << operacion.fechaRegistro << " | " << operacion.fechaTransaccion << " | "
                  << operacion.categoria << " | " << operacion.monto << " | " << operacion.descripcion << "\n";
    }
}

#endif
