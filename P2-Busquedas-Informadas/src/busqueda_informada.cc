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

#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include "busqueda_informada.h"

bool BusquedaInformada::BusquedaAStar(const std::pair<size_t, size_t>& inicio) {
  // Reset de estadísticas
  nodos_generados_ = 0;
  nodos_inspeccionados_ = 0;
  camino_encontrado_.clear();
  
  InicializarMatrizCostes();
  std::vector<nodo> A{};
  std::set<std::pair<size_t, size_t>> C{};

  // USAR el parámetro 'inicio' en lugar de laberinto_->coordenadas_start()
  std::pair<size_t, size_t> pos_actual = inicio;
  double g_cost = 0; 
  double h_cost = laberinto_->Heuristica(pos_actual.first, pos_actual.second);
  
  nodo S(pos_actual, g_cost, h_cost, {-1, -1});
  std::cerr << "Nodo inicial: " << S;
  matriz_costes_[pos_actual.first][pos_actual.second] = S;
  A.push_back(S);
  ++nodos_generados_;

  // Paso 2: Repetir mientras A no esté vacía
  while (!A.empty()) {
    // Paso 2(a): Seleccionar el nodo de menor coste f(n)
    auto it_mejor = A.begin();
    for (auto it = A.begin(); it != A.end(); ++it) {
      if (it->f_cost < it_mejor->f_cost) {
        it_mejor = it;
      }
    }
    nodo actual = *it_mejor;
    A.erase(it_mejor);
    
    // Insertarlo en la lista de nodos cerrados C
    C.insert(actual.posicion);
    ++nodos_inspeccionados_;
    std::cerr << "Procesando nodo: " << actual;
    
    // Verificar si llegamos a la salida
    if (actual.posicion == laberinto_->coordenadas_exit()) {
      std::cout << "¡Camino encontrado!" << std::endl;
      camino_encontrado_ = ReconstruirCamino(actual.posicion);
      return true;
    }
    
    // Paso 2(b): Para cada nodo vecino
    std::vector<std::pair<size_t, size_t>> vecinos = 
        laberinto_->GetVecinosCasilla(actual.posicion.first, actual.posicion.second);
    
    for (const auto& vecino_pos : vecinos) {
      size_t vecino_fila = vecino_pos.first;
      size_t vecino_col = vecino_pos.second;
      
      // Calcular costes para el vecino
      double move_cost = laberinto_->MoveCost(actual.posicion.first, actual.posicion.second, 
                                             vecino_fila, vecino_col);
      double g_cost_vecino = actual.g_cost + move_cost;
      double h_cost_vecino = laberinto_->Heuristica(vecino_fila, vecino_col);
      
      nodo vecino(vecino_pos, g_cost_vecino, h_cost_vecino, 
                 {static_cast<int>(actual.posicion.first), static_cast<int>(actual.posicion.second)});
      
      // Verificar si el nodo está en C (cerrados)
      bool en_C = (C.find(vecino_pos) != C.end());

      // Verificar si el nodo está en A (abiertos)
      bool en_A = false;
      auto it_en_A = A.end();
      for (auto it = A.begin(); it != A.end(); ++it) {
        if (it->posicion == vecino_pos) {
          en_A = true;
          it_en_A = it;
          break;
        }
      }
      
      // Paso 2(b)i: Si el nodo no está ni en A ni en C
      if (!en_A && !en_C) {
        vecino.padre = {static_cast<int>(actual.posicion.first), static_cast<int>(actual.posicion.second)};
        matriz_costes_[vecino_fila][vecino_col] = vecino;
        A.push_back(vecino);
        ++nodos_generados_;
        std::cerr << "  Nuevo nodo añadido a A: " << vecino;
      }
      // Paso 2(b)ii: Si el nodo está en A
      else if (en_A) {
        // Verificar si encontramos un camino mejor (menor g_cost)
        if (g_cost_vecino < it_en_A->g_cost) {
          std::cerr << "  Actualizando nodo en A: g_cost mejorado de " 
                    << it_en_A->g_cost << " a " << g_cost_vecino << std::endl;
          
          // Actualizar coste g(n) y por lo tanto f(n)
          it_en_A->g_cost = g_cost_vecino;
          it_en_A->f_cost = g_cost_vecino + it_en_A->h_cost;
          it_en_A->padre = {static_cast<int>(actual.posicion.first), static_cast<int>(actual.posicion.second)};
          
          // Actualizar también en la matriz de costes
          matriz_costes_[vecino_fila][vecino_col] = *it_en_A;
        }
      }
      // Si el nodo está en C, no hacemos nada (ya fue procesado)
    }
  }
  
  // Paso 3: Si A está vacía y no se llegó a la salida
  std::cout << "No existe camino desde la entrada hasta la salida." << std::endl;
  return false;
}

