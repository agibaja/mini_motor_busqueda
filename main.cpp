/**
 * @file main.cpp
 * @brief Programa principal: mini motor de busqueda
 * @brief Tecnun - 2026. Curso de Estructura de Datos y Algoritmia
 * @authors: Ana Carolina Casares y Ainhoa Gibaja
 */
#include <chrono>
#include <iomanip>
#include "miniMotor.hpp"

void menu() {
    std::cout << "\n==========================================\n";
    std::cout << "        MINI MOTOR DE BUSQUEDA\n";
    std::cout << "==========================================\n";
    std::cout << "1. Crear indice.txt a partir de corpus.txt\n";
    std::cout << "2. Crear indice_deduplicado.txt a partir de corpus_duplicado.txt\n";
    std::cout << "3. Procesar consultas de consultas.txt segun el indice\n";
    std::cout << "4. Procesar una serie de peticiones de peticiones.txt\n";
    std::cout << "5. Experimento 1: tabla Hash con distintos tamaños\n";
    std::cout << "6. Experimento 2: tabla Hash con y sin control de duplicados\n";
    std::cout << "0. Salir\n";
    std::cout << "==========================================\n";
    std::cout << "Opcion: ";
}

int opcion = 0;
bool continuar = true;
bool indiceCreado = false;

int main() {
    while (continuar) {
        menu();
        std::cin >> opcion;
        std::cin.ignore();
        switch (opcion) {
        case 1: {
            auto inicio1 = std::chrono::high_resolution_clock::now();
            miniMotor::parte1();
            auto fin1 = std::chrono::high_resolution_clock::now();
            auto duracion1 = std::chrono::duration_cast<std::chrono::microseconds>(fin1 - inicio1);
            std::cout << "Parte 1 (vector + find): " << duracion1.count() << " microsegundos\n";
            indiceCreado = true;
            break;
        }
        case 2: {
            auto inicio2 = std::chrono::high_resolution_clock::now();
            miniMotor::parte2();
            auto fin2 = std::chrono::high_resolution_clock::now();
            auto duracion2 = std::chrono::duration_cast<std::chrono::microseconds>(fin2 - inicio2);
            std::cout << "Parte 2 (unordered_set): " << duracion2.count() << " microsegundos\n";
            break;
        }
        case 3: {
            if (!indiceCreado) {
                std::cout << "Error: primero debes crear el indice (opcion 1).\n";
                break;
            }
            auto inicio3 = std::chrono::high_resolution_clock::now();
            miniMotor::parte3();
            auto fin3 = std::chrono::high_resolution_clock::now();
            auto duracion3 = std::chrono::duration_cast<std::chrono::microseconds>(fin3 - inicio3);
            std::cout << "Parte 3 (consultas): " << duracion3.count() << " microsegundos\n";
            break;
        }
        case 4: {
            if (!indiceCreado) {
                std::cout << "Error: primero debes crear el indice (opcion 1).\n";
                break;
            }
            auto inicio4 = std::chrono::high_resolution_clock::now();
            miniMotor::parte4();
            auto fin4 = std::chrono::high_resolution_clock::now();
            auto duracion4 = std::chrono::duration_cast<std::chrono::microseconds>(fin4 - inicio4);
            std::cout << "Parte 4 (cola de consultas): " << duracion4.count() << " microsegundos\n";
            break;
        }
        case 5: {
            std::vector<std::pair<std::string, std::string>> experimentos = {
                {"1.000 palabras",   "corpus_1k.txt"},
                {"10.000 palabras",  "corpus_10k.txt"},
                {"100.000 palabras", "corpus_100k.txt"}
            };

            std::cout << "\n--- Experimento 1: tabla hash con distintos corpus ---\n";
            std::cout << std::left
                << std::setw(20) << "Tamano"
                << std::setw(25) << "Insercion (us)"
                << std::setw(25) << "Busqueda (us)" << "\n";
            std::cout << std::string(70, '-') << "\n";

            for (auto& [etiqueta, archivo] : experimentos) {
                miniMotor::ResultadoExperimento res;
                miniMotor::parte1(archivo, &res);
                std::cout << std::left
                    << std::setw(20) << etiqueta
                    << std::setw(25) << res.tiempoInsercion
                    << std::setw(25) << res.tiempoBusqueda << "\n";
            }
            break;
        }
        case 6: {
            std::string archivo = "corpus_con_duplicados_ex2.txt";

            miniMotor::ResultadoExperimento sinControl, conControl;
            miniMotor::parte1(archivo, &sinControl);
            miniMotor::parte2(archivo, &conControl);

            std::cout << "\n--- Experimento 2: con y sin control de duplicados ---\n";
            std::cout << std::left
                << std::setw(30) << ""
                << std::setw(25) << "Sin control (parte1)"
                << std::setw(25) << "Con control (parte2)" << "\n";
            std::cout << std::string(80, '-') << "\n";
            std::cout << std::left
                << std::setw(30) << "Tiempo insercion (us)"
                << std::setw(25) << sinControl.tiempoInsercion
                << std::setw(25) << conControl.tiempoInsercion << "\n";
            std::cout << std::left
                << std::setw(30) << "Entradas en indice"
                << std::setw(25) << sinControl.tamanoIndice
                << std::setw(25) << conControl.tamanoIndice << "\n";
            std::cout << std::left
                << std::setw(30) << "IDs totales en indice"
                << std::setw(25) << sinControl.numIdsTotal
                << std::setw(25) << conControl.numIdsTotal << "\n";
            break;
        }
        case 0: {
            std::cout << "Puede encontrar los archivos en la carpeta del ejecutable.\n";
            std::cout << "Muchas gracias y hasta pronto!\n";
            continuar = false;
            break;
        }
        }
    }
    return 0;
}