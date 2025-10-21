/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial
 *
 * @author Marco Aguiar Álvarez alu0101620961@ull.edu.es
 * @date  07 10 25
 * @name laberinto.cc
 * @brief Implementacion de la clase Laberinto y todos los metodos que la componen (principales y auxiliares) 
 * para la practica 02 sobre A*.
 * 
 */ 


 #include "laberinto.h"


/**
 * @brief Procesa el archivo de entrada para inicializar el laberinto.
 * @param input_name: Nombre del archivo de entrada.
 * @param output_name: Nombre del archivo de salida.
 * @param entrada_teclado: Indica si las posiciones de inicio y fin se proporcion
 * an por teclado.
 * @param input_posiciones: Vector con las posiciones de inicio y fin si se proporcionan
 * por teclado.
 * @return true si el archivo se procesa correctamente, false en caso contrario.
 */
bool Laberinto::ProcesarArchivoEntrada(const std::string& input_name, const std::string& output_name, bool entrada_teclado, std::vector<std::pair<int, int>> input_posiciones) {
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

  // Informacion del laberinto cargada correctamente
  std::cout << "Laberinto de " << filas_ << " filas y " << columnas_ << " columnas cargado." << std::endl;

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
    grid_ [inicio_.first][inicio_.second] = 1; // Convertir inicio en pared
    grid_ [fin_.first][fin_.second] = 1; // Convertir fin en pared
    // asegurar que las posiciones sean válidas y estén en los bordes del laberinto
    for (const auto& pos : input_posiciones) {
      if (pos.first < 0 || pos.first >= filas_ || pos.second < 0 || pos.second >= columnas_) {
      std::cerr << "Posición inválida: (" << pos.first << ", " << pos.second << ")" << std::endl;
      return false;
      }
      if (pos.first != 0 && pos.first != filas_ - 1 && pos.second != 0 && pos.second != columnas_ - 1) {
        std::cerr << "La posición debe estar en los bordes del laberinto: (" << pos.first << ", " << pos.second << ")" << std::endl;
        return false;
      }
    }
    inicio_ = input_posiciones[0];
    fin_ = input_posiciones[1];
    grid_ [inicio_.first][inicio_.second] = 3; // Marcar nuevo inicio
    grid_ [fin_.first][fin_.second] = 4; // Marcar nuevo fin
  }
  // Crear o limpiar el archivo de salida
  std::ofstream output(output_name);
  if(!output.is_open()) {
    std::cerr << "No se pudo abrir el archivo de salida, revise el nombre o que este exista" << std::endl;
    return false;
  }
  output.close();
  return true;
}


/**
  * @brief Implementación del algoritmo A* para encontrar el camino desde el inicio hasta el fin del laberinto.
  * @return true si se encuentra un camino, false en caso contrario
 */
bool Laberinto::Aestrella() {
  const int max_intentos = 5; // Número máximo de intentos
  int intentos = 0;

  while (intentos < max_intentos) {
    std::vector<Nodo*> abiertos;
    std::vector<Nodo*> cerrados;

    //reiniciamos en cada iteracion los contadores y logs para cada iteracion de A*
    //log_generados_.str("");
    //log_inspeccionados_.str("");
    //nodos_generados_ = 0;
    //nodos_inspeccionados_ = 0;
    

    // Inicializamos e insertamos el nodo inicial en A
    Nodo* actual = new Nodo(inicio_, nullptr, 0, Heuristica(inicio_));
    abiertos.push_back(actual);

    // mientras A no esté vacío
    while (!abiertos.empty()) {
      // Seleccionar el nodo con el menor f(n) en A como el nodo actual
      std::sort(abiertos.begin(), abiertos.end(), [](Nodo* a, Nodo* b) {
        return a->GetF() < b->GetF();
      });
      actual = abiertos[0];
      nodos_inspeccionados_++;
      log_inspeccionados_ << "(" << actual->GetPosicion().first << ", " << actual->GetPosicion().second << ") ";

      // Prueba meta
      if (actual->GetPosicion() == fin_) {
        // std::cout << "Camino encontrado en el intento " << intentos + 1 << std::endl;
        MarcarCaminoAestrella(actual);
        //
        inicio_ = MoverAgente(actual)->GetPosicion(); 
        // Liberar memoria de nodos
        for (auto nodo : abiertos) delete nodo;
        for (auto nodo : cerrados) delete nodo;


        return true;
      }

      // Mover actual de abiertos a cerrados
      cerrados.push_back(actual);
      abiertos.erase(abiertos.begin());

      // Para cada vecino del nodo actual
      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          if (i == 0 && j == 0) continue; // Evitar el mismo nodo
          std::pair<int, int> vecino_pos = {actual->GetPosicion().first + i, actual->GetPosicion().second + j};
          // Verificar que el vecino esté dentro de los límites del laberinto y no sea una pared
          if (vecino_pos.first < 0 || vecino_pos.first >= filas_ ||
             vecino_pos.second < 0 || vecino_pos.second >= columnas_) continue; // Fuera de límites
          if (grid_[vecino_pos.first][vecino_pos.second] == 1) continue; // Pared

          // Calcular el costo de movimiento: diagonal = 7, ortogonal = 5
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
          // Si está en abiertos, verificar si el nuevo camino es mejor
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
            nodos_generados_++;
            log_generados_ << "(" << vecino->GetPosicion().first << ", " << vecino->GetPosicion().second << ") ";
          }
        } 
      }
      
    }

    // Si no se encontró camino, liberar memoria de nodos
    for (auto nodo : abiertos) delete nodo;
    for (auto nodo : cerrados) delete nodo;

    // Incrementar el contador de intentos
    intentos++;

    // Si no se encontró camino y aún quedan intentos, eliminar un muro aleatorio
    if (intentos < max_intentos) {
      std::cout << "No se encontró camino. Eliminando un muro aleatorio e intentando de nuevo..." << std::endl;
      bool muro_eliminado = false;
      while (!muro_eliminado) {
        int fila = std::rand() % filas_;
        int columna = std::rand() % columnas_;
        if (grid_[fila][columna] == 1) { // Si es un muro
          grid_[fila][columna] = 0; // Convertirlo en espacio libre
          muro_eliminado = true;
        }
      }
    }
  }

  // Si se alcanzó el número máximo de intentos y no se encontró camino
  std::cout << "No se encontró camino después de " << max_intentos << " intentos." << std::endl;
  return false;
}


