/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Inteligencia Artificial 2025-2026
  * 3º Año de Carrera
  * Practica 2: Busquedas informadas
  *
  * @author Alberto Del Castillo Díaz alu0101627137@ull.edu.es
  * @date Oct 11 2025
  * @brief Definicion de las funciones clientes usadas en el main
  * @version 1.0
  */

#include "funciones_cliente.h"
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
                 "Modo de empleo: ./BusquedasInformadas input.txt [output.txt]\n"
                 "Pruebe ‘BusquedasInformadas --help’ para más información." << std::endl;
    return false;
  }
  std::string help{argv[1]};
  if (argc == 2 && (help == "--help" || help == "-h")) {
    Usage();
    return false;
  } 
  std::ifstream input_file{argv[1]}; 
  if(!input_file.is_open()) {
    std::cerr << "$ ./BusquedasNoInformadas\n"
                 "Modo de empleo: ./BusquedasNoInformadas input.txt [output.txt]\n"
                 "Pruebe ‘BusquedasNoInformadas --help’ para más información." << std::endl;
    return false;
  } 
  input_file.close();
  return true;
}

/**
 * @brief Muestra un mensaje si el programa se ejecuta de forma correcta con el proposito del programa.
 */
void PrintProgramPorpouse(void) {
  std::cout << "Programa: Busquedas informadas en entornos dinámicos\n";
  std::cout << "Descripción: Este programa construye un objeto Laberinto a partir de un fichero de entrada\n";
  std::cout << "y aplica estrategias de búsqueda informadas (en este caso A*) para encontrar un camino\n";
  std::cout << "de salida y el resultado de todas las iteraciones es almacenado en un fichero de salida.\n" << std::endl;
}


/**
 * @brief Muestra un mensaje en pantalla de como usar el program de forma detallada y con todas sus opciones y lo que hacen
 * 
 */
void Usage(void) {
    std::cout << "./BusquedasInformadas -- Búsquedas informadas sobre un laberinto" << std::endl;
  std::cout << "Modo de uso: ./BusquedasInformadas input.txt [output.txt]" << std::endl;
  std::cout << std::endl;
  std::cout << "Argumentos:" << std::endl;
  std::cout << "  input.txt    (obligatorio) Fichero de entrada con la definición del laberinto inicial." << std::endl;
  std::cout << "  output.txt   (opcional)    Nombre del fichero donde guardar la salida generada." << std::endl;
  std::cout << std::endl;
  std::cout << "Notas:" << std::endl;
  std::cout << "  - Si no se especifica 'output.txt', la salida se mostrará por un nombre definido ya." << std::endl;
  std::cout << "  - Use -h o --help para ver esta ayuda." << std::endl;
  std::cout << std::endl;
  std::cout << "Ejemplos:" << std::endl;
  std::cout << "  ./BusquedasInformadas test/M_1.txt" << std::endl;
  std::cout << "  ./BusquedasNoInformadas test/M_1.txt resultados.txt" << std::endl;
  std::cout << std::endl;
}

/**
 * @brief Menu de opciones para realizar busquedas sobre un laberinto
 * 
 * @param laberinto Objeto laberinto
 * @param A Agente para buscar los caminos
 * @param output_file Nombre del fichero de salida
 */
void Menu(Laberinto& laberinto, BusquedaInformada& A, const std::string& output_file) {
  int opcion = 0;
  do {
    std::cout << "\n========== BÚSQUEDAS INFORMADAS ==========\n";
    std::cout << "1. Cambiar entrada y salida\n";
    std::cout << "2. A* (Entorno Estático)\n";
    std::cout << "3. A* (Entorno Dinámico)\n";
    std::cout << "4. Mostrar laberinto actual\n";
    std::cout << "5. Dinamizar laberinto\n";
    std::cout << "6. A* (Entorno Estático) OCTILE\n";
    std::cout << "7. A* (Entorno Dinámico) OCTILE\n";
    std::cout << "8. Salir\n";
    std::cout << "Seleccione una opción: ";
    std::cin >> opcion;
    switch (opcion) {
      case 1: 
        // Cambiar entrada y salida juntas
        size_t fila_start, col_start, fila_exit, col_exit;
        std::cout << "\nCoordenadas ENTRADA (fila columna): ";
        std::cin >> fila_start >> col_start;
        std::cout << "Coordenadas SALIDA (fila columna): ";
        std::cin >> fila_exit >> col_exit;
        try {
          laberinto.CambiarEntradaYSalida({fila_start, col_start}, 
                                          {fila_exit, col_exit});
          std::cout << "Entrada y salida actualizadas.\n";
        } catch (const std::exception& e) {
          std::cerr << "Error: " << e.what() << "\n";
        }
        break;
      
      case 2: 
        // A* Estático
        std::cout << "\n=== BÚSQUEDA A* ESTÁTICA ===\n";
        if (A.BusquedaAStar(laberinto.coordenadas_start())) {
          if (!output_file.empty()) {
            std::ofstream out(output_file);
            A.GenerarReporteCompleto(output_file, "Manhattan", out);
            std::cout << "Resultados en: " << output_file << "\n";
          } else {
            A.GenerarReporteCompleto(output_file, "Manhattan", std::cout);
          }
        } else {
          std::cout << "No se encontró camino.\n";
        }
        break;
      
      case 3: 
        // A* Dinámico
        std::cout << "\n=== BÚSQUEDA A* DINÁMICA ===\n";
        if (A.BusquedaAStarDinamica(output_file)) {
          std::cout << "Resultados en: " << output_file << "\n";
        } else {
          std::cout << "No se pudo completar la búsqueda dinámica.\n";
        }
        break;
      
      case 4: 
        // Mostrar laberinto
        std::cout << "\n=== LABERINTO ACTUAL ===\n";
        std::cout << laberinto << "\n";
        break;
      
      case 5:
        std::cout << "\n=== LABERINTO DINAMICO ===\n";
        laberinto.ActualizarDinamismo();
        std::cout << laberinto << "\n";
        break;

      case 6: 
        // A* Estático OCTILE
        std::cout << "\n=== BÚSQUEDA A* ESTÁTICA ===\n";
        if (A.BusquedaAStarH2(laberinto.coordenadas_start())) {
          if (!output_file.empty()) {
            std::ofstream out(output_file);
            A.GenerarReporteCompleto(output_file, "Manhattan", out);
            std::cout << "Resultados en: " << output_file << "\n";
          } else {
            A.GenerarReporteCompleto(output_file, "Manhattan", std::cout);
          }
        } else {
          std::cout << "No se encontró camino.\n";
        }
        break;
      
      case 7: 
        // A* Dinámico
        std::cout << "\n=== BÚSQUEDA A* DINÁMICA ===\n";
        if (A.BusquedaAStarDinamicaH2(output_file)) {
          std::cout << "Resultados en: " << output_file << "\n";
        } else {
          std::cout << "No se pudo completar la búsqueda dinámica.\n";
        }
        break;
      
      case 8:
        std::cout << "\nSaliendo del programa...\n";
        break;
        
      default:
        std::cout << "Opción inválida.\n";
        break;
    }
  } while (opcion != 8);
}