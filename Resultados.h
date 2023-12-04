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

// funcion que calcula el balance por periodo o por categoria
std::vector<double> calcularBalance (std::string perfil, int modo, bool omitir) {
    int i, j, op;
    double balanceTotal = 0;
    std::vector<double> balance;
    std::string linea, periodo;
    
    if (Categorias.size() > 0) {
        Categorias.clear();
    }
    cargarCategorias(perfil, Categorias, Tipos);
    i = Categorias.size();
    std::vector<double> totales(Categorias.size());
    std::fill(totales.begin(), totales.end(), 0);


    if (Operaciones.size() > 0) {
        Operaciones.clear();
    }
    Operaciones = cargarOperaciones(perfil);
    if (Operaciones.size() == 0) {
        std::cout << "\nAun no se ha registrado ninguna operacion, volviendo al menu principal.\n";
        system("pause");
        return balance;
    }

    if (modo == 1) {
    	periodo = mostrarOperaciones(true);
        system("cls");
        if (periodo == "false") {
            return balance;
        }
        // llenado del vector balance (por categorias)
    	for (i = 0; i < Categorias.size(); i++) {
        	for (j = 0; j < Operaciones.size(); j++) {
            	if (Operaciones[j].fechaTransaccion.substr(3, 7) == periodo && Operaciones[j].categoria == Categorias[i]) {
                	totales[i] += Operaciones[j].monto;
            	}
        	}
        	balance.push_back(totales[i]);
    	}
    	if (omitir == true) {
            return balance;
        }
        std::cout << "Balance del periodo " << periodo << "\n\n"
        << '+' << std::setw(150) << std::setfill('-') << std::right << '+' << std::setfill(' ') << '\n'
        << std::setw(8) << std::left << "|  Codigo "
        << std::setw(20) << std::left << " | Fecha de Registro"
        << std::setw(23) << std::left << " | Fecha de Transaccion"
        << std::setw(38) << std::left << " |              Categoria    "
        << std::setw(18) << std::left << " |      Monto     " << " |           Descripcion                 |\n"
		<< '|' << std::setfill('-') << std::setw(11) << std::right << '+' << std::setw(20) << '+' << std::setw(23) << '+' << std::setw(38) << '+' << std::setw(18) << '+' << std::setw(40) << '|' << std::setfill(' ') << '\n';

        for (i = 0; i < Operaciones.size(); i++) {
            if (Operaciones[i].fechaTransaccion.substr(3, 7) == periodo) {
                std::cout << "|  " << std::setw(8) << std::left << Operaciones[i].codigo
                << "| " << std::setw(17) << std::left << Operaciones[i].fechaRegistro
                << " | " << std::setw(20) << std::left << Operaciones[i].fechaTransaccion
                << " | ";
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
                std::cout << std::setw(13) << std::setprecision(2) << std::fixed << std::right << Operaciones[i].monto 
                << ANSI_COLOR_RESET << "  | " << std::setw(37) << std::left << Operaciones[i].descripcion << " |\n";
                for (j = 0; j < Categorias.size(); j++) {
                    if (Operaciones[i].categoria == Categorias[j]) {
                        if (Tipos[j] == 'I') {
                            balanceTotal += Operaciones[i].monto;
                        } else {
                            balanceTotal -= Operaciones[i].monto;
                        }
                        break;
                    }
                }
            }
        }
        std::cout << '|' << std::setfill('-') << std::setw(11) << std::right << '+' << std::setw(81) << '+' << std::setw(18) << '+' << std::setw(40) << '+' << std::setfill(' ') << '\n'
        << std::left << "|  Balance |" << std::setw(83) << std::setfill(' ') << std::right << "|  ";
        if (balanceTotal < 0) {
            std::cout << ANSI_COLOR_RED;
        } else {
            std::cout << ANSI_COLOR_GREEN;
        }
        std::cout << std::setw(13) << std::setprecision(2) << std::fixed << std::right << balanceTotal 
        << ANSI_COLOR_RESET << "  |\n" << '+' << std::setw(111) << std::setfill('-') << std::right << "+\n" << std::left << std::setfill(' ');
        return balance;
        
	} else if (modo == 2) {
		periodo = mostrarOperaciones(true);
        system("cls");
        if (periodo == "false") {
            return balance;
        }
        // llenado del vector balance (por categorias)
    	for (i = 0; i < Categorias.size(); i++) {
        	for (j = 0; j < Operaciones.size(); j++) {
            	if (Operaciones[j].fechaTransaccion.substr(3, 7) == periodo && Operaciones[j].categoria == Categorias[i]) {
                	totales[i] += Operaciones[j].monto;
            	}
        	}
        	balance.push_back(totales[i]);
    	}
        if (omitir == true) {
            return balance;
        }
        std::cout << "Balance por categorias del periodo " << periodo << "\n\n"
        << '+' << std::setw(56) << std::setfill('-') << std::right << '+' << std::setfill(' ') << std::left << '\n'
        << std::setw(37) << std::left << "|              Categoria    "
        << std::setw(18) << std::left << " |      Total      |\n"
        << '|' << std::setw(38) << std::setfill('-') << std::right << '+' << std::setw(18) << '|' << std::setfill(' ') << std::left << '\n';
        for (i = 0; i < Categorias.size(); i++) {
            if (Tipos[i] == 'I') {
                balanceTotal += totales[i];
            } else {
                balanceTotal -= totales[i];
            }
            std::cout << "| ";
            if (Tipos[i] == 'I') {
                std::cout << ANSI_COLOR_GREEN;
            } else {
                std::cout << ANSI_COLOR_RED;
            }
            std::cout << std::setw(35) << std::left << Categorias[i] << ANSI_COLOR_RESET << " |  ";
            if (Tipos[i] == 'I') {
                std::cout << ANSI_COLOR_GREEN;
            } else {
                std::cout << ANSI_COLOR_RED;
            }
            std::cout << std::setw(13) << std::left << std::setprecision(2) << std::fixed << std::right << totales[i] 
            << ANSI_COLOR_RESET << "  | \n";
        }
        std::cout << '|' << std::setw(38) << std::setfill('-') << std::right << '+' << std::setw(18) << '|' << std::setfill(' ') << std::left << '\n'
        << std::setw(39) << std::left << "|               Balance               |";
        if (balanceTotal < 0) {
            std::cout << ANSI_COLOR_RED;
        } else {
            std::cout << ANSI_COLOR_GREEN;
        }
        std::cout << std::setw(15) << std::right << balanceTotal << ANSI_COLOR_RESET << "  |\n"
        << '+' << std::setw(56) << std::setfill('-') << '+' << std::setfill(' ') << std::left << '\n';
        return balance;
	}
}

