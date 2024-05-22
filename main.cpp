#include <cstdio>
#include "board.h"
#include "game.h"
#include <SFML/Graphics.hpp>
#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>
#include <thread>

// Taille du damier
const int BOARD_SIZE = 800;
const int CELL_SIZE = BOARD_SIZE / 10;

// Fonction pour dessiner le damier
void drawBoard(sf::RenderWindow &window) {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    bool isWhite = true;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (isWhite) {
                cell.setFillColor(sf::Color::White);
            } else {
                cell.setFillColor(sf::Color::Black);
            }
            cell.setPosition(i * CELL_SIZE, j * CELL_SIZE);
            window.draw(cell);
            isWhite = !isWhite;
        }
        isWhite = !isWhite;
    }
}

// Fonction pour dessiner les pions
void drawPawns(sf::RenderWindow &window, PawnType board[NUM_CELL][NUM_CELL]) {
    sf::CircleShape pawn(CELL_SIZE / 2 - 10);
    pawn.setOrigin(CELL_SIZE / 2 - 10, CELL_SIZE / 2 - 10);

    for (int i = 0; i < NUM_CELL; ++i) {
        for (int j = 0; j < NUM_CELL; ++j) {
            if (board[i][j] == PAWN_WHITE) {
                pawn.setFillColor(sf::Color::White);
                pawn.setOutlineColor(sf::Color::Black);
                pawn.setOutlineThickness(2);
                pawn.setPosition(j * CELL_SIZE + CELL_SIZE / 2, i * CELL_SIZE + CELL_SIZE / 2);
                window.draw(pawn);
            } else if (board[i][j] == PAWN_BLACK) {
                pawn.setFillColor(sf::Color::Black);
                pawn.setOutlineColor(sf::Color::White);
                pawn.setOutlineThickness(2);
                pawn.setPosition(j * CELL_SIZE + CELL_SIZE / 2, i * CELL_SIZE + CELL_SIZE / 2);
                window.draw(pawn);
            }
        }
    }
}

void getPlayerMove(char* from, char* to) {
    printf("Entrer un coup: (ex: a3 b4): ");
    scanf("%s %s", from, to);
}

void handleAIMove(PawnType board[NUM_CELL][NUM_CELL], int curPlayer) {
    printf("Tour des %s\n", curPlayer == PAWN_WHITE ? "Blanc" : "Noir");
    Move bestMove = findBestMoveAI(board, curPlayer);
    makeMove(board, bestMove.row, bestMove.col, bestMove.toRow, bestMove.toCol);
}

