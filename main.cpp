#include <iostream>
#include <conio.h>
#include <windows.h>

// Размеры игрового поля
const int WIDTH = 40;
const int HEIGHT = 20;

// Клавиши управления
const char UP_P1 = 'w';
const char DOWN_P1 = 's';
const char UP_P2 = 'i';
const char DOWN_P2 = 'k';

int ballX, ballY;
int ballDirX, ballDirY;
int pad1Y, pad2Y;
int padLength = 4;
int score1, score2;
bool gameOver;

// Функция для перемещения курсора в начало, чтобы избежать мерцания экрана
void setCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Скрытие курсора консоли
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void Setup() {
    gameOver = false;
    ballX = WIDTH / 2;
    ballY = HEIGHT / 2;
    ballDirX = (rand() % 2 == 0) ? 1 : -1;
    ballDirY = (rand() % 2 == 0) ? 1 : -1;

    pad1Y = HEIGHT / 2 - padLength / 2;
    pad2Y = HEIGHT / 2 - padLength / 2;

    score1 = 0;
    score2 = 0;
    hideCursor();
}

void Draw() {
    setCursorPosition(0, 0);

    // Верхняя стена
    for (int i = 0; i < WIDTH + 2; i++) std::cout << "#";
    std::cout << "\n";

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Левая стена и ракетка 1
            if (x == 0) {
                if (y >= pad1Y && y < pad1Y + padLength) std::cout << "#|";
                else std::cout << "# ";
            }
            
            // Мяч
            if (x == ballX && y == ballY) {
                std::cout << "O";
            }
            // Пустое пространство
            else if (!(x == 0 || x == WIDTH - 1)) {
                std::cout << " ";
            }

            // Правая стена и ракетка 2
            if (x == WIDTH - 1) {
                if (y >= pad2Y && y < pad2Y + padLength) std::cout << "|#";
                else std::cout << " #";
            }
        }
        std::cout << "\n";
    }

    // Нижняя стена
    for (int i = 0; i < WIDTH + 2; i++) std::cout << "#";
    std::cout << "\n\n";

    // Счет
    std::cout << " Игрок 1 (W/S): " << score1 << "   |   Игрок 2 (I/K): " << score2 << "\n";
    std::cout << " Для выхода нажмите 'Q'\n";
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case UP_P1:   if (pad1Y > 0) pad1Y--; break;
            case DOWN_P1: if (pad1Y < HEIGHT - padLength) pad1Y++; break;
            case UP_P2:   if (pad2Y > 0) pad2Y--; break;
            case DOWN_P2: if (pad2Y < HEIGHT - padLength) pad2Y++; break;
            case 'q': case 'Q': gameOver = true; break;
        }
    }
}

void Logic() {
    // Движение мяча
    ballX += ballDirX;
    ballY += ballDirY;

    // Отскок от верхней и нижней стен
    if (ballY <= 0 || ballY >= HEIGHT - 1) {
        ballDirY = -ballDirY;
    }

    // Отскок от ракетки 1 (слева)
    if (ballX == 1 && ballY >= pad1Y && ballY < pad1Y + padLength) {
        ballDirX = -ballDirX;
    }

    // Отскок от ракетки 2 (справа)
    if (ballX == WIDTH - 2 && ballY >= pad2Y && ballY < pad2Y + padLength) {
        ballDirX = -ballDirX;
    }

    // Пропуск мяча (Гол)
    if (ballX <= 0) {
        score2++;
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
        ballDirX = 1;
    }
    else if (ballX >= WIDTH - 1) {
        score1++;
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
        ballDirX = -1;
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(40); // Ограничение скорости игры (FPS)
    }
    return 0;
}
