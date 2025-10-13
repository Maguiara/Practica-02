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
  std::srand(std::time(nullptr)); // Semilla para números aleatorios

  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo_entrada>" << std::endl;
    return 1;
  }


  //char opcion;
  //std::cout << "¿Quiere introducir el laberinto por teclado? (s/n): ";
  //std::cin >> opcion;
  //bool entrada_teclado = (opcion == 's' || opcion == 'S');

  Laberinto laberinto;
  if (!laberinto.ProcesarArchivoEntrada(argv[1], false)) {
    return 1;
  }

  laberinto.ImprimirLaberinto();
  //laberinto.Randomizer();
  //laberinto.ImprimirLaberinto();
  
  // Ejecutar el algoritmo A*
  if (!laberinto.Aestrella()) {
    std::cerr << "Ha ocurrido un error al ejecutar el algoritmo A*" << std::endl;
    return 1;
  }
  return 0;
}