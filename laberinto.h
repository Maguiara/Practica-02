/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Inteligencia Artificial
  *
  * @author Marco Aguiar Álvarez alu0101620961@ull.edu.es
  * @date 07 10 25
  * @brief Implementacion de la clase Laberinto
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



enum Movimientos {
  ARRIBA = 5,
  ABAJO = 5,
  IZQUIERDA = 5,
  DERECHA = 5,
  DIAGONAL_ARRIBA_IZQUIERDA = 7,
  DIAGONAL_ARRIBA_DERECHA = 7,
  DIAGONAL_ABAJO_IZQUIERDA = 7,
  DIAGONAL_ABAJO_DERECHA = 7
};




typedef std::vector<std::vector<int>> Matrix; 

class Laberinto {
 public:

 // Constructor
  bool ProcesarArchivoEntrada(const std::string&, bool);

  // Funciones principales
  bool Aestrella();
  void Randomizer();

  // Heurística: Distancia de Manhattan * 3
  int Heuristica(std::pair<int, int> actual) {
    const int W = 3; // Peso de la heurística
    int dx = abs(fin_.first - actual.first);
    int dy = abs(fin_.second - actual.second);
    return W * (dx + dy);
  }

  // Funciones auxiliares
  void ImprimirLaberinto() const;

  /// @brief Marca el camino encontrado en el laberinto.
  /// @param nodo Puntero al nodo actual.
  void MarcarCamino(Nodo* nodo) {
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
    double total = filas_ * columnas_, obstaculos = 0;
    for (const auto& fila : grid_) {
      obstaculos += std::count(fila.begin(), fila.end(), 1);
    }
    return obstaculos / total;
  }
  
  
  private:
  int filas_;  // Número de filas del laberinto
  int columnas_;  // Número de columnas del laberinto
  Matrix grid_;  // Matriz que representa el laberinto
  std::pair<int, int> inicio_;  // Posición de inicio (fila, columna)
  std::pair<int, int> fin_;  // Posición de fin (fila, columna)

  float pin_ = 0.5; // Probabilidad de que una celda libre se convierta en obstáculo
  float pout_ = 0.5; // Probabilidad de que una celda con
};




#endif 