int playGameCLI() {
    PawnType board[NUM_CELL][NUM_CELL];
    initBoard(board);

    int gameMode = 1;

    int curPlayer = PAWN_WHITE;
    char from[3], to[3];
    int fromRow, fromCol, toRow, toCol;

    while (checkWinner(board) == PAWN_NULL) {
        printBoard(board);

        if (curPlayer == PAWN_WHITE && gameMode == 1) {
            getPlayerMove(from, to);
            convertCoordinate(from, &fromRow, &fromCol);
            convertCoordinate(to, &toRow, &toCol);

            if (canCapture(board, fromRow, fromCol)) {
                Move captureMoves[NUM_CELL * NUM_CELL];
                int captureCount = getCaptureMoves(board, fromRow, fromCol, captureMoves);
                int validCapture = 0;
                for (int i = 0; i < captureCount; i++) {
                    if (captureMoves[i].toRow == toRow && captureMoves[i].toCol == toCol) {
                        validCapture = 1;
                        break;
                    }
                }
                if (!validCapture) {
                    printf("La prise est obligatoire.\n");
                    continue;
                }
            } else {
                int anyCapture = 0;
                for (int i = 0; i < NUM_CELL; i++) {
                    for (int j = 0; j < NUM_CELL; j++) {
                        if (board[i][j] == PAWN_WHITE && canCapture(board, i, j)) {
                            anyCapture = 1;
                            break;
                        }
                    }
                    if (anyCapture) break;
                }

                if (anyCapture) {
                    printf("La prise est obligatoire.\n");
                    continue;
                }

                if (!isValidMove(board, fromRow, fromCol, toRow, toCol)) {
                    printf("Coup invalide, re essayer.\n");
                    continue;
                }
            }

            makeMove(board, fromRow, fromCol, toRow, toCol);
            curPlayer = PAWN_BLACK;
        } else {
            printf("Tour des noirs (IA)...\n");
            Move bestMove = findBestMoveAI(board, curPlayer);
            if (canCapture(board, bestMove.row, bestMove.col)) {
                Move captureMoves[NUM_CELL * NUM_CELL];
                int captureCount = getCaptureMoves(board, bestMove.row, bestMove.col, captureMoves);
                int validCapture = 0;
                for (int i = 0; i < captureCount; i++) {
                    if (captureMoves[i].toRow == bestMove.toRow && captureMoves[i].toCol == bestMove.toCol) {
                        validCapture = 1;
                        break;
                    }
                }
                // on force l'ia a prendre si possible
                if (!validCapture) {
                    bestMove = captureMoves[0];
                }
            } else {
                int anyCapture = 0;
                for (int i = 0; i < NUM_CELL; i++) {
                    for (int j = 0; j < NUM_CELL; j++) {
                        if (board[i][j] == PAWN_BLACK && canCapture(board, i, j)) {
                            anyCapture = 1;
                            break;
                        }
                    }
                    if (anyCapture) break;
                }

                if (anyCapture) {
                    Move captureMoves[NUM_CELL * NUM_CELL];
                    bestMove = captureMoves[0];
                }
            }

            makeMove(board, bestMove.row, bestMove.col, bestMove.toRow, bestMove.toCol);
            curPlayer = PAWN_WHITE;
        }
    }

    printBoard(board);
    printf("Le gagnant est : %s\n", checkWinner(board) == PAWN_WHITE ? "Blanc" : "Noir");

    return 0;
}

