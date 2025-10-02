/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Inteligencia Artificial 2025-2026
  * 3º Año de Carrera
  * Practica 1: Busquedas no informadas
  *
  * @author Alberto Del Castillo Díaz alu0101627137@ull.edu.es
  * @date Sep 01 2025
  * @brief Definicion de las funciones clientes usadas en el main
  * @version 1.0
  */

#include "funciones_cliente.h"
#include "grafo.h"
#include <iostream>
#include <fstream> // Fichero de cabezera para trabajar con ficheros
#include <string> // std::string

/**
 * @brief Comprueba los datos introducidos por parametros sean los esperados.
 * @param argc Numero de parametros introducidos por linea de comandos
 * @param argv Array de caracteres que contiene los parametros introducidos
 * @return Devuelve False si no son los datos deseados o true si son los esperados
 */
bool CheckCorrectsParameters(const int argc, char** argv) {
  if (argc != 3 && argc != 2) {
    std::cerr << "$ ./BusquedasNoInformadas\n"
                 "Modo de empleo: ./BusquedasNoInformadas input.txt [output.txt]\n"
                 "Pruebe ‘BusquedasNoInformadas --help’ para más información." << std::endl;
    return false;
  }

  
  std::string help{argv[1]};
    
  if (argc == 2 && help == "--help") {
    Usage();
    return false;
  } 

  std::ifstream input_file_1{argv[1]}; 
  if(!input_file_1.is_open()) {
    std::cerr << "$ ./BusquedasNoInformadas\n"
                 "Modo de empleo: ./BusquedasNoInformadas input.txt [output.txt]\n"
                 "Pruebe ‘BusquedasNoInformadas --help’ para más información." << std::endl;
    return false;
  } 
  input_file_1.close();
  return true;
}

/**
 * @brief Muestra un mensaje si el programa se ejecuta de forma correcta con el proposito del programa.
 */
void PrintProgramPorpouse(void) {
  std::cout << "Programa: Busquedas no informadas en grafos\n";
  std::cout << "Descripción: Este programa construye un objeto Grafo a partir de un fichero de entrada\n";
  std::cout << "y aplica estrategias de búsqueda no informadas (p.ej., BFS/DFS) para encontrar un camino\n";
  std::cout << "entre dos vértices y el resultado es almacenado en un fichero de salida.\n" << std::endl;
}


/**
 * @brief Muestra un mensaje en pantalla de como usar el program de forma detallada y con todas sus opciones y lo que hacen
 * 
 */
void Usage(void) {
    std::cout << "./BusquedasNoInformadas -- Búsquedas no informadas sobre un grafo" << std::endl;
  std::cout << "Modo de uso: ./BusquedasNoInformadas input.txt [output.txt]" << std::endl;
  std::cout << std::endl;
  std::cout << "Argumentos:" << std::endl;
  std::cout << "  input.txt    (obligatorio) Fichero de entrada con la definición del grafo." << std::endl;
  std::cout << "  output.txt   (opcional)    Nombre del fichero donde guardar la salida generada." << std::endl;
  std::cout << std::endl;
  std::cout << "Notas:" << std::endl;
  std::cout << "  - El constructor de la clase Grafo recibe 'input.txt' como único parámetro obligatorio." << std::endl;
  std::cout << "  - Si no se especifica 'output.txt', la salida se mostrará por un nombre definido ya." << std::endl;
  std::cout << "  - Use -h o --help para ver esta ayuda." << std::endl;
  std::cout << std::endl;
  std::cout << "Ejemplos:" << std::endl;
  std::cout << "  ./BusquedasNoInformadas datos/grafo01.txt" << std::endl;
  std::cout << "  ./BusquedasNoInformadas datos/grafo01.txt resultados.txt" << std::endl;
  std::cout << std::endl;
}

/**
 * @brief Genera un archivo de salida con los resultados de la búsqueda en el grafo.
 * 
 * Esta función escribe en un archivo los detalles del grafo, el algoritmo utilizado,
 * las iteraciones realizadas durante la búsqueda, y el resultado final (camino encontrado
 * y su costo, o un mensaje indicando que no se encontró camino).
 * 
 * @param resultado Objeto de tipo SearchResult que contiene los datos de la búsqueda,
 * incluyendo los nodos generados, inspeccionados y el camino encontrado.
 * @param grafo Objeto de tipo Grafo que representa el grafo sobre el cual se realizó la búsqueda.
 * @param origen Índice del vértice de origen (base 0).
 * @param destino Índice del vértice de destino (base 0).
 * @param archivo_salida Nombre del archivo donde se guardará la salida generada.
 */