void graficoBarras (std::vector<double> &balance) {
    int i, j, largo;
    double max = 0;
    for (double valor : balance) {
        if (valor > max) {
            max = valor;
        }
    }

    double escala = 50.0 / max;

    std::cout << "Representacion grafica\n\n";
    for (i = 0; i < balance.size(); i++) {
        largo = int(balance[i] * escala);
        if (Tipos[i] == 'I') {
            std::cout << ANSI_COLOR_GREEN;
        } else {
            std::cout << ANSI_COLOR_RED;
        }
        std::cout << std::setw(25) << std::left << Categorias[i] << ANSI_COLOR_RESET << " | ";
        if (Tipos[i] == 'I') {
            std::cout << ANSI_COLOR_GREEN;
        } else {
            std::cout << ANSI_COLOR_RED;
        }
        for (j = 0; j < largo; j++) {
            std::cout << '=';
        }
        std::cout << ANSI_COLOR_RESET << '\n' << std::setw(27) << std::setfill(' ') << std::right << '|' << std::left << '\n';
    }
    i = int(max * escala);
    std::cout << std::setw(27) << std::setfill(' ') << std::right << '|'
    << std::setw(i + 4) << std::setfill('-') << '\n' << std::setfill(' ')
    << std::setw(28) << '0' << std::setw(i + 10) << max;
}

