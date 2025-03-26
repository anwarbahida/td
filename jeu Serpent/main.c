#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 600
#define CELL_SIZE 10
#define MAX_OBSTACLES 40
int num_choisi;
int obstacleX[MAX_OBSTACLES];
int obstacleY[MAX_OBSTACLES];
int numObstacles ;
int snakeX[100], snakeY[100];
int snakeLength = 4;
int directionX = 1, directionY = 0;
int foodX=0, foodY=0;
int score ;
int INITIAL_SPEED ;
int lives = 3;
char playerName[50];
bool gameOver = false;
bool isPaused = true;
int wallX = WINDOW_WIDTH / 2;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;


bool checkCollisionWithObstacles(int x, int y) {
    for (int i = 0; i < numObstacles; i++) {
        if (x == obstacleX[i] && y == obstacleY[i]) {
            return true;
        }
    }
    return false;
}
void generateFood() {
    foodX = (rand() % ((WINDOW_WIDTH - CELL_SIZE) / CELL_SIZE)) * CELL_SIZE;
    foodY = (rand() % ((WINDOW_HEIGHT - CELL_SIZE) / CELL_SIZE)) * CELL_SIZE;
    // Vérification des conditions
    while (
        foodX == 0 || foodX == WINDOW_WIDTH ||
        foodY == 0 || foodY == WINDOW_HEIGHT ||
        checkCollisionWithObstacles(foodX, foodY)||(foodX == wallX)
    ) {
        // Si les conditions ne sont pas remplies, régénérer les coordonnées
        foodX = (rand() % ((WINDOW_WIDTH - CELL_SIZE) / CELL_SIZE)) * CELL_SIZE;
        foodY = (rand() % ((WINDOW_HEIGHT - CELL_SIZE) / CELL_SIZE)) * CELL_SIZE;
    }
}

void initializeGame() {

    directionX = 1, directionY = 0;
    snakeX[0] = WINDOW_WIDTH / 2;
    snakeY[0] = WINDOW_HEIGHT / 2;

    generateFood();
    initializeObstacles();
}
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void load() {
    int r, q;
    gotoxy(40, 14);
    printf("\t\t");
    printf("\x1b[4;31mCHARGEMENT....\x1b[0m");
    gotoxy(24, 15);
    printf("\t\t\t");
    for (r = 1; r <= 25; r++) {
        for (q = 0; q <= 100000000; q++); // pour afficher le caractère lentement
            printf("%c", 178);
    }

    getch();
    system("cls");
    printf("\n \n");
    printf("\n\t\t\t\t \x1b[4;31m*****Choisir le niveau que vous voulez :*****\x1b[0m\n\n\n\n\n\x1b[34m--> 1 : Niveau facile \x1b[0m\n\n\n\x1b[34m--> 2 : Niveau moyen\x1b[0m\n\n\n\x1b[34m--> 3 : Niveau defficile\x1b[0m\n\n\n\x1b[34m--> 4 : Niveau tres defficile\x1b[0m\n\n\n\n\t\t\t\t\t\x1b[4;31mLe niveau que Vous avez Choisi est :\x1b[0m");
    scanf("%d",&num_choisi);
    if( num_choisi==1){
        numObstacles=14;
        INITIAL_SPEED=250;
    }
    else{
        if(num_choisi==2){
           numObstacles=18;
           INITIAL_SPEED=100;
        }
        else {
            if(num_choisi==3){
                    numObstacles=25;
                    INITIAL_SPEED=60;
            }
            else{
                if (num_choisi==4){
                        numObstacles=35;
                        INITIAL_SPEED=35;
                }
            }

        }
    }
    printf("\n\n\t\t\t\t\t\x1b[4;31mVeuillez saisir votre nom :\x1b[0m ");
    scanf("%s", playerName);

    system("cls");

    gotoxy(40, 14);
    printf("\t\t");
    printf("\x1b[4;31mCHARGEMENT....\x1b[0m");
    gotoxy(24, 15);
    printf("\t\t\t");
    for (r = 1; r <= 25; r++) {
        for (q = 0; q <= 100000000; q++); // pour afficher le caractère lentement
            printf("%c", 178);
    }
    getch();
}

