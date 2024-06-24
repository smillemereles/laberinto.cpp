#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

const char MURO = '\u2588';
const char CAMINO = '.';
const char CURSOR = '*';

class Laberinto {
private:
    std::vector<std::vector<char>> grid;
    int ancho, alto;

    void limpiar_pantalla() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    bool es_valido(int x, int y) {
        return x >= 0 && x < ancho && y >= 0 && y < alto;
    }

    void crear_camino(int x, int y) {
        std::vector<std::vector<int>> direcciones = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        std::vector<int> orden_dir = {0, 1, 2, 3};
        grid[y][x] = CAMINO;

        for (int i = 3; i > 0; i--) {
            int j = rand() % (i + 1);
            std::swap(orden_dir[i], orden_dir[j]);
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + 2 * direcciones[orden_dir[i]][0];
            int ny = y + 2 * direcciones[orden_dir[i]][1];
            if (es_valido(nx, ny) && grid[ny][nx] == MURO) {
                grid[y + direcciones[orden_dir[i]][1]][x + direcciones[orden_dir[i]][0]] = CAMINO;

                limpiar_pantalla();
                grid[ny][nx] = CURSOR;
                imprimir_laberinto();
                grid[ny][nx] = MURO;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                crear_camino(nx, ny);
            }
        }
    }

    void imprimir_caracter_en_color(char c, int color_code) {
        // Cambiar el color del texto usando códigos ANSI
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color_code);  // Cambiar el color del texto en Windows
        std::cout << c;
#else
        std::cout << "\033[38;5;" << color_code << "m" << c << "\033[0m";  // Cambiar el color del texto en Unix/Linux
#endif
    }

public:
    Laberinto(int w, int h) : ancho(w), alto(h) {
        grid = std::vector<std::vector<char>>(alto, std::vector<char>(ancho, MURO));
    }

    void imprimir_laberinto() {
        for (const auto& fila : grid) {
            for (char celda : fila) {
                if (celda == MURO) {
                    imprimir_caracter_en_color(MURO, 9);  // Código de color 9 para el color rosa (magenta)
                } else if (celda == CAMINO) {
                    std::cout << CAMINO;
                } else if (celda == CURSOR) {
                    imprimir_caracter_en_color(CURSOR, 9);  // Código de color 9 para el color rosa (magenta)
                }
            }
            std::cout << '\n';
        }
    }

    void generar_laberinto() {
        srand(time(nullptr));

        grid[0][1] = CAMINO;
        grid[alto - 1][ancho - 2] = CAMINO;
        crear_camino(1, 1);
    }

    bool resolver_laberinto(int x, int y) {
        grid[y][x] = CURSOR;

        if (x == ancho - 2 && y == alto - 1) {
            limpiar_pantalla();
            imprimir_laberinto();
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        limpiar_pantalla();
        imprimir_laberinto();

        if (es_valido(x + 1, y) && grid[y][x + 1] == CAMINO && resolver_laberinto(x + 1, y)) {
            return true;
        }
        if (es_valido(x, y + 1) && grid[y + 1][x] == CAMINO && resolver_laberinto(x, y + 1)) {
            return true;
        }
        if (es_valido(x - 1, y) && grid[y][x - 1] == CAMINO && resolver_laberinto(x - 1, y)) {
            return true;
        }
        if (es_valido(x, y - 1) && grid[y - 1][x] == CAMINO && resolver_laberinto(x, y - 1)) {
            return true;
        }

        grid[y][x] = CAMINO;
        return false;
    }
};

int main() {
    int ancho, alto;

    // Solicitar dimensiones del laberinto
    std::cout << "Ingrese el ancho del laberinto: ";
    std::cin >> ancho;
    std::cout << "Ingrese la altura del laberinto: ";
    std::cin >> alto;

    // Asegurar dimensiones impares
    if (ancho % 2 == 0) ancho++;
    if (alto % 2 == 0) alto++;

    // Generar y mostrar el laberinto
    Laberinto lab(ancho, alto);
    lab.generar_laberinto();
    lab.imprimir_laberinto();

    // Resolver el laberinto
    std::cout << "\nResolviendo laberinto...\n";
    if (lab.resolver_laberinto(1, 1)) {
        std::cout << "\nLaberinto resuelto!\n";
    } else {
        std::cout << "\nNo se pudo resolver el laberinto.\n";
    }

    return 0;
}
