/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial
 *
 * @author Marco Aguiar Álvarez alu0101620961@ull.edu.es
 * @date  07 10 25
 * @name main.cc
 * @brief Implementacion del programa principal de la practica 02 sobre A* 
 * 
 */ 

#include "laberinto.h"

int main(int argc, char* argv[]) {
  std::srand(std::time(nullptr)); // Semilla para números aleatorios

  // Comprobar argumentos
  if (argc != 3) {
    std::cerr << "Uso: " << argv[0] << " <archivo_entrada> <archivo_salida>" << std::endl;
    return 1;
  }

  // Preguntar si se quiere introducir la entrada por teclado
  char opcion;
  std::cout << "¿Quiere introducir el laberinto por teclado? (s/n): ";
  std::cin >> opcion;
  bool entrada_teclado = (opcion == 's' || opcion == 'S');
  std::vector<std::pair<int, int>> input_posiciones;
  if (entrada_teclado) {
    int inicio_fila, inicio_columna, fin_fila, fin_columna;
    std::cout << "Introduzca la fila y columna de inicio (separadas por espacio): ";
    std::cin >> inicio_fila >> inicio_columna;
    std::cout << "Introduzca la fila y columna de fin (separadas por espacio): ";
    std::cin >> fin_fila >> fin_columna;
    input_posiciones.push_back({inicio_fila, inicio_columna});
    input_posiciones.push_back({fin_fila, fin_columna});
  }

  // Inicializar el laberinto
  Laberinto laberinto;
  if (!laberinto.ProcesarArchivoEntrada(argv[1], argv[2], entrada_teclado, input_posiciones)) {
    return 1;
  }

  // Realizar la práctica
  laberinto.Practica(argv[2]);

  
  return 0;
}