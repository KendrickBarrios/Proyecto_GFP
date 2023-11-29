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
std::vector<double> calcularBalance (std::string perfil, int modo) {
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
    	periodo = mostrarOperaciones();
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
                << " | " << std::setw(35) << std::left << Operaciones[i].categoria
                << " |  " << std::setw(13) << std::left << std::setprecision(2) << std::fixed << std::right << Operaciones[i].monto << "  | " << std::setw(37) << std::left << Operaciones[i].descripcion << " |\n";
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
        << std::left << "|  Balance |" << std::setw(83) << std::setfill(' ') << std::right << "|  " << std::setw(13) << std::setprecision(2) << std::fixed << std::right << balanceTotal << "  |\n"
		<< '+' << std::setw(111) << std::setfill('-') << std::right << "+\n" << std::left << std::setfill(' ');
        return balance;
        
	} else if (modo == 2) {
		periodo = mostrarOperaciones();
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
            std::cout << "| " << std::setw(35) << std::left << Categorias[i]
            << " |  " << std::setw(13) << std::left << std::setprecision(2) << std::fixed << std::right << totales[i] << "  | \n";
        }
        std::cout << '|' << std::setw(38) << std::setfill('-') << std::right << '+' << std::setw(18) << '|' << std::setfill(' ') << std::left << '\n'
        << std::setw(39) << std::left << "|               Balance               |"
        << std::setw(15) << std::right << balanceTotal << "  |\n"
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

    std::cout << "Grafico\n\n";
    for (i = 0; i < balance.size(); i++) {
        largo = int(balance[i] * escala);
        std::cout << std::setw(25) << std::left << Categorias[i] << " | ";
        for (j = 0; j < largo; j++) {
            std::cout << '=';
        }
        std::cout << '\n' << std::setw(27) << std::setfill(' ') << std::right << '|' << std::left << '\n';
    }
    i = int(max * escala);
    std::cout << std::setw(27) << std::setfill(' ') << std::right << '|'
    << std::setw(i + 4) << std::setfill('-') << '\n' << std::setfill(' ')
    << std::setw(28) << '0' << std::setw(i + 10) << max;
}
