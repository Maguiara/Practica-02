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
  bool ProcesarArchivoEntrada(const std::string&);

    



  // Imprime el laberinto en la consola
  void ImprimirLaberinto() const {
    for (const auto& fila : grid_) {
      for (const auto& celda : fila) {
        switch (celda) {
          case 0:
            std::cout << " □";  // Espacio libre
            break;
          case 1:
            std::cout << "██";  // Pared
            break;
          case 3:
            std::cout << "S ";  // Inicio
            break;
          case 4:
            std::cout << " E";  // Salida
            break;
          default:
            std::cout << "? ";  // Desconocido
            break;
        }
      }
      std::cout << std::endl;
    }
  }



 private:
  int filas_;  // Número de filas del laberinto
  int columnas_;  // Número de columnas del laberinto
  Matrix grid_;  // Matriz que representa el laberinto
};




#endif 