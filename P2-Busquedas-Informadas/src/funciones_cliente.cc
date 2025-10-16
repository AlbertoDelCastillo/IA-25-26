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


void Menu(Laberinto& laberinto, const std::string& output_file) {
  int opcion = 0;
  std::cout << "DEBUG: Archivo de salida = '" << output_file << "'" << std::endl; 
  do {
    std::cout << "\n========== MENÚ DE PRUEBAS ==========\n";
    std::cout << "1. Imprimir laberinto\n";
    std::cout << "2. Cambiar entrada (Start)\n";
    std::cout << "3. Cambiar salida (Exit)\n";
    std::cout << "4. Dinamismo laberinto\n";
    std::cout << "5. Salir\n";
    std::cout << "Seleccione una opción: ";
    std::cin >> opcion;
    switch (opcion) {
      case 1: 
        std::cout << "\n--- LABERINTO ACTUAL ---\n";
        laberinto.ImprimirLaberinto();
        break;
      case 2: 
        size_t fila, columna;
        std::cout << "\nIngrese coordenadas para la nueva ENTRADA (fila columna): ";
        std::cin >> fila >> columna;
        try {
          laberinto.CambiarStart(fila, columna);
          std::cout << "Entrada cambiada correctamente.\n";
        } catch (const std::exception& e) {
          std::cerr << "Error: " << e.what() << "\n";
        }
        break;
      case 3: {
        size_t fila, columna;
        std::cout << "\nIngrese coordenadas para la nueva SALIDA (fila columna): ";
        std::cin >> fila >> columna;
        try {
          laberinto.CambiarExit(fila, columna);
          std::cout << "Salida cambiada correctamente.\n";
        } catch (const std::exception& e) {
          std::cerr << "Error: " << e.what() << "\n";
        }
        break;
      }
      case 4:
        std::cout << "\nAplicando dinamismo al laberinto...\n";
        laberinto.ActualizarDinamismo();
        
        // MOSTRAR SIEMPRE el laberinto actualizado
        std::cout << "\n--- LABERINTO ACTUALIZADO ---\n";
        laberinto.ImprimirLaberinto();
        
        // Guardar en archivo si se especificó
        if (!output_file.empty()) {
          std::ofstream Output(output_file, std::ios::app);
          if (Output.is_open()) {
            Output << laberinto;
            Output << "\n";
            std::cout << "Laberinto guardado en: " << output_file << "\n";
            Output.close();
          } else {
            std::cerr << "Error: No se pudo abrir el archivo " << output_file << " para escritura.\n";
          }
        }
        break;
      case 5:
        std::cout << "\nSaliendo del programa...\n";
        break;
      default:
        std::cout << "Opción no válida. Intente de nuevo.\n";
        break;
    }
  } while (opcion != 5);
}