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
