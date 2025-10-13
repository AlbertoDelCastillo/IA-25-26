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
  * @brief Definicion de los metodos de la clase grafo.
  * @version 1.0
  */

#include "grafo.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <queue> 
#include <ctime> // Para numeros aleatorios
#include <cstdlib> // Para numeros aleatorios


// ============================================================================
// MÉTODOS DE CONSTRUCCIÓN Y VISUALIZACIÓN
// ============================================================================

/**
 * @brief Carga un grafo desde un archivo de texto con formato específico
 * 
 * @param inputfile Ruta del archivo que contiene la definición del grafo
 * @throw std::runtime_error Si el archivo está incompleto o no puede leerse
 * 
 * El formato del archivo debe ser:
 * - Primera línea: número de nodos
 * - Líneas siguientes: distancias entre pares de nodos (i,j) con i < j
 * - Las distancias son simétricas y -1 indica que no hay arista
 * - La diagonal principal se establece automáticamente a 0
 */
void Grafo::ProcesarGrafo(const std::string& inputfile) {
  std::ifstream fichero_grafo{inputfile};
  std::string line{};
  getline(fichero_grafo, line);
  numero_nodos_ = std::stoi(line);
  // Inicializar matriz de adyacencia n x n con -1
  matriz_adyacencia_.resize(numero_nodos_, std::vector<double>(numero_nodos_, -1.0));
  // Diagonal principal a 0 (d(i,i) = 0)
  for (int i = 0; i < numero_nodos_; ++i) {
    matriz_adyacencia_[i][i] = 0.0;
  }
  // Leer las distancias para los pares (i,j) donde i < j
  for (int i = 0; i < numero_nodos_ - 1; ++i) {
    for (int j = i + 1; j < numero_nodos_; ++j) {
      if (!getline(fichero_grafo, line)) {
        throw std::runtime_error("Archivo incompleto: se esperaban más datos");
      }
      double distancia = std::stod(line);
      // Asignar distancia simétrica
      matriz_adyacencia_[i][j] = distancia;
      matriz_adyacencia_[j][i] = distancia;
      // Contar aristas (si distancia > 0, hay arista)
      if (distancia > 0) {
        numero_aristas_++;
      }
    }
  }
  fichero_grafo.close();
}

/**
 * @brief Muestra la matriz de adyacencia del grafo en formato tabular
 * 
 * Imprime una representación visual de la matriz donde:
 * - 0.000 indica la diagonal principal
 * - INF indica que no hay conexión entre nodos (-1)
 * - Los valores positivos representan el peso de las aristas
 * - La salida incluye encabezados de filas y columnas para mejor legibilidad
 */
void Grafo::ImprimirMatriz() const {
  std::cout << "Matriz de Adyacencia (" << numero_nodos_ << "x" << numero_nodos_ << "):\n";
  std::cout << "    ";
  // Imprimir encabezado de columnas
  for (int j = 0; j < numero_nodos_; ++j) {
    std::cout << std::setw(8) << j + 1;
  }
  std::cout << "\n";
  // Imprimir separador
  std::cout << "    ";
  for (int j = 0; j < numero_nodos_; ++j) {
    std::cout << "--------";
  }
  std::cout << "\n";
  // Imprimir filas de la matriz
  for (int i = 0; i < numero_nodos_; ++i) {
    std::cout << std::setw(2) << i + 1 << " |";
    for (int j = 0; j < numero_nodos_; ++j) {
      if (matriz_adyacencia_[i][j] == -1) {
        std::cout << std::setw(8) << "INF";
      } else {
        std::cout << std::setw(8) << std::fixed << std::setprecision(3) << matriz_adyacencia_[i][j];
      }
    }
    std::cout << "\n";
  }
}

// ============================================================================
// MÉTODOS AUXILIARES DE CONSULTA
// ============================================================================

/**
 * @brief Obtiene los nodos vecinos conectados a un nodo dado
 * 
 * @param nodo Índice del nodo del cual se quieren obtener los vecinos
 * @return std::vector<int> Lista de índices de nodos vecinos conectados
 * @throw std::out_of_range Si el índice del nodo está fuera de rango
 */
std::vector<int> Grafo::GetVecinos(int nodo) const {
  if (nodo < 0 || nodo >= numero_nodos_) {
    throw std::out_of_range("Índice de nodo fuera de rango");
  }
  std::vector<int> vecinos{};
  for (int i{0}; i < numero_nodos_; ++i) {
    if (matriz_adyacencia_[nodo][i] > 0) {
      vecinos.push_back(i);
    }
  }
  return vecinos;
}