void compararPeriodos (std::string perfil) {

    if (Categorias.size() > 0) {
        Categorias.clear();
    }
    cargarCategorias(perfil, Categorias, Tipos);
    if (Operaciones.size() > 0) {
        Operaciones.clear();
    }
    Operaciones = cargarOperaciones(perfil);
    if (Operaciones.size() == 0) {
        std::cout << "\nAun no se ha registrado ninguna operacion, volviendo al menu principal.\n";
        return;
    }

    int i, j, k, ancho;
    bool coincidencia, pase;
    std::vector<std::string> periodos;
    std::vector<int> indices;
    double max = 0;
    
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
    
    if (periodos.size() < 2) {
        std::cout << "\nSe requieren al menos dos periodos para poder comparar, volviendo al menu principal.\n";
        return;
    }

    do {
        pase = false;
        system("cls");
        std::cout << "\nPeriodos disponibles:\n";
        for (i = 0; i < periodos.size(); i++) {
            std::cout << i + 1 << " - " << periodos[i] << '\n';
        }
        std::cout << "\nIngrese los numeros de los periodos que desea comparar (0 para confirmar, o -1 para volver al menu principal):\n";
        while (pase == false) {
            validar(&j, 0);
            if (j == -1) {
                return;
            } else if (j > 0 && j <= periodos.size()) {
                if (indices.size() == 0) {
                    indices.push_back(j - 1);
                } else {
                    coincidencia = false;
                    for (i = 0; i < indices.size(); i++) {
                        if (j == indices[i]) {
                            coincidencia = true;
                        }
                    }
                    if (coincidencia == true) {
                        std::cout << "\nEl numero ingresado ya fue ingresado anteriormente, por favor, ingrese un numero diferente: ";
                    } else {
                        indices.push_back(j - 1);
                    }
                }
            } else if (j == 0 && indices.size() > 1) {
                pase = true;
            } else if (j == 0 && indices.size() < 2) {
                std::cout << "\nDebe ingresar al menos dos periodos para realizar la comparacion.\n";
            } else {
                std::cout << "\nEl numero ingresado no corresponde a ningun periodo, por favor verifique.\n";
            }
        }
    } while (pase == false);
    std::sort(indices.begin(), indices.end());
    double balances[Categorias.size()][indices.size()];
    double balancesTotales[indices.size()];

    for (i = 0; i < Categorias.size(); i++) {
        for (j = 0; j < indices.size(); j++) {
            balances[i][j] = 0;
        }
    }

    system("cls");
    std::cout << "Periodos a comparar:\n";
    for (i = 0; i < indices.size(); i++) {
        balancesTotales[i] = 0;
        std::cout << i + 1 << " - " << periodos[indices[i]] << '\n';
        for (j = 0; j < Operaciones.size(); j++) {
            if (Operaciones[j].fechaTransaccion.substr(3, 7) == periodos[indices[i]]) {
                if (Operaciones[j].tipo == 'I') {
                    balancesTotales[i] += Operaciones[j].monto;
                } else {
                    balancesTotales[i] -= Operaciones[j].monto;
                }
                for (k = 0; k < Categorias.size(); k++) {
                    if (Operaciones[j].categoria == Categorias[k]) {
                        balances[k][i] += Operaciones[j].monto;
                    }
                }
            }
        }
    }
    for (i = 0; i < Categorias.size(); i++) {
        for (j = 0; j < indices.size(); j++) {
            if (balances[i][j] > max) {
                max = balances[i][j];
            }
        }
    }
    
    ancho = 29 + (16 * indices.size());
    std::cout << "\n+" << std::setw(ancho) << std::setfill('-') << std::right << "+\n" << std::setfill(' ')
    << std::setw(28) << std::left << "|         Categoria";
    for (i = 0; i < indices.size(); i++) {
        std::cout << "|    " << std::setw(11) << periodos[indices[i]];
    }
    std::cout << "|\n";
    std::cout << "|" << std::setw(28) << std::setfill('-');
    for (i = 0; i < indices.size(); i++) {
        std::cout << std::right << '+' << std::setw(16);
    }
    std::cout << '|' << '\n';
    std::cout << std::left << std::setfill(' ');
    for (i = 0; i < Categorias.size(); i++) {
        std::cout << "| ";
        if (Tipos[i] == 'I') {
            std::cout << ANSI_COLOR_GREEN;
        } else {
            std::cout << ANSI_COLOR_RED;
        }
        std::cout << std::setw(25) << std::left << Categorias[i] << ANSI_COLOR_RESET << " |";
        for (j = 0; j < indices.size(); j++) {
            if (Tipos[i] == 'I') {
                std::cout << ANSI_COLOR_GREEN;
            } else {
                std::cout << ANSI_COLOR_RED;
            }
            std::cout << std::setw(13) << std::right << std::fixed << std::setprecision(2) << balances[i][j] 
            << ANSI_COLOR_RESET << std::left << "  |";
        }
        std::cout << '\n';
    }
    std::cout << "|" << std::setw(28) << std::setfill('-');
    for (i = 0; i < indices.size(); i++) {
        std::cout << std::right << '+' << std::setw(16);
    }
    std::cout << '|' << std::left << std::setfill(' ') << '\n'
    << std::setw(28) << "|          Balance";
    for (i = 0; i < indices.size(); i++) {
        std::cout << "|    " << std::setw(11) << balancesTotales[i];
    }
    std::cout << "|\n+" << std::setw(ancho) << std::setfill('-') << std::right << "+\n" << std::setfill(' ');

    double escala = 50.0 / max;

    std::cout << "\n\nRepresentacion grafica\n\n";
    for (i = 0; i < Categorias.size(); i++) {
        if (Tipos[i] == 'I') {
            std::cout << ANSI_COLOR_GREEN;
        } else {
            std::cout << ANSI_COLOR_RED;
        }
        std::cout << std::setw(25) << std::left << Categorias[i] << ANSI_COLOR_RESET << " |\n";
        for (j = 0; j < indices.size(); j++) {
            ancho = int(balances[i][j] * escala);
            std::cout << std::setw(25) << std::left << periodos[indices[j]] << ANSI_COLOR_RESET << " | ";
            if (Tipos[i] == 'I') {
                std::cout << ANSI_COLOR_GREEN;
            } else {
                std::cout << ANSI_COLOR_RED;
            }
            for (k = 0; k < ancho; k++) {
                std::cout << '=';
            }
            std::cout << ANSI_COLOR_RESET << '\n';
        }
        std::cout << std::setw(27) << std::setfill(' ') << std::right << '|' << std::left << '\n';
    }
    i = int(max * escala);
    std::cout << std::setw(27) << std::setfill(' ') << std::right << '|'
    << std::setw(i + 4) << std::setfill('-') << '\n' << std::setfill(' ')
    << std::setw(28) << '0' << std::setw(i + 10) << max;
}
