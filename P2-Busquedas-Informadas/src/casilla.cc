/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Inteligencia Artificial 2025-2026
  * 3º Año de Carrera
  * Practica 2: Busquedas informadas
  *
  * @author Alberto Del Castillo Díaz alu0101627137@ull.edu.es
  * @date Oct 11 2025
  * @brief Implementación de los métodos de la clase Casilla
  */

#include "casilla.h"
#include <stdexcept>

/**
 * @brief Convierte un entero en su tipo de casilla correspondiente
 * @param valor Entero que representa el tipo de casilla
 * @return Tipo de casilla correspondiente
 * @throws std::runtime_error Si el valor no corresponde a ningún tipo válido
 */
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

/**
  * @brief Obtiene el carácter que representa visualmente la casilla
  * @return Carácter representativo (' ' para libre, '#' para obstáculo, etc.)
  */
char Casilla::ImprimirCasilla() const noexcept {
  switch (tipo_) {
    case Tipo_Casilla::Libre:
      return ' ';
    case Tipo_Casilla::Obstaculo:
      return '#';
    case Tipo_Casilla::Start:
      return '3';
    case Tipo_Casilla::Exit:
      return '4';
    default:
      return '?';
  }
  return '?';
}

/**
  * @brief Verifica si la casilla es transitable
  * @return true si la casilla no es un obstáculo, false en caso contrario
  */
bool Casilla::EsTransitable() const noexcept {
  return tipo_ != Tipo_Casilla::Obstaculo;
}
