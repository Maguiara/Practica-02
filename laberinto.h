/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial
 *
 * @author Marco Aguiar Álvarez alu0101620961@ull.edu.es
 * @date  07 10 25
 * @name laberinto.h
 * @brief Declaracion de la clase Laberinto y todos los metodos que la componen (principales y auxiliares) 
 * para la practica 02 sobre A*.
 * 
 */ 

#ifndef LABERINTO_H
#define LABERINTO_H

#include <iostream>
#include <utility>
#include <vector>  
#include <string>
#include <fstream>
#include <algorithm>
#include <random> 
#include <ctime>
#include <sstream>


/**
 * @brief Clase que representa un nodo en el algoritmo A*.
 */
class Nodo {

 public:
  Nodo(std::pair<int, int> pos, Nodo* padre = nullptr, int g = 0, int h = 0)  : posicion_(pos), padre_(padre), g_(g), h_(h), f_(g + h) {}

  //Getters
  std::pair<int, int> GetPosicion() const { return posicion_; }
  Nodo* GetPadre() const { return padre_; }
  int GetG() const { return g_; }
  int GetH() const { return h_; }
  int GetF() const { return f_; }

  //Setters
  void SetPosicion(const std::pair<int, int>& posicion) { posicion_ = posicion; }
  void SetPadre(Nodo* padre_) { this->padre_ = padre_; }
  void SetG(int g) { g_ = g; f_ = g_ + h_; }
  void SetH(int h) { h_ = h; f_ = g_ + h_; }
  void SetF(int f) { f_ = f; }

  // Sobrecarga para que se comparen los nodos por f(n)
  bool operator<(const Nodo& other) const {
    return f_ < other.f_;
  }

 private:
  std::pair<int, int> posicion_;
  Nodo* padre_;
  int g_;
  int h_;
  int f_;
};




// Type def para usar Matrix (se ve mas cool que vector de vectores)
typedef std::vector<std::vector<int>> Matrix; 

/**
 * @brief Clase que representa un laberinto y contiene métodos para procesarlo y aplicar el algoritmo A*.
 */
class Laberinto {
 public:

 // Constructor
  bool ProcesarArchivoEntrada(const std::string&, const std::string&, bool, std::vector<std::pair<int, int>>);

  // Setter
  void SetInicio(const std::pair<int, int>& inicio) { inicio_ = inicio; }
  void SetFin(const std::pair<int, int>& fin) { fin_ = fin; }

  // Funciones principales
  bool Aestrella();
  void Randomizer(const double pin = 0.5, const double pout = 0.5);
  void Practica (const std::string& archivo_salida); 

  // Heurística: Distancia de Manhattan * 3
  int Heuristica(std::pair<int, int> actual) {
    const int W = 3; // Peso de la heurística
    int dx = abs(fin_.first - actual.first);
    int dy = abs(fin_.second - actual.second);
    return W * (dx + dy);
  }

  // Funciones auxiliares
  void ImprimirLaberinto(const std::string&) const;

  /// @brief Marca el camino encontrado en el laberinto.
  /// @param nodo Puntero al nodo actual.
  void MarcarCaminoAestrella(Nodo* nodo) {
      while (nodo != nullptr) {
        if (nodo->GetPosicion() != inicio_ && nodo->GetPosicion() != fin_) {
          grid_[nodo->GetPosicion().first][nodo->GetPosicion().second] = 5;
        }
        nodo = nodo->GetPadre();
      }
  }


  /// @brief Asegura que no haya más de un 25% de obstáculos en el laberinto.
  void AsegurarObstaculosMinimos() {
    double porcentaje_obstaculos = GetPorcentajeObstaculos();
    while (porcentaje_obstaculos > 0.25) {
      for (int i = 0; i < filas_; ++i) {
        for (int j = 0; j < columnas_; ++j) {
          if (grid_[i][j] == 1) { // Casilla con obstáculo
            float random_value = static_cast<float>(std::rand()) / RAND_MAX;
            if (random_value >= 0.5) grid_[i][j] = 0; // Convertir en libre
          }
        }
      }
      porcentaje_obstaculos = GetPorcentajeObstaculos();
    }
  }

