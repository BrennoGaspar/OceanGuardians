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
#include "raylib/raymath.h" // Adicionado para Vector2 e funções relacionadas

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

const int LIXO_WIDTH = 40;
const int LIXO_HEIGHT = 40;

const int LIXEIRA_WIDTH = 70;
const int LIXEIRA_HEIGHT = 100;

/*---------------------------------------------
 * Custom types (enums, structs, unions, etc.)
 *-------------------------------------------*/
typedef enum TipoDoLixo {
    PLASTICO, VIDRO, METAL, PAPEL, NENHUM
} TipoDoLixo;

typedef struct Jogador {
    int spriteX;
    int spriteY;
    float vel;
    TipoDoLixo tipoLixo;
    int pontuacao;
    Vector2 pos;
    Vector2 dim;
    Texture2D sprite;
} Jogador;

typedef struct Lixo {
    Vector2 pos;
    TipoDoLixo type;
    Texture2D sprite;
    bool active; // Checa se o lixo esta na tela 
} Lixo;

typedef struct Lixeira {
    Rectangle rect;      // Posição e tamanho (será a hitbox)
    TipoDoLixo type;     // Tipo de lixo que ela aceita
    Texture2D sprite;    // A imagem da lixeira
} Lixeira;


/*---------------------------------------------
 * Global variables.
 *-------------------------------------------*/
Jogador jogador;
Texture2D background;
Texture2D fire;
Font tituloFont;
Texture2D papelLixo;
Texture2D vidroLixo;
Texture2D plasticoLixo;
Texture2D metalLixo; 
Texture2D frame;
Texture2D hand;

float tempoRestante = 300.0f; // tempo em segundos

#define MAX_LIXOS 20 // O maximo de lixos que podem aparecer na tela
#define NUM_LIXEIRAS 4

Lixo itensLixo[MAX_LIXOS]; // Array para os lixos
Texture2D spritesLixo[4]; // Array para os 4 tipos de lixo

Texture2D lixeiraPlastico;
Texture2D lixeiraVidro;
Texture2D lixeiraMetal;
Texture2D lixeiraPapel;
Lixeira lixeiras [NUM_LIXEIRAS];

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
    papelLixo = LoadTexture("resources/images/paperGarbage.png");
    vidroLixo = LoadTexture("resources/images/glassGarbage.png");
    plasticoLixo = LoadTexture("resources/images/plasticGarbage.png");
    metalLixo = LoadTexture("resources/images/metalGarbage.png");
    jogador.sprite = LoadTexture("resources/images/player.png");
    lixeiraPlastico = LoadTexture("resources/images/lixeira_plastico.png");
    lixeiraVidro = LoadTexture("resources/images/lixeira_vidro.png");
    lixeiraMetal = LoadTexture("resources/images/lixeira_metal.png");
    lixeiraPapel = LoadTexture("resources/images/lixeira_papel.png");
    frame = LoadTexture("resources/images/frame.png");
    hand = LoadTexture("resources/images/hand.png");
    
    jogador.pos = (Vector2){ GetScreenWidth()/2- 40, GetScreenHeight()/2 - 60 };
    jogador.dim = (Vector2){ 100, 100 }; //tamanho do mergulhador
    jogador.vel = 200; // velocidade do mergulhador
    jogador.tipoLixo = NENHUM;
    jogador.pontuacao = 0;

    spritesLixo[PLASTICO] = plasticoLixo;
    spritesLixo[VIDRO] = vidroLixo;
    spritesLixo[PAPEL] = papelLixo;
    spritesLixo[METAL] = metalLixo;

    //loop array lixo
    for (int i = 0; i < MAX_LIXOS; i++) {
       itensLixo[i].active = false;
    } 

    float startY = GetScreenHeight() - LIXEIRA_HEIGHT - 20; // 20 pixels de margem do fundo

    // Configura cada lixeira (tipo e sprite)
    lixeiras[PLASTICO] = (Lixeira){ .type = PLASTICO, .sprite = lixeiraPlastico };
    lixeiras[VIDRO] = (Lixeira){ .type = VIDRO, .sprite = lixeiraVidro };
    lixeiras[METAL] = (Lixeira){ .type = METAL, .sprite = lixeiraMetal };
    lixeiras[PAPEL] = (Lixeira){ .type = PAPEL, .sprite = lixeiraPapel };

    // Configura a posição e tamanho de cada lixeira
    for (int i = 0; i < NUM_LIXEIRAS; i++) {
        lixeiras[i].rect.x = 100 + i * (LIXEIRA_WIDTH + 100); // Posição X com espaçamento
        lixeiras[i].rect.y = startY;
        lixeiras[i].rect.width = LIXEIRA_WIDTH;
        lixeiras[i].rect.height = LIXEIRA_HEIGHT;
    }

    // game loop
    while ( !WindowShouldClose() ) {
        update( GetFrameTime() );
        draw();
    }

    // Unload resources before closing
    UnloadTexture(background);
    UnloadFont(tituloFont);
    UnloadTexture(fire);
    UnloadTexture(papelLixo);
    UnloadTexture(vidroLixo);
    UnloadTexture(plasticoLixo);
    UnloadTexture(metalLixo);
    UnloadTexture(lixeiraPlastico);
    UnloadTexture(lixeiraVidro);
    UnloadTexture(lixeiraMetal);
    UnloadTexture(lixeiraPapel);
    UnloadTexture(frame);
    UnloadTexture(hand);

    //Liberação da textura do mergulhador
    UnloadTexture(jogador.sprite);
    // close audio device only if your game uses sounds
    CloseWindow();

    return 0;

}

