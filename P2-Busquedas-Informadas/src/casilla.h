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


#ifndef CASILLA_H
#define CASILLA_H

#include <iostream>

enum class Tipo_Casilla : int {
  Libre = 0,
  Obstaculo = 1,
  Start = 3,
  Exit = 4
};

Tipo_Casilla FromInt(const int);

/**
 * 
 * 
 */
class Casilla {
 public:
  // Constructores
  Casilla() = default;
  Casilla(const Tipo_Casilla& tipo) : tipo_{tipo} {}
  // Metodos auxiliares
  char ImprimirCasilla() const noexcept;
  bool EsTransitable() const noexcept;
  // Metodos de utilidad para A*
  // Google style getters
  Tipo_Casilla tipo() const noexcept { return tipo_; }
  // Setters
  void setTipoCasilla(const Tipo_Casilla& tipo) { tipo_ = tipo; }
 private:
  Tipo_Casilla tipo_{};
};

#endif