  /// @brief Calcula el porcentaje de obstáculos en el laberinto.
  /// @return Porcentaje de obstáculos (valor entre 0 y 1).
  double GetPorcentajeObstaculos() const {
    double total = filas_ * columnas_;
    double obstaculos = 0;
    for (const auto& fila : grid_) {
      obstaculos += std::count(fila.begin(), fila.end(), 1);
    }
    double porcentaje = obstaculos / total;
    return porcentaje;
  }

  /// @brief Mueve el agente una vez hacia la posición del siguiente paso en el camino encontrado por A*.
  Nodo* MoverAgente(Nodo* nodo) {
     while (nodo->GetPadre() != nullptr && nodo->GetPadre()->GetPadre() != nullptr) {
        nodo = nodo->GetPadre();
      }
      // si me movi diagonal sumo 7 si no sumo 5
      if (nodo->GetPadre()->GetPosicion().first != nodo->GetPosicion().first && nodo->GetPadre()->GetPosicion().second != nodo->GetPosicion().second) 
        costo_total_ += 7;
      else costo_total_ += 5;

      // Almacenar el paso en el vector camino_
      camino_.push_back(nodo->GetPosicion());
      return nodo;
  }

  void BorrarCamino() {
    for (int i = 0; i < filas_; ++i) {
      for (int j = 0; j < columnas_; ++j) {
        if (grid_[i][j] == 5) {
          grid_[i][j] = 0; // Convertir en libre
        }
      }
    }
  }
  
  void ImprimirDetallesLaberinto(const std::string& output_file) const {
    std::ofstream output(output_file, std::ios::app);
    if (!output.is_open()) {
      std::cerr << "Error al abrir el archivo de salida: " << output_file << std::endl;
      return;
    }
    output << "========================" << std::endl;
    output << "Detalles del Laberinto:" << std::endl;
    output << "Posición del Agente:       (" << inicio_.first << ", " << inicio_.second << ")" << std::endl;
    output << "Posición de la Salida:     (" << fin_.first << ", " << fin_.second << ")" << std::endl;
    output << "Porcentaje de Obstáculos:  " << GetPorcentajeObstaculos() * 100 << "%" << std::endl;
    output << "Nodos Generados:           " << nodos_generados_ << std::endl;
    output << log_generados_.str() << std::endl;
    output << "Nodos Inspeccionados:      " << nodos_inspeccionados_ << std::endl;
    output << log_inspeccionados_.str() << std::endl;
    output << "Camino del Agente:         ";
    for (const auto& paso : camino_) {
      output << "(" << paso.first << ", " << paso.second << ") ";
    }
    output << std::endl;
    output << "Pasos del Agente:          " << numeros_pasos_ << std::endl;
    output << "Costo Total del Camino:    " << costo_total_ << std::endl;
    output << "========================" << std::endl;
    output.close();
  }

  /**
   * @brief Imprime los detalles iniciales del laberinto en el archivo de salida.
   * @param output_file: Nombre del archivo de salida donde se imprimirán los detalles.
   */
  void ImprimirDetallesIniciales(const std::string& output_file) const {
    std::ofstream output(output_file);
    if (!output.is_open()) {
      std::cerr << "Error al abrir el archivo de salida: " << output_file << std::endl;
      return;
    }
    output << "Laberinto Inicial:" << std::endl;
    ImprimirLaberinto(output_file);
    ImprimirDetallesLaberinto(output_file);
  } 
  
  private:
  // Atributos del laberinto
  int filas_;  // Número de filas del laberinto
  int columnas_;  // Número de columnas del laberinto
  Matrix grid_;  // Matriz que representa el laberinto
  std::pair<int, int> inicio_;  // Posición de inicio (fila, columna)
  std::pair<int, int> fin_;  // Posición de fin (fila, columna)

  //Contadores para las metricas
  int nodos_generados_ = 0; // Contador de nodos generados
  int nodos_inspeccionados_ = 0; // Contador de nodos inspeccionados
  int numeros_pasos_ = 0; // Contador de pasos realizados por el agente
  int costo_total_ = 0; // Costo total del camino encontrado
  std::vector<std::pair<int, int>> camino_ ; // Vector para almacenar el camino encontrado
  
  // Probabilidades para el randomizer
  float pin_ = 0.5; // Probabilidad de que una celda libre se convierta en obstáculo
  float pout_ = 0.5; // Probabilidad de que una celda con

  // log con los nodos generados e inspeccionados
  std::ostringstream log_generados_;
  std::ostringstream log_inspeccionados_;
};




#endif 