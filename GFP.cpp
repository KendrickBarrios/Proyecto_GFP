#include <iostream>
#include <fstream>
#include <locale.h>
#include <cstring>
#include <string>
#include <vector>
#include "gestionperfil.h"

int main () {
    setlocale(LC_ALL, "spanish");
    crearperfil();
    modificarperfil();
    return 0;
}