void handleInput() {
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            gameOver = true;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    if (directionY != 1) {
                        directionX = 0;
                        directionY = -1;
                    }
                    break;
                case SDLK_DOWN:
                    if (directionY != -1) {
                        directionX = 0;
                        directionY = 1;
                    }
                    break;
                case SDLK_LEFT:
                    if (directionX != 1) {
                        directionX = -1;
                        directionY = 0;
                    }
                    break;
                case SDLK_RIGHT:
                    if (directionX != -1) {
                        directionX = 1;
                        directionY = 0;
                    }
                    break;
                 case SDLK_RETURN:  // Touche "Entrée" pour mettre en pause/reprendre le jeu
                    isPaused = !isPaused;
                    break;
                default:
                    break;
            }
        }
    }
}
void Print() {
    printf("\n\n\n\n\n\n");
    printf("\t Bienvenue au mini-jeu classique serpent.(appuyez sur n'importe quelle touche pour continuer)\n");
    getch();
    system("cls");
    printf("\n\n");
    printf("\t\t\t \x1b[4;31mInstructions du jeu ( pour vous aidez a bien comprendre la logique de jeu ) : \x1b[0m\n");
    printf("\n\n--> Utilisez les touches flechees pour deplacer le serpent.\n\n--> Vous trouverez de la nourriture dans la zone de serpent que vous devez manger.\n\n--> Chaque fois que vous mangez de la nourriture,la longueur du serpent augmente de 1 element.\n\n--> Vous disposez de trois vies.\n\n--> Votre vie diminuera si vous heurtez le frontiere ou le corps du serpent.\n\n--> Pour commencer le jeu , cliquer sur la zone de serpent");
    printf("\n\n--> Utilisez la touche  'Entree' pour faire une pause/continue.\n\n--> Vous trouverez les  anciennes resultats de tous les joueures.\n\n--> Vous trouverez votre position parmi les anciens joueurs .\n\n--> vous informez aussi si vous etes le majorant.\n\n");
    printf("\n\n\n\t\t\t\x1b[4;31mAppuyez sur n'importe quelle touche pour jouer au jeu...\x1b[0m");

    if (getch() == 27)
        exit(0);
}
int getScore() {
    return score;
}