void BusquedaInformada::InicializarMatrizCostes() {
  size_t filas = laberinto_->filas();
  size_t cols = laberinto_->columnas();
  
  matriz_costes_.clear();
  matriz_costes_.resize(filas, std::vector<nodo>(cols));
}

// Implementa en busqueda_informada.cc
void BusquedaInformada::ImprimirCamino(std::ostream& os) const {
  if (camino_encontrado_.empty()) {
    os << "No se encontró camino.\n";
    return;
  }
  
  os << "Camino encontrado (" << camino_encontrado_.size() << " pasos):\n";
  for (size_t i = 0; i < camino_encontrado_.size(); ++i) {
    auto [fila, col] = camino_encontrado_[i];
    os << "(" << fila << ", " << col << ")";
    if (i + 1 < camino_encontrado_.size()) os << " -> ";
  }
  os << "\n";
}

void BusquedaInformada::ImprimirEstadisticas(std::ostream& os) const {
  os << "\n=== Estadísticas A* ===\n";
  os << "Nodos generados: " << nodos_generados_ << "\n";
  os << "Nodos inspeccionados: " << nodos_inspeccionados_ << "\n";
  os << "Camino encontrado: " << (camino_encontrado_.empty() ? "NO" : "SÍ") << "\n";
  if (!camino_encontrado_.empty()) {
    os << "Longitud del camino: " << camino_encontrado_.size() << " pasos\n";
    auto [exit_f, exit_c] = laberinto_->coordenadas_exit();
    os << "Coste total: " << matriz_costes_[exit_f][exit_c].g_cost << "\n";
  }
}

std::vector<std::pair<size_t, size_t>> BusquedaInformada::ReconstruirCamino(
    const std::pair<size_t, size_t>& meta) {
  
  std::vector<std::pair<size_t, size_t>> camino;
  std::pair<int, int> actual = {static_cast<int>(meta.first), 
                                static_cast<int>(meta.second)};
  
  // Seguir los padres desde meta hasta inicio
  while (actual != std::make_pair(-1, -1)) {
    camino.push_back({static_cast<size_t>(actual.first), 
                      static_cast<size_t>(actual.second)});
    
    size_t fila = static_cast<size_t>(actual.first);
    size_t col = static_cast<size_t>(actual.second);
    actual = matriz_costes_[fila][col].padre;
  }
  
  // Invertir (está desde meta a inicio)
  std::reverse(camino.begin(), camino.end());
  return camino;
}

void BusquedaInformada::ImprimirLaberintoConCamino(std::ostream& os) const {
    if (!laberinto_) return;
    
    // Crear un conjunto con las posiciones del camino para verificación rápida
    std::set<std::pair<size_t, size_t>> camino_set;
    for (const auto& pos : camino_encontrado_) {
        camino_set.insert(pos);
    }
    
    auto start = laberinto_->coordenadas_start();
    auto exit = laberinto_->coordenadas_exit();
    
    for (size_t i = 0; i < static_cast<size_t>(laberinto_->filas()); ++i) {
        for (size_t j = 0; j < static_cast<size_t>(laberinto_->columnas()); ++j) {
            std::pair<size_t, size_t> pos_actual(i, j);
            
            // Verificar si es parte del camino
            if (camino_set.find(pos_actual) != camino_set.end()) {
                if (pos_actual == start) {
                    os << "S";  // Start
                } else if (pos_actual == exit) {
                    os << "E";  // Exit
                } else {
                    os << "*";  // Camino
                }
            } else {
                // Imprimir el laberinto normal
                char simbolo = laberinto_->getCasilla(i, j).ImprimirCasilla();
                os << simbolo;
            }
            
            if (j + 1 < static_cast<size_t>(laberinto_->columnas())) os << ' ';
        }
        os << "\n";
    }
}

