/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Inteligencia Artificial 2025-2026
  * 3º Año de Carrera
  * Practica 2: Busquedas informadas
  *
  * @author Alberto Del Castillo Díaz alu0101627137@ull.edu.es
  * @date Oct 16 2025
  * @brief Implementación de los métodos de la clase Laberinto
  * @version 1.0
  */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include "casilla.h"
#include "laberinto.h"


//============================================================================
// MÉTODOS DE INICIALIZACIÓN Y CONFIGURACIÓN
//============================================================================

/**
 * @brief Procesa y carga un laberinto desde un archivo de texto
 * @param inputfile Ruta al archivo de entrada
 * @throws std::runtime_error Si el archivo no se puede abrir o tiene errores
 * 
 * Lee las dimensiones del laberinto y construye la matriz de casillas.
 * Identifica automáticamente las posiciones de entrada (3) y salida (4).
 */
void Laberinto::ProcesarLaberinto(const std::string& inputfile) {
  std::ifstream fichero_laberinto{inputfile};
  std::string line{};
  getline(fichero_laberinto, line);
  filas_ = std::stoul(line);
  getline(fichero_laberinto, line);
  columnas_ = std::stoul(line);
  matriz_casillas_.resize(filas_, std::vector<Casilla>(columnas_));
  for (size_t i{0}; i < filas_; ++i) {
    for (size_t j{0}; j < columnas_; ++j) {
      size_t valor{};
      fichero_laberinto >> valor;
      if (valor == 3) {
        coordenadas_start_ = {i, j};
      }
      if (valor == 4) {
        coordenadas_exit_ = {i, j};
      }
      Tipo_Casilla tipo = FromInt(valor);
      matriz_casillas_[i][j] = Casilla(tipo);
    }
  }
  fichero_laberinto.close();
}


/**
 * @brief Cambia simultáneamente la entrada y salida del laberinto
 * @param entrada Par (fila, columna) de la nueva entrada
 * @param salida Par (fila, columna) de la nueva salida
 * @throws std::invalid_argument Si alguna posición no es válida o son iguales
 */
void Laberinto::CambiarEntradaYSalida(const std::pair<size_t, size_t>& entrada,
                                      const std::pair<size_t, size_t>& salida) {
  // Validar que las posiciones son diferentes
  if (entrada == salida) {
    throw std::invalid_argument("La entrada y salida no pueden ser la misma posición.");
  }
  
  // Validar entrada
  if (!EsEntradaValida(entrada.first, entrada.second)) {
    throw std::invalid_argument("Posición de entrada no válida.");
  }
  
  // Validar salida
  if (!EsSalidaValida(salida.first, salida.second)) {
    throw std::invalid_argument("Posición de salida no válida.");
  }
  
  // Restaurar las antiguas posiciones a casilla libre
  matriz_casillas_[coordenadas_start_.first][coordenadas_start_.second].setTipoCasilla(Tipo_Casilla::Libre);
  matriz_casillas_[coordenadas_exit_.first][coordenadas_exit_.second].setTipoCasilla(Tipo_Casilla::Libre);
  
  // Establecer nuevas entrada y salida
  matriz_casillas_[entrada.first][entrada.second].setTipoCasilla(Tipo_Casilla::Start);
  matriz_casillas_[salida.first][salida.second].setTipoCasilla(Tipo_Casilla::Exit);
  
  // Actualizar coordenadas almacenadas
  coordenadas_start_ = entrada;
  coordenadas_exit_ = salida;
}

//============================================================================
// MÉTODOS DE DINAMISMO
//============================================================================


  /**
   * @brief Aplica transformaciones dinámicas al laberinto
   * 
   * Ejecuta dos operaciones:
   * 1. Mutación probabilística de casillas (pin=0.5, pout=0.5)
   * 2. Enforcement del límite del 25% de obstáculos
   * 
   * Este método simula cambios en el entorno durante la navegación.
   * 
   * @note Las posiciones de entrada y salida nunca mutan
   * @see MutarCeldas, EnforceMaxBloqueo
   */
void Laberinto::ActualizarDinamismo() {
  std::random_device rd;
  std::mt19937 gen(rd());
  double pin{0.5}, pout{0.5};
  MutarCeldas(pin, pout, gen);
  EnforceMaxBloqueo(0.25, gen);
}

//============================================================================
// MÉTODOS PARA ALGORITMO A*
//============================================================================

