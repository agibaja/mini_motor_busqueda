// @file miniMotor.cpp 
// @brief Implementacón: mini motor de busqueda
// @brief Tecnun - 2026. Curso de Estructura de Datos y Algoritmia
// @authors Ana Carolina Casares y Ainhoa Gibaja

#include "miniMotor.hpp"

namespace miniMotor {

    int parte1(const std::string& archivoCorpus, ResultadoExperimento* resultado) {
        //Recoger archivo
        std::ifstream archivo(archivoCorpus);
        //Responde a ¿en qué documentos aparece la palabra X?
        std::unordered_map<std::string, std::vector<int>> indice;
        //Crear indice mientras recorre el archivo
        std::string linea;
        int docActual = -1;

        // ── CRONÓMETRO INSERCIÓN ──
        auto inicioIns = std::chrono::high_resolution_clock::now();

        while (std::getline(archivo, linea)) {
            if (linea.substr(0, 4) == "DOC:") {
                docActual = std::stoi(linea.substr(4));
            }
            else if (!linea.empty() && docActual != -1) {
                std::istringstream ss(linea);
                std::string palabra;
                while (ss >> palabra) {
                    if (palabra.length() > 4) {
                        indice[palabra].push_back(docActual);
                    }
                }
            }
        }

        auto finIns = std::chrono::high_resolution_clock::now();

        // ── CRONÓMETRO BÚSQUEDA (muestra de 1000 palabras) ──
        std::vector<std::string> muestra;
        for (auto& [palabra, _] : indice) {
            muestra.push_back(palabra);
            if (muestra.size() >= 1000) break;
        }

        auto inicioBus = std::chrono::high_resolution_clock::now();
        volatile int encontradas = 0;
        for (const auto& p : muestra)
            if (indice.find(p) != indice.end()) encontradas++;
        auto finBus = std::chrono::high_resolution_clock::now();

        int total = 0;
        for (auto& [palabra, ids] : indice) total += (int)ids.size();
        // rellenar struct solo si se llama desde el experimento
        if (resultado) {
            resultado->tiempoInsercion = std::chrono::duration_cast<std::chrono::microseconds>(finIns - inicioIns).count();
            resultado->tiempoBusqueda = std::chrono::duration_cast<std::chrono::microseconds>(finBus - inicioBus).count();
            resultado->tamanoIndice = (int)indice.size();
            resultado->numIdsTotal = total;
        }

        // escribir fichero solo en uso normal (no experimento)
        
        std::ofstream salida("indice.txt");
        for (auto& [palabra, ids] : indice) {
            salida << palabra << " -> [";
            for (int i = 0; i < (int)ids.size(); i++) {
                salida << ids[i];
                if (i < (int)ids.size() - 1) salida << ", ";
            }
            salida << "]\n";
        }
        
        return 0;
    }

    int parte2(const std::string& archivoCorpus, ResultadoExperimento* resultado) {
        std::ifstream archivo(archivoCorpus);
        std::unordered_map<std::string, std::unordered_set<int>> indice;
        std::string linea;
        int docActual = -1;

        auto inicioIns = std::chrono::high_resolution_clock::now();

        while (std::getline(archivo, linea)) {
            if (linea.substr(0, 4) == "DOC:") {
                docActual = std::stoi(linea.substr(4));
            }
            else if (!linea.empty() && docActual != -1) {
                std::istringstream ss(linea);
                std::string palabra;
                while (ss >> palabra) {
                    if (palabra.length() > 4) {
                        indice[palabra].insert(docActual);
                    }
                }
            }
        }

        auto finIns = std::chrono::high_resolution_clock::now();
        int total = 0;
        for (auto& [palabra, ids] : indice) total += (int)ids.size();
        if (resultado) {
            resultado->tiempoInsercion = std::chrono::duration_cast<std::chrono::microseconds>(finIns - inicioIns).count();
            resultado->tamanoIndice = (int)indice.size();
            resultado->numIdsTotal = total;
        }

        std::ofstream salida("indice_deduplicado.txt");
        for (auto& [palabra, ids] : indice) {
            salida << palabra << " -> [";
            bool primero = true;
            for (auto& id : ids) {
                if (!primero) salida << ", ";
                salida << id;
                primero = false;
            }
            salida << "]\n";
        }
        return 0;
    }

    // funciones estaticas compartidas por las partes 3 y 4

    static std::unordered_map<std::string, std::vector<int>> cargarIndice(const std::string& archivo) {
        std::unordered_map<std::string, std::vector<int>> indice;
        std::ifstream f(archivo);
        std::string linea;
        while (std::getline(f, linea)) {
            auto sep = linea.find(" -> [");
            if (sep == std::string::npos) continue;
            std::string palabra = linea.substr(0, sep);
            std::string resto = linea.substr(sep + 5);
            if (!resto.empty() && resto.back() == ']') resto.pop_back();
            std::istringstream ss(resto);
            std::string token;
            while (std::getline(ss, token, ',')) {
                token.erase(std::remove(token.begin(), token.end(), ' '), token.end());
                if (!token.empty()) {
                    indice[palabra].push_back(std::stoi(token));
                }
            }
        }
        return indice;
    }

    static bool esOperador(const std::string& t) {
        return t == "AND" || t == "OR";
    }

    static int precedencia(const std::string& op) {
        if (op == "AND") return 2;
        if (op == "OR")  return 1;
        return 0;
    }

