#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define NUMOBSTACULOS 5// de 6 foi pra 5 pra ter espaco pra botar informacoes na tela
#define LINHA1 1// linha inicial da moldura
#define LINHA2 25// linha final da moldura
#define COLUNA1 1// coluna inicial da moldura
#define COLUNA2 80// coluna final da moldura
#define LINHAINICIAL 6// linha inicial do submarino controlado pelo jogador de 4 foi pra 7 necessario revisar condicao para deixar sub ir para cima?
#define COLUNAINICIAL 36// coluna inicial do submarino controlado pelo jogador
// provavelmente apenas uma das de baixo seram uteis pois se sei a porcentagem de um posso usar a de outro
// no codigo so usei porcentagme de mergulhadores
#define PORCENTAGEMMERGULHADORES 30// porcentagem de spawn de mergulhadores
#define PORCENTAGEMSUBMARINOSINIMIGOS 50// porcentagem de spawn dos submarinos inimigos

#define TEMPODELOOP 50

#define LINHAINTERFACESUPERIOR LINHA1+2
#define LINHAINTERFACEINFERIOR LINHA2-2

// talvez seja util mudar os valores das constantes abaixo e ajeitar o codigo
// ajeitar para mostrar a altura em si(2) e o comprimento(10)
#define ALTURASUBMARINO 3// nao seria altura em si mas significa que para escrever um submarino eh necessario 3 linhas
#define COMPRIMENTOSUBMARINO 11// nao seria comprimento em si mas significa que para escrever um submarino eh necessario 11 colunas

#define SUBMARINOINIMIGO 1// 1 eh o numero que indica que o obstaculo eh um submarino inimigo
#define MERGULHADOR 2// 2 indica que o obstaculo eh um mergulhador
#define SEMOBSTACULO 0// 0 indica que nao ha obstaculo na estrutura

// 4 colunas para escrever mergulhador talvez seja util trocar para tres pelo mesmo motivo da altura sub se quiser trocar la
#define COMPRIMENTOMERGULHADOR 4

#define DIREITA 1// foi convencionado que orientacao 1 eh direita
#define ESQUERDA 0// foi convencionado que orientacao 0 eh esquerda
// so tornei mais generico o inicioagua
#define INICIOAGUA LINHAINICIAL+ALTURASUBMARINO// o submarino pode estar dentro ou fora dagua, se  estiver em 7 ou abaixo esta abaixo da agua
#define CAMINHOPORLOOP 3// caminho andado por loop, a cade loop os obstaculos avancam 3 posicoes no caso
#define METADEX COLUNA2 / 2-3// alterei pra ficar mais no meio o texto
#define METADEY LINHA2 / 2
#define SPAWNESQUERDAOBSTACULO 2// se o obstaculo der spawn na esquerda seu x sera 2
#define SPAWNDIREITASUBMARINO 69// se o obstaculo der spawn na direita seu x sera 69
#define SPAWNDIREITAMERGULHADOR 76

//Definir a pontuacao para cada evento do jogo
#define SALVAMERGULHADOR 20
#define DESTRUIRSUBINIMIGO 10
#define PONTUACAOPORTEMPOSUBMERGIDO 1

// constantes para teclado
#define TECLASAUXILIARES -32
#define ENTER 13
#define ESC 27
#define SETABAIXO 80
#define SETACIMA 72
#define SETAESQUERDA 75
#define SETADIREITA 77

// constantes para menu
#define NOVOJOGO 0
#define CARREGARJOGO 1
#define RECORDES 2
#define CREDITOS 3
#define SAIR 4

#define MERGULHADORESMAXIMOS 3
#define VIDASINICIAIS 3
#define OXIGENIOMAXIMO 30*1000/TEMPODELOOP
#define BLOCOSDEOXIGENIO 1000/TEMPODELOOP
#define COLUNAVIDAS COLUNA2-9
#define COLUNAOXIGENIO COLUNA2-40-1-1

#define PONTUACAOSALVARMERGULHADORES 20

// se tem a estrutura mesmo e nao um ponteiro para acessar um atributo usa estrutura_mesmo.atributo ex: submarino_do_kuss_kussler.orientacao
// se tem um ponteiro para a estrutura usa estrutura_pointeiro->atributo ex: ponteiro_para_submarino_do_kuss_kussler->orientacao
// se tem um vetor de obstaculos por exemplo e quer acessar atributos de cada e isso esta dentro da funcao
// entao foi passado para a funcao um ponteiro para a primeira posicao do vetor
// entao para acessar um atributo de algum desses dentro de um for faz de 0 ate <NUMOBSTACULOS e bota obstaculos_kussler[i].orientacao

