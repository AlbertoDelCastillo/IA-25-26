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

#include "casilla.h"
#include <stdexcept>

Tipo_Casilla FromInt(const int valor){
  switch (valor) {
    case 0: 
      return Tipo_Casilla::Libre;
    case 1: 
      return Tipo_Casilla::Obstaculo;
    case 3: 
      return Tipo_Casilla::Start;
    case 4: 
      return Tipo_Casilla::Exit;
    default:
      throw std::runtime_error("Valor de tipo de casilla no válido: " + std::to_string(valor));
  }
}

char Casilla::ImprimirCasilla() const noexcept {
  switch (tipo_) {
    case Tipo_Casilla::Libre:
      return '0';
    case Tipo_Casilla::Obstaculo:
      return '1';
    case Tipo_Casilla::Start:
      return '3';
    case Tipo_Casilla::Exit:
      return '4';
    default:
      return '?';
  }
  return '?';
}

bool Casilla::EsTransitable() const noexcept {
  return tipo_ != Tipo_Casilla::Obstaculo;
}
