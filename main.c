#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

#define NUMOBSTACULOS 6
#define LINHA1 1// era 1
#define LINHA2 25// era 20
#define COLUNA1 1// era 10
#define COLUNA2 80// era 70
#define LINHAINICIAL 4
#define COLUNAINICIAL 36
#define PORCENTAGEMMERGULHADORES 50
#define PORCENTAGEMSUBMARINOSINIMIGOS 50
#define ALTURASUBMARINO 3
#define COMPRIMENTOSUBMARINO 11
#define SUBMARINOINIMIGO 1
#define COMPRIMENTOMERGULHADOR 4
#define MERGULHADOR 2
#define DIREITA 1
#define ESQUERDA 0
#define INICIOAGUA 7
#define CAMINHOPORLOOP 3
#define METADEX COLUNA2 / 2
#define METADEY LINHA2 / 2

void menu();
void creditos();
void proMeio();
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

void imprime_submarino_inimigo(OBSTACULO submarino_inimigo) {
    if (submarino_inimigo.orientacao) {// eh diferente de 0 logo direita
        cputsxy(submarino_inimigo.posicao.X+5,submarino_inimigo.posicao.Y-2,"_");
        cputsxy(submarino_inimigo.posicao.X+1,submarino_inimigo.posicao.Y-1,"___| |___");
        cputsxy(submarino_inimigo.posicao.X,submarino_inimigo.posicao.Y,">\\________)");

    } else {// eh zero logo esquerda
        cputsxy(submarino_inimigo.posicao.X+5,submarino_inimigo.posicao.Y-2,"_");
        cputsxy(submarino_inimigo.posicao.X+1,submarino_inimigo.posicao.Y-1,"___| |___");
        cputsxy(submarino_inimigo.posicao.X,submarino_inimigo.posicao.Y,"(________/<");
    }
}

void imprime_mergulhador(OBSTACULO mergulhador) {
    if (mergulhador.orientacao) {// eh diferente de 0 logo direita
        cputsxy(mergulhador.posicao.X,mergulhador.posicao.Y,">->O");

    } else {// eh zero logo esquerda
        cputsxy(mergulhador.posicao.X,mergulhador.posicao.Y,"O<-<");
    }
}

void apaga_submarino_inimigo(OBSTACULO submarino_inimigo) {
    /*if (submarino_inimigo.orientacao) {// eh diferente de 0 logo direita
        cputsxy(submarino_inimigo.posicao.X+5,submarino_inimigo.posicao.Y-2," ");
        cputsxy(submarino_inimigo.posicao.X+1,submarino_inimigo.posicao.Y-1,"         ");
        cputsxy(submarino_inimigo.posicao.X,submarino_inimigo.posicao.Y,"           ");

    } else {// eh zero logo esquerda
        cputsxy(submarino_inimigo.posicao.X+5,submarino_inimigo.posicao.Y-2," ");
        cputsxy(submarino_inimigo.posicao.X+1,submarino_inimigo.posicao.Y-1,"         ");
        cputsxy(submarino_inimigo.posicao.X,submarino_inimigo.posicao.Y,"           ");
    }*/
    cputsxy(submarino_inimigo.posicao.X+5,submarino_inimigo.posicao.Y-2," ");
    cputsxy(submarino_inimigo.posicao.X+1,submarino_inimigo.posicao.Y-1,"         ");
    cputsxy(submarino_inimigo.posicao.X,submarino_inimigo.posicao.Y,"           ");
}

void apaga_mergulhador(OBSTACULO mergulhador) {
    cputsxy(mergulhador.posicao.X,mergulhador.posicao.Y,"    ");
}

void imprime_obstaculos (OBSTACULO *obstaculo) {
    int i;
    for (i = 0;i<NUMOBSTACULOS;i++) {
        if (obstaculo[i].tipo==SUBMARINOINIMIGO) {
            imprime_submarino_inimigo(obstaculo[i]);
        } else if (obstaculo[i].tipo==MERGULHADOR) {
            imprime_mergulhador(obstaculo[i]);       
        }
    }
}

void apaga_obstaculos (OBSTACULO *obstaculo) {
    int i;
    for (i = 0;i<NUMOBSTACULOS;i++) {
        if (obstaculo[i].tipo==SUBMARINOINIMIGO) {
            apaga_submarino_inimigo(obstaculo[i]);
        } else if (obstaculo[i].tipo==MERGULHADOR){
            apaga_mergulhador(obstaculo[i]);       
        }
    }
}



