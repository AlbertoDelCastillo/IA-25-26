/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Inteligencia Artificial 2025-2026
  * 3º Año de Carrera
  * Practica 2: Busquedas informadas
  *
  * @author Alberto Del Castillo Díaz alu0101627137@ull.edu.es
  * @date 
  * @brief 
  * @version 1.0
  */

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include "casilla.h"
#include "laberinto.h"

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

void Laberinto::ImprimirLaberinto() const {
  std::cout << "Coordenadas entrada: " << coordenadas_start_.first << "," << coordenadas_start_.second << std::endl;
  std::cout << "Coordenadas salida: " << coordenadas_exit_.first << "," << coordenadas_exit_.second << std::endl;
  for (size_t i{0}; i < filas_; ++i) {
    for (size_t j{0}; j < columnas_; ++j) {
      std::cout << matriz_casillas_[i][j].ImprimirCasilla();
      if (j + 1 < columnas_) std::cout << ' ';
    }
    std::cout << std::endl;
  }
}

void Laberinto::CambiarStart(const size_t fila, const size_t columna) {
  if (!EsEntradaValida(fila, columna)) {
    throw std::invalid_argument("Posición de entrada no válida.");
  }
  // Restaurar la antigua casilla start al tipo anterior 
  Tipo_Casilla old_tipo = matriz_casillas_[fila][columna].tipo();
  matriz_casillas_[coordenadas_start_.first][coordenadas_start_.second].setTipoCasilla(old_tipo);
  // Marcar la nueva como Start
  matriz_casillas_[fila][columna].setTipoCasilla(Tipo_Casilla::Start);
  coordenadas_start_ = {fila, columna};
}

void Laberinto::CambiarExit(const size_t fila, const size_t columna) {
  if (!EsSalidaValida(fila, columna)) {
    throw std::invalid_argument("Posición de salida no válida.");
  }
  // Restaurar la antigua casilla exit al tipo anterior (libre)
  Tipo_Casilla old_tipo = matriz_casillas_[fila][columna].tipo();
  matriz_casillas_[coordenadas_exit_.first][coordenadas_exit_.second].setTipoCasilla(old_tipo);
  // Marcar la nueva como Exit
  matriz_casillas_[fila][columna].setTipoCasilla(Tipo_Casilla::Exit);
  coordenadas_exit_ = {fila, columna};
}

bool Laberinto::EsCoordenadaValida(size_t fila, size_t columna) const noexcept {
  return (fila < filas_ && columna < columnas_);
}

bool Laberinto::EsBorde(size_t fila, size_t columna) const noexcept {
  if (!EsCoordenadaValida(fila, columna)) {
  return false;
  }
  return (fila == 0 || columna == 0 || fila == filas_ - 1 || columna == columnas_ - 1);
}

bool Laberinto::EsSalidaValida(size_t fila, size_t columna) const noexcept {
  if (!EsCoordenadaValida(fila, columna)) return false;
  if (!EsBorde(fila, columna)) return false;
  // if (matriz_casillas_[fila][columna].tipo() == Tipo_Casilla::Obstaculo) return false;
  return true;
}

bool Laberinto::EsEntradaValida(size_t fila, size_t columna) const noexcept {
  if (!EsCoordenadaValida(fila, columna)) return false;
  if (!EsBorde(fila, columna)) return false;
  // if (matriz_casillas_[fila][columna].tipo() == Tipo_Casilla::Obstaculo) return false;
  return true;
}

bool Laberinto::EsMovimientoDiagonal(size_t fila_actual, size_t col_actual,
                                    size_t fila_vecina, size_t col_vecina) const {
  int diff_fila = std::abs(static_cast<int>(fila_vecina) - static_cast<int>(fila_actual));
  int diff_col = std::abs(static_cast<int>(col_vecina) - static_cast<int>(col_actual));
  return (diff_fila == 1 && diff_col == 1);
}

bool Laberinto::EsMovimientoHorizontalOVertical(size_t fila_actual, size_t col_actual,
                                               size_t fila_vecina, size_t col_vecina) const {
  int diff_fila = std::abs(static_cast<int>(fila_vecina) - static_cast<int>(fila_actual));
  int diff_col = std::abs(static_cast<int>(col_vecina) - static_cast<int>(col_actual));
  return (diff_fila == 1 && diff_col == 0) || (diff_fila == 0 && diff_col == 1);
}

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

void Laberinto::ActualizarDinamismo() {
  std::random_device rd;
  std::mt19937 gen(rd());
  double pin{0.5}, pout{0.5};
  MutarCeldas(pin, pout, gen);
  EnforceMaxBloqueo(0.25, gen);
}

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


size_t Laberinto::ContarPorcentajeBloqueadas() const noexcept {
  size_t casillas_obstaculo = ContarObstaculos();
  size_t total_casillas{filas_ * columnas_};
  double porcentaje{(static_cast<double>(casillas_obstaculo) / static_cast<double>(total_casillas)) * 100.0};
  return static_cast<size_t>(porcentaje);
}

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

double Laberinto::Heuristica(const size_t fila_actual, const size_t col_actual) const {
  constexpr double W = 3.0;
  int diff_fila = std::abs(static_cast<int>(coordenadas_exit_.first) - static_cast<int>(fila_actual));
  int diff_col = std::abs(static_cast<int>(coordenadas_exit_.second) - static_cast<int>(col_actual));
  return (diff_fila + diff_col) * W;
}