/**
 * @brief Obtiene las casillas vecinas transitables de una posición
 * @param fila Fila de la casilla origen
 * @param columna Columna de la casilla origen
 * @return Vector con las coordenadas de los vecinos válidos
 */
std::vector<std::pair<size_t, size_t>> Laberinto::GetVecinosCasilla(const size_t fila, const size_t columna) const {
  std::vector<std::pair<size_t, size_t>> vecinos;
  
  // Definir todas las direcciones posibles (8-vecindad)
  const std::vector<std::pair<int, int>> direcciones = {
    {-1, -1}, {-1, 0}, {-1, 1},  // Superior izquierda, superior, superior derecha
    {0, -1},           {0, 1},   // Izquierda, derecha
    {1, -1},  {1, 0},  {1, 1}    // Inferior izquierda, inferior, inferior derecha
  };
  
  for (const auto& dir : direcciones) {
    size_t nueva_fila = fila + dir.first;
    size_t nueva_columna = columna + dir.second;
    
    if (EsMovimientoValido(fila, columna, nueva_fila, nueva_columna)) {
      vecinos.emplace_back(nueva_fila, nueva_columna);
    }
  }
  
  return vecinos;
}

/**
  * @brief Calcula el coste de moverse entre dos casillas adyacentes
  * @param fila_actual Fila de la casilla origen
  * @param col_actual Columna de la casilla origen
  * @param fila_vecina Fila de la casilla destino
  * @param col_vecina Columna de la casilla destino
  * @return Coste del movimiento (5.0 ortogonal, 7.0 diagonal, ∞ inválido)
  * @note Solo calcula costes entre casillas directamente adyacentes
  */
double Laberinto::MoveCost(const size_t fila_actual, const size_t col_actual, 
                          const size_t fila_vecina, const size_t col_vecina) const {
  // Verificar que el movimiento es válido
  if (!EsMovimientoValido(fila_actual, col_actual, fila_vecina, col_vecina)) {
    return std::numeric_limits<double>::infinity(); // Costo infinito para movimientos inválidos
  }
  // Costo base según el tipo de movimiento
  if (EsMovimientoDiagonal(fila_actual, col_actual, fila_vecina, col_vecina)) {
    return 7.0; // Costo diagonal ≈ 1.414
  } else if (EsMovimientoHorizontalOVertical(fila_actual, col_actual, fila_vecina, col_vecina)) {
    return 5.0; // Costo horizontal/vertical
  }
  // En caso de algún movimiento no contemplado
  return std::numeric_limits<double>::infinity();
}

/**
  * @brief Calcula la heurística de Manhattan ponderada hacia la salida
  * @param fila_actual Fila de la casilla actual
  * @param col_actual Columna de la casilla actual
  * @return Distancia heurística multiplicada por peso W=3.0
  * Utiliza la distancia de Manhattan:
  * h(n) = W × (|x_goal - x_actual| + |y_goal - y_actual|)
  * @note Esta heurística es admisible ya que W × Manhattan ≤ coste_real
  */
double Laberinto::HeuristicaManhattan(const size_t fila_actual, const size_t col_actual) const {
  constexpr double W = 3.0;
  int diff_fila = std::abs(static_cast<int>(coordenadas_exit_.first) - static_cast<int>(fila_actual));
  int diff_col = std::abs(static_cast<int>(coordenadas_exit_.second) - static_cast<int>(col_actual));
  return (diff_fila + diff_col) * W;
}


double Laberinto::HeuristicaOctile(const size_t fila_actual, const size_t col_actual) const {
  int diff_fila = std::abs(static_cast<int>(coordenadas_exit_.first) - static_cast<int>(fila_actual));
  int diff_col = std::abs(static_cast<int>(coordenadas_exit_.second) - static_cast<int>(col_actual));
  int max_x = std::max(diff_col, diff_fila);
  int min_y = std::min(diff_col, diff_fila);
  constexpr double D{5.0};
  constexpr double D2{7.0};
  return D2 * static_cast<double>(min_y) + D * (static_cast<double>(max_x) - static_cast<double>(min_y));
}

/**
 * @brief Calcula el coste total de un camino dado
 * @param camino Vector con las coordenadas del camino (secuencia de posiciones)
 * @return Coste total del camino
 * 
 * Calcula la suma de los costes de movimiento entre cada par de posiciones
 * consecutivas en el camino. No verifica si las casillas son transitables,
 * solo calcula el coste geométrico del movimiento.
 * 
 * @note Útil para calcular el coste real de un camino ya recorrido,
 *       incluso si el entorno cambió después.
 */