/**
 * @brief Verifica si existe una arista entre dos nodos
 * 
 * @param nodo_origen Índice del nodo origen
 * @param nodo_destino Índice del nodo destino  
 * @return true Existe una arista entre los nodos
 * @return false No existe arista entre los nodos
 * @throw std::out_of_range Si algún índice está fuera de rango
 */
bool Grafo::ExisteArista(int nodo_origen, int nodo_destino) const {
  if (nodo_origen < 0 || nodo_origen >= numero_nodos_ || 
      nodo_destino < 0 || nodo_destino >= numero_nodos_) {
    throw std::out_of_range("Índice de nodo fuera de rango");
  }
  return (matriz_adyacencia_[nodo_origen][nodo_destino] > 0);
}

/**
 * @brief Obtiene el peso de una arista entre dos nodos
 * 
 * @param nodo_origen Índice del nodo origen
 * @param nodo_destino Índice del nodo destino
 * @return double Peso de la arista, o -2 si no existe
 * @throw std::out_of_range Si algún índice está fuera de rango
 */
double Grafo::GetPeso(int nodo_origen, int nodo_destino) const {
  if (nodo_origen < 0 || nodo_origen >= numero_nodos_ || 
      nodo_destino < 0 || nodo_destino >= numero_nodos_) {
    throw std::out_of_range("Índice de nodo fuera de rango");
  }
  if (ExisteArista(nodo_origen, nodo_destino)) {
    return matriz_adyacencia_[nodo_origen][nodo_destino];
  } 
  return -2;
}

// ============================================================================
// MÉTODOS DE BÚSQUEDA NO INFORMADA
// ============================================================================

/**
 * @brief Implementa el algoritmo de Búsqueda en Amplitud con multiarranque (Multistart BFS)
 * 
 * @param nodo_origen Índice del nodo de inicio de la búsqueda
 * @param nodo_destino Índice del nodo objetivo a encontrar
 * @return SearchResult Estructura con el camino encontrado, costo e historial de búsqueda
 * 
 * El algoritmo multiarranque funciona de la siguiente manera:
 * 1. Inspecciona el nodo inicial
 * 2. Selecciona aleatoriamente entre el mejor (menor peso) o peor (mayor peso) nodo hijo
 * 3. Realiza una búsqueda BFS completa desde ese nodo seleccionado
 * 4. Si encuentra el destino, retorna el camino
 * 5. Si no, repite hasta 10 ejecuciones máximo
 */