// to fingindo q sei o q to fazendo
// parece que j� tem o tipo COORDENADA na conio
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


typedef struct interface_jogo {
    int vidas;
    int mergulhadores;
    int oxigenio;
    int pontuacao;
} INTERFACEJOGO;


typedef struct torpedo {
    COORD posicao;
    int direcao;
    int status;
} TORPEDO;

void menu();
void creditos();
void proMeio();
void testa_colisao_submarino_obstaculos(SUBMARINO* submarino, OBSTACULO obstaculos[]);
void imprime_moldura();


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

void imprime_submarino_inimigo(OBSTACULO submarino_inimigo) {// imprime sub inimigo uso do if pois depende da orientacao
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

void imprime_mergulhador(OBSTACULO mergulhador) {// imprime mergulhador uso do if pois depende da orientacao
    if (mergulhador.orientacao) {// eh diferente de 0 logo direita
        cputsxy(mergulhador.posicao.X,mergulhador.posicao.Y,">->O");

    } else {// eh zero logo esquerda
        cputsxy(mergulhador.posicao.X,mergulhador.posicao.Y,"O<-<");
    }
}

void apaga_submarino_inimigo(OBSTACULO submarino_inimigo) {
    // apaga sub inimigo nao necessita if pois independente da orientacao tem que apagar mesmos espacos
    cputsxy(submarino_inimigo.posicao.X+5,submarino_inimigo.posicao.Y-2," ");
    cputsxy(submarino_inimigo.posicao.X+1,submarino_inimigo.posicao.Y-1,"         ");
    cputsxy(submarino_inimigo.posicao.X,submarino_inimigo.posicao.Y,"           ");
}

void apaga_mergulhador(OBSTACULO mergulhador) {
    // apaga mergulhador nao necessita if pois independente da orientacao tem que apagar mesmos espacos
    cputsxy(mergulhador.posicao.X,mergulhador.posicao.Y,"    ");
}

void imprime_obstaculos (OBSTACULO *obstaculo) {
    // passando um ponteiro para a pos inicial de um vetor de obstaculos imprime os obstaculos dele at� NUMOBSTACULOS
    // utilizacao do else if no final pois foi convencionado que se o tipo for 0 nao representa nenhum nem outro
    // ou seja nao foi inicializado ainda ou terminou sua rota
    int i;
    for (i = 0;i<NUMOBSTACULOS;i++) {
        if (obstaculo[i].tipo==SUBMARINOINIMIGO) {
            imprime_submarino_inimigo(obstaculo[i]);
        } else if (obstaculo[i].tipo==MERGULHADOR) {
            imprime_mergulhador(obstaculo[i]);
        }
    }
}

void imprime_agua() {
    int i;
    textcolor (CYAN);
    for (i = COLUNA1+1;i<COLUNA2;i++) {
        putchxy(i,LINHAINICIAL,'~');
    }
    textcolor (WHITE);
}


void apaga_obstaculos (OBSTACULO *obstaculo) {
    // passando um ponteiro para a pos inicial de um vetor de obstaculos apaga os obstaculos dele at� NUMOBSTACULOS
    // utilizacao do else if no final pois foi convencionado que se o tipo for 0 nao representa nenhum nem outro
    // ou seja nao foi inicializado ainda ou terminou sua rota
    int i;
    for (i = 0;i<NUMOBSTACULOS;i++) {
        if (obstaculo[i].tipo==SUBMARINOINIMIGO) {
            apaga_submarino_inimigo(obstaculo[i]);
        } else if (obstaculo[i].tipo==MERGULHADOR){
            apaga_mergulhador(obstaculo[i]);
        }
    }
}


/*
void atualiza_submarino_inimigo (OBSTACULO *submarino_inimigo) {
    if (submarino_inimigo->orientacao==ESQUERDA) {
        submarino_inimigo->posicao.X -= CAMINHOPORLOOP;
    } else {
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
*/


// as duas funcoes anteriores foram incorporadas em um so pois todos obstaculos andam o mesmo caminho por loop
void atualiza_obstaculo (OBSTACULO *obstaculo) {
    // atualiza a pos de um obstaculo ou seja se esta virado para esquerda anda o caminhoporloop para esquerda
    // se esta virado para a direita anda o caminhoporllop para direita
    if (obstaculo->orientacao) {// eh direita
        obstaculo->posicao.X += CAMINHOPORLOOP;
    } else {// se n direita eh esquerda
        obstaculo->posicao.X -= CAMINHOPORLOOP;
    }
}

void atualiza_obstaculos(OBSTACULO *obstaculo) {// atualiza a posicao dos obstaculos
    // atualiza a pos dos obstaculos ou seja se esta virado para esquerda anda o caminhoporloop para esquerda
    // se esta virado para a direita anda o caminhoporllop para direita
    int i;
    for(i = 0; i<NUMOBSTACULOS;i++) {
         if (obstaculo[i].tipo==SUBMARINOINIMIGO || obstaculo[i].tipo==MERGULHADOR) {
            atualiza_obstaculo(&obstaculo[i]);
        }
    }
}

void imprime_submarino(SUBMARINO submarino) {// imprime o sub do jogador
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

void apaga_submarino(SUBMARINO submarino) {// posicoes que devem ser apagadas n dependem da orientacao logo sem if
    // funcao que apaga sub
    cputsxy(submarino.posicao.X+5,submarino.posicao.Y-2," ");
    cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"         ");
    cputsxy(submarino.posicao.X,submarino.posicao.Y,"           ");

}

void animacao_sem_vidas(SUBMARINO *submarino,OBSTACULO *obstaculos) {// faz uma anima��o quando o jogador perde
    int i;
    apaga_obstaculos(obstaculos);// tira os obstaculos da tela
    while(submarino->posicao.Y<LINHA2-1) {// vai at� a linha antes da moldura
        for (i = 0;i<4;i++) {
            if (i%3) {
                if (submarino->posicao.X-1>COLUNA1) {
                    submarino->posicao.X--;
                }
            } else {
                if (submarino->posicao.X+COMPRIMENTOSUBMARINO<COLUNA2) {//submarino->posicao.X+1+COMPRIMENTOSUBMARINO-1
                    submarino->posicao.X++;
                }
            }
            imprime_submarino(*submarino);
            Sleep(100);
            apaga_submarino(*submarino);
        }
        submarino->posicao.Y+=ALTURASUBMARINO;
    }
    clrscr();
    cputsxy(METADEX,METADEY,"GAME OVER");
    getch();
}


// funcao que gera obstaculos
//rand()%100+1 retorna um numero entre 1 e 100 logo pode ser usado para porcentagem (poderia tirar o +1 mais eh mais intuivo)
// pensar com um numero entre 1 e 100 do que com 0 e 99 em termos de porcentagem
//na versao atual da funcao n precisa rand()%100+1 pois foi utilizado 50 por cento para mergulhadores e 50 para sub inimigos
// mas esta ai caso queira fazer um numero mais quebrado
void gera_obstaculos(OBSTACULO *obstaculo) {
    int i;
    int probabilidade_tipo;
    int probabilidade_gerar;
    int orientacao;
    srand(time(0));
    for(i = 0; i<NUMOBSTACULOS;i++) {
        probabilidade_tipo = rand()%100+1;
         if (obstaculo[i].tipo==SEMOBSTACULO) {
            probabilidade_gerar = rand()%2;
            if (probabilidade_gerar) {
                if (probabilidade_tipo<=PORCENTAGEMMERGULHADORES) {
                    orientacao = rand()%2;
                    obstaculo[i].tipo = MERGULHADOR;
                    obstaculo[i].orientacao = orientacao;
                    obstaculo[i].posicao.Y = INICIOAGUA+i*ALTURASUBMARINO;
                    if (orientacao) {
                       obstaculo[i].posicao.X = SPAWNESQUERDAOBSTACULO;
                    } else {
                        obstaculo[i].posicao.X = SPAWNDIREITAMERGULHADOR;
                    }
                } else if (probabilidade_tipo>PORCENTAGEMMERGULHADORES) {
                    orientacao = rand()%2;
                    obstaculo[i].tipo = SUBMARINOINIMIGO;
                    obstaculo[i].orientacao = orientacao;
                    obstaculo[i].posicao.Y = INICIOAGUA+i*3;
                    if (orientacao) {
                       obstaculo[i].posicao.X = SPAWNESQUERDAOBSTACULO;
                    } else {
                        obstaculo[i].posicao.X = SPAWNDIREITASUBMARINO;
                    }
                }
            }
        }
    }
}

// verifica se um obstaculo em sua nova posicao colide com a tela
// caso colida significa que o obstaculo completou sua rota ate o outro lado
// logo ele eh "deletado" e as linhas que ele ocupava esperam ate que seja gerado outro obstaculo
// o -1 foi inserido nos testes com a coluna2 pois por exemplo um mergulhador ocupa 4 colunas
// logo se ele esta na pos x ele ocupa x x+1 x+2 e x+3 logo o teste com a coluna 2 deve ser feito com x+3
// e como o comprimento do mergulhador foi feito como a quantidade de linhas necessaria para denhar ele (4)
// dai o x+3 eh x+comprimentomergulhador-1, mesma logica para o submarino
// por isso que foi comentado na definicao de constantes se deveria trocar o valor do comp do sub e do mergulhador
// por que dai n depende do -1
void testa_colisao_obstaculos_tela(OBSTACULO *obstaculos) {
    int i;
    for (i = 0; i<NUMOBSTACULOS;i++) {
        if (obstaculos[i].tipo==MERGULHADOR) {
            if (obstaculos[i].posicao.X+COMPRIMENTOMERGULHADOR-1>=COLUNA2 || obstaculos[i].posicao.X<=COLUNA1) {
                obstaculos[i].tipo = SEMOBSTACULO;
            }
        } else if (obstaculos[i].tipo==SUBMARINOINIMIGO) {
            if (obstaculos[i].posicao.X+COMPRIMENTOSUBMARINO-1>=COLUNA2 || obstaculos[i].posicao.X<=COLUNA1) {
                obstaculos[i].tipo = SEMOBSTACULO;
            }
        }
    }
}

// testa se houve colisao entre as bordas da tela e os obstaculos(o obstaculo completou sua rota logo eh apagado)
// eventualmente deve testar colisao entre submarino e obstaculos e entre misseis e obstaculos
void testa_colisao(SUBMARINO *submarino,OBSTACULO *obstaculos) {
    testa_colisao_obstaculos_tela(obstaculos);
    testa_colisao_submarino_obstaculos(submarino,obstaculos);
}

void switch_game_loop(char *a, SUBMARINO *submarino,OBSTACULO *obstaculos) {
    if (kbhit()) {
            //fflush(stdin);
            *a = getch();
            if (*a==TECLASAUXILIARES) {
                    *a = getch();
                    switch(*a) {// deixei as funcoes de apagar e imprimir dentro dos ifs para n ficar imprimindo e apagando quando desnecessario
                        case SETACIMA:
                            if (submarino->posicao.Y-2*ALTURASUBMARINO+1>LINHA1) {
                                apaga_submarino(*submarino);
                                submarino->posicao.Y -=ALTURASUBMARINO;
                                imprime_submarino(*submarino);
                                if (submarino->posicao.Y==LINHAINICIAL) {
                                    imprime_agua();
                                }
                            }
                            break;
                        case SETABAIXO:
                            if (submarino->posicao.Y+ALTURASUBMARINO<LINHAINTERFACEINFERIOR) {
                                apaga_submarino(*submarino);
                                if (submarino->posicao.Y==LINHAINICIAL) {
                                    imprime_agua();
                                }
                                submarino->posicao.Y +=ALTURASUBMARINO;
                                imprime_submarino(*submarino);
                            }
                            //y +=1;
                            break;
                        case SETADIREITA:
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
                                if (submarino->posicao.Y==LINHAINICIAL) {
                                    imprime_agua();
                                }
                            }
                            //x +=1;
                            break;
                        case SETAESQUERDA:
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
                                if (submarino->posicao.Y==LINHAINICIAL) {
                                    imprime_agua();
                                }
                            }
                            //x -=1;
                            break;
                    }
            }
        }
}