double Laberinto::CalcularCosteCamino(const std::vector<std::pair<size_t, size_t>>& camino) const {
   // Camino vacío o con solo una posición tiene coste 0
  if (camino.size() <= 1) {
    return 0.0;
  }
  
  double coste_total = 0.0;
  
  // Calcular coste entre cada par de posiciones consecutivas
  for (size_t i = 0; i < camino.size() - 1; ++i) {
    auto [fila_actual, col_actual] = camino[i];
    auto [fila_siguiente, col_siguiente] = camino[i + 1];
    
    // Calcular diferencias para determinar tipo de movimiento
    int diff_fila = std::abs(static_cast<int>(fila_siguiente) - 
                            static_cast<int>(fila_actual));
    int diff_col = std::abs(static_cast<int>(col_siguiente) - 
                           static_cast<int>(col_actual));
    
    // Verificar que el movimiento es adyacente (máximo 1 casilla)
    if (diff_fila > 1 || diff_col > 1) {
      // Movimiento no adyacente (salto inválido)
      return std::numeric_limits<double>::infinity();
    }
    
    // Calcular coste según tipo de movimiento
    if (diff_fila == 1 && diff_col == 1) {
      // Movimiento diagonal
      coste_total += 7.0;
    } else if ((diff_fila == 1 && diff_col == 0) || 
               (diff_fila == 0 && diff_col == 1)) {
      // Movimiento ortogonal (horizontal o vertical)
      coste_total += 5.0;
    }
    // Si diff_fila == 0 && diff_col == 0: misma posición, coste 0
  }
  
  return coste_total;
}

//============================================================================
// MÉTODOS PRIVADOS DE VALIDACIÓN
//============================================================================

  /**
   * @brief Verifica si unas coordenadas están dentro de los límites
   * @param fila Fila a verificar
   * @param columna Columna a verificar
   * @return true si está dentro de los límites, false en caso contrario
   */
bool Laberinto::EsCoordenadaValida(size_t fila, size_t columna) const noexcept {
  return (fila < filas_ && columna < columnas_);
}

/**
 * @brief Verifica si una posición es válida para ser salida
 * @param fila Fila a verificar
 * @param columna Columna a verificar
 * @return true si puede ser salida (está en borde y es válida)
 */
bool Laberinto::EsSalidaValida(size_t fila, size_t columna) const noexcept {
  if (!EsCoordenadaValida(fila, columna)) return false;
  if (!EsBorde(fila, columna)) return false;
  // if (matriz_casillas_[fila][columna].tipo() == Tipo_Casilla::Obstaculo) return false;
  return true;
}

/**
 * @brief Verifica si una posición es válida para ser entrada
 * @param fila Fila a verificar
 * @param columna Columna a verificar
 * @return true si puede ser entrada (está en borde y es válida)
 */
bool Laberinto::EsEntradaValida(size_t fila, size_t columna) const noexcept {
  if (!EsCoordenadaValida(fila, columna)) return false;
  if (!EsBorde(fila, columna)) return false;
  // if (matriz_casillas_[fila][columna].tipo() == Tipo_Casilla::Obstaculo) return false;
  return true;
}

/**
 * @brief Verifica si una coordenada está en el borde del laberinto
 * @param fila Fila a verificar
 * @param columna Columna a verificar
 * @return true si está en el borde, false en caso contrario
 */
bool Laberinto::EsBorde(size_t fila, size_t columna) const noexcept {
  // Primero verificar que la coordenada es válida
  if (!EsCoordenadaValida(fila, columna)) {
    return false;
  }
  // Está en el borde si está en la primera o última fila/columna
  return (fila == 0 || columna == 0 || fila == filas_ - 1 || columna == columnas_ - 1);
}

//============================================================================
// MÉTODOS PRIVADOS DE MOVIMIENTO
//============================================================================

/**
 * @brief Determina si un movimiento es ortogonal
 * @param fila_actual Fila origen
 * @param col_actual Columna origen
 * @param fila_vecina Fila destino
 * @param col_vecina Columna destino
 * @return true si el movimiento es horizontal o vertical 
 */