/**
 * @brief Modifica el laberinto aleatoriamente, cambiando algunas casillas libres a obstáculos y viceversa.
 * Asegura que no haya más de un 25% de obstáculos en el laberinto.
 */
void Laberinto::Randomizer(const double pin, const double pout) {
  std::srand(static_cast<unsigned>(std::time(nullptr))); // Inicializar la semilla aleatoria
  for (int i = 0; i < filas_; ++i) {
    for (int j = 0; j < columnas_; ++j) {
      if (grid_[i][j] == 0 || grid_[i][j] == 5 || grid_[i][j] == 7) { // Casilla libre, parte del camino proyectado o posición del agente
        double random_value = static_cast<double>(std::rand()) / RAND_MAX;
        if (random_value >= (1 - pin)) {
          if (grid_[i][j] == 7) grid_[i][j] = 6; // Marcar una posicion del agente que se convierte en pared
          else grid_[i][j] = 1; // Convertir en obstáculo
        }
      } else if (grid_[i][j] == 1 || grid_[i][j] == 6) { // Casilla con obstáculo o que fue parte del camino del agente y se convirtió en pared
        double random_value = static_cast<double>(std::rand()) / RAND_MAX;
        if (random_value >= (1 - pout)) {
          if (grid_[i][j] == 6) grid_[i][j] = 7; // Marcar una posicion del agente que se convierte en libre
          else grid_[i][j] = 0; // Convertir en libre
        }
      }
    }
  } 
  // Asegurar que no haya mas de un 25% de obstáculos
  AsegurarObstaculosMinimos();
}

/**
 * @brief Realiza la práctica completa: mueve el agente desde el inicio hasta el fin,
 * aplicando A*, imprimiendo el laberinto y modificándolo con el randomizer en cada paso.
 * @param output_name: Nombre del archivo de salida donde se imprimirá el laberinto en cada paso. 
 */
void Laberinto::Practica(const std::string& output_name) {
  // Imprimir el laberinto inicial
  ImprimirDetallesIniciales(output_name);
  while (inicio_ != fin_) {
    // Iteración de A estrella
    Aestrella();
    //Marcar el movimiento del agente
    grid_[inicio_.first][inicio_.second] = 7;
    // Incrementar el contador de pasos
    numeros_pasos_++;
    // Impresión del laberinto
    ImprimirLaberinto(output_name);
    // Borrar el camino marcado
    BorrarCamino();
    // Impresion de los detalles del laberinto
    ImprimirDetallesLaberinto(output_name);
    // Randomizer
    Randomizer();
  }
}




/**
 * @brief Imprime el laberinto en la consola.
 * Representa espacios libres con '-', paredes con '█', el inicio con 'S', la salida con 'E', 
 * el camino encontrado con '*', las posiciones del agente con 'A', y las posiciones que fueron modificadas a pared con '^'.
 * @param output_name: Nombre del archivo de salida donde se imprimirá el laberinto
 */
void Laberinto::ImprimirLaberinto(const std::string& output_name) const {
  std::ofstream output(output_name, std::ios::app);
  if (!output.is_open()) {
    std::cerr << "Error al abrir el archivo de salida: " << output_name << std::endl;
    return;
  }
  for (const auto& fila : grid_) {
    for (const auto& celda : fila) {
      switch (celda) {
        case 0:
          output << "-";  // Espacio libre
          break;
        case 1:
          output << "█";  // Pared
          break;
        case 3:
          output << "S";  // Inicio
          break;
        case 4:
          output << "E";  // Salida
          break;
        case 5:
            output << "*";  // Camino encontrado
          break;
        case 6:
            output << "░";  // Camino que fue modificado a pared
          break;
        case 7:
            output << "A"; // Posición actual del agente
          break;
        default:
            output << "? ";  // Desconocido
          break;
      }
    }
    output << std::endl;
  }
  output.close();
}