void updateGame() {
    int prevX = snakeX[0];
    int prevY = snakeY[0];
    int tempX, tempY;

    snakeX[0] += directionX * CELL_SIZE;
    snakeY[0] += directionY * CELL_SIZE;
   // Vérifier la collision avec le mur (sauf la porte)
    if (snakeX[0] == wallX && !(snakeY[0] >= WINDOW_HEIGHT / 2 - CELL_SIZE && snakeY[0] + CELL_SIZE <= WINDOW_HEIGHT / 2 + CELL_SIZE)) {

        lives--;
        initializeGame();
        return;
    }



    for (int i = 0; i < numObstacles; i++) {
        if (snakeX[0] == obstacleX[i] && snakeY[0] == obstacleY[i]) {
            lives--;
            // INITIALISER LA POSITION DE SERPENT APRES LA COLLISION
            initializeGame();
            return;  //EXIT LA FONCTION LORSEQUE ON A UN COLLISION AVEC LES OBSTACLES
        }
    }

    //VERIFIER SI LE SERPENT A UN COLLISION AVEC LUI_MEME
    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
            lives--;
           // INITIALISER LA POSITION DE SERPENT APRES LA COLLISION
            initializeGame();
            return;  //EXIT LA FONCTION LORSEQUE ON A UN COLLISION AVEC LES OBSTACLES
        }
    }

    //VERIFIER SI LE SERPENT MONGE LA NOURRITURE

    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        snakeLength++;
        generateFood();

        score++;
    }

    // Update the position of the snake's body
    for (int i = 1; i < snakeLength; i++) {
        tempX = snakeX[i];
        tempY = snakeY[i];
        snakeX[i] = prevX;
        snakeY[i] = prevY;
        prevX = tempX;
        prevY = tempY;
    }

    // VERIFIER SI LE SERPENT A UNE COLLISION AVEC LE BORDURE
    if (snakeY[0] == 0) {
        lives--;
        //INITIALISER LA POSITION DE SERPENT APRES LA COLLISION
        initializeGame();
        return;
    }

    // VERIFIER SI LE SERPENT A UNE COLLISION AVEC LE BORDURE
    if (snakeY[0] == WINDOW_HEIGHT - CELL_SIZE) {
        lives--;
        // Reset the snake's position after collision
        initializeGame();
        return;
    }

    // VERIFIER SI LE SERPENT A UNE COLLISION AVEC LE BORDURE GAUCHE
    if (snakeX[0] == 0) {
        lives--;
        // INITIALISER LA POSITION DE SERPENT APRES LA COLLISION
        initializeGame();
        return;
    }

    // VERIFIER SI LE SERPENT A UNE COLLISION AVEC LE BORDURE DROITE
    if (snakeX[0] == WINDOW_WIDTH - CELL_SIZE) {
        lives--;
        // INITIALISER LA POSITION DE SERPENT APRES LA COLLISION
        initializeGame();
        return;
    }
    if (lives == 0) {
        printf("\n\n\n");
        printf("\t\x1b[34mGame Over\x1b[0m\n");
        gameOver = true;
        return;
    }
}
void renderGame() {
    // Fond vert pour la première moitié de la fenêtre
    SDL_SetRenderDrawColor(renderer, 0, 240, 160, 255);
    SDL_Rect firstHalfRect = {0, 0, wallX, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &firstHalfRect);

    // Fond BLUE pour la deuxième moitié de la fenêtre
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 230);
    SDL_Rect secondHalfRect = {wallX, 0, WINDOW_WIDTH - wallX, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &secondHalfRect);

    // Dessiner le mur vertical
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Couleur noire pour le mur
    SDL_Rect wallRect = {wallX, 0, CELL_SIZE, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &wallRect);


    // Dessiner la porte
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 230);
    SDL_Rect portalRect = {wallX , WINDOW_HEIGHT / 2 - CELL_SIZE * 2, CELL_SIZE * 7, CELL_SIZE * 4};
    SDL_RenderFillRect(renderer, &portalRect);
    renderObstacles();

    for (int i = 0; i < snakeLength; i++) {
    SDL_Rect snakeCell = {(int)snakeX[i], (int)snakeY[i], CELL_SIZE, CELL_SIZE};

    // Choisir une couleur différente pour la tête et le corps
    if (i == 0) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Couleur de la tête (blanc)
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Couleur du corps (vert)
    }

    // Ajustez la taille des cercles en fonction de la position dans le corps du serpent
    int circleRadius = CELL_SIZE / 2 + (i == 0 ? 4 : 0);  // La tête est plus grande

    // Dessinez un cercle au lieu d'un point
    for (int angle = 0; angle < 360; angle += 5) {
        double radianAngle = angle * M_PI / 180.0;
        int circleX = snakeCell.x + CELL_SIZE / 2 + (int)(circleRadius * cos(radianAngle));
        int circleY = snakeCell.y + CELL_SIZE / 2 + (int)(circleRadius * sin(radianAngle));
        SDL_RenderDrawPoint(renderer, circleX, circleY);
    }

    // Ajoutez les yeux pour la tête
    if (i == 0) {
        SDL_SetRenderDrawColor(renderer, 255,0,0, 255);  // Couleur des yeux (BLANC)
        int eyeRadius = 2;  // Ajustez la taille des yeux si nécessaire
        int eyeDistance = 5;  // Ajustez la distance entre les yeux si nécessaire

        // Dessinez les yeux du serpent
        for (int j = -1; j <= 1; j += 2) {
            int eyeX = snakeCell.x + CELL_SIZE / 2 + j * eyeDistance;
            int eyeY = snakeCell.y + CELL_SIZE / 3;
            for (int angle = 0; angle < 360; angle += 5) {
                double radianAngle = angle * M_PI / 180.0;
                int eyePosX = eyeX + (int)(eyeRadius * cos(radianAngle));
                int eyePosY = eyeY + (int)(eyeRadius * sin(radianAngle));
                SDL_RenderDrawPoint(renderer, eyePosX, eyePosY);
            }
        }
    }
}
SDL_RenderFillCircle(renderer, foodX, foodY, CELL_SIZE / 2, 255, 165, 0, 255);
    // Afficher le rendu
    SDL_RenderPresent(renderer);
}
void initializeObstacles() {
    for (int i = 0; i < numObstacles; i++) {
        obstacleX[i] = rand() % (WINDOW_WIDTH / CELL_SIZE) * CELL_SIZE;
        obstacleY[i] = rand() % (WINDOW_HEIGHT / CELL_SIZE) * CELL_SIZE;
    }
}

