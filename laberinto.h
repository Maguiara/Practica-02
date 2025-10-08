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

class Nodo {

  public:
  //Constructores
  Nodo() = default;
  Nodo(std::pair<int, int> posicion, Nodo* padre = nullptr, int g = 0, int h = 0)  : posicion_(posicion), padre_(padre), g_(g), h_(h), f_(g + h) {}

  // Getters
  int get_g() const { return g_; }
  int get_h() const { return h_; }
  int get_f() const { return f_; }
  std::pair<int, int> get_posicion() const { return posicion_; }
  Nodo* get_padre() const { return padre_; }
  // Setters
  void set_g(int g) { g_ = g; }
  void set_h(std::pair<int, int> posicion_actual, std::pair<int, int> fin) { 
    const int W = 3; // Peso de la heurística
    h_ = abs(fin.first - posicion_actual.first) + abs(fin.second - posicion_actual.second) * W;
  }
  void set_f(int f) { f_ = f; }
  void set_posicion(std::pair<int, int> posicion) { posicion_ = posicion; }
  void set_padre(Nodo* padre) { padre_ = padre; }

 private:
  std::pair<int, int> posicion_;
  Nodo* padre_;
  int g_;  // Costo desde el nodo inicial
  int h_;  // Heurística (costo estimado al nodo objetivo)
  int f_;  // Costo total (g + h)
};



typedef std::vector<std::vector<int>> Matrix; 

class Laberinto {
 public:

 // Constructor
  bool ProcesarArchivoEntrada(const std::string&, bool);

  bool Aestrella(std::pair<int, int> inicio, std::pair<int, int> fin);

  


  // Imprime el laberinto en la consola
  void ImprimirLaberinto() const;

  
  
  private:
  int filas_;  // Número de filas del laberinto
  int columnas_;  // Número de columnas del laberinto
  Matrix grid_;  // Matriz que representa el laberinto
  std::vector<Nodo> nodos_abiertos_;  // Lista de nodos abiertos
  std::vector<Nodo> nodos_cerrados_;  // Lista de nodos cerrados
  std::vector<std::pair<int, int>> ruta_;  // Ruta desde el inicio hasta la salida
  std::pair<int, int> inicio_;  // Posición de inicio (fila, columna)
  std::pair<int, int> fin_;  // Posición de fin (fila, columna)
};




#endif 