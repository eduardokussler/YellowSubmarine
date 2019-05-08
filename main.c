#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <windows.h>

#define NUMOBSTACULOS 10
#define LINHA1 1// era 1
#define LINHA2 25// era 20
#define COLUNA1 1// era 10
#define COLUNA2 80// era 70
// to fingindo q sei o q to fazendo
// parece que já tem o tipo COORDENADA na conio
typedef struct submarino {
    COORD posicao;
    int orientacao;// 1 direita 0 esquerda?
    int vidas;
    int oxigenio;
    int pontuacao;
    int mergulhadores;
    // n botei a cor
} SUBMARINO;

typedef struct obstaculo {
    int tipo;//1: submarino 2: mergulhador
    COORD posicao;
    int orientacao;// 1 direita 0 esquerda?
    // n botei a cor
} OBSTACULO;


void missel_atualiza(int x,int y) {
    putchxy(x,y,'>');
}


void imprime_char_x_y() {
    char a;
    int x ,y;
    scanf("%c",&a);
    scanf("%d",&x);
    scanf("%d",&y);
    putchxy(x,y,a);
}

void le_tecla_imprime (){
    char a;

    //fflush(stdin);

    a = getch();
    switch(a) {
        case -32:
            a = getch();
            switch(a) {
                case 72:
                    printf("CIMA");
                    break;
                case 80:
                    printf("BAIXO");
                    break;
                case 77:
                    printf("DIREITA");
                    break;
                case 75:
                    printf("ESQUERDA");
                    break;
            }
            break;
        case 27:
            printf("ESC");
            break;
        default:
            printf("DESCONHECIDO");
    }
}

void le_tecla_imprime2 (){
    char a;
    int x,y;
    int misselx,missely;
    //int i = 1;
    int missel = 0;
    x = COLUNA1+1;
    y = LINHA1+1;
    int sair = 0;
    char letra = 'a';

    do {
        Sleep(50);
        if (missel) {
            if (misselx<COLUNA2-1) {
                    putchxy(misselx-1,missely,'\0');
                putchxy(misselx,missely,'>');
                //missel_atualiza(misselx+i,missely);
                misselx++;
            } else {
                missel = 0;
                //i = 0;
                putchxy(misselx-1,missely,'\0');
            }
        }
        putchxy(x,y,letra);
        if (kbhit()) {
        fflush(stdin);
        a = getch();
        switch(a) {
            case -32:
                a = getch();
                putchxy(x,y,'\0');
                switch(a) {
                    case 72:
                        if (y-1>LINHA1) {
                            y -=1;
                        }
                        break;
                    case 80:
                        if (y+1<LINHA2) {
                            y +=1;
                        }
                        //y +=1;
                        break;
                    case 77:
                        if (x+1<COLUNA2) {
                            x +=1;
                        }
                        //x +=1;
                        break;
                    case 75:
                        if (x-1>COLUNA1) {
                            x -=1;
                        }
                        //x -=1;
                        break;
                }
                break;
            case 27:
                sair  = 1;
                break;
            case 32:
                if (missel!=1) {
                    missel = 1;
                    misselx = x+1;
                    missely = y;
                }
                break;
            /*
            default:
                printf("DESCONHECIDO");
            */
        }
        }
    } while(sair==0);
    clrscr();
}

void imprime_moldura() {
    int x,y;

    for(x = COLUNA1+1;x<COLUNA2;x++) {
            putchxy(x,LINHA1,'-');
            putchxy(x,LINHA2,'-');
    }
    for(y = LINHA1+1;y<LINHA2;y++) {
            putchxy(COLUNA1,y,'|');
            putchxy(COLUNA2,y,'|');
    }
    //printf("\n");


}

int main() {
    OBSTACULO  obstaculos [NUMOBSTACULOS];
    // o oxigenio depende do sleep entre os lacos
    // tem 30 segundos de oxigenio mas se o cada laço demora
    // meio segundo seria botar 60 de oxigenio e decrementar
    // 1 por laco
    SUBMARINO sub = {{40,3},1,3,30,0,0};
    //char leitura ;
   imprime_moldura();
   //le_tecla_imprime2();

}