void renderObstacles() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Couleur NOIR pour les obstacles
    for (int i = 0; i < numObstacles; i++) {
        SDL_Rect obstacleRect = {obstacleX[i], obstacleY[i], CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &obstacleRect);
    }
}
void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (int w = 0; w <= radius * 2; w++) {
        for (int h = 0; h <= radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}
void saveScore(int score, const char* playerName) {
    FILE *file = fopen("score.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %d\n", playerName, score);
        fclose(file);
    } else {
        printf("Erreur lors de l'ouverture du fichier de score.\n");
    }
}
void displayScores() {
    int compteur =0;
    int joueur=0;
    FILE *file = fopen("score.txt", "r");
    if (file != NULL) {
        printf("\n\n\t\x1b[33mScores enregistres :\n");
        printf("\t---------------------\n");

        char playerName[50];
        int playerScore;

        while (fscanf(file, "%s %d", playerName, &playerScore) != EOF) {
            printf("\t%s : %d\n", playerName, playerScore);
            joueur++;
            if (playerScore<score){
                compteur ++;
            }
        }

        if (compteur ==0){
            printf("\n vous etes le dernier joueur ! \n ");
        }
        if (compteur+1  ==joueur){
            printf("\nvous avez la 1er score !!!!!!! Bravau \n");
        }

        else {
           printf("\n vous etes le meilleur que %d anciens joueurs \n", compteur);
        }
    }
}
void clearScores() {
    FILE *file = fopen("score.txt", "a");
    if (file != NULL) {
        fclose(file);
    } else {
        printf("Erreur lors de l'ouverture du fichier de score.\n");
    }
}
int main(int argc, char* args[]) {
    printf("    _______\n");    // tête du serpent
    printf("   /       \\\n");   // cou du serpent
    printf("  | (o) (o) |\n");   // première partie du corps
    printf("   \\  ___  /\n");     // queue du serpent

    // Corps du serpent (cercles répétés 40 fois)
    for (int i = 0; i < 5; ++i) {
        printf("    |  o  |\n");
    }


    Print();

    system("cls");

    load();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL n'a pas pu s'initialiser ! Erreur SDL : %s\n", SDL_GetError());

        return -1;
    }

    window = SDL_CreateWindow("Jeu du Serpent", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("La fenêtre n'a pas pu être créée ! Erreur SDL : %s\n", SDL_GetError());

        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Le rendu n'a pas pu être créé ! Erreur SDL : %s\n", SDL_GetError());

        return -1;
    }

    initializeGame();

    Uint32 lastTime = SDL_GetTicks();
    Uint32 deltaTime = 0;
    Uint32 speed = INITIAL_SPEED;

    int previousScore = -1;
    int previousLives = -1;

    while (!gameOver) {
        handleInput();
        if (!isPaused) {
        updateGame();
        }
        if (score != previousScore) {
            system("cls");
            printf("\n\n\t\x1b[33mScore: %d \n\tLives: %d\n\x1b[0m", score, lives);
            previousScore = score;
        }
        if (lives != previousLives) {
            system("cls");
            printf("\n\n\t\x1b[33mScore: %d \n\tLives: %d\n\x1b[0m", score, lives);
            previousLives = lives;

        }

        renderGame();
        deltaTime = SDL_GetTicks() - lastTime;
        if (deltaTime < speed) {
            SDL_Delay(speed - deltaTime);
        }
        lastTime = SDL_GetTicks();
    }
    saveScore(score, playerName);
    displayScores();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