bool Laberinto::EsMovimientoHorizontalOVertical(size_t fila_actual, size_t col_actual,
                                               size_t fila_vecina, size_t col_vecina) const {
  int diff_fila = std::abs(static_cast<int>(fila_vecina) - static_cast<int>(fila_actual));
  int diff_col = std::abs(static_cast<int>(col_vecina) - static_cast<int>(col_actual));
  return (diff_fila == 1 && diff_col == 0) || (diff_fila == 0 && diff_col == 1);
}

/**
 * @brief Determina si un movimiento es diagonal
 * @param fila_actual Fila origen
 * @param col_actual Columna origen
 * @param fila_vecina Fila destino
 * @param col_vecina Columna destino
 * @return true si el movimiento es diagonal 
 */
bool Laberinto::EsMovimientoDiagonal(size_t fila_actual, size_t col_actual,
                                    size_t fila_vecina, size_t col_vecina) const {
  int diff_fila = std::abs(static_cast<int>(fila_vecina) - static_cast<int>(fila_actual));
  int diff_col = std::abs(static_cast<int>(col_vecina) - static_cast<int>(col_actual));
  return (diff_fila == 1 && diff_col == 1);
}

/**
 * @brief Verifica si un movimiento entre dos casillas es válido
 * @param fila_actual Fila origen
 * @param col_actual Columna origen
 * @param fila_vecina Fila destino
 * @param col_vecina Columna destino
 * @return true si el movimiento es legal, false en caso contrario
 */
bool Laberinto::EsMovimientoValido(size_t fila_actual, size_t col_actual, 
                                  size_t fila_vecina, size_t col_vecina) const {
  // Verificar que las coordenadas son válidas
  if (!EsCoordenadaValida(fila_vecina, col_vecina)) {
    return false;
  }
  
  // Verificar que no es la misma casilla
  if (fila_actual == fila_vecina && col_actual == col_vecina) {
    return false;
  }
  
  // Verificar que la casilla vecina no es un obstáculo
  if (matriz_casillas_[fila_vecina][col_vecina].tipo() == Tipo_Casilla::Obstaculo) {
    return false;
  }
  
  // Verificar que el movimiento es adyacente (incluyendo diagonales)
  int diff_fila = std::abs(static_cast<int>(fila_vecina) - static_cast<int>(fila_actual));
  int diff_col = std::abs(static_cast<int>(col_vecina) - static_cast<int>(col_actual));
  
  if (diff_fila > 1 || diff_col > 1) {
    return false; // Movimiento demasiado lejano
  }
  
  // Para movimientos diagonales, verificar que no hay obstáculos en las celdas adyacentes
  if (diff_fila == 1 && diff_col == 1) {
    // Verificar que ambas celdas adyacentes no son obstáculos
    bool obstaculo_horizontal = matriz_casillas_[fila_actual][col_vecina].tipo() == Tipo_Casilla::Obstaculo;
    bool obstaculo_vertical = matriz_casillas_[fila_vecina][col_actual].tipo() == Tipo_Casilla::Obstaculo;
    
    if (obstaculo_horizontal && obstaculo_vertical) {
      return false; // No se puede mover en diagonal si ambas adyacentes son obstáculos
    }
  }
  
  return true;
}

//============================================================================
// MÉTODOS PRIVADOS DE DINAMISMO
//============================================================================

/**
 * @brief Aplica mutaciones probabilísticas a las casillas
 * @param pin Probabilidad de que una casilla libre se convierta en obstáculo
 * @param pout Probabilidad de que un obstáculo se convierta en casilla libre
 * @param rd Generador de números aleatorios
 * @note Los valores típicos son pin=pout=0.5 para máxima aleatoriedad
 */
void Laberinto::MutarCeldas(const double pin, const double pout, std::mt19937& rd) {
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  for (size_t i{0}; i < filas_; ++i) {
    for (size_t j{0}; j < columnas_; ++j) {
      if (std::make_pair(i, j) == coordenadas_start_ || std::make_pair(i, j) == coordenadas_exit_) {
        continue;
      }
      double U = dist(rd);
      // std::cerr << U << std::endl; // DEBUG
      auto& casilla = matriz_casillas_[i][j];
      if (casilla.tipo() == Tipo_Casilla::Libre) {
        if (U >= (1.0 - pin)) {
          casilla.setTipoCasilla(Tipo_Casilla::Obstaculo);
        }
      } else if (casilla.tipo() == Tipo_Casilla::Obstaculo) {
        if (U >= (1.0 - pout)) {
          casilla.setTipoCasilla(Tipo_Casilla::Libre);
        }
      }
    }
  }
}


