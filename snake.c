#include <stdio.h>             // Biblioteca padrão para entrada e saída, utilizada para printf.
#include "pico/stdlib.h"       // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h"      // Biblioteca para controle do ADC (Conversor Analógico-Digital).
#include <time.h>
#include "stdlib.h"

#include "animation.c"

// Definições dos pinos para o joystick
#define VRX_PIN 26    // Define o pino GP26 para o eixo X do joystick (Canal ADC0).
#define VRY_PIN 27    // Define o pino GP27 para o eixo Y do joystick (Canal ADC1).
#define SW_PIN 22     // Define o pino GP22 para o botão do joystick (entrada digital).


#define TAM 5

#define JOYSTICK_MEIO 2047

enum OBJETOS {vazio = 0,cobra = 1, maca = 2} objetos;
enum DIRECAO {cima = 1,baixo = -1, direita = 2 , esquerda= -2} dir ;


int tabuleiro[TAM][TAM];

enum JOGO {morto = 0 ,vivo = 1 } vida;

int ticks;

typedef struct cobra{
    int x;
    int y;
    struct cobra *prox;
}No_Cobra;

No_Cobra *cabeca = NULL;


int snake_tam(){
    int count;
    for(int i=0; i <TAM;i++){
        for(int j = 0 ; j <TAM ; j++){
            if(tabuleiro[i][j] == cobra)count++;
        }
    }
    return count;
}

void generate_apple(){
    srand(time(0));
    int x = rand() % TAM;
    int y = rand() % TAM;
    while(tabuleiro[x][y] == cobra){
        x++;
        if(x == TAM){
            x=0;
            y++;
        }
        if(y == TAM){
            x=0;
            y=0;
        }
    }
    tabuleiro[x][y] = maca;
}

void add(int x, int y) {
  No_Cobra *novo = malloc(sizeof(No_Cobra));
  novo->x = x;
  novo->y = y;
  novo->prox = cabeca;
  cabeca = novo;
}

void remover() {
No_Cobra *aux ;
aux = cabeca;
  if (cabeca != NULL) {

    if(aux->prox == NULL){
        cabeca = NULL;
        free(aux);
        
    }else{
      while(aux->prox->prox != NULL){
        aux = aux->prox;
      }
      free(aux->prox->prox);
    }
    
    aux->prox = NULL;
  } else {
    cabeca = NULL;
    
  }
}

void gameover(){

    if(snake_tam() > 24){
        printf("\nVOCE VENCEU");
        animation_win();
    }else{
        printf("\nVOCE PERDEU\n");
        animation_death();
    }
    vida = morto;

    while(cabeca){
        remover();
    }
}

int mover(int direcao){
    int x ,y;
    x = cabeca->x;
    y=cabeca->y;
    printf("\ndirecao %d\n\n",direcao);
    switch (direcao)
    {
    case cima:
        x--;
        break;
    case baixo:
        x++;
        break;
    case direita:
        y++;
        break;
    case esquerda:
        y--;
        break;
    }
    if(x>=TAM|| x < 0 || y>=TAM|| y < 0 ){
        gameover();
        return -1;
    }
    add(x,y);
    switch (tabuleiro[x][y])
    {
    case vazio:
        remover();
        break;
    case cobra:
        gameover();
        break;
    case maca:
        break;
    }

    play_pacman_walk_sound();
    return 0;
}

void verifica(){
    for(int i = 0; i <TAM ; i++){
        for(int j = 0; j <TAM; j++){
            if(tabuleiro[i][j] != maca){
                tabuleiro[i][j] = vazio;
            }
        }
    }
    
    No_Cobra *aux;
    aux = cabeca;
    while(aux){
        tabuleiro[aux->x][aux->y] = cobra;
        aux = aux->prox;
    }
}

int direcao(int x, int y){
    int direcao = 0;
    direcao = 0;
    if( x == -1 || y == -1){
        printf("\n Valor X: ");
        scanf("%d",&x);
        printf("\n Valor Y: ");
        scanf("%d",&y);
    }


    if(abs(x-JOYSTICK_MEIO) > abs(y- JOYSTICK_MEIO) ){
        if(abs(x-JOYSTICK_MEIO) > 500){
            if(x-JOYSTICK_MEIO > 0){
                direcao = baixo;
            }
            if(x-JOYSTICK_MEIO < 0){
                direcao = cima;
            }
        }
        
    }
    if(abs(y-JOYSTICK_MEIO) > abs(x- JOYSTICK_MEIO) ){
        if(abs(y-JOYSTICK_MEIO) > 500){
            if(y-JOYSTICK_MEIO > 0){
                direcao = direita;
            }
            if(y-JOYSTICK_MEIO < 0){
                direcao = esquerda;
            }
        }
    }
    if((direcao + dir == 0 && snake_tam() > 1)|| direcao == 0 ) return dir; 
    dir = direcao;
    return direcao;
}

void show(){
    for(int i = 0;i<TAM;i++){
        for(int j = 0 ; j <TAM;j++){
            printf("%d |", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void set_matrix(){ 
    for (size_t i = 0; i < TAM; i++)
    {
        for (size_t j = 0; j < TAM; j++)
        {
            if(i%2 == 0){

                if(i == cabeca->x && TAM - j - 1 == cabeca->y ){
                    set_pixel_color(MATRIX[i][j], 20,20,0);
                }else{
                    switch (tabuleiro[i][TAM - j - 1])
                    {
                    case vazio:
                        set_pixel_color(MATRIX[i][j], 0,0,0);
                        break;
                    case cobra:
                        set_pixel_color(MATRIX[i][j], 20,0,0);
                        break;
                    case maca:
                        set_pixel_color(MATRIX[i][j], 0,20,0);
                        break;
                    }
                }
                
            }else{
                if(i == cabeca->x && j == cabeca->y ){
                    set_pixel_color(MATRIX[i][j], 50,50,0);
                }else{
                    switch (tabuleiro[i][j])
                    {
                    case vazio:
                        set_pixel_color(MATRIX[i][j], 0,0,0);
                        break;
                    case cobra:
                        set_pixel_color(MATRIX[i][j], 50,0,0);
                        break;
                    case maca:
                        set_pixel_color(MATRIX[i][j], 0,50,0);
                        break;
                    }
                }
                
            }
            
        }
        
    }
    
}


int tick(int x,int y){

    mover(direcao(x,y));
    if(!vida){
        return -1;
    }
    verifica();
    show();
    set_matrix();
    if(ticks >= 5){
      generate_apple();
      ticks = 0;  
    }else{
      ticks++;
    }
    return 0;
}

void dir_init(int x_inicial,int y_inicial){

    int direcao = rand() % 2 + 1;
    int sentido = rand() % 2;
    dir = sentido == 1? direcao: direcao*-1;

    if(x_inicial == 0 && dir == cima){
        dir = baixo;
    }
    if(x_inicial == TAM -1 && dir == baixo){
        dir = cima;
    }
    if(y_inicial == 0 && dir == esquerda){
        dir = direita;
    }
    if(y_inicial == TAM-1  && dir == direita){
        dir = esquerda;
    }
}

void init(){
    srand(time(0));
    vida = vivo;
    ticks = 0;
    int x_inicial = 2;
    int y_inicial = 2;
    for(int i = 0 ; i < TAM ; i++){
        for(int j = 0 ; j <TAM ; j++){
            tabuleiro[i][j] = vazio;
        }
    }
    tabuleiro[x_inicial][y_inicial] = cobra;
    add(x_inicial,y_inicial);
    dir_init(x_inicial,y_inicial);
}