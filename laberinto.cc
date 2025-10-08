#include "laberinto.h"



/**
 * @brief Procesa el archivo de entrada para construir el laberinto.
 * @param input_name: Nombre del archivo de entrada
 * @return: true si el archivo se procesa correctamente, false en caso contrario
 */
bool Laberinto::ProcesarArchivoEntrada(const std::string& input_name, bool entrada_teclado) {
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
  for(int i = 0; i < filas_; ++i) {
    for(int j = 0; j < columnas_; ++j) {
      input >> casilla;
      if(input.fail()) {
        std::cerr << "Error al leer la casilla (" << i << ", " << j << ")" << std::endl;
        return false;
      }
      grid_[i][j] = casilla;
      if (casilla == 3) inicio_ = {i, j};
      if (casilla == 4) fin_ = {i, j};
    }
  }
  input.close();
  return true;
}


/**
 * @brief Implementación del algoritmo A*
 * @param inicio: Par de enteros que representa la posición inicial (fila, columna)
 * @param fin: Par de enteros que representa la posición final (fila, columna)
 * @return: true si se encuentra una ruta, false en caso contrario
 */
bool Laberinto::Aestrella(std::pair<int, int> inicio, std::pair<int, int> fin) {
 
  //Paso 1: Calcular f(n), g(n) y h(n) para el punto de entrada al laberinto, S, que se inserta
  //en la lista de nodos abiertos A.
  Nodo nodo_inicial;
  nodo_inicial.set_posicion(inicio);
  nodo_inicial.set_g(0);
  nodo_inicial.set_h(inicio, fin);
  nodo_inicial.set_f(nodo_inicial.get_g() + nodo_inicial.get_h());
  nodos_abiertos_.push_back(nodo_inicial);

  //Paso 2: Repetir mientras A no esta vacia
  while (!nodos_abiertos_.empty()) {
  //  (a) Seleccionar el nodo de menor coste f(n), e insertarlo en la 
  //      lista de nodos cerrados C.
    Nodo actual;
    for (auto nodo : nodos_cerrados_) {
      if (nodo.get_f() < actual.get_f()) actual = nodo; 
    }
    nodos_cerrados_.push_back(actual);

    //  (b) (b) Para cada nodo vecino, realizar las siguientes acciones:
    std::vector<Nodo> nodos_vecinos;

  //      (b.1) Si el nodo no está ni en A, ni en C, su nodo padre será el nodo analizado
  //          y será insertado en A. Realizar las acciones que corresponda.

  //      (b.2) Si el nodo está en A, tener en cuenta que quizás sea necesario actualizar
  //            su coste g(n) y, por lo tanto, su padre en el camino. Nótese que se deben
  //            recalcular los costes necesarios.
  
  // Paso 3: Si A = ∅ y no se ha llegado a la salida del laberinto, E, no existe camino y se
  // deberá mostrar en pantalla un mensaje que así lo indique.
  }
  return true; // Placeholder
}




/**
 * @brief Imprime el laberinto en la consola.
 * Representa espacios libres con '-', paredes con '█', el inicio con 'S' y la salida con 'E'.
 */
void Laberinto::ImprimirLaberinto() const {
  for (const auto& fila : grid_) {
    for (const auto& celda : fila) {
      switch (celda) {
        case 0:
          std::cout << "-";  // Espacio libre
          break;
        case 1:
          std::cout << "█";  // Pared
          break;
        case 3:
          std::cout << "S";  // Inicio
          break;
        case 4:
          std::cout << "E";  // Salida
          break;
        default:
          std::cout << "? ";  // Desconocido
          break;
      }
    }
    std::cout << std::endl;
  }
}