void atualiza_submarino_inimigo (OBSTACULO *submarino_inimigo) {
    if (submarino_inimigo->orientacao==ESQUERDA) {
        //submarino_inimigo->posicao.X -= COMPRIMENTOSUBMARINO;
        submarino_inimigo->posicao.X -= CAMINHOPORLOOP;
    } else {
        //submarino_inimigo->posicao.X += COMPRIMENTOSUBMARINO;
        submarino_inimigo->posicao.X += CAMINHOPORLOOP;
    }
}

void atualiza_mergulhador (OBSTACULO *mergulhador) {
    if (mergulhador->orientacao==ESQUERDA) {
        mergulhador->posicao.X -= CAMINHOPORLOOP;
    } else {
        mergulhador->posicao.X += CAMINHOPORLOOP;
    }
}

void atualiza_obstaculos(OBSTACULO *obstaculo) {
    int i;
    for(i = 0; i<NUMOBSTACULOS;i++) {
         if (obstaculo[i].tipo==SUBMARINOINIMIGO) {
            atualiza_submarino_inimigo(&obstaculo[i]);
        } else if (obstaculo[i].tipo==MERGULHADOR){
            atualiza_mergulhador(&obstaculo[i]);       
        }
    } 

}

void imprime_submarino(SUBMARINO submarino) {
    // 14 <-> YELLOW
    textcolor (YELLOW);// troca a cor do texto para amarelo
    if (submarino.orientacao) {// eh diferente de 0 logo direita
        cputsxy(submarino.posicao.X+5,submarino.posicao.Y-2,"_");
        cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"___| |___");
        cputsxy(submarino.posicao.X,submarino.posicao.Y,">\\________)");

    } else {// eh zero logo esquerda
        cputsxy(submarino.posicao.X+5,submarino.posicao.Y-2,"_");
        cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"___| |___");
        cputsxy(submarino.posicao.X,submarino.posicao.Y,"(________/<");
    }
    textcolor(WHITE);// troca a cor do texto para branco, volta a normal

}

void apaga_submarino(SUBMARINO submarino) {
    /*
    if (submarino.orientacao) {// eh diferente de 0 logo direita
        cputsxy(submarino.posicao.X+5,submarino.posicao.Y-2," ");
        cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"         ");
        cputsxy(submarino.posicao.X,submarino.posicao.Y,"           ");

    } else {// eh zero logo esquerda
        cputsxy(submarino.posicao.X+5,submarino.posicao.Y-2," ");
        cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"         ");
        cputsxy(submarino.posicao.X,submarino.posicao.Y,"           ");
    }*/
    cputsxy(submarino.posicao.X+5,submarino.posicao.Y-2," ");
    cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"         ");
    cputsxy(submarino.posicao.X,submarino.posicao.Y,"           ");

}

void gera_obstaculos(OBSTACULO *obstaculo) {
    int i;
    int probabilidade_tipo;
    int probabilidade_gerar;
    int orientacao;
    srand(time(0)); 
    for(i = 0; i<NUMOBSTACULOS;i++) {
        /*
        probabilidade_tipo = rand()%99+1;//rand()%99+1 retorna um numero entre 1 e 100 logo pode ser usado para porcentagem
         if (obstaculo[i].tipo==0) {// significa que o obstaculo n foi inicializado
            if (probabilidade_tipo<=PORCENTAGEMMERGULHADORES) {
                orientacao = rand()%2;
                obstaculo[i].tipo = MERGULHADOR;
                obstaculo[i].orientacao = orientacao;
                obstaculo[i].posicao.Y = INICIOAGUA+i*3;
                if (orientacao) {
                   obstaculo[i].posicao.X = 2; 
                } else {
                    obstaculo[i].posicao.X = 68;
                }
            } else if (probabilidade_tipo>PORCENTAGEMMERGULHADORES && probabilidade_tipo<=PORCENTAGEMMERGULHADORES+PORCENTAGEMSUBMARINOSINIMIGOS) {
                orientacao = rand()%2;
                obstaculo[i].tipo = SUBMARINOINIMIGO;
                obstaculo[i].orientacao = orientacao;
                obstaculo[i].posicao.Y = INICIOAGUA+i*3;
                if (orientacao) {
                   obstaculo[i].posicao.X = 2; 
                } else {
                    obstaculo[i].posicao.X = 68;
                }
            }
        }*/ 
        probabilidade_tipo = rand()%99+1;//rand()%99+1 retorna um numero entre 1 e 100 logo pode ser usado para porcentagem
         if (obstaculo[i].tipo==0) {// significa que o obstaculo n foi inicializado
            probabilidade_gerar = rand()%2;
            if (probabilidade_gerar) {
            if (probabilidade_tipo<=PORCENTAGEMMERGULHADORES) {
                orientacao = rand()%2;
                obstaculo[i].tipo = MERGULHADOR;
                obstaculo[i].orientacao = orientacao;
                obstaculo[i].posicao.Y = INICIOAGUA+i*3;
                if (orientacao) {
                   obstaculo[i].posicao.X = 2; 
                } else {
                    obstaculo[i].posicao.X = 68;
                }
            } else if (probabilidade_tipo>PORCENTAGEMMERGULHADORES) {
                orientacao = rand()%2;
                obstaculo[i].tipo = SUBMARINOINIMIGO;
                obstaculo[i].orientacao = orientacao;
                obstaculo[i].posicao.Y = INICIOAGUA+i*3;
                if (orientacao) {
                   obstaculo[i].posicao.X = 2; 
                } else {
                    obstaculo[i].posicao.X = 68;
                }
            }
        }    }
    } 
}

