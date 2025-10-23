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
  * @brief Definición de la clase Laberinto para representar entornos dinámicos
  * @version 1.0
  * 
  * Este archivo contiene la definición de la clase Laberinto, que representa
  * un entorno de navegación dinámico compuesto por una matriz de casillas.
  * El laberinto puede contener obstáculos que cambian con el tiempo según
  * probabilidades de mutación.
  */

#ifndef LABERINTO_H
#define LABERINTO_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <random>
#include "casilla.h"

/**
 * @class Laberinto
 * @brief Representa un laberinto dinámico con obstáculos mutables
 * 
 * Esta clase modela un entorno de navegación bidimensional donde:
 * - Los obstáculos pueden aparecer/desaparecer dinámicamente
 * - Existe un punto de entrada (Start) y uno de salida (Exit)
 * - Se puede calcular la heurística y coste de movimiento entre casillas
 * - Se garantiza un máximo del 25% de casillas bloqueadas
 * 
 * @note El laberinto utiliza 8-conectividad para el movimiento
 * @see Casilla
 */
class Laberinto {
 public:
  // Constructores
  Laberinto() = default;
  Laberinto(const std::string& laberinto) { ProcesarLaberinto(laberinto); }
  // Metodos de utilidad
  void ProcesarLaberinto(const std::string&);
  void CambiarEntradaYSalida(const std::pair<size_t, size_t>&, const std::pair<size_t, size_t>& );
  void ActualizarDinamismo();
  // Metodos de utilidad para A*
  std::vector<std::pair<size_t, size_t>> GetVecinosCasilla(const size_t, const size_t) const;
  double MoveCost(const size_t, const size_t, const size_t, const size_t) const;
  double HeuristicaManhattan(const size_t, const size_t) const;
  double HeuristicaOctile(const size_t, const size_t) const;
  double CalcularCosteCamino(const std::vector<std::pair<size_t, size_t>>& camino) const;
  // Google Style getters
  int filas() const noexcept { return filas_; }
  int columnas() const noexcept {return columnas_; }
  const Casilla& getCasilla(const size_t fila, const size_t columna) const { return matriz_casillas_[fila][columna]; }
  std::pair<size_t, size_t> coordenadas_start() const noexcept {   return coordenadas_start_; }
  std::pair<size_t, size_t> coordenadas_exit() const noexcept {  return coordenadas_exit_; }
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
  bool EsSalidaValida(size_t fila, size_t columna) const noexcept; 
  bool EsEntradaValida(size_t fila, size_t columna) const noexcept; 
  bool EsBorde(size_t, size_t) const noexcept;
  // Metodos Auxiliares privados para dinamico
  void MutarCeldas(const double, const double, std::mt19937&);
  void EnforceMaxBloqueo(const double, std::mt19937&);
  std::vector<std::pair<size_t, size_t>> ObtenerCasillasBloqueadas() const;
  size_t ContarPorcentajeBloqueadas() const noexcept;
  size_t ContarObstaculos() const noexcept;
  // Métodos auxiliares para movimiento y vecinos
  bool EsMovimientoValido(size_t fila_actual, size_t col_actual, 
                         size_t fila_vecina, size_t col_vecina) const;
  bool EsMovimientoDiagonal(size_t fila_actual, size_t col_actual,
                           size_t fila_vecina, size_t col_vecina) const;
  bool EsMovimientoHorizontalOVertical(size_t fila_actual, size_t col_actual,
                                      size_t fila_vecina, size_t col_vecina) const;
};

#endif