void imprime_interface_mergulhadores() {
    int j;/*
    for (i = 0; i<mergulhadores;i++) {
       cputsxy(COLUNA1+1+COMPRIMENTOMERGULHADOR*i,LINHAINTERFACEINFERIOR+1,">->O");
    }*/
    for (j = 0; j<MERGULHADORESMAXIMOS;j++) {
       cputsxy(COLUNA1+1+COMPRIMENTOMERGULHADOR*j,LINHAINTERFACEINFERIOR+1,"    ");
    }
}

void atualiza_interface_mergulhadores(SUBMARINO *submarino,INTERFACEJOGO *interface_jogo) {
    if (submarino->posicao.Y==LINHAINICIAL) {
        imprime_interface_mergulhadores();
        //submarino->mergulhadores = 0;
    } else {
        cputsxy(COLUNA1+1+COMPRIMENTOMERGULHADOR*interface_jogo->mergulhadores,LINHAINTERFACEINFERIOR+1,">->O");
    }
}

void imprime_interface_pontuacao() {
    gotoxy(COLUNA1+1,LINHAINTERFACESUPERIOR-1);
    printf("Pontos: ");
}


void atualiza_pontuacao(INTERFACEJOGO *interface_jogo) {
    //submarino->pontuacao += submarino->mergulhadores*20;
    cputsxy(COLUNA1+9,LINHAINTERFACESUPERIOR-1,"               ");
    gotoxy(COLUNA1+9,LINHAINTERFACESUPERIOR-1);
    printf("%d",interface_jogo->pontuacao);
    //cputsxy(COLUNA1+1,LINHAINTERFACESUPERIOR-1,);
}