int playGameGUI() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Jeu de dames");

    // Initialisation du plateau
    PawnType board[NUM_CELL][NUM_CELL];
    initBoard(board);
    int movesCounter = 0;
    std::vector<std::string> moves;

    // Initialisation d'une variable permettant de savoir à qui c'est le tour
    int curPlayer = PAWN_WHITE;

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    int fromRow, fromCol, toRow, toCol;
                    int prisePion = 0;
                    if (checkWinner(board) != PAWN_NULL) break;

                    int x = event.mouseButton.x / CELL_SIZE;
                    int y = event.mouseButton.y / CELL_SIZE;

                    // on évite les sorties
                    if (x < 0 || x >= NUM_CELL || y < 0 || y >= NUM_CELL) {
                        break;
                    }

                    if (curPlayer == PAWN_WHITE) {
                        if (movesCounter % 2 == 0) {
                            // Le coup est valide si la case cliquée est un pion blanc
                            if (board[y][x] == PAWN_WHITE || board[y][x] == QUEEN_WHITE) {
                                // Obtention de la lettre correspondante pour x
                                char letter = 'a' + x;

                                // Conversion des coordonnées en positions du tableau
                                std::stringstream ss;
                                ss << letter << y + 1;
                                moves.push_back(ss.str());

                                movesCounter++;
                            } else {
                                printf("La première case cliquée n'est pas un pion blanc. Veuillez réessayer.\n");
                            }
                        } else {
                            // Obtention de la lettre correspondante pour x
                            char letter = 'a' + x;

                            // Conversion des coordonnées en positions du tableau
                            std::stringstream ss;
                            ss << letter << y + 1;
                            moves.push_back(ss.str());

                            std::string from = moves[moves.size() - 2];
                            std::string to = moves[moves.size() - 1];

                            convertCoordinate(from.c_str(), &fromRow, &fromCol);
                            convertCoordinate(to.c_str(), &toRow, &toCol);

                            if (canCapture(board, fromRow, fromCol)) {
                            prisePion = 1;
                                Move captureMoves[NUM_CELL * NUM_CELL];
                                int captureCount = getCaptureMoves(board, fromRow, fromCol, captureMoves);
                                int validCapture = 0;
                                for (int i = 0; i < captureCount; i++) {
                                    if (captureMoves[i].toRow == toRow && captureMoves[i].toCol == toCol) {
                                        validCapture = 1;
                                        break;
                                    }
                                }
                                if (!validCapture) {
                                    printf("La prise est obligatoire.\n");
                                    // Faire en sorte qu'on ai un nombre impair de coups pour rejouer le premier coup
                                    movesCounter++;
                                    continue;
                                }
                            } else {
                                int anyCapture = 0;
                                for (int i = 0; i < NUM_CELL; i++) {
                                    for (int j = 0; j < NUM_CELL; j++) {
                                        if (board[i][j] == PAWN_WHITE && canCapture(board, i, j)) {
                                            anyCapture = 1;
                                            break;
                                        }
                                    }
                                    if (anyCapture) break;
                                }

                                if (anyCapture) {
                                    printf("La prise est obligatoire.\n");
                                    // Faire en sorte qu'on ai un nombre impair de coups pour rejouer le premier coup
                                    movesCounter++;
                                    continue;
                                }

                                if (!isValidMove(board, fromRow, fromCol, toRow, toCol)) {
                                    printf("Coup invalide, veuillez jouer un autre coup.\n");
                                    // Faire en sorte qu'on ai un nombre impair de coups pour rejouer le premier coup
                                    movesCounter++;
                                    continue;
                                }
                            }

                            makeMove(board, fromRow, fromCol, toRow, toCol);

                            if (canCapture(board, toRow, toCol) && prisePion == 1) {
                                printf("Une autre prise est possible. Jouez la prise obligatoire.\n");
                            } else {
                            curPlayer = PAWN_BLACK;

                            // Délai de 1 seconde avant de laisser l'IA jouer
                            window.clear();
                            drawBoard(window);
                            drawPawns(window, board);
                            window.display();
                            sf::sleep(sf::milliseconds(1000)); // Une seconde de délai
                            movesCounter++;
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        if (curPlayer == PAWN_BLACK && checkWinner(board) == PAWN_NULL) {
            printf("Tour des noirs (IA)...\n");
            Move bestMove = findBestMoveAI(board, curPlayer);
            if (canCapture(board, bestMove.row, bestMove.col)) {
                Move captureMoves[NUM_CELL * NUM_CELL];
                int captureCount = getCaptureMoves(board, bestMove.row, bestMove.col, captureMoves);
                int validCapture = 0;
                for (int i = 0; i < captureCount; i++) {
                    if (captureMoves[i].toRow == bestMove.toRow && captureMoves[i].toCol == bestMove.toCol) {
                        validCapture = 1;
                        break;
                    }
                }
                // on force l'IA à prendre si possible
                if (!validCapture) {
                    bestMove = captureMoves[0];
                }
            } else {
                int anyCapture = 0;
                for (int i = 0; i < NUM_CELL; i++) {
                    for (int j = 0; j < NUM_CELL; j++) {
                        if (board[i][j] == PAWN_BLACK && canCapture(board, i, j)) {
                            anyCapture = 1;
                            break;
                        }
                    }
                    if (anyCapture) break;
                }

                if (anyCapture) {
                    Move captureMoves[NUM_CELL * NUM_CELL];
                    bestMove = captureMoves[0];
                }
            }

            makeMove(board, bestMove.row, bestMove.col, bestMove.toRow, bestMove.toCol);
            curPlayer = PAWN_WHITE;
        }

        window.clear();

        // Dessiner le damier
        drawBoard(window);

        // Dessiner les pions
        drawPawns(window, board);

        window.display();
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 2 && strcmp(argv[1], "-gui") == 0) {
        playGameGUI();
    } else {
        playGameCLI();
    }

    return 0;
}