void update( float delta ) {
    if (ESTADO == PARADO) {
        
        // Botao iniciar
        Rectangle iniciar = { 310, 267, 180, 50 };
        bool isCollision = CheckCollisionPointRec( GetMousePosition(), iniciar );
        if( isCollision ){
            if( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ){
                ESTADO = RODANDO;
            }
        }
        
    } else if (ESTADO == RODANDO) {

        // cronometro 
        if( tempoRestante > 0 ) {
            tempoRestante -= GetFrameTime();
        } else {
            ESTADO = PARADO;
        }
        
        AtualizarJogador(&jogador, KEY_A, KEY_D, KEY_W, KEY_S, delta);

        Rectangle jogadorRec = { jogador.pos.x, jogador.pos.y, jogador.dim.x, jogador.dim.y };

        // Aperte E para pegar o lixo
        if( IsKeyPressed(KEY_E) ){
            for( int i = 0; i < MAX_LIXOS; i++ ){
                if( itensLixo[i].active ){
                    Rectangle lixoRec = { itensLixo[i].pos.x, itensLixo[i].pos.y, LIXO_WIDTH, LIXO_HEIGHT };
                    if( CheckCollisionRecs(jogadorRec, lixoRec) ){
                        jogador.tipoLixo = itensLixo[i].type;                    
                        itensLixo[i].active = false;
                        break;
                    }
                }
            }
        }

        // Aperte Q para descartar o lixo
        if( IsKeyPressed(KEY_Q) && jogador.tipoLixo != NENHUM ){
            for( int i = 0; i < NUM_LIXEIRAS; i++ ){
                Rectangle lixeiraRec = lixeiras[i].rect;
                if (CheckCollisionRecs(jogadorRec, lixeiraRec)) {
                    if (jogador.tipoLixo == lixeiras[i].type) {
                        printf("Lixo descartado corretamente na lixeira %d!\n", i);
                        jogador.pontuacao += 100;
                    } else {
                        printf("Tipo de lixo incorreto. Tente outra lixeira.\n");
                        jogador.pontuacao -= 50;
                    }
                    jogador.tipoLixo = NENHUM;
                    break;
                }
            }
        }
        
        // Aperte G para spawnar o lixo
        if(IsKeyPressed(KEY_G)){
            for(int i = 0; i < MAX_LIXOS; i++){
                if(!itensLixo[i].active){
                    itensLixo[i].active = true;
                    itensLixo[i].pos.x = GetRandomValue(50, GetScreenWidth() - 50); 
                    itensLixo[i].pos.y = GetRandomValue(200, GetScreenHeight() - 50);
                    
                    int tipoAleatorio = GetRandomValue(0, 3); 
                    itensLixo[i].type = (TipoDoLixo)tipoAleatorio;
                    itensLixo[i].sprite = spritesLixo[tipoAleatorio]; 
                    
                    break;
                }
            }
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
    
    // background
    Rectangle sourceRecBackground = { 0.0f, 0.0f, (float)background.width, (float)background.height };
    Rectangle destRecBackground = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 originBackground = { 0.0f, 0.0f };
    DrawTexturePro(background, sourceRecBackground, destRecBackground, originBackground, 0.0f, WHITE);

    // pontuacao
    DrawText( TextFormat( "%d", jogador.pontuacao ), 20, 17.5, 30, BLACK );

    // cronometro
    int minutos = (int)(tempoRestante / 60);
    int segundos = (int)(tempoRestante) % 60;
    DrawText( TextFormat( "%02d:%02d", minutos, segundos ), GetScreenWidth()/2 - 30, 15, 40, BLACK );

    // item na mao (frame)
    Rectangle frameSourceRec = { 0, 0, (float)frame.width, (float)frame.height };
    Rectangle frameDestRec = { GetScreenWidth() - 75, 10, 55, 55 }; 
    Vector2 frameOrigin = { 0, 0 };   
    DrawTexturePro(frame, frameSourceRec, frameDestRec, frameOrigin, 0, WHITE);

    // item na mao (lixo)
    if (jogador.tipoLixo != NENHUM) {
        Texture2D itemSprite = spritesLixo[jogador.tipoLixo];
        Rectangle itemSourceRec = { 0, 0, (float)itemSprite.width, (float)itemSprite.height };
        Rectangle itemDestRec = { GetScreenWidth() - 65, 20, 35, 35 };
        Vector2 itemOrigin = { 0, 0 };
        DrawTexturePro(itemSprite, itemSourceRec, itemDestRec, itemOrigin, 0, WHITE);
    } else {
        Rectangle handSourceRec = { 0, 0, (float)hand.width, (float)hand.height };
        Rectangle handDestRec = { GetScreenWidth() - 63, 22, 33, 33 }; 
        Vector2 handOrigin = { 0, 0 };   
        DrawTexturePro(hand, handSourceRec, handDestRec, handOrigin, 0, WHITE);
    }

    // mergulhador(player)
    Rectangle source = { 0.0f, 0.0f, (float)jogador.sprite.width, (float)jogador.sprite.height };
    Rectangle dest = { jogador.pos.x, jogador.pos.y, jogador.dim.x, jogador.dim.y };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(jogador.sprite, source, dest, origin, 0.0f, WHITE);
    
    // geracao do lixo na tela
    for (int i = 0; i < MAX_LIXOS; i++) {
        if (itensLixo[i].active) {
            Rectangle source = {0.0f, 0.0f, (float)itensLixo[i].sprite.width,
            (float)itensLixo[i].sprite.height };

            Rectangle dest = {itensLixo[i].pos.x, itensLixo[i].pos.y, LIXO_WIDTH, LIXO_HEIGHT};

            Vector2 origin = {0.0f, 0.0f};

            DrawTexturePro(itensLixo[i].sprite, source, dest, origin, 0.0f, WHITE);
        }
    }

    // desenho das lixeiras
    for (int i = 0; i < NUM_LIXEIRAS; i++) {
        Rectangle source = { 0.0f, 0.0f, (float)lixeiras[i].sprite.width, (float)lixeiras[i].sprite.height };
        DrawTexturePro(lixeiras[i].sprite, source, lixeiras[i].rect, (Vector2){0,0}, 0.0f, WHITE);
    }
}

// Função para movimento do jogador
void AtualizarJogador(Jogador *jogador, int teclaEsquerda, int teclaDireita, int teclaCima, int teclaBaixo, float delta){

    // Movimento do jogador
    if ( IsKeyDown( teclaEsquerda ) ) {
        jogador->pos.x -= jogador->vel * delta;
    }

    if ( IsKeyDown( teclaDireita ) ) {
        jogador->pos.x += jogador->vel * delta;
    } 

    if (IsKeyDown(teclaCima)) {
        jogador->pos.y -= jogador->vel * delta;
    }

    if (IsKeyDown(teclaBaixo)) {
        jogador->pos.y += jogador->vel * delta;
    } 

    // Verificação de limites para manter o jogador na tela
    // Limite esquerdo
    if ( jogador->pos.x < 0 ) {
        jogador->pos.x = 0;
    }
    
    // Limite direito
    if ( jogador->pos.x + jogador->dim.x > GetScreenWidth() ) {
        jogador->pos.x = GetScreenWidth() - jogador->dim.x;
    }

    // Limite superior
    if ( jogador->pos.y < 0 ) {
        jogador->pos.y = 0;
    }
    
    // Limite inferior
    if ( jogador->pos.y + jogador->dim.y > GetScreenHeight() ) {
        jogador->pos.y = GetScreenHeight() - jogador->dim.y;
    }
}