void resgatou_mergulhadores(SUBMARINO *submarino) {
    if (submarino->mergulhadores && submarino->posicao.Y==LINHAINICIAL) {
        submarino->pontuacao += SALVAMERGULHADOR*submarino->mergulhadores;
        submarino->mergulhadores = 0;
    }
}

void atualiza_vidas(INTERFACEJOGO *interface_jogo) {
    cputsxy(COLUNAVIDAS+3*(interface_jogo->vidas-1),LINHAINTERFACESUPERIOR-1,"  ");
}

void atualiza_oxigenio(SUBMARINO *submarino,INTERFACEJOGO *interface_jogo) {

        if (interface_jogo->oxigenio/BLOCOSDEOXIGENIO<submarino->oxigenio/BLOCOSDEOXIGENIO) {// imprime oxigenio
            cputsxy(COLUNAOXIGENIO+11+interface_jogo->oxigenio/BLOCOSDEOXIGENIO,LINHAINTERFACEINFERIOR+1,"|");
        } else {// deletar oxigenio
            cputsxy(COLUNAOXIGENIO+11+interface_jogo->oxigenio/BLOCOSDEOXIGENIO,LINHAINTERFACEINFERIOR+1," ");
        }

        /*
        if (submarino->oxigenio%10==0) {
            cputsxy(COLUNAOXIGENIO+11+submarino->oxigenio/10,LINHAINTERFACEINFERIOR+1," ");
        }
        submarino->oxigenio--;
        if (submarino->oxigenio==0) {
            cputsxy(COLUNAOXIGENIO+11+submarino->oxigenio/10,LINHAINTERFACEINFERIOR+1," ");
        }
        submarino->oxigenio++;
        if (submarino->oxigenio%10==0) {
            cputsxy(COLUNAOXIGENIO+11+submarino->oxigenio/10,LINHAINTERFACEINFERIOR+1,"|");
        }*/
}


