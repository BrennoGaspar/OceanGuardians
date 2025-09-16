/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Main function and logic for the game. Simplified template for game
 * development in C using Raylib (https://www.raylib.com/).
 * * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*---------------------------------------------
 * Library headers.
 *-------------------------------------------*/
#include "raylib/raylib.h"
//#include "raylib/raymath.h"

/*---------------------------------------------
 * Project headers.
 *-------------------------------------------*/

/*---------------------------------------------
 * Macros. 
 *-------------------------------------------*/

/*--------------------------------------------
 * Constants. 
 *------------------------------------------*/
const int PARADO = 0;
const int RODANDO = 1;
<<<<<<< Updated upstream
int ESTADO = PARADO; 
=======
int ESTADO = PARADO;
>>>>>>> Stashed changes


/*---------------------------------------------
 * Custom types (enums, structs, unions, etc.)
 *-------------------------------------------*/
typedef struct Jogador {
    int spriteX;
    int spriteY;
    float vel;
    Vector2 pos;
    Vector2 dim;
    Texture2D sprite;
} Jogador;

/*---------------------------------------------
 * Global variables.
 *-------------------------------------------*/
Jogador jogador;

/*---------------------------------------------
 * Function prototypes. 
 *-------------------------------------------*/
/**
 * @brief Reads user input and updates the state of the game.
 */
void update( float delta );
    
/**
 * @brief Draws the state of the game.
 */
void draw( void );
void AtualizarJogador(Jogador *jogador, int teclaEsquerda, int teclaDireita, int teclaCima, int teclaBaixo, float delta);

void AtualizarJogador(Jogador *jogador, int teclaEsquerda, int teclaDireita, int teclaCima, int teclaBaixo, float delta);

/**
 * @brief Game entry point.
 */
int main( void ) {
    
    // antialiasing
    SetConfigFlags( FLAG_MSAA_4X_HINT );

    // creates a new window 800 pixels wide and 450 pixels high
    InitWindow( 800, 600, "Ocean Guardians: The game" );

    // init audio device only if your game uses sounds
    //InitAudioDevice();

    // FPS: frames per second
    SetTargetFPS( 60 );     

    // you must load game resources here
    // Correção: Use uma variável temporária do tipo Image
    
    
    // Atribui a textura à struct Jogador
    
    // Agora que a textura foi criada, podemos descarregar a imagem da memória

    // 2. Definir a posição inicial do sprite
<<<<<<< Updated upstream
    jogador.spriteX = 100;
    jogador.spriteY = 100;
=======
    // jogador.spriteX = 100;
    // jogador.spriteY = 100;
>>>>>>> Stashed changes

    // game loop
    while ( !WindowShouldClose() ) {
        update( GetFrameTime() );
        draw();
    }

    // Unload the texture before closing
    UnloadTexture(jogador.sprite);
    
    // close audio device only if your game uses sounds
    //CloseAudioDevice();
    CloseWindow();

    return 0;

}

void update( float delta ) {
    // Lógica para mover o jogador
    if (IsKeyDown(KEY_D)) {
        jogador.spriteX += 100 * delta;
    }
}

void draw( void ) {

    Font titulo = LoadFont("resources/font/Asimovian-Regular.ttf");
    Vector2 tituloPos = {GetScreenWidth()/2 - 260 , 100}; 
    
<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
    BeginDrawing();
    
    ClearBackground( WHITE );

<<<<<<< Updated upstream
    //Tela Inicial do game
    if(ESTADO == PARADO){
        DrawTextEx(titulo, "Ocean Guardian", tituloPos , 100, 1, BLUE);
        DrawRectangle(310,280, 200, 90, BLUE);
        DrawText("INICIAR", GetScreenWidth()/2 - 50, 300, 30, GREEN);
        DrawText("APERTE ESPAÇO",GetScreenWidth()/2 - 80, 330, 20, GREEN);
        DrawText("Desenvolvido por estudantes do segundo semestre de ciencia da computacao", 10, 580,19,BLACK);
        DrawText("Melhor", 30, GetScreenHeight()/2 - 20, 20,RED);
        DrawText("Pontuacao", 30, GetScreenHeight()/2 - 5, 20, RED);
        DrawText("Aperte Q para descartar o lixo", GetScreenWidth()/2 - 150 , 390,20, BLACK);
        DrawText("Aperte E para pegar o lixo", GetScreenWidth()/2 - 150 , 410,20, BLACK);
        DrawText("WASD para movimentacao", GetScreenWidth()/2 - 150 , 430,20, BLACK);

=======
    // Desenha o sprite do jogador
    //DrawTexture(jogador.sprite, jogador.spriteX, jogador.spriteY, WHITE);

    //DrawTextureEx( jogador.sprite, (Vector2){ (float)jogador.spriteX, (float)jogador.spriteY }, 0.23f, 0.2f, WHITE );
    
    //Tela Inicial do game
    if(ESTADO == PARADO){
        DrawTextEx(titulo, "Ocean Guardian", tituloPos , 100, 1, BLUE);
        DrawRectangle(310,280, 200, 90, BLUE);
        DrawText("INICIAR", GetScreenWidth()/2 - 50, 300, 30, GREEN);
        DrawText("APERTE ESPAÇO",GetScreenWidth()/2 - 80, 330, 20, GREEN);
        DrawText("Desenvolvido por estudantes do segundo semestre de ciencia da computacao", 10, 580,19,BLACK);
        DrawText("Melhor", 30, GetScreenHeight()/2 - 20, 20,RED);
        DrawText("Pontuacao", 30, GetScreenHeight()/2 - 5, 20, RED);
        DrawText("Aperte Q para descartar o lixo", GetScreenWidth()/2 - 150 , 390,20, BLACK);
        DrawText("Aperte E para pegar o lixo", GetScreenWidth()/2 - 150 , 410,20, BLACK);
        DrawText("WASD para movimentacao", GetScreenWidth()/2 - 150 , 430,20, BLACK);

>>>>>>> Stashed changes
    }


    if(IsKeyPressed(KEY_SPACE)){
        ESTADO = RODANDO;
    }
    // Desenha o sprite do jogador

    //DrawTexture(jogador.sprite, jogador.spriteX, jogador.spriteY, WHITE);

    //DrawTextureEx( jogador.sprite, (Vector2){ (float)jogador.spriteX, (float)jogador.spriteY }, 0.23f, 0.2f, WHITE );
    
    DrawFPS( 20, 20 );

    EndDrawing();
}

void AtualizarJogador(Jogador *jogador, int teclaEsquerda, int teclaDireita, int teclaCima, int teclaBaixo, float delta){
        if ( IsKeyDown( teclaEsquerda ) ) {
            jogador->pos.x -= jogador->vel * delta;
        }

        if ( IsKeyDown( teclaDireita ) ) {
            jogador->pos.x += jogador->vel * delta;
        }   

        if ( jogador->pos.x < 0 ) {
            jogador->pos.x = 0;
        } else if ( jogador->pos.x + jogador->dim.x > GetScreenWidth() ) {
            jogador->pos.x = GetScreenWidth() - jogador->dim.x;
        }
    }