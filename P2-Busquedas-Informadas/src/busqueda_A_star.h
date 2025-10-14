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

#ifndef BUSQUEDA_A_STAR_H
#define BUSQUEDA_A_STAR_H

#include <iostream>
#include <fstream>
#include <set>
#include <queue>
#include "laberinto.h"

/**
 * 
 * 
 */
class BusquedaInformada {
 public:
  BusquedaInformada() = default;
  BusquedaInformada(const Laberinto& laberinto) : laberinto_{laberinto} {}
  //Metodo de busqueda A*
  void BusquedaAStar() const;
  
 private:
  Laberinto laberinto_{};
};

#endif