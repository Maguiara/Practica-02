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

  bool Aestrella();

  
  int Heuristica(std::pair<int, int> actual) {
    const int W = 3; // Peso de la heurística
    return abs(fin_.first - actual.first) + abs(fin_.second - actual.second) * W;
  }


  // Imprime el laberinto en la consola
  void ImprimirLaberinto() const;

  
  
  private:
  int filas_;  // Número de filas del laberinto
  int columnas_;  // Número de columnas del laberinto
  Matrix grid_;  // Matriz que representa el laberinto
  std::pair<int, int> inicio_;  // Posición de inicio (fila, columna)
  std::pair<int, int> fin_;  // Posición de fin (fila, columna)
};




#endif 