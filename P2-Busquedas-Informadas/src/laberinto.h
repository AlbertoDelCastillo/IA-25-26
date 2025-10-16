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

#ifndef LABERINTO_H
#define LABERINTO_H

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <fstream>
#include <utility>
#include <random>
#include "casilla.h"

/**
 * 
 * 
 */
class Laberinto {
 public:
  // Constructores
  Laberinto() = default;
  Laberinto(const std::string& laberinto) { ProcesarLaberinto(laberinto); }
  // Metodos de utilidad
  void ProcesarLaberinto(const std::string&);
  void ImprimirLaberinto() const;
  void CambiarStart(const size_t, const size_t);
  void CambiarExit(const size_t, const size_t);
  void ActualizarDinamismo();
  // Metodos de utilidad para A*
  // std::vector<std::pair<size_t, size_t>> GetVecinosCasilla(const size_t, const size_t) const;
  // size_t MoveCost(const size_t, const size_t) const;
  // Google Style getters
  int filas() const noexcept { return filas_; }
  int columnas() const noexcept {return columnas_; }
  // Sobrecarga operadores
  friend std::ostream& operator<<(std::ostream&, const Laberinto&);
 private:
  size_t filas_{};
  size_t columnas_{};
  std::vector<std::vector<Casilla>> matriz_casillas_{};
  std::pair<size_t, size_t> coordenadas_start_{};
  std::pair<size_t, size_t> coordenadas_exit_{};
  // Metodo Auxiliares privados
  bool EsCoordenadaValida(size_t fila, size_t columna) const noexcept;
  bool EsBorde(size_t fila, size_t columna) const noexcept;
  bool EsSalidaValida(size_t fila, size_t columna) const noexcept; 
  bool EsEntradaValida(size_t fila, size_t columna) const noexcept; 
  // Metodos Auxiliares privados para dinamico
  void MutarCeldas(const double, const double, std::mt19937&);
  void EnforceMaxBloqueo(const double, std::mt19937&);
  std::vector<std::pair<size_t, size_t>> ObtenerCasillasBloqueadas() const;
  size_t ContarPorcentajeBloqueadas() const noexcept;
  size_t ContarObstaculos() const noexcept;
};

#endif
