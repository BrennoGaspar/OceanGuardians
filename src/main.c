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
int ESTADO = PARADO; 


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
Texture2D background;
Texture2D fire;
Font tituloFont;

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
void draw_menu(void);
void draw_gameplay(void);

void AtualizarJogador(Jogador *jogador, int teclaEsquerda, int teclaDireita, int teclaCima, int teclaBaixo, float delta);

/**
 * @brief Game entry point.
 */
int main( void ) {
    
    // antialiasing
    SetConfigFlags( FLAG_MSAA_4X_HINT );

    // creates a new window 800 pixels wide and 600 pixels high
    InitWindow( 800, 600, "Ocean Guardians - O Jogo" );

    // init audio device only if your game uses sounds
    //InitAudioDevice();

    // FPS: frames per second
    SetTargetFPS( 60 );

    // Load all game resources here
    background = LoadTexture( "resources/images/fundo.jpg" );
    fire = LoadTexture( "resources/images/fire.png" );
    tituloFont = LoadFont("resources/font/Asimovian-Regular.ttf");

    // game loop
    while ( !WindowShouldClose() ) {
        update( GetFrameTime() );
        draw();
    }

    // Unload resources before closing
    UnloadTexture(background);
    UnloadFont(tituloFont);
    UnloadTexture(fire);

    // close audio device only if your game uses sounds
    CloseWindow();

    return 0;

}

void update( float delta ) {
    if (ESTADO == PARADO) {

        if (IsKeyPressed(KEY_SPACE)) {
            ESTADO = RODANDO;
        }

    } else if (ESTADO == RODANDO) {
        
        // Lógica de movimentação do jogador no estado de jogo
        if (IsKeyDown(KEY_D)) {
            jogador.spriteX += 100 * delta;
        }

    }
}

void draw( void ) {

    BeginDrawing();
    ClearBackground( WHITE );

    if(ESTADO == PARADO) {
        draw_menu();
    } else if (ESTADO == RODANDO) {
        draw_gameplay();
    }

    EndDrawing();

}

void draw_menu(void) {

    // Fundo
    Rectangle sourceRec = { 0.0f, 0.0f, (float)background.width, (float)background.height };
    Rectangle destRec = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 origin = { 0.0f, 0.0f };
    Color transparentWhite = ColorAlpha(WHITE, 0.9f);
    DrawTexturePro(background, sourceRec, destRec, origin, 0.0f, transparentWhite);

    // Titulo e botao
    Vector2 tituloPos = { GetScreenWidth() / 2 - MeasureTextEx(tituloFont, "Ocean Guardian", 100, 1).x / 2, 100 };
    DrawTextEx(tituloFont, "Ocean Guardian", tituloPos, 100, 1, WHITE);
    
    DrawRectangle(310, 267, 180, 50, BLUE);
    int iniciarTextWidth = MeasureText("INICIAR", 30);
    DrawText("INICIAR", GetScreenWidth()/2 - iniciarTextWidth/2, 280, 30, WHITE);

    // Textos secundarios
    DrawRectangle( (GetScreenWidth() / 2 - 175), (GetScreenHeight() / 2 + 75), 370, 90, BLUE );
    DrawText("Aperte Q para descartar o lixo", GetScreenWidth()/2 - 150 , 390, 20, WHITE);
    DrawText("Aperte E para pegar o lixo", GetScreenWidth()/2 - 150 , 410, 20, WHITE);
    DrawText("WASD para movimentacao", GetScreenWidth()/2 - 150 , 430, 20, WHITE);

    Rectangle fireSourceRec = { 0, 0, (float)fire.width, (float)fire.height};
    Rectangle fireDestRec = { 5, GetScreenHeight() / 2 - 110, 250, 190 };
    Vector2 fireOrigin = { 0 , 0 };
    DrawTexturePro(fire, fireSourceRec, fireDestRec, fireOrigin, 0, WHITE);
    DrawText("Melhor", 75, GetScreenHeight()/2 - 20, 20, WHITE);
    DrawText("Pontuacao:", 75, GetScreenHeight()/2 - 5, 20, WHITE);
    DrawText("XXX", 105, GetScreenHeight()/2 + 20, 20, WHITE);

    DrawText("Desenvolvido por estudantes do segundo semestre de ciencia da computacao", 10, 580, 19, BLACK);

}

void draw_gameplay(void) {
    
    

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