void BusquedaInformada::GenerarReporteCompleto(const std::string& nombre_instancia,
                                              const std::string& nombre_heuristica,
                                              std::ostream& os) const {
    if (!laberinto_) return;
    
    // 1. Tabla de resultados
    os << "Búsqueda A*. Función heurística " << nombre_heuristica << "\n";
    os << "| Instancia | n | m | S | E | Camino | Coste | Número de nodos generados | Número de nodos inspeccionados |\n";
    os << "|-----------|---|---|---|----|--------|-------|--------------------------|--------------------------------|\n";
    
    auto start = laberinto_->coordenadas_start();
    auto exit = laberinto_->coordenadas_exit();
    double coste_total = camino_encontrado_.empty() ? 0 : 
                        matriz_costes_[exit.first][exit.second].g_cost;
    
    os << "| " << nombre_instancia 
       << " | " << laberinto_->filas()
       << " | " << laberinto_->columnas()
       << " | (" << start.first << "," << start.second << ")"
       << " | (" << exit.first << "," << exit.second << ")"
       << " | " << (camino_encontrado_.empty() ? "NO" : "SI")
       << " | " << coste_total
       << " | " << nodos_generados_
       << " | " << nodos_inspeccionados_ << " |\n\n";
    
    // 2. Laberinto con camino marcado
    os << "Laberinto con camino solución:\n";
    ImprimirLaberintoConCamino(os);
    os << "\n";
    
    // 3. Estadísticas detalladas
    ImprimirEstadisticas(os);
    os << "\n";
    
    // 4. Detalles del camino encontrado
    if (!camino_encontrado_.empty()) {
        os << "Detalle del camino:\n";
        ImprimirCamino(os);
    }
}

// ========== busqueda_informada.cc (AÑADIR/REEMPLAZAR) ==========

// ========== busqueda_informada.cc (MODIFICAR) ==========



