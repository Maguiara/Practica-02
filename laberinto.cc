#include "laberinto.h"


bool Laberinto::ProcesarArchivoEntrada(const std::string& input_name) {
  std::ifstream input(input_name);
  if(!input.is_open()) {
    std::cerr << "No se pudo abrir el archivo de entrada, revise el nombre o que este exista" << std::endl;
    return false;
  }

  input >> filas_ >> columnas_;
  if(input.fail() || filas_ <= 0 || columnas_ <= 0) {
    std::cerr << "Error al leer el numero de filas o columnas, o estos son invalidos" << std::endl;
    return false;
  }

  grid_.resize(filas_, std::vector<int>(columnas_));

  int casilla;
  while(input >> casilla) {
    for(int i = 0; i < filas_; ++i) {
      for(int j = 0; j < columnas_; ++j) {
        grid_[i][j] = casilla;
      }
    }
  }
  return true;
}