SearchResult Grafo::BFS(int nodo_origen, int nodo_destino) {
  SearchResult resultado_final;
  if (nodo_origen < 0 || nodo_origen >= numero_nodos_ ||
      nodo_destino < 0 || nodo_destino >= numero_nodos_) {
    return resultado_final;
  }
  // Si el origen y destino son el mismo nodo
  if (nodo_origen == nodo_destino) {
    resultado_final.path = {nodo_origen};
    resultado_final.cost = 0.0;
    return resultado_final;
  }
  // Máximo de ejecuciones permitidas AÑADIDO
  const int max_ejecuciones = 10;
  int ejecuciones_realizadas = 0;
  // Semilla para números aleatorios AÑADIDO
  std::srand(std::time(0));
  while (ejecuciones_realizadas < max_ejecuciones) { // AÑADIDO
    SearchResult r;
    std::vector<bool> visitado(numero_nodos_, false);
    std::vector<int> padre(numero_nodos_, -1);
    std::queue<int> q;
    // Vectores acumulados para el log
    std::vector<int> generados;
    std::vector<int> inspeccionados;
    // PRIMERA FASE: Inspeccionar el nodo inicial
    q.push(nodo_origen);
    visitado[nodo_origen] = true;
    generados.push_back(nodo_origen);
    // Iteración 1: antes de inspeccionar a nadie
    {
      IterationLog l;
      l.generated_accumulated = generados;
      l.inspected_accumulated = inspeccionados;
      r.logs.push_back(l);
    }
    // Inspeccionar el nodo inicial
    int nodo_inicial = q.front(); q.pop();
    inspeccionados.push_back(nodo_inicial);
    // Generar vecinos del nodo inicial y almacenar con sus pesos AÑADIDO
    std::vector<std::pair<int, double>> vecinos_con_peso; // (nodo, peso)
    std::vector<int> vecinos_inicial = GetVecinos(nodo_inicial);
    for (int vecino : vecinos_inicial) {
      if (!visitado[vecino]) {
        double peso = GetPeso(nodo_inicial, vecino);
        vecinos_con_peso.push_back({vecino, peso});
        visitado[vecino] = true;
        padre[vecino] = nodo_inicial;
        generados.push_back(vecino);
      }
    }
    // Snapshot tras inspeccionar nodo inicial
    {
      IterationLog l;
      l.generated_accumulated = generados;
      l.inspected_accumulated = inspeccionados;
      r.logs.push_back(l);
    }
    // SEGUNDA FASE: Selección aleatoria entre mejor y peor nodo hijo basado en peso AÑADIDO
    if (!vecinos_con_peso.empty()) {
      // Ordenar vecinos por peso (de menor a mayor)
      std::sort(vecinos_con_peso.begin(), vecinos_con_peso.end(),
                [](const std::pair<int, double>& peso_1, const std::pair<int, double>& peso_2) {
                  return peso_1.second < peso_2.second;
                });
      int mejor_nodo = vecinos_con_peso.front().first;    // Nodo con menor peso (mejor)
      int peor_nodo = vecinos_con_peso.back().first;      // Nodo con mayor peso (peor)
      // Mostrar información de debug (opcional)
      std::cout << "Ejecución " << (ejecuciones_realizadas + 1) << ": ";
      std::cout << "Mejor nodo: " << mejor_nodo << " (peso: " << vecinos_con_peso.front().second << "), ";
      std::cout << "Peor nodo: " << peor_nodo << " (peso: " << vecinos_con_peso.back().second << "), ";
      // Selección aleatoria: 50% probabilidad de elegir mejor o peor
      int nodo_seleccionado = (std::rand() % 2 == 0) ? mejor_nodo : peor_nodo;
      r.nodo_seleccionado = nodo_seleccionado;
      std::cout << "Seleccionado: " << nodo_seleccionado << std::endl;
      // TERCERA FASE: Búsqueda BFS desde el nodo seleccionado
      std::queue<int> q_secundaria;
      q_secundaria.push(nodo_seleccionado);
      // Continuar la búsqueda desde el nodo seleccionado BFS NORMAL
      while (!q_secundaria.empty()) {
        int u = q_secundaria.front(); 
        q_secundaria.pop();
        // Inspeccionar nodo u
        if (std::find(inspeccionados.begin(), inspeccionados.end(), u) == inspeccionados.end()) {
          inspeccionados.push_back(u);
        }
        // Generar vecinos de u
        for (int v : GetVecinos(u)) {
          if (!visitado[v]) {
            visitado[v] = true;
            padre[v] = u;
            q_secundaria.push(v);
            if (std::find(generados.begin(), generados.end(), v) == generados.end()) {
              generados.push_back(v);
            }
          }
        }
        // Snapshot tras inspeccionar u
        {
          IterationLog l;
          l.generated_accumulated = generados;
          l.inspected_accumulated = inspeccionados;
          r.logs.push_back(l);
        }
        // Si encontramos el destino, terminar esta ejecución
        if (u == nodo_destino) {
          break;
        }
      }
    }
    // Reconstrucción de camino si se llegó al destino
    if (visitado[nodo_destino]) {
      std::vector<int> path;
      for (int v = nodo_destino; v != -1; v = padre[v]) {
        path.push_back(v);
      }
      std::reverse(path.begin(), path.end());
      r.path = path;
      // Calcular costo del camino
      double costo = 0.0;
      for (size_t i = 1; i < r.path.size(); ++i) {
        costo += GetPeso(r.path[i-1], r.path[i]);
      }
      r.cost = costo;
      
      std::cout << "Camino encontrado en ejecución " << (ejecuciones_realizadas + 1) << std::endl;
      r.numero_intentos = (ejecuciones_realizadas + 1);
      return r;
    }
    ejecuciones_realizadas++;
    std::cout << "Ejecución " << ejecuciones_realizadas << " no encontró camino" << std::endl;
    // Si esta ejecución no encontró camino, preparamos para la siguiente
    resultado_final = r;
  }
  // Si llegamos aquí, no se encontró camino en las 10 ejecuciones
  std::cout << "No se encontró camino después de " << max_ejecuciones << " ejecuciones" << std::endl;
  return resultado_final;
}

/**
 * @brief Función auxiliar recursiva para el algoritmo de Búsqueda en Profundidad (DFS)
 * 
 * @param actual Nodo actual que se está explorando
 * @param destino Nodo objetivo de la búsqueda
 * @param visitado Vector que registra los nodos ya visitados
 * @param padre Vector que almacena el nodo padre de cada nodo para reconstruir el camino
 * @param generados Vector acumulativo de todos los nodos generados durante la búsqueda
 * @param inspeccionados Vector acumulativo de todos los nodos inspeccionados/expandidos
 * @param logs Vector donde se almacena el historial de cada iteración de la búsqueda
 * @return true Si se encontró un camino al nodo destino
 * @return false Si no se encontró camino desde el nodo actual
 * 
 * Esta función implementa el núcleo recursivo del DFS. Explora en profundidad cada rama
 * completamente antes de retroceder (backtracking). Cuando encuentra el destino, la
 * recursión se desenrolla y se reconstruye el camino mediante el vector de padres.
 */