void atualiza_oxigenio_submarino(SUBMARINO *submarino) {
    if (submarino->posicao.Y!=LINHAINICIAL) {
        submarino->oxigenio--;
    } else if (submarino->oxigenio<OXIGENIOMAXIMO) {
        submarino->oxigenio++;
    }
}

void imprime_vidas(INTERFACEJOGO *interface_jogo) {
    int i;
    //int j;
    for (i = 0; i<interface_jogo->vidas;i++) {
       cputsxy(COLUNAVIDAS+3*i,LINHAINTERFACESUPERIOR-1,"<3 ");
    }/*
    for (j = i; j<VIDASINICIAIS;j++) {
       cputsxy(COLUNAVIDAS+3*j,LINHAINTERFACESUPERIOR-1,"  ");
    }*/
}

void imprime_oxigenio(INTERFACEJOGO *interface_jogo) {
    int i;
    //int j;
    int oxigenio = 30;
    putchxy(COLUNA2-1,LINHAINTERFACEINFERIOR+1,']');
    cputsxy(COLUNAOXIGENIO,LINHAINTERFACEINFERIOR+1,"Oxigenio: [");
    for (i = 0; i<OXIGENIOMAXIMO/BLOCOSDEOXIGENIO;i++) {
       cputsxy(COLUNAOXIGENIO+11+i,LINHAINTERFACEINFERIOR+1,"|");
    }
    /*
    for (j = i; j<OXIGENIOMAXIMO;j++) {
       cputsxy(COLUNAOXIGENIO+11+j,LINHAINTERFACEINFERIOR+1,"|");
    }*/
}