bool BusquedaInformada::BusquedaAStarDinamica() {
  if (!laberinto_) {
    std::cerr << "Error: Laberinto no configurado.\n";
    return false;
  }

  std::ofstream archivo_salida("salida_dinamica.txt");
  if (!archivo_salida.is_open()) {
    std::cerr << "Error: No se pudo crear salida_dinamica.txt\n";
    return false;
  }

  archivo_salida << "========================================\n";
  archivo_salida << "BÚSQUEDA A* EN ENTORNO DINÁMICO\n";
  archivo_salida << "========================================\n\n";

  auto start_pos = laberinto_->coordenadas_start();
  auto exit_pos = laberinto_->coordenadas_exit();
  std::pair<size_t, size_t> posicion_actual = start_pos;

  size_t iteracion = 0;
  size_t pasos_totales = 0;
  size_t nodos_generados_acumulados = 0;
  size_t nodos_inspeccionados_acumulados = 0;
  size_t reintentos_sin_exito_consecutivos = 0;
  const size_t MAX_REINTENTOS = 5;

  std::vector<std::pair<size_t, size_t>> camino_real_seguido;
  camino_real_seguido.push_back(posicion_actual);

  std::cout << "\n=== INICIANDO BÚSQUEDA A* DINÁMICA ===\n";
  std::cout << "Inicio: (" << posicion_actual.first << "," << posicion_actual.second << ")\n";
  std::cout << "Meta: (" << exit_pos.first << "," << exit_pos.second << ")\n\n";

  while (posicion_actual != exit_pos) {
    ++iteracion;

    archivo_salida << "----------------------------------------\n";
    archivo_salida << "ITERACIÓN " << iteracion << "\n";
    archivo_salida << "----------------------------------------\n";
    archivo_salida << "Posición actual del agente: (" 
                   << posicion_actual.first << "," << posicion_actual.second << ")\n\n";

    std::cout << "Iteración " << iteracion << ": Ejecutando A* desde (" 
              << posicion_actual.first << "," << posicion_actual.second << ")...\n";

    bool camino_encontrado = BusquedaAStar(posicion_actual);

    if (camino_encontrado) {
      reintentos_sin_exito_consecutivos = 0;

      // CAMBIO AQUÍ: Usar el nuevo método con agente y camino histórico
      archivo_salida << "Laberinto con camino planificado:\n";
      archivo_salida << "Símbolos: @ = Agente | * = Camino planificado | · = Pasos dados | E = Meta\n\n";
      ImprimirLaberintoConCaminoYAgente(archivo_salida, posicion_actual, camino_real_seguido);
      archivo_salida << "\n";

      nodos_generados_acumulados += nodos_generados_;
      nodos_inspeccionados_acumulados += nodos_inspeccionados_;

      archivo_salida << "Camino planificado (longitud " << camino_encontrado_.size() << "):\n";
      for (const auto& [f, c] : camino_encontrado_) {
        archivo_salida << "(" << f << "," << c << ") ";
      }
      archivo_salida << "\n\n";

      archivo_salida << "Estadísticas de esta planificación:\n";
      archivo_salida << "  - Nodos generados: " << nodos_generados_ << "\n";
      archivo_salida << "  - Nodos inspeccionados: " << nodos_inspeccionados_ << "\n\n";

      if (camino_encontrado_.size() >= 2) {
        posicion_actual = camino_encontrado_[1];
        camino_real_seguido.push_back(posicion_actual);
        ++pasos_totales;

        archivo_salida << "Agente avanza a: (" 
                       << posicion_actual.first << "," << posicion_actual.second << ")\n\n";
        
        std::cout << "  → Avanzó a (" << posicion_actual.first << "," 
                  << posicion_actual.second << ")\n";

        if (posicion_actual == exit_pos) {
          archivo_salida << "*** ¡AGENTE LLEGÓ A LA META! ***\n\n";
          std::cout << "\n¡ÉXITO! El agente llegó a la meta.\n";
          break;
        }

        archivo_salida << "Actualizando entorno dinámico...\n\n";
        laberinto_->ActualizarDinamismo();
        
      } else {
        archivo_salida << "*** ¡AGENTE YA ESTÁ EN LA META! ***\n\n";
        break;
      }

    } else {
      archivo_salida << "*** NO SE ENCONTRÓ CAMINO EN ESTA ITERACIÓN ***\n";
      archivo_salida << "Reintento " << (reintentos_sin_exito_consecutivos + 1) 
                     << " de " << MAX_REINTENTOS << "\n\n";
      
      std::cout << "  ⚠ No hay camino. Reintento " << (reintentos_sin_exito_consecutivos + 1) 
                << "/" << MAX_REINTENTOS << "\n";

      ++reintentos_sin_exito_consecutivos;

      if (reintentos_sin_exito_consecutivos >= MAX_REINTENTOS) {
        archivo_salida << "*** MÁXIMO DE REINTENTOS ALCANZADO ***\n";
        archivo_salida << "*** NO ES POSIBLE LLEGAR AL DESTINO ***\n\n";
        
        std::cout << "\n❌ FALLO: No se pudo llegar al destino tras " 
                  << MAX_REINTENTOS << " reintentos.\n";
        
        ImprimirResumenFinal(archivo_salida, iteracion, pasos_totales, 
                           nodos_generados_acumulados, nodos_inspeccionados_acumulados,
                           camino_real_seguido, false);
        archivo_salida.close();
        return false;
      }

      archivo_salida << "Actualizando entorno para reintentar...\n\n";
      laberinto_->ActualizarDinamismo();
    }
  }

  ImprimirResumenFinal(archivo_salida, iteracion, pasos_totales, 
                      nodos_generados_acumulados, nodos_inspeccionados_acumulados,
                      camino_real_seguido, true);

  archivo_salida.close();
  std::cout << "\n✅ Resultados guardados en: salida_dinamica.txt\n";
  return true;
}