bool Grafo::DFSRecursivo(int actual, int destino, 
                        std::vector<bool>& visitado, std::vector<int>& padre,
                        std::vector<int>& generados, std::vector<int>& inspeccionados,
                        std::vector<IterationLog>& logs) {
    
    visitado[actual] = true;
    // Registrar el nodo actual como inspeccionado si no lo está
    if (std::find(inspeccionados.begin(), inspeccionados.end(), actual) == inspeccionados.end()) {
        inspeccionados.push_back(actual);
    }
    // Guardar el estado actual de la búsqueda
    logs.push_back({generados, inspeccionados});
    // Caso base: se encontró el destino
    if (actual == destino) {
        return true;
    }
    // Explorar recursivamente todos los vecinos no visitados
    for (int vecino : GetVecinos(actual)) {
        if (!visitado[vecino]) {
            // Registrar el vecino como generado si no lo está
            if (std::find(generados.begin(), generados.end(), vecino) == generados.end()) {
                generados.push_back(vecino);
            }
            padre[vecino] = actual;
            // Llamada recursiva: explorar en profundidad esta rama
            bool encontrado = DFSRecursivo(vecino, destino, visitado, padre, 
                                         generados, inspeccionados, logs);
            if (encontrado) {
                return true;
            }
            // Si no se encontró en esta rama, continúa con el siguiente vecino
            // (backtracking implícito en la recursión)
        }
    }
    return false;
}

/**
 * @brief Implementa el algoritmo de Búsqueda en Profundidad (Depth-First Search)
 * 
 * @param nodo_origen Índice del nodo de inicio de la búsqueda
 * @param nodo_destino Índice del nodo objetivo a encontrar
 * @return SearchResult Estructura con el camino encontrado, costo e historial de búsqueda
 * 
 * El algoritmo DFS explora el grafo en profundidad, siguiendo cada rama hasta su final
 * antes de retroceder y probar caminos alternativos. Utiliza recursión (pila de llamadas)
 * para gestionar el backtracking.
 * 
 * Características:
 * - Completo: En grafos finitos, encuentra solución si existe
 * - No óptimo: Puede encontrar caminos más largos primero
 * - Complejidad: O(V + E) en tiempo, O(V) en espacio (profundidad de recursión)
 * - Eficiente en memoria para grafos con mucha profundidad
 */
SearchResult Grafo::DFS(int nodo_origen, int nodo_destino) {
    SearchResult r;
    if (nodo_origen < 0 || nodo_origen >= numero_nodos_ ||
        nodo_destino < 0 || nodo_destino >= numero_nodos_) {
        return r;
    }
    std::vector<bool> visitado(numero_nodos_, false);
    std::vector<int> padre(numero_nodos_, -1);
    std::vector<int> generados;
    std::vector<int> inspeccionados;
    std::vector<IterationLog> logs;
    // Estado inicial: nodo origen generado pero no inspeccionado
    generados.push_back(nodo_origen);
    logs.push_back({generados, inspeccionados});
    // Iniciar búsqueda recursiva
    bool encontrado = DFSRecursivo(nodo_origen, nodo_destino, visitado, padre,
                                  generados, inspeccionados, logs);
    r.logs = logs;
    if (encontrado) {
        // Reconstruir camino desde el destino hasta el origen
        std::vector<int> path;
        for (int v = nodo_destino; v != -1; v = padre[v]) {
            path.push_back(v);
        }
        std::reverse(path.begin(), path.end());
        r.path = path;
        r.cost = CalcularCostoCamino(path);
    }
    return r;
}

// ============================================================================
// MÉTODOS AUXILIARES DE CAMINOS
// ============================================================================

/**
 * @brief Reconstruye un camino desde el origen hasta el destino usando el vector de padres
 * 
 * @param parent Mapa que relaciona cada nodo con su nodo padre en el árbol de búsqueda
 * @param start Nodo de inicio del camino
 * @param goal Nodo destino del camino
 * @return std::vector<int> Secuencia de nodos que forman el camino desde start hasta goal
 */
std::vector<int> Grafo::ReconstruirCamino(const std::unordered_map<int, int>& parent, 
                                         int start, int goal) const {
    std::vector<int> path;
    int current = goal;
    while (current != start) {
        path.push_back(current);
        current = parent.at(current);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

/**
 * @brief Calcula el costo total de un camino sumando los pesos de sus aristas
 * 
 * @param path Vector con la secuencia de nodos que forman el camino
 * @return double Costo total del camino como suma de pesos de aristas consecutivas
 */
double Grafo::CalcularCostoCamino(const std::vector<int>& path) const {
    double total_cost = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        total_cost += GetPeso(path[i], path[i + 1]);
    }
    return total_cost;
}