/**
 * @brief Garantiza que no más del max_ratio de casillas sean obstáculos
 * @param max_ratio Ratio máximo permitido de obstáculos (0.0-1.0)
 * @param rd Generador de números aleatorios
 * @note El valor típico es max_ratio=0.25 (25% máximo de obstáculos)
 */
void Laberinto::EnforceMaxBloqueo(const double max_ratio,std::mt19937& rd) {
  const size_t porcentaje_bloqueadas = ContarPorcentajeBloqueadas();
  if (porcentaje_bloqueadas < static_cast<size_t>(max_ratio * 100)) {
    return;
  }
  std::vector<std::pair<size_t, size_t>> casillas_bloqueadas = ObtenerCasillasBloqueadas();
  const size_t max_bloqueadas = static_cast<size_t>(max_ratio * filas_ * columnas_);
  size_t exceso = casillas_bloqueadas.size() - max_bloqueadas;
  std::shuffle(casillas_bloqueadas.begin(), casillas_bloqueadas.end(), rd);
  for (size_t i{0}; i < exceso; ++i) {
    auto [f, c] = casillas_bloqueadas[i];
    // std::cerr << f << ", " << c << std::endl; // DEBUG
    matriz_casillas_[f][c].setTipoCasilla(Tipo_Casilla::Libre);
  }
}

/**
 * @brief Obtiene una lista de todas las posiciones con obstáculos
 * @return Vector con las coordenadas de todas las casillas bloqueadas
 * @note No incluye entrada ni salida, solo obstáculos puros
 */
std::vector<std::pair<size_t, size_t>> Laberinto::ObtenerCasillasBloqueadas() const {
  std::vector<std::pair<size_t, size_t>> casillas_bloqueadas{};
  for (size_t i{0}; i < filas_; ++i) {
    for (size_t j{0}; j < columnas_; ++j) {
      if (matriz_casillas_[i][j].tipo() == Tipo_Casilla::Obstaculo) {
        casillas_bloqueadas.emplace_back(i, j);
      }
    }
  }
  return casillas_bloqueadas;
}

/**
 * @brief Cuenta el número total de obstáculos en el laberinto
 * @return Cantidad de casillas de tipo Obstáculo
 * @note No cuenta entrada ni salida, solo obstáculos puros
 */
size_t Laberinto::ContarObstaculos() const noexcept {
  size_t casillas_obstaculo{};
  for (size_t i{0}; i < filas_; ++i) {
    for (size_t j{0}; j < columnas_; ++j) {
      const auto& casilla = matriz_casillas_[i][j];
      if (casilla.tipo() == Tipo_Casilla::Obstaculo) {
        ++casillas_obstaculo;
      }
    }
  }
  return casillas_obstaculo;
}

  /**
   * @brief Calcula el porcentaje de casillas bloqueadas
   * @return Porcentaje entero (0-100) de casillas que son obstáculos
   */
size_t Laberinto::ContarPorcentajeBloqueadas() const noexcept {
  size_t casillas_obstaculo = ContarObstaculos();
  size_t total_casillas{filas_ * columnas_};
  double porcentaje{(static_cast<double>(casillas_obstaculo) / static_cast<double>(total_casillas)) * 100.0};
  return static_cast<size_t>(porcentaje);
}

//============================================================================
// OPERADORES SOBRECARGADOS
//============================================================================

/**
 * @brief Operador de salida para imprimir el laberinto
 * @param os Stream de salida
 * @param laberinto Laberinto a imprimir
 * @return Referencia al stream de salida
 */
std::ostream& operator<<(std::ostream& os, const Laberinto& laberinto) {
  os << "Coordenadas entrada: " << laberinto.coordenadas_start_.first << "," << laberinto.coordenadas_start_.second << std::endl;
  os << "Coordenadas salida: " << laberinto.coordenadas_exit_.first << "," << laberinto.coordenadas_exit_.second << std::endl;
  for (size_t i{0}; i < laberinto.filas_; ++i) {
    for (size_t j{0}; j < laberinto.columnas_; ++j) {
      os << laberinto.matriz_casillas_[i][j].ImprimirCasilla();
      if (j + 1 < laberinto.columnas_) os << ' ';
    }
    os << std::endl;
  }
  os << "Total casillas ocupadas: " << laberinto.ContarObstaculos(); 
  os << std::endl;
  os << "Porcentaje casillas ocupadas: " << laberinto.ContarPorcentajeBloqueadas(); 
  os << std::endl;
  return os;
}
