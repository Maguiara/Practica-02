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

  if (entrada_teclado) {
    std::cout << "Laberinto cargado desde archivo:" << std::endl;
    ImprimirLaberinto();
  }
  return true;
}


/**
 * @brief Implementación del algoritmo A*
 * @param inicio: Par de enteros que representa la posición inicial (fila, columna)
 * @param fin: Par de enteros que representa la posición final (fila, columna)
 * @return: true si se encuentra una ruta, false en caso contrario
 */
bool Laberinto::Aestrella() {
 
  std::vector<Nodo*> abiertos;
  std::vector<Nodo*> cerrados;
  // Inicializamos e insertamos el nodo inicial en A
  Nodo* actual = new Nodo(inicio_, nullptr, 0, Heuristica(inicio_));
  abiertos.push_back(actual);

  // int iteracion = 1;
  
  // mientras A no esté vacío
  while (!abiertos.empty()) {
    // debug 
    //std::cout << "Iteracion " << iteracion++ << std::endl;

  // Seleccionar el nodo con el menor f(n) en A como el nodo actual
    std::sort(abiertos.begin(), abiertos.end(), [] (Nodo* a, Nodo* b) {
      return a->GetF() < b->GetF();
    });
    actual = abiertos[0];
    
    // debug
    //std::cout << "Nodo actual: (" << actual->GetPosicion().first << ", " << actual->GetPosicion().second << ") with f(n) = " << actual->GetF() << std::endl;
  
  // Prueba meta
    if (actual->GetPosicion() == fin_) {
      std::cout << "Wacho, encontre camino" << std::endl;
      // Reconstruir el camino desde el nodo actual hasta el inicio
      MarcarCamino(actual);
      ImprimirLaberinto();
      break;
    }

    // Mover actual de abiertos a cerrados
    cerrados.push_back(actual);
    abiertos.erase(abiertos.begin()); 

  // recorrer los vecinos del nodo actual
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        if (i == 0 && j == 0) continue; // Evitar el mismo nodo
        std::pair<int, int> vecino_pos = {actual->GetPosicion().first + i, actual->GetPosicion().second + j};
        // Verificar que el vecino esté dentro de los límites del laberinto y no sea una pared
        if (vecino_pos.first < 0 || vecino_pos.first >= filas_ || vecino_pos.second < 0 
        || vecino_pos.second >= columnas_) continue; // Fuera de límites
        if (grid_[vecino_pos.first][vecino_pos.second] == 1) continue; // Pared

        // Calcular el costo de movimiento (diagonal = 7, ortogonal = 5)
        int costo_movimiento = (abs(i) == abs(j)) ? 7 : 5;
        int nuevo_g = actual->GetG() + costo_movimiento;

        // Verificar si el vecino ya está en la lista de cerrados
        auto it_cerrados = std::find_if(cerrados.begin(), cerrados.end(), [&vecino_pos](Nodo* n) {
          return n->GetPosicion() == vecino_pos;
        });
        // Si está en cerrados, ignorarlo
        if (it_cerrados != cerrados.end()) continue;

        // Verificar si el vecino ya está en la lista de abiertos
        auto it_abiertos = std::find_if(abiertos.begin(), abiertos.end(), [&vecino_pos](Nodo* n) {
          return n->GetPosicion() == vecino_pos;
        });

        if (it_abiertos != abiertos.end()) {
          // Actualizar el nodo si el nuevo camino es mejor
          if (nuevo_g < (*it_abiertos)->GetG()) {
            (*it_abiertos)->SetPadre(actual);
            (*it_abiertos)->SetG(nuevo_g);
          }
        } else {
          // Crear el nodo si no está en abiertos
          Nodo* vecino = new Nodo(vecino_pos, actual, nuevo_g, Heuristica(vecino_pos));
          abiertos.push_back(vecino);
        }
      }
    }
    // debug
    //ImprimirLaberinto(); 
    //std::cout << "------------------------" << std::endl;
  }
  // Paso 3: Si A = ∅ y no se ha llegado a la salida del laberinto, E, no existe camino y se
  // deberá mostrar en pantalla un mensaje que así lo indique.
  if (abiertos.empty() && actual->GetPosicion() != fin_) {
    std::cout << "No se encontro camino" << std::endl;
    return false;
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
        case 5:
            std::cout << "*";  // Camino encontrado
          break;
        default:
          std::cout << "? ";  // Desconocido
          break;
      }
    }
    std::cout << std::endl;
  }
}
