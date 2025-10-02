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
  * @brief Declaracion de los metodos de la clase grafo.
  * @version 1.0
  */

#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip> // Para std::setprecision y std::setw
#include <queue> // Cola para el BFS 
#include <stack> // Pila para el DFS
#include <unordered_map> // Para std::unordered_map

// Logs de la busqueda
struct IterationLog {
  std::vector<int> generated_accumulated{};     
  std::vector<int> inspected_accumulated{};  
};

// Resultado de una búsqueda en el grafo
struct SearchResult {
  std::vector<int> path{};          
  double cost = 0.0;              
  std::vector<IterationLog> logs{}; 
};

/**
 * Clase Grafo
 *       Trabaja con un grafo en el cual podemos hacer busquedas BFS y DFS
 */
class Grafo {
 public:
  // Constructores
  Grafo() = default;
  explicit Grafo(const std::string& grafo) { ProcesarGrafo(grafo); }
  // Google Style getters
  int numero_nodos() const noexcept { return numero_nodos_; }
  int numero_aristas() const noexcept { return numero_aristas_; } 
  // Metodos de utiidad y debug
  void ProcesarGrafo(const std::string&);
  void ImprimirMatriz() const;
  // Metodos de busqueda
  SearchResult BFS(int nodo_origen, int nodo_destino);
  SearchResult DFS(int nodo_origen, int nodo_destino);

 private:
  int numero_nodos_{};
  int numero_aristas_{};
  std::vector<std::vector<double>> matriz_adyacencia_{};
  // Metodos auxiliares
  std::vector<int> GetVecinos(int) const;
  bool ExisteArista(int, int) const;
  double GetPeso(int, int) const;
  std::vector<int> ReconstruirCamino(const std::unordered_map<int, int>& parent, int start, int goal) const;
  double CalcularCostoCamino(const std::vector<int>& path) const;
  // DFS auxiliar
  bool DFSRecursivo(int actual, int destino, std::vector<bool>& visitado, std::vector<int>& padre,
                    std::vector<int>& generados, std::vector<int>& inspeccionados,
                    std::vector<IterationLog>& logs);

};

#endif