    static std::vector<std::string> infixAPostfix(const std::vector<std::string>& tokens) {
        std::vector<std::string> salida;
        std::stack<std::string> pila;

        for (const auto& t : tokens) {
            if (!esOperador(t)) {
                salida.push_back(t);
            }
            else {
                while (!pila.empty() && esOperador(pila.top()) &&
                    precedencia(pila.top()) >= precedencia(t)) {
                    salida.push_back(pila.top());
                    pila.pop();
                }
                pila.push(t);
            }
        }
        while (!pila.empty()) {
            salida.push_back(pila.top());
            pila.pop();
        }
        return salida;
    }

    static std::vector<int> interseccion(std::vector<int> a, std::vector<int> b) {
        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        std::vector<int> res;
        std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(res));
        return res;
    }

    static std::vector<int> unionVec(std::vector<int> a, std::vector<int> b) {
        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        std::vector<int> res;
        std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(res));
        return res;
    }

    static std::vector<int> evaluarPostfix(
        const std::vector<std::string>& postfix,
        const std::unordered_map<std::string, std::vector<int>>& indice)
    {
        std::stack<std::vector<int>> pila;

        for (const auto& t : postfix) {
            if (!esOperador(t)) {
                auto it = indice.find(t);
                if (it != indice.end()) pila.push(it->second);
                else                    pila.push({});
            }
            else {
                if (pila.size() < 2) { pila.push({}); continue; }
                auto b = pila.top(); pila.pop();
                auto a = pila.top(); pila.pop();
                if (t == "AND") pila.push(interseccion(a, b));
                else            pila.push(unionVec(a, b));
            }
        }
        if (pila.empty()) return {};
        return pila.top();
    }

    static std::string formatearIds(const std::vector<int>& ids) {
        std::string s = "[";
        for (int i = 0; i < (int)ids.size(); i++) {
            s += std::to_string(ids[i]);
            if (i < (int)ids.size() - 1) s += ", ";
        }
        s += "]";
        return s;
    }

    int parte3() {
        auto indice = cargarIndice("indice.txt");
        std::ifstream consultas("consultas.txt");
        std::ofstream salida("resultados_consultas.txt");

        std::string linea;
        while (std::getline(consultas, linea)) {
            if (linea.empty()) continue;
            std::istringstream ss(linea);
            std::vector<std::string> tokens;
            std::string tok;
            while (ss >> tok) tokens.push_back(tok);

            auto postfix = infixAPostfix(tokens);
            auto resultado = evaluarPostfix(postfix, indice);

            std::string postfixStr;
            for (int i = 0; i < (int)postfix.size(); i++) {
                postfixStr += postfix[i];
                if (i < (int)postfix.size() - 1) postfixStr += " ";
            }

            salida << "Consulta: " << linea << "\n";
            salida << "  Postfija: " << postfixStr << "\n";
            salida << "  Resultado: " << formatearIds(resultado) << "\n\n";
        }

        std::cout << "Puede encontrar el archivo resultados_consultas.txt en la carpeta del ejecutable.\n";
        return 0;
    }

    struct Peticion {
        std::string timestamp;
        std::string usuario;
        std::string consulta;
    };

    int parte4() {
        auto indice = cargarIndice("indice.txt");
        std::ifstream archivoPeticiones("peticiones.txt");
        std::queue<Peticion> cola;

        std::string linea;
        while (std::getline(archivoPeticiones, linea)) {
            if (linea.empty()) continue;
            auto sep1 = linea.find(" | ");
            if (sep1 == std::string::npos) continue;
            auto sep2 = linea.find(" | ", sep1 + 3);
            if (sep2 == std::string::npos) continue;

            Peticion p;
            p.timestamp = linea.substr(0, sep1);
            p.usuario = linea.substr(sep1 + 3, sep2 - sep1 - 3);
            p.consulta = linea.substr(sep2 + 3);
            cola.push(p);
        }

        std::ofstream log("log_procesamiento.txt");
        int totalProcesadas = 0;
        bool ordenRespetado = true;
        std::string timestampAnterior = "";

        while (!cola.empty()) {
            Peticion p = cola.front();
            cola.pop();

            if (!timestampAnterior.empty() && p.timestamp < timestampAnterior)
                ordenRespetado = false;
            timestampAnterior = p.timestamp;

            std::istringstream ss(p.consulta);
            std::vector<std::string> tokens;
            std::string tok;
            while (ss >> tok) tokens.push_back(tok);

            auto postfix = infixAPostfix(tokens);

            auto inicio = std::chrono::high_resolution_clock::now();
            auto resultado = evaluarPostfix(postfix, indice);
            auto fin = std::chrono::high_resolution_clock::now();
            long us = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();

            log << "[" << p.timestamp << "] PROCESANDO -> "
                << p.usuario << ": '" << p.consulta << "'\n";
            log << "  -> Documentos encontrados: " << formatearIds(resultado)
                << " | Tiempo: " << us << "us\n";

            totalProcesadas++;
        }

        log << "\n--- Estadisticas ---\n";
        log << "Total peticiones procesadas: " << totalProcesadas << "\n";
        log << "Orden de procesamiento respetado: " << (ordenRespetado ? "SI" : "NO") << "\n";

        std::cout << "Puede encontrar el archivo log_procesamiento.txt en la carpeta del ejecutable.\n";
        return 0;
    }

}