void atualiza_interface(SUBMARINO *submarino,INTERFACEJOGO *interface_jogo) {
    /*
    atualiza_oxigenio(submarino);
    if (submarino->posicao.Y==LINHAINICIAL) {
        atualiza_pontuacao(submarino);
        atualiza_interface_mergulhadores(submarino);
    }*/
    if (submarino->pontuacao!=interface_jogo->pontuacao) {
        interface_jogo->pontuacao = submarino->pontuacao;
        atualiza_pontuacao(interface_jogo);
    }
    if (submarino->oxigenio!=interface_jogo->oxigenio) {
        atualiza_oxigenio(submarino,interface_jogo);
        interface_jogo->oxigenio = submarino->oxigenio;
    }
    if (submarino->vidas!=interface_jogo->vidas) {
        atualiza_vidas(interface_jogo);
        interface_jogo->vidas = submarino->vidas;
    }
    if (submarino->mergulhadores!=interface_jogo->mergulhadores) {
        atualiza_interface_mergulhadores(submarino,interface_jogo);
        interface_jogo->mergulhadores = submarino->mergulhadores;
    }
}
/*
void imprime_interface_auxiliar() {
    putchxy(COLUNA2-1,LINHAINTERFACEINFERIOR+1,']');
    cputsxy(COLUNAOXIGENIO,LINHAINTERFACEINFERIOR+1,"Oxigenio: [");
    gotoxy(COLUNA1+1,LINHAINTERFACESUPERIOR-1);
    printf("Pontos: 0");
}*/


void imprime_interface(INTERFACEJOGO *interface_jogo) {
    //imprime_interface_auxiliar();
    imprime_interface_pontuacao();
    atualiza_pontuacao(interface_jogo);//
    imprime_oxigenio(interface_jogo);// imprime completo
    imprime_vidas(interface_jogo);
    //imprime_interface_mergulhadores();

}

// acho q pular para cima e baixo esta certo mas para os lados incorreto, mas achei meio estranho mover uma so posicao para direita ou esquerda
// pois a tela eh muito pequena para andar tao rapido assim
// talvez a melhor solucao seja mover entre 1 e 11
// funcao move o submarino e os obstaculos
// n vou comentar individualmente as condicoes para ver se o submarino atravessou a borda mas o raciocinio eh semelhante ao usado
// para ver se o submarino inimigo fez a mesma coisa
void game_loop(SUBMARINO *submarino, OBSTACULO *obstaculos){// deixei ainda com dois switch por que vai ter 3 comandos esc espaco e setas
    // passei por ponteiro para alterar no da funcao principal

    // quando apertar <- ou -> se estiver virado para o lado oposto ele apenas virara para o lado certo se
    // ja estiver virada para o lado certo ele "anda", fiz isso pq achei meio roubado virar e andar num movimento so
    // facil de tirar
    // obs: n foi proposital mas do jeito que esta fica impossivel caso esteja na borda esquerda virado para a esquerda
    //      e vira para a direita nao ha como virar novamente para a esquerda
    char a;
    INTERFACEJOGO interface_jogo = {submarino->vidas, submarino->mergulhadores, submarino->oxigenio, submarino->pontuacao};
    imprime_moldura();
    imprime_submarino(*submarino);// imprime o submarino inicialmente
    imprime_agua();
    imprime_interface(&interface_jogo);
    do {
        Sleep(TEMPODELOOP);// para dar um tempo entre loops
        gera_obstaculos(obstaculos);// funcao que gera obstaculos nas posicoes que n tem obstaculos
        apaga_obstaculos(obstaculos);// apaga os os obstaculos
        switch_game_loop(&a,submarino,obstaculos);
        atualiza_interface(submarino,&interface_jogo);
        // depois que faz o comando do submarino apaga os obstaculos na tela e atualiza as posicoes dos obstaculos
        // dai testa se alguma dessas novas posicoes bate na tela, ou seja se o obstaculo atravessou a tela
        // se esse for o caso esse obstaculo desaparece e imprime os obstaculos que ainda nao atravessaram
        /*
        apaga_obstaculos(obstaculos);
        imprime_submarino(*submarino);// gambiarra pro caso de um escapar do outro
        */
        atualiza_obstaculos(obstaculos);
        testa_colisao(submarino,obstaculos);
        imprime_obstaculos(obstaculos);
        resgatou_mergulhadores(submarino);
        atualiza_oxigenio_submarino(submarino);
        atualiza_interface(submarino,&interface_jogo);
    } while(a!=ESC && submarino->vidas>0 && submarino->oxigenio>0);
    if (submarino->vidas==0 || submarino->oxigenio==0) {
        animacao_sem_vidas(submarino,obstaculos);
    }
    clrscr();
}

