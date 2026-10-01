/**
 * @file miniMotor.hpp
 * @brief Mini motor de búsqueda
 * @brief Tecnun - 2026. Curso de Estructura de Datos y Algoritmia
 * @authors: Ana Carolina Casares and Ainhoa Gibaja
 *
 * Este módulo implementa:
 * - Parte 1: índice invertido con unordered_map y vector
 * - Parte 2: control de duplicados con unordered_set
 * - Parte 3
 * - Parte 4
 */
#ifndef MINIMOTOR_HPP
#define MINIMOTOR_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <stack>
#include <queue>
#include <chrono>

namespace miniMotor {

    // Struct para recoger los tiempos del experimento
    struct ResultadoExperimento {
        long tiempoInsercion; // microsegundos
        long tiempoBusqueda;  // microsegundos
        int tamanoIndice;
        int numIdsTotal;
    };

    // Parte 1: lee corpus.txt, filtra palabras >4 letras, genera indice.txt
    // Si se pasa resultado != nullptr, mide tiempos y no escribe fichero (modo experimento)
    int parte1(const std::string& archivoCorpus = "corpus.txt", ResultadoExperimento* resultado = nullptr);

    // Parte 2: lee corpus_duplicado.txt, evita duplicados, genera indice_deduplicado.txt
    int parte2(const std::string& archivoCorpus = "corpus_con_duplicados.txt", ResultadoExperimento* resultado = nullptr);

    // Parte 3: lee indice.txt y consultas.txt, evalua con Shunting Yard, genera resultados_consultas.txt
    int parte3();

    // Parte 4: lee indice.txt y peticiones.txt, procesa cola FIFO, genera log_procesamiento.txt
    int parte4();

}  // namespace miniMotor

#endif  // MINIMOTOR_HPP