void testa_colisao_obstaculos_tela(OBSTACULO *obstaculos) {
    int i;
    for (i = 0; i<NUMOBSTACULOS;i++) {
        if (obstaculos[i].tipo==MERGULHADOR) {
            if (obstaculos[i].posicao.X+COMPRIMENTOMERGULHADOR-1>=COLUNA2 || obstaculos[i].posicao.X<=COLUNA1) {
                obstaculos[i].tipo = 0;
            }
        } else if (obstaculos[i].tipo==SUBMARINOINIMIGO) {
            if (obstaculos[i].posicao.X+COMPRIMENTOSUBMARINO-1>=COLUNA2 || obstaculos[i].posicao.X<=COLUNA1) {
                obstaculos[i].tipo = 0;
            }
        }
    }
}

void testa_colisao(SUBMARINO *submarino,OBSTACULO *obstaculos) {
    testa_colisao_obstaculos_tela(obstaculos);
    //testa_colisao_submarino_obstaculos(submarino,obstaculos);
}


// funcao que move o submarino(porra isso eh meio obvio)(mas ela faz o submarino pular posicoes,executando fica claro)
// acho q pular para cima e baixo esta certo mas para os lados incorreto, mas achei meio estranho mover uma so posicao para direita ou esquerda
// pois a tela eh muito pequena para andar tao rapido assim
// talvez a melhor solucao seja mover entre 1 e 11
void move_sub(SUBMARINO *submarino, OBSTACULO *obstaculos){// deixei ainda com dois switch por que vai ter 3 comandos esc espaco e setas
    // passei por ponteiro para alterar no da funcao principal

    // quando apertar <- ou -> se estiver virado para o lado oposto ele apenas virara para o lado certo se 
    // ja estiver virada para o lado certo ele "anda", fiz isso pq achei meio roubado virar e andar num movimento so
    // facil de tirar
    // obs: n foi proposital mas do jeito que esta fica impossivel caso esteja na borda esquerda virado para a esquerda
    //      e vira para a direita nao ha como virar novamente para a esquerda
    char a;
    int sair = 0;
    imprime_submarino(*submarino);
    do {
        Sleep(100);
        gera_obstaculos(obstaculos);
        if (kbhit()) {
            //fflush(stdin);
            a = getch();
            switch(a) {
                case -32:
                    a = getch();
                    switch(a) {// deixei as funcoes de apagar e imprimir dentro dos ifs para n ficar imprimindo e apagando quando desnecessario
                        case 72:
                            if (submarino->posicao.Y-2*ALTURASUBMARINO+1>LINHA1) {
                                apaga_submarino(*submarino);
                                submarino->posicao.Y -=ALTURASUBMARINO;
                                imprime_submarino(*submarino);
                            }
                            break;
                        case 80:
                            if (submarino->posicao.Y+ALTURASUBMARINO<LINHA2) {
                                apaga_submarino(*submarino);
                                submarino->posicao.Y +=ALTURASUBMARINO;
                                imprime_submarino(*submarino);
                            }
                            //y +=1;
                            break;
                        case 77:
                            // se deseja pular if (submarino->posicao.X+2*COMPRIMENTOSUBMARINO<COLUNA2) {
                            // se n deseja pular if (submarino->posicao.X+COMPRIMENTOSUBMARINO+1<COLUNA2) {
                            if (submarino->posicao.X+COMPRIMENTOSUBMARINO+CAMINHOPORLOOP-1<COLUNA2) {
                                // 2 vezes pois precisa verificar se a ponto (11) cabe na proxima posicao
                                // logo precisa verificar 22
                                apaga_submarino(*submarino);
                                if (submarino->orientacao==DIREITA) {
                                    submarino->posicao.X +=CAMINHOPORLOOP;
                                    // se n deseja pular submarino->posicao.X +=1;
                                    //submarino->posicao.X +=COMPRIMENTOSUBMARINO; se deseja pular
                                }else {
                                    submarino->orientacao = DIREITA;
                                }    
                                imprime_submarino(*submarino);
                            }
                            //x +=1;
                            break;
                        case 75:
                            // se deseja pular if (submarino->posicao.X-COMPRIMENTOSUBMARINO>COLUNA1) {    
                            // se n deseja pular if (submarino->posicao.X-1>COLUNA1) {  
                            if (submarino->posicao.X-CAMINHOPORLOOP>COLUNA1) {                              
                                apaga_submarino(*submarino);
                                if (submarino->orientacao==ESQUERDA) {
                                    submarino->posicao.X -=CAMINHOPORLOOP;
                                    // se n deseja pular submarino->posicao.X -=1;
                                    // submarino->posicao.X -=COMPRIMENTOSUBMARINO; se deseja pular
                                } else {
                                    submarino->orientacao = ESQUERDA;
                                }                                
                                imprime_submarino(*submarino);
                            }
                            //x -=1;
                            break;
                    }
                    break;
                case 27:
                    sair  = 1;
                    break;
            }
        }
        apaga_obstaculos(obstaculos);
        atualiza_obstaculos(obstaculos);
        testa_colisao(submarino,obstaculos);
        imprime_obstaculos(obstaculos);
    } while(sair==0 && submarino->vidas>0);
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


//funcao que colocar o cursor no meio da tela:
//o parametro linha serve para deslocar o cursor
// para baixo pelo numero de vezes que for passado
void proMeio(int linha){
    gotoxy(COLUNA2/ 2, (LINHA2 / 2) + linha);
}
//funcao menu: desenha o menu e chama a função de acordo com o
//numero selecionado
void menu(){
    char resp;
    clrscr();
    imprime_moldura();
    cputsxy(METADEX, METADEY, "[1]Novo Jogo");
    cputsxy(METADEX, METADEY + 1, "[2]Carregar Jogo");
    cputsxy(METADEX, METADEY + 2, "[3]Recordes");
    cputsxy(METADEX, METADEY + 3, "[4]Creditos");
    cputsxy(METADEX, METADEY + 4, "[5]Sair");
    resp = getch();
    switch(resp){
        case '1':
            //novoJogo();
            //imprime_moldura();
            //move_sub(&sub);
            break;
        case '2':
            //carregaJogo();
            break;
        case '3':
            //recordes();
            break;
        case '4':
            creditos();
            break;
        case '5':
            clrscr();
            printf("Espero que volte logo...");
            exit(0);
            break;

    }
}

void creditos(){
    char voltar;
    clrscr();
    imprime_moldura();
    cputsxy(METADEX, METADEY, "FEITO POR:");//ir para o meio da tela
    cputsxy(METADEX, METADEY + 1, "Eduardo Eugenio Kussler");
    cputsxy(METADEX, METADEY + 2, "E");
    cputsxy(METADEX, METADEY + 3, "Gabriel Couto");
    cputsxy(METADEX, METADEY + 6, "Pressione ESC para ");
    cputsxy(METADEX, METADEY + 7, "voltar ao menu principal");
    do{
        voltar = getch();
    }while(voltar != 27);
    clrscr();
    return menu();
}


int main() {
    OBSTACULO  obstaculos [NUMOBSTACULOS] = {};
    // o oxigenio depende do sleep entre os lacos
    // tem 30 segundos de oxigenio mas se o cada laço demora
    // meio segundo seria botar 60 de oxigenio e decrementar
    // 1 por laco
    SUBMARINO sub = {{COLUNAINICIAL,LINHAINICIAL},DIREITA,3,30,0,0};
    //char leitura ;
    //imprime_moldura();
    //menu();
    //le_tecla_imprime2();
    //move_sub(&sub);
    imprime_moldura();
    move_sub(&sub,obstaculos);
    return 0;
}
