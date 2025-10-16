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
  * @brief Declaracion de funciones de informacion y comprobacion en el programa cliente
  * @version 1.0
  */
 
#ifndef FUNCIONES_CLIENTE_H
#define FUNCIONES_CLIENTE_H

#include "laberinto.h"

bool CheckCorrectsParameters(const int, char**);
void PrintProgramPorpouse(void);
void Usage(void);
void Menu(Laberinto&, const std::string&);

#endif