void GenerarOutput(const SearchResult& resultado, const Grafo& grafo, int origen, int destino, 
                   const std::string& archivo_salida, const std::string& tipo_busqueda) {
  std::ofstream archivo(archivo_salida);
  // Cabecera
  archivo << "Número de nodos del grafo: " << grafo.numero_nodos() << "\n";
  archivo << "Número de aristas del grafo: " << grafo.numero_aristas() << "\n";
  archivo << "Vértice origen: " << origen + 1 << "\n";
  archivo << "Vértice destino: " << destino + 1 << "\n";
  archivo << "Algoritmo: " << tipo_busqueda << "\n";
  archivo << "--------------------------------------------------\n\n";
  // Iteraciones
  for (size_t i = 0; i < resultado.logs.size(); ++i) {
    const auto& log = resultado.logs[i];
    archivo << "Iteración " << i + 1 << "\n";
    // Nodos generados (acumulados)
    archivo << "Nodos generados: ";
    if (log.generated_accumulated.empty()) {
        archivo << "-";
    } else {
      for (size_t j = 0; j < log.generated_accumulated.size(); ++j) {
        archivo << log.generated_accumulated[j] + 1;
        if (j < log.generated_accumulated.size() - 1) archivo << ", ";
      }
    }
    archivo << "\n";
    // Nodos inspeccionados (acumulados)
    archivo << "Nodos inspeccionados: ";
    if (log.inspected_accumulated.empty()) {
        archivo << "-";
    } else {
        for (size_t j = 0; j < log.inspected_accumulated.size(); ++j) {
            archivo << log.inspected_accumulated[j] + 1;
            if (j < log.inspected_accumulated.size() - 1) archivo << ", ";
        }
    }
    archivo << "\n";
    archivo << "--------------------------------------------------\n";
  }
  // Separador antes del resultado final
  archivo << "\n";
  // Resultado final
  if (!resultado.path.empty()) {
    archivo << "Camino: ";
    for (size_t i = 0; i < resultado.path.size(); ++i) {
      archivo << resultado.path[i] + 1;
      if (i < resultado.path.size() - 1) archivo << " - ";
    }
    archivo << "\n";
    archivo << "Costo: " << std::fixed << std::setprecision(2) << resultado.cost << "\n";
  } else {
    archivo << "No se encontró camino\n";
  }
  archivo.close();
}

/**
 * @brief 
 * 
 * @param grafo 
 * @param origen 
 * @param destino 
 */
void Menu(Grafo& grafo, int origen, int destino, const std::string& outputfile) {
  int opcion;
  std::cout << "\nSeleccione algoritmo:\n";
  std::cout << "1. BFS (Búsqueda en Amplitud)\n";
  std::cout << "2. DFS (Búsqueda en Profundidad)\n";
  std::cout << "Opción: ";
  std::cin >> opcion;
  if (opcion == 1) {
    std::cout << "\n=== EJECUTANDO BFS ===\n";
    SearchResult resultado_bfs = grafo.BFS(origen, destino);
    std::string archivo_salida_bfs{};
    if (outputfile != "") {
      archivo_salida_bfs = outputfile;
    } else {
      archivo_salida_bfs = "resultado_bfs.txt";
    }
    GenerarOutput(resultado_bfs, grafo, origen, destino, archivo_salida_bfs, "BFS");
    std::cout << "=== RESULTADO BFS ===\n";
    if (!resultado_bfs.path.empty()) {
      std::cout << "Camino encontrado: ";
      for (size_t i = 0; i < resultado_bfs.path.size(); ++i) {
        std::cout << resultado_bfs.path[i] + 1;
        if (i < resultado_bfs.path.size() - 1) std::cout << " -> ";
      }
      std::cout << "\nCosto: " << resultado_bfs.cost << "\n";
    } else {
      std::cout << "No se encontró camino\n";
    }
    std::cout << "Iteraciones BFS: " << resultado_bfs.logs.size() << "\n";
    std::cout << "Resultados BFS guardados en: " << archivo_salida_bfs << "\n\n";
  }
  if (opcion == 2) {
    std::cout << "\n=== EJECUTANDO DFS ===\n";
    SearchResult resultado_dfs = grafo.DFS(origen, destino);
    std::string archivo_salida_dfs{};
    if (outputfile != "") {
      archivo_salida_dfs = outputfile;
    } else {
      archivo_salida_dfs = "resultado_bfs.txt";
    }
    GenerarOutput(resultado_dfs, grafo, origen, destino, archivo_salida_dfs, "DFS");
    std::cout << "=== RESULTADO DFS ===\n";
    if (!resultado_dfs.path.empty()) {
      std::cout << "Camino encontrado: ";
      for (size_t i = 0; i < resultado_dfs.path.size(); ++i) {
        std::cout << resultado_dfs.path[i] + 1;
        if (i < resultado_dfs.path.size() - 1) std::cout << " -> ";
      }
      std::cout << "\nCosto: " << resultado_dfs.cost << "\n";
    } else {
      std::cout << "No se encontró camino\n";
    }
    std::cout << "Iteraciones DFS: " << resultado_dfs.logs.size() << "\n";
    std::cout << "Resultados DFS guardados en: " << archivo_salida_dfs << "\n\n";
  }
  std::cout << "Búsqueda(s) completada(s).\n";
}