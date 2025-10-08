/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Informática Básica
 *
 * @author Marco Aguiar Álvarez alu0101620961@ull.edu.es
 * @date 26 10 23
 * @brief Pritns the binary form of a decimal number
 *
 *
 */ 

#include "laberinto.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_entrada>" << std::endl;
    return 1;
  }

  char opcion;
  std::cout << "¿Quiere introducir el laberinto por teclado? (s/n): ";
  std::cin >> opcion;
  bool entrada_teclado = (opcion == 's' || opcion == 'S');

  Laberinto laberinto;
  if (!laberinto.ProcesarArchivoEntrada(argv[1], entrada_teclado)) {
    return 1;
  }

  laberinto.ImprimirLaberinto();
  return 0;
}