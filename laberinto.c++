// Inclusión de bibliotecas necesarias
#include <iostream>     // Para entrada/salida estándar
#include <ctime>        // Para generar semilla aleatoria
#include <cstdlib>      // Para funciones rand() y srand()
#include <vector>       // Para usar vectores dinámicos
#include <stack>        // Para la resolución del laberinto (aunque no se usa directamente)

// Uso del espacio de nombres estándar
using namespace std;

// Definición de constantes para representar los elementos del laberinto
const int PARED = 1;
const int CAMINO = 0;
const int SOLUCION = 2;
const int ENTRADA = 3;
const int SALIDA = 4;

// Función para imprimir el laberinto
void pintar(int alto, int largo, vector<vector<int>>& matriz) {
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < largo; j++) {
            // Imprime diferentes caracteres según el tipo de celda
            switch(matriz[i][j]) {
                case PARED: cout << "█"; break;     // Pared
                case CAMINO: cout << " "; break;    // Camino libre
                case SOLUCION: cout << "·"; break;  // Parte de la solución
                case ENTRADA: cout << "E"; break;   // Entrada del laberinto
                case SALIDA: cout << "S"; break;    // Salida del laberinto
            }
        }
        cout << endl;  // Nueva línea al final de cada fila
    }
}

// Función para generar el laberinto
vector<vector<int>> generarLab(int largo, int alto, float densidad) {
    int FParedes = densidad * 8;  // Factor de paredes
    int numParedes = densidad * (largo * alto) / 4;  // Número total de paredes a generar
    cout << "Alto: " << alto << " Largo: " << largo << "\n";
    cout << "FParedes: " << FParedes << " Densidad: " << numParedes << "\n";

    // Inicializar la matriz del laberinto con caminos
    vector<vector<int>> matriz(alto, vector<int>(largo, CAMINO));

    // Crear bordes del laberinto
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < largo; j++) {
            if (i == 0 || i == alto - 1 || j == 0 || j == largo - 1) {
                matriz[i][j] = PARED;
            }
        }
    }

    // Generar paredes aleatorias
    srand(time(NULL));  // Inicializar la semilla aleatoria
    for (int i = 0; i < numParedes; i++) {
        // Seleccionar una posición aleatoria (siempre par para mantener simetría)
        int x = rand() % (largo - 3) + 2;
        x = (x / 2) * 2;
        int y = rand() % (alto - 3) + 2;
        y = (y / 2) * 2;
        matriz[y][x] = PARED;

        // Generar paredes adicionales alrededor de la pared inicial
        for (int j = 0; j < FParedes; j++) {
            int px[4] = { x + 2, x - 2, x, x };
            int py[4] = { y, y, y + 2, y - 2 };
            int p = rand() % 4;

            // Verificar si la nueva pared está dentro de los límites y en un camino
            if (px[p] >= 0 && px[p] < largo && py[p] >= 0 && py[p] < alto && matriz[py[p]][px[p]] == CAMINO) {
                matriz[py[p]][px[p]] = PARED;
                matriz[(py[p] + y) / 2][(px[p] + x) / 2] = PARED;
            }
        }
    }

    // Establecer entrada y salida
    matriz[1][1] = ENTRADA;
    matriz[alto-2][largo-2] = SALIDA;

    return matriz;
}

// Función recursiva para resolver el laberinto
bool resolverLaberinto(vector<vector<int>>& laberinto, int x, int y) {
    // Si llegamos a la salida, hemos resuelto el laberinto
    if (laberinto[y][x] == SALIDA) {
        return true;
    }

    // Si encontramos una pared o ya hemos pasado por aquí, retrocedemos
    if (laberinto[y][x] == PARED || laberinto[y][x] == SOLUCION) {
        return false;
    }

    // Marcar el camino actual como parte de la solución
    if (laberinto[y][x] != ENTRADA) {
        laberinto[y][x] = SOLUCION;
    }

    // Definir los movimientos posibles (arriba, derecha, abajo, izquierda)
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};

    // Intentar moverse en todas las direcciones
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (resolverLaberinto(laberinto, nx, ny)) {
            return true;  // Si encontramos una solución, propagamos el éxito
        }
    }

    // Si no encontramos solución desde aquí, desmarcamos y retrocedemos
    if (laberinto[y][x] != ENTRADA) {
        laberinto[y][x] = CAMINO;
    }
    return false;
}

// Función principal
int main() {
    int largo, alto;
    float densidad;

    // Solicitar dimensiones y densidad del laberinto al usuario
    cout << "Ingrese el largo del laberinto: ";
    cin >> largo;
    cout << "Ingrese el alto del laberinto: ";
    cin >> alto;
    cout << "Ingrese la densidad del laberinto (0.0 - 1.0): ";
    cin >> densidad;

    // Generar el laberinto
    vector<vector<int>> laberinto = generarLab(largo, alto, densidad);
    
    // Mostrar el laberinto generado
    cout << "\nLaberinto generado:\n";
    pintar(alto, largo, laberinto);
    
    // Intentar resolver el laberinto
    cout << "\nResolviendo el laberinto...\n";
    if (resolverLaberinto(laberinto, 1, 1)) {
        cout << "Solución encontrada:\n";
        pintar(alto, largo, laberinto);
    } else {
        cout << "No se encontró solución.\n";
    }
    
    // Pausar la consola antes de cerrar (útil en Windows)
    system("pause");
    return 0;
}