void BusquedaInformada::ImprimirResumenFinal(std::ostream& os, 
                                             size_t iteraciones,
                                             size_t pasos,
                                             size_t nodos_gen,
                                             size_t nodos_insp,
                                             const std::vector<std::pair<size_t, size_t>>& camino,
                                             bool exito) const {
  os << "========================================\n";
  os << "RESUMEN FINAL\n";
  os << "========================================\n\n";

  os << "Estado final: " << (exito ? "ÉXITO ✓" : "FALLO ✗") << "\n\n";

  os << "Métricas globales:\n";
  os << "  - Total de iteraciones: " << iteraciones << "\n";
  os << "  - Pasos realizados por el agente: " << pasos << "\n";
  os << "  - Total nodos generados (acumulado): " << nodos_gen << "\n";
  os << "  - Total nodos inspeccionados (acumulado): " << nodos_insp << "\n\n";

  if (exito && !camino.empty()) {
    os << "Camino real seguido por el agente (" << camino.size() << " posiciones):\n";
    for (size_t i = 0; i < camino.size(); ++i) {
      os << "(" << camino[i].first << "," << camino[i].second << ")";
      if (i + 1 < camino.size()) os << " -> ";
    }
    os << "\n\n";

    // Calcular coste total del camino real
    if (laberinto_ && camino.size() > 1) {
      double coste_total = 0.0;
      for (size_t i = 0; i < camino.size() - 1; ++i) {
        auto [f1, c1] = camino[i];
        auto [f2, c2] = camino[i + 1];
        double coste = laberinto_->MoveCost(f1, c1, f2, c2);
        coste_total += coste;
      }
      os << "Coste total del camino seguido: " << coste_total << "\n\n";
    }
  }

  os << "========================================\n";
  os << "FIN DEL REPORTE\n";
  os << "========================================\n";
}

void BusquedaInformada::ImprimirLaberintoConCaminoYAgente(
    std::ostream& os,
    const std::pair<size_t, size_t>& posicion_agente,
    const std::vector<std::pair<size_t, size_t>>& camino_historico) const {
    
    if (!laberinto_) return;
    
    // Crear conjuntos para búsquedas rápidas
    std::set<std::pair<size_t, size_t>> camino_planificado_set;
    for (const auto& pos : camino_encontrado_) {
        camino_planificado_set.insert(pos);
    }
    
    std::set<std::pair<size_t, size_t>> camino_historico_set;
    for (const auto& pos : camino_historico) {
        camino_historico_set.insert(pos);
    }
    
    auto start = laberinto_->coordenadas_start();
    auto exit = laberinto_->coordenadas_exit();
    
    for (size_t i = 0; i < static_cast<size_t>(laberinto_->filas()); ++i) {
        for (size_t j = 0; j < static_cast<size_t>(laberinto_->columnas()); ++j) {
            std::pair<size_t, size_t> pos_actual(i, j);
            
            // PRIORIDAD DE SÍMBOLOS (de mayor a menor importancia)
            
            // 1. Posición del agente (la más importante)
            if (pos_actual == posicion_agente) {
                os << "@";  // Agente actual
            }
            // 2. Start y Exit originales (si no han sido reemplazados)
            else if (pos_actual == exit) {
                os << "E";  // Meta/Exit
            }
            else if (pos_actual == start && camino_historico_set.find(pos_actual) == camino_historico_set.end()) {
                os << "S";  // Start (solo si no ha sido pisado)
            }
            // 3. Camino planificado (donde va a ir)
            else if (camino_planificado_set.find(pos_actual) != camino_planificado_set.end()) {
                os << "*";  // Camino planificado futuro
            }
            // 4. Camino histórico (por donde ya pasó)
            else if (camino_historico_set.find(pos_actual) != camino_historico_set.end()) {
                os << "·";  // Pasos ya dados (punto medio)
            }
            // 5. Laberinto normal
            else {
                char simbolo = laberinto_->getCasilla(i, j).ImprimirCasilla();
                os << simbolo;
            }
            
            if (j + 1 < static_cast<size_t>(laberinto_->columnas())) os << ' ';
        }
        os << "\n";
    }
}