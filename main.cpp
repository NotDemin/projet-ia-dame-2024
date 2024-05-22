#include <stdio.h>
#include "board.h"
#include "game.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <string.h>

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

    int gameMode;
    printf("Choisissez le mode de jeu:\n1. Joueur contre IA\n2. IA contre IA\n");
    scanf("%d", &gameMode);

    // sanitize gamem ode
    if (gameMode != 1 && gameMode != 2) {
        gameMode = 1;
    }
    printf("Mode de jeu: %d\n", gameMode);

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
//            handleAIMove(board, curPlayer);
            curPlayer = PAWN_WHITE;
        }
//        curPlayer = (curPlayer == PAWN_WHITE) ? PAWN_BLACK : PAWN_WHITE;
    }


    printBoard(board);
    printf("Le gagnant est : %s\n", checkWinner(board) == PAWN_WHITE ? "Blanc" : "Noir");

    return 0;
}

int playGameGUI() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Jeu de dames");

    // on dessine la grille en 10x10
    sf::VertexArray hlines = sf::VertexArray(sf::Lines, 20);
    sf::VertexArray vlines = sf::VertexArray(sf::Lines, 20);

    for (int i = 0; i < (int)hlines.getVertexCount(); i += 2) {
        hlines[i].position = sf::Vector2f(0, i * 800 / hlines.getVertexCount());
        hlines[i + 1].position = sf::Vector2f(800, i * 800 / hlines.getVertexCount());
    }

    for (int i = 0; i < (int)vlines.getVertexCount(); i += 2) {
        vlines[i].position = sf::Vector2f(i * 800 / vlines.getVertexCount(), 0);
        vlines[i + 1].position = sf::Vector2f(i * 800 / vlines.getVertexCount(), 800);
    }

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event)) {

            switch (event.type) {

                case (sf::Event::Closed): {
                    window.close();
                    break;
                }

                case (sf::Event::KeyPressed): {

                }

                case (sf::Event::MouseButtonPressed): {


                    break;

                }
                default: {
                    break;
                }
            }
        }

        window.clear();

        window.draw(hlines);
        window.draw(vlines);

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