void imprime_moldura() {// imprime a moldura do jogo
    int x,y;

    for(x = COLUNA1+1;x<COLUNA2;x++) {
            putchxy(x,LINHA1,'-');
            putchxy(x,LINHA2,'-');
            putchxy(x,LINHAINTERFACESUPERIOR,'-');
            putchxy(x,LINHAINTERFACEINFERIOR,'-');
    }
    for(y = LINHA1+1;y<LINHA2;y++) {
            putchxy(COLUNA1,y,'|');
            putchxy(COLUNA2,y,'|');
    }
    //printf("\n");


}

void imprime_moldura_menu() {
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

void novo_jogo() {
    OBSTACULO  obstaculos [NUMOBSTACULOS] = {};
    SUBMARINO sub = {{COLUNAINICIAL,LINHAINICIAL},DIREITA,VIDASINICIAIS,OXIGENIOMAXIMO,0,0};
    game_loop(&sub,obstaculos);
}


//funcao que colocar o cursor no meio da tela:
//o parametro linha serve para deslocar o cursor
// para baixo pelo numero de vezes que for passado
void proMeio(int linha){
    gotoxy(COLUNA2/ 2, (LINHA2 / 2) + linha);
}
//funcao menu: desenha o menu e chama a fun��o de acordo com o
//numero selecionado
void menu(){
    char resp;
    clrscr();
    imprime_moldura_menu();
    cputsxy(METADEX, METADEY, "[1]Novo Jogo");
    cputsxy(METADEX, METADEY + 1, "[2]Carregar Jogo");
    cputsxy(METADEX, METADEY + 2, "[3]Recordes");
    cputsxy(METADEX, METADEY + 3, "[4]Creditos");
    cputsxy(METADEX, METADEY + 4, "[5]Sair");
    resp = getch();
    switch(resp){
        case '1':
            novo_jogo();
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


void le_tecla_menu (char *tecla, int *opcao_atual){// funcao que deve ser usada com menu2
    *tecla = getch();
    if(*tecla==TECLASAUXILIARES) {
        *tecla = getch();
        if (*tecla==SETACIMA) {// se for cima e tiver como ir para cima vai para cima
            if(*opcao_atual!=NOVOJOGO) {
                putchxy(METADEX-1,METADEY+*opcao_atual,'\0');
                (*opcao_atual)--;
                putchxy(METADEX-1,METADEY+*opcao_atual,'|');
            }
        } else if (*tecla==SETABAIXO) {// se for baixo e tiver como ir para baixo vai para baixo
            if(*opcao_atual!=SAIR) {
                putchxy(METADEX-1,METADEY+*opcao_atual,'\0');
                (*opcao_atual)++;
                putchxy(METADEX-1,METADEY+*opcao_atual,'|');
            }
        }
    }
}

void menu2(){// outro menu
    char resp;
    int opcao;// novo jogo
    do {
        opcao = 0;
        clrscr();
        imprime_moldura_menu();
        cputsxy(METADEX, METADEY, "Novo Jogo");
        cputsxy(METADEX, METADEY + 1, "Carregar Jogo");
        cputsxy(METADEX, METADEY + 2, "Recordes");
        cputsxy(METADEX, METADEY + 3, "Creditos");
        cputsxy(METADEX, METADEY + 4, "Sair");
        putchxy(METADEX-1,METADEY,'|');// print inicial
        do {// fica atualizando posicao ate que de enter
            le_tecla_menu(&resp,&opcao);
        } while(resp!=ENTER);
        clrscr();// todas que chamar tem q limpar a tela
        switch(opcao){// ve qual a opcao que deu enter
            case NOVOJOGO:
                novo_jogo();
                //imprime_moldura();
                //move_sub(&sub);
                break;
            case CARREGARJOGO:
                //carregaJogo();
                break;
            case RECORDES:
                //recordes();
                break;
            case CREDITOS:
                creditos();
                break;
            case SAIR:
                printf("Espero que volte logo...");
                exit(0);
                break;

        }
    } while (opcao!=4);
}

void creditos(){
    char voltar;
    //clrscr();
    imprime_moldura_menu();
    cputsxy(METADEX, METADEY, "FEITO POR:");//ir para o meio da tela
    cputsxy(METADEX, METADEY + 1, "Eduardo Eugenio Kussler");
    cputsxy(METADEX, METADEY + 2, "E");
    cputsxy(METADEX, METADEY + 3, "Gabriel Couto Domingues");
    cputsxy(METADEX, METADEY + 6, "Pressione ESC para ");
    cputsxy(METADEX, METADEY + 7, "voltar ao menu principal");
    do{
        voltar = getch();
    }while(voltar != ESC);
    clrscr();
    //return menu();
}

//A fun��o recebe as coordenadas do submarino e as de um obstaculo
//e testa se est�o a menos de uma coordenada de distancia
//se estiver, retorna 1
//sen�o, retorna 0
int colidiu(COORD sub, COORD obstaculo, int tipo){//testa se houve colisao do submarino com alguma outra coisa

    if(tipo == MERGULHADOR){
        //testa se alguma parte do retangulo que representam o tamanho dos obst�culos
        //teve uma intersec��o com o retangulo que representa o tamanho do submarino aliado
        if((sub.Y == obstaculo.Y) && ((sub.X + COMPRIMENTOSUBMARINO - 1) >= obstaculo.X) && (sub.X <= (obstaculo.X + COMPRIMENTOMERGULHADOR - 1))){
            return 1;
        }else {
            return 0;
        }
        return 0;
    } else if(tipo == SUBMARINOINIMIGO){
        //
        if((sub.Y == obstaculo.Y) && ((sub.X + COMPRIMENTOSUBMARINO - 1) >= obstaculo.X) && (sub.X <= (obstaculo.X + COMPRIMENTOSUBMARINO - 1))){
            return 1;
        }else{
            return 0;
        }
    }
}
void testa_colisao_submarino_obstaculos(SUBMARINO* submarino, OBSTACULO obstaculos[]){
    int i;

    int colisao;
    for(i = 0; i < NUMOBSTACULOS; i++){
        colisao = colidiu((*submarino).posicao, obstaculos[i].posicao, obstaculos[i].tipo);
        if(colisao){
            if(obstaculos[i].tipo == SUBMARINOINIMIGO){
                (*submarino).vidas--;
                //atualiza_vidas(submarino->vidas);
                if((*submarino).vidas){
                    apaga_submarino(*submarino);
                    submarino->posicao.X = COLUNAINICIAL;
                    submarino->posicao.Y = LINHAINICIAL;
                    submarino->orientacao = DIREITA;
                    submarino->oxigenio = OXIGENIOMAXIMO;
                    submarino->mergulhadores = 0;
                    //imprime_oxigenio();
                    imprime_submarino(*submarino);
                    imprime_agua();
                }
            }else if(obstaculos[i].tipo == MERGULHADOR){
                apaga_mergulhador(obstaculos[i]);
                obstaculos[i].tipo = SEMOBSTACULO;
                if (submarino->mergulhadores<3) {
                    submarino->mergulhadores++;
                    //atualiza_interface_mergulhadores(submarino);
                }
                imprime_submarino(*submarino);
            }
        }
    }
}


int main() {
    /*
    OBSTACULO  obstaculos [NUMOBSTACULOS] = {};// inicializa tudo com 0
    // o oxigenio depende do sleep entre os lacos
    // tem 30 segundos de oxigenio mas se o cada la�o demora
    // meio segundo seria botar 60 de oxigenio e decrementar
    // 1 por laco
    SUBMARINO sub = {{COLUNAINICIAL,LINHAINICIAL},DIREITA,3,30,0,0};
    //char leitura ;
    //imprime_moldura();
    //menu();
    //le_tecla_imprime2();
    //move_sub(&sub);
    imprime_moldura();
    move_sub(&sub,obstaculos);*/

    menu2();
    return 0;
}




