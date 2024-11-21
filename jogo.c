#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h> // Para Sleep e manipulação do console

// Definição do tamanho do tabuleiro
#define WIDTH 70  // Largura maior
#define HEIGHT 30 // Altura maior

// Direções
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

int gameOver;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int direction;
char screen[HEIGHT][WIDTH]; // Buffer de tela para evitar piscar

void setup() {
    gameOver = 0;
    direction = 0; // Inicialmente parada
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = -1; // Fruta começa invisível
    fruitY = -1; // Fruta começa invisível
    score = 0;
    nTail = 0;
}

void clearConsole() {
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

void draw() {
    // Limpa o buffer de tela (preenche com espaços)
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            screen[i][j] = ' '; // Espaço vazio
        }
    }

    // Adiciona a borda ao buffer
    for (int i = 0; i < WIDTH; i++) {
        screen[0][i] = '#'; // Bordas superiores
        screen[HEIGHT - 1][i] = '#'; // Bordas inferiores
    }
    for (int i = 0; i < HEIGHT; i++) {
        screen[i][0] = '#'; // Bordas laterais
        screen[i][WIDTH - 1] = '#';
    }

    // Adiciona a cobra ao buffer
    screen[y][x] = 'O'; // Cabeça da cobra
    for (int i = 0; i < nTail; i++) {
        screen[tailY[i]][tailX[i]] = 'o';
    }

    // Só desenha a fruta se a posição for válida
    if (fruitX != -1 && fruitY != -1) {
        screen[fruitY][fruitX] = 'F'; // Fruta visível
    }

    // Imprime o buffer na tela
    clearConsole(); // Apenas reposiciona o cursor
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", screen[i][j]);
        }
        printf("\n");
    }

    printf("Pontuação: %d\n", score);
}

void input() {
    if (_kbhit()) { // Verifica se uma tecla foi pressionada
        switch (_getch()) {
            case 'w': direction = UP; break;
            case 's': direction = DOWN; break;
            case 'a': direction = LEFT; break;
            case 'd': direction = RIGHT; break;
            case 'x': gameOver = 1; break; // Finaliza o jogo
        }
    }
}

void logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (direction) {
        case UP: y--; break;
        case DOWN: y++; break;
        case LEFT: x--; break;
        case RIGHT: x++; break;
    }

    // Verifica colisão com bordas
    if (x >= WIDTH - 1 || x <= 0 || y >= HEIGHT - 1 || y <= 0)
        gameOver = 1;

    // Verifica colisão com o próprio corpo
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;
    }

    // Verifica se comeu a fruta
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = -1; // A fruta não será visível até ser reposicionada
        fruitY = -1; // A fruta não será visível até ser reposicionada
        nTail++;
    }
}

void generateFruit() {
    // Gere uma nova posição para a fruta, garantindo que não sobreponha a cobra
    do {
        fruitX = rand() % (WIDTH - 2) + 1;
        fruitY = rand() % (HEIGHT - 2) + 1;
    } while (screen[fruitY][fruitX] == 'O' || screen[fruitY][fruitX] == 'o'); // Garante que a fruta não apareça na cobra
}

int main() {
    // Ajusta o tamanho do console para tela cheia
    system("mode con cols=80 lines=35");

    setup();

    while (!gameOver) {
        if (fruitX == -1 && fruitY == -1) {
            generateFruit(); // Gere a fruta quando a posição estiver vazia
        }

        draw();
        input();
        logic();
        Sleep(100); // Controla a velocidade do jogo
    }

    printf("Game Over! Pontuação final: %d\n", score);
    return 0;
}