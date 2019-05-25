#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define NUMOBSTACULOS 8// de 6 foi pra 5 pra ter espaco pra botar informacoes na tela
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

#define TEMPODELOOP 70

#define LINHAINTERFACESUPERIOR LINHA1+2
#define LINHAINTERFACEINFERIOR LINHA2-2

// talvez seja util mudar os valores das constantes abaixo e ajeitar o codigo
// ajeitar para mostrar a altura em si(2) e o comprimento(10)
#define ALTURASUBMARINO 2// nao seria altura em si mas significa que para escrever um submarino eh necessario 3 linhas
#define COMPRIMENTOSUBMARINO 11// nao seria comprimento em si mas significa que para escrever um submarino eh necessario 11 colunas

#define SUBMARINOINIMIGO 1// 1 eh o numero que indica que o obstaculo eh um submarino inimigo
#define MERGULHADOR 2// 2 indica que o obstaculo eh um mergulhador
#define SEMOBSTACULO 0// 0 indica que nao ha obstaculo na estrutura

// 4 colunas para escrever mergulhador talvez seja util trocar para tres pelo mesmo motivo da altura sub se quiser trocar la
#define COMPRIMENTOMERGULHADOR 4

//2 colunas para escrever o missil
#define COMPRIMENTOTORPEDO 2

#define DIREITA 1// foi convencionado que orientacao 1 eh direita
#define ESQUERDA 0// foi convencionado que orientacao 0 eh esquerda
#define NAODISPARADO 2

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
#define ESPACO 32

#define SEGUNDO 1000//milissegundos

// constantes para menu
#define NOVOJOGO 0
#define CARREGARJOGO 1
#define RECORDES 2
#define CREDITOS 3
#define SAIR 4

#define MERGULHADORESMAXIMOS 3
#define VIDASINICIAIS 3
#define OXIGENIOMAXIMO 30
//#define BLOCOSDEOXIGENIO (1000/TEMPODELOOP)
#define COLUNAVIDAS COLUNA2-9
#define COLUNAOXIGENIO COLUNA2-40-1-1

//#define PONTUACAOSALVARMERGULHADORES 20
#define PONTUACAODESTRUICAOSUBINIMIGO 10

#define PENALIDADETORPEDO 10
// se tem a estrutura mesmo e nao um ponteiro para acessar um atributo usa estrutura_mesmo.atributo ex: submarino_do_kuss_kussler.orientacao
// se tem um ponteiro para a estrutura usa estrutura_pointeiro->atributo ex: ponteiro_para_submarino_do_kuss_kussler->orientacao
// se tem um vetor de obstaculos por exemplo e quer acessar atributos de cada e isso esta dentro da funcao
// entao foi passado para a funcao um ponteiro para a primeira posicao do vetor
// entao para acessar um atributo de algum desses dentro de um for faz de 0 ate <NUMOBSTACULOS e bota obstaculos_kussler[i].orientacao

// to fingindo q sei o q to fazendo
// parece que jatem o tipo COORDENADA na conio
typedef struct  {
    COORD posicao;
    int orientacao;// 1 direita 0 esquerda?
    int vidas;
    int oxigenio;
    int pontuacao;
    int mergulhadores;
    unsigned long int tempo;
    unsigned long int tempo_agua;
    // n botei a cor
} SUBMARINO;

typedef struct  {
    int tipo;//1: submarino 2: mergulhador 3: torpedo
    COORD posicao;
    int orientacao;// 1 direita 0 esquerda?
    // n botei a cor
} OBSTACULO;


typedef struct  {
    int vidas;
    int mergulhadores;
    int oxigenio;
    int pontuacao;
    unsigned long int tempo;
} INTERFACEJOGO;


typedef struct  {
    COORD posicao;
    int status;//0 esquerda, 1 direita, 2 nao disparado
} TORPEDO;

void menu();
void creditos();
void proMeio();
void testa_colisao_submarino_obstaculos(SUBMARINO* submarino, OBSTACULO obstaculos[]);
void imprime_moldura();
void colisao_torpedo(TORPEDO*, OBSTACULO *, SUBMARINO*);

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
        //cputsxy(submarino_inimigo.posicao.X+5,submarino_inimigo.posicao.Y-2,"_");
        cputsxy(submarino_inimigo.posicao.X+1,submarino_inimigo.posicao.Y-1,"___|O|___");
        cputsxy(submarino_inimigo.posicao.X,submarino_inimigo.posicao.Y,">\\________)");

    } else {// eh zero logo esquerda
        //cputsxy(submarino_inimigo.posicao.X+5,submarino_inimigo.posicao.Y-2,"_");
        cputsxy(submarino_inimigo.posicao.X+1,submarino_inimigo.posicao.Y-1,"___|O|___");
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
    //cputsxy(submarino_inimigo.posicao.X+5,submarino_inimigo.posicao.Y-2," ");
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
        //cputsxy(submarino.posicao.X+5,submarino.posicao.Y-2,"_");
        cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"___|O|___");
        cputsxy(submarino.posicao.X,submarino.posicao.Y,">\\________)");

    } else {// eh zero logo esquerda
        //cputsxy(submarino.posicao.X+5,submarino.posicao.Y-2,"_");
        cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"___|O|___");
        cputsxy(submarino.posicao.X,submarino.posicao.Y,"(________/<");
    }
    textcolor(WHITE);// troca a cor do texto para branco, volta a normal

}

void apaga_submarino(SUBMARINO submarino) {// posicoes que devem ser apagadas n dependem da orientacao logo sem if
    // funcao que apaga sub
    //cputsxy(submarino.posicao.X+5,submarino.posicao.Y-2," ");
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
            Sleep(TEMPODELOOP);
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
                    obstaculo[i].posicao.Y = INICIOAGUA+i*ALTURASUBMARINO;
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

void atualiza_torpedo(TORPEDO *torpedo, SUBMARINO sub){
    if(torpedo->status == DIREITA){
        torpedo->posicao.X += CAMINHOPORLOOP;//adiciona a quantidade que o torpedo deve andar
    }else if(torpedo->status == ESQUERDA){
        torpedo->posicao.X -= CAMINHOPORLOOP;//subtrai a quantidade que o torpedo deve andar
    }
    if(torpedo->posicao.X <= COLUNA1 || torpedo->posicao.X >= COLUNA2){ // atingiu as bordas
        torpedo->status = NAODISPARADO; //seta o status para "Nao disparado" assim o jogador pode disparar outro torpedo
        //torpedo->posicao = sub.posicao;//reseta a posicao do torpedo
    }
}

void desenha_torpedo(TORPEDO *torpedo, SUBMARINO sub){
        if(torpedo->status == DIREITA){
            cputsxy(torpedo->posicao.X, torpedo->posicao.Y, "  ");//apaga o torpedo
            atualiza_torpedo(torpedo, sub);//atualiza a posicao do torpedo
            if(torpedo->status == DIREITA){//checa se ainda eh possivel desenhar o torpedo
                cputsxy(torpedo->posicao.X, torpedo->posicao.Y, "->");//desenha o torpedo
            }
        }else if(torpedo->status == ESQUERDA){
            cputsxy(torpedo->posicao.X, torpedo->posicao.Y, "  ");//apaga o torpedo
            atualiza_torpedo(torpedo, sub);//atualiza a posicao do torpedo
            if(torpedo->status == ESQUERDA){//checa se ainda eh possivel desenhar o torpedo
                cputsxy(torpedo->posicao.X, torpedo->posicao.Y, "<-");//desenha o torpedo em direcao a esquerda
            }
        }
}



void dispara_torpedo(SUBMARINO *submarino, TORPEDO *torpedo){
    if(submarino->posicao.Y >= INICIOAGUA){
        if(submarino->orientacao == DIREITA && torpedo->status == NAODISPARADO){
            torpedo->posicao = submarino->posicao;//torpedo esta na mesma posicao do submarino
            torpedo->posicao.X += COMPRIMENTOSUBMARINO;//o comprimento do submarino eh adicionado para que parte do submarino nao seja apagada
            torpedo->status = DIREITA;//seta a direcao para a qual o torpedo sera disparado
            submarino->oxigenio -= PENALIDADETORPEDO;// penalidade por usar o torpedo
            desenha_torpedo(torpedo, *submarino);//chamada da funcao que desenha o torpedo

        }else if(torpedo->status == NAODISPARADO){
            torpedo->posicao = submarino->posicao;//torpedo esta na mesma posicao do submarino
            torpedo->posicao.X -= 2;//duas posicoes sao subtraidas da posicao para que parte do submarino nao seja apagada
            torpedo->status = ESQUERDA;//seta a direcao para a qual o torpedo sera disparado
            submarino->oxigenio -= PENALIDADETORPEDO;// penalidade por usar o torpedo
            desenha_torpedo(torpedo, *submarino);//chamada da funcao que desenha o torpedo
        }
    }
}

void switch_game_loop(char *a, SUBMARINO *submarino,OBSTACULO *obstaculos, TORPEDO *torpedo) {

    if (kbhit()) {
            //fflush(stdin);
            *a = getch();
            if (*a==TECLASAUXILIARES) {
                    *a = getch();
                    switch(*a) {// deixei as funcoes de apagar e imprimir dentro dos ifs para n ficar imprimindo e apagando quando desnecessario
                        case SETACIMA:
                            if (submarino->posicao.Y-2*ALTURASUBMARINO+1>LINHAINTERFACESUPERIOR) {
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
            if(*a == 32){
                dispara_torpedo(submarino, torpedo);

            }
        }
}

void imprime_mergulhadores_vazio() {
    int j;/*
    for (i = 0; i<mergulhadores;i++) {
       cputsxy(COLUNA1+1+COMPRIMENTOMERGULHADOR*i,LINHAINTERFACEINFERIOR+1,">->O");
    }*/
    for (j = 0; j<MERGULHADORESMAXIMOS;j++) {
       cputsxy(COLUNA1+1+(COMPRIMENTOMERGULHADOR+1)*j,LINHAINTERFACEINFERIOR+1,"     ");
    }
}

void atualiza_interface_mergulhadores(SUBMARINO *submarino,INTERFACEJOGO *interface_jogo) {
    if (submarino->posicao.Y==LINHAINICIAL) {
        imprime_mergulhadores_vazio();
        //submarino->mergulhadores = 0;
    } else {
        cputsxy(COLUNA1+1+(COMPRIMENTOMERGULHADOR+1)*interface_jogo->mergulhadores,LINHAINTERFACEINFERIOR+1,">->O ");
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
        int i;
        if ((interface_jogo->oxigenio)<(submarino->oxigenio)) {// imprime oxigenio
            //cputsxy(COLUNAOXIGENIO+11+(interface_jogo->oxigenio+9)/BLOCOSDEOXIGENIO,LINHAINTERFACEINFERIOR+1,"|");
            for (i = interface_jogo->oxigenio; i < submarino->oxigenio ;i++) {
                cputsxy(COLUNAOXIGENIO+11+i,LINHAINTERFACEINFERIOR+1,"|");
            }
        } else if((interface_jogo->oxigenio)>(submarino->oxigenio)) {// deletar oxigenio
            //cputsxy(COLUNAOXIGENIO+11-1+(interface_jogo->oxigenio+9)/BLOCOSDEOXIGENIO,LINHAINTERFACEINFERIOR+1," ");
            for (i = interface_jogo->oxigenio; i > submarino->oxigenio ;i--) {
                cputsxy(COLUNAOXIGENIO+11-1+i,LINHAINTERFACEINFERIOR+1," ");
            }
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


void respawn_submarino(SUBMARINO *submarino) {
    if (submarino->vidas>0) {
        apaga_submarino(*submarino);
        submarino->posicao.X = COLUNAINICIAL;
        submarino->posicao.Y = LINHAINICIAL;
        submarino->orientacao = DIREITA;
        submarino->oxigenio = OXIGENIOMAXIMO;
        submarino->mergulhadores = 0;
        imprime_submarino(*submarino);
        imprime_agua();
    }
}


void testa_morte_oxigenio (SUBMARINO *submarino) {
    if (submarino->oxigenio==0 && submarino->vidas>0) {
        submarino->vidas--;
        respawn_submarino(submarino);
    }
}

void atualiza_oxigenio_submarino(SUBMARINO *submarino) {
    if (submarino->posicao.Y!=LINHAINICIAL) {
        //if ((submarino->tempo-submarino->tempo_agua)%SEGUNDO==0 && submarino->tempo_agua!=0 && submarino->tempo_agua!=submarino->tempo) {
       if ((submarino->tempo-submarino->tempo_agua)%SEGUNDO<TEMPODELOOP && submarino->tempo_agua!=0 && submarino->tempo_agua!=submarino->tempo) {
            submarino->oxigenio--;
        } else if (submarino->tempo_agua==0) {
            submarino->tempo_agua = submarino->tempo;
        }
    } else if (submarino->oxigenio<OXIGENIOMAXIMO) {
        if (submarino->tempo_agua!=0) {
            submarino->tempo_agua==0;
        }
        submarino->oxigenio = OXIGENIOMAXIMO;
    }/*
    if (submarino->posicao.Y!=LINHAINICIAL) {
        submarino->oxigenio--;
    } else if (submarino->oxigenio<OXIGENIOMAXIMO) {
        submarino->oxigenio++;
    }*/
}

void atualiza_pontuacao_submerso(SUBMARINO *submarino) {
    if (submarino->posicao.Y!=LINHAINICIAL) {
        //if ((submarino->tempo-submarino->tempo_agua)%SEGUNDO==0 && submarino->tempo_agua!=0 && submarino->tempo_agua!=submarino->tempo) {
       if ((submarino->tempo-submarino->tempo_agua)%SEGUNDO<TEMPODELOOP && submarino->tempo_agua!=0 && submarino->tempo_agua!=submarino->tempo) {
            submarino->pontuacao++;
       }
    }
}

void atualiza_tempo (SUBMARINO *submarino ) {
    submarino->tempo += TEMPODELOOP;
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
    for (i = 0; i<OXIGENIOMAXIMO;i++) {
       cputsxy(COLUNAOXIGENIO+11+i,LINHAINTERFACEINFERIOR+1,"|");
    }
    /*
    for (j = i; j<OXIGENIOMAXIMO;j++) {
       cputsxy(COLUNAOXIGENIO+11+j,LINHAINTERFACEINFERIOR+1,"|");
    }*/
}

void atualiza_interface_tempo (INTERFACEJOGO *interface_jogo) {
    gotoxy(38,2);// acho q nas horas n vai da problema
    printf("%02d:%02d:%02d",interface_jogo->tempo/1000/60/60,interface_jogo->tempo/1000/60,interface_jogo->tempo/1000%60);
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
    }/*
    if (submarino->tempo!=interface_jogo->mergulhadores) {
        atualiza_interface_mergulhadores(submarino,interface_jogo);
        interface_jogo->mergulhadores = submarino->mergulhadores;
    }*/
    if ((submarino->tempo-interface_jogo->tempo)%SEGUNDO<TEMPODELOOP) {// a diferença sempre sera !=0
        interface_jogo->tempo = submarino->tempo;
        atualiza_interface_tempo(interface_jogo);
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
    atualiza_interface_tempo(interface_jogo);
    //imprime_interface_mergulhadores();

}

// acho q pular para cima e baixo esta certo mas para os lados incorreto, mas achei meio estranho mover uma so posicao para direita ou esquerda
// pois a tela eh muito pequena para andar tao rapido assim
// talvez a melhor solucao seja mover entre 1 e 11
// funcao move o submarino e os obstaculos
// n vou comentar individualmente as condicoes para ver se o submarino atravessou a borda mas o raciocinio eh semelhante ao usado
// para ver se o submarino inimigo fez a mesma coisa
void game_loop(SUBMARINO *submarino, OBSTACULO *obstaculos, TORPEDO *torpedo){// deixei ainda com dois switch por que vai ter 3 comandos esc espaco e setas
    // passei por ponteiro para alterar no da funcao principal

    // quando apertar <- ou -> se estiver virado para o lado oposto ele apenas virara para o lado certo se
    // ja estiver virada para o lado certo ele "anda", fiz isso pq achei meio roubado virar e andar num movimento so
    // facil de tirar
    // obs: n foi proposital mas do jeito que esta fica impossivel caso esteja na borda esquerda virado para a esquerda
    //      e vira para a direita nao ha como virar novamente para a esquerda
    char a;
    INTERFACEJOGO interface_jogo = {submarino->vidas, submarino->mergulhadores, submarino->oxigenio, submarino->pontuacao,submarino->tempo};
    imprime_moldura();
    imprime_submarino(*submarino);// imprime o submarino inicialmente
    imprime_agua();
    imprime_interface(&interface_jogo);
    do {
        Sleep(TEMPODELOOP);// para dar um tempo entre loops
        atualiza_tempo (submarino);
        gera_obstaculos(obstaculos);// funcao que gera obstaculos nas posicoes que n tem obstaculos
        apaga_obstaculos(obstaculos);// apaga os os obstaculos
        switch_game_loop(&a,submarino,obstaculos, torpedo );
        //atualiza_interface(submarino,&interface_jogo);
        // depois que faz o comando do submarino apaga os obstaculos na tela e atualiza as posicoes dos obstaculos
        // dai testa se alguma dessas novas posicoes bate na tela, ou seja se o obstaculo atravessou a tela
        // se esse for o caso esse obstaculo desaparece e imprime os obstaculos que ainda nao atravessaram
        /*
        apaga_obstaculos(obstaculos);
        imprime_submarino(*submarino);// gambiarra pro caso de um escapar do outro
        */
        desenha_torpedo(torpedo, *submarino);
        colisao_torpedo(torpedo, obstaculos, submarino);
        //atualiza_torpedo(torpedo);
        atualiza_obstaculos(obstaculos);
        testa_colisao(submarino,obstaculos);
        imprime_obstaculos(obstaculos);
        resgatou_mergulhadores(submarino);
        atualiza_oxigenio_submarino(submarino);
        atualiza_pontuacao_submerso(submarino);
        atualiza_interface(submarino,&interface_jogo);
        testa_morte_oxigenio (submarino);
        //atualiza_interface(submarino,&interface_jogo);
    } while(a!=ESC && submarino->vidas>0);
    if (submarino->vidas==0) {
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
    TORPEDO torpedo = {sub.posicao, NAODISPARADO};
    game_loop(&sub,obstaculos, &torpedo);
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

void imprime_titulo() {
    int i = 20;
    int j = 13;
    textcolor(YELLOW);
    cputsxy(2+i,2, "___ ________ ____  ____  _____ __  __ ");
    cputsxy(2+i,3, "\\  |  /   __/  _/ /  _/ /  _  /   /  \\  ");
    cputsxy(2+i,4, " |   ||   __|  |--|  |--|  |  |  /\\  |  ");
    cputsxy(2+i,5, " \\___/\\_____\\_____\\_____\\_____\\__/\\__/");

    cputsxy(2+j,6, " _____ __ __ _____ __  __ _____ _____ ___ _____ _____ ");
    cputsxy(2+j,7, "/  ___/  |  /  _  /  \\/  /  _  /  _  /___/  _  /   __\\");
    cputsxy(2+j,8, "|___  |  |  |  _  |  \\/  |  _  |  _  |   |  |  |   __|");
    cputsxy(2+j,9, "<_____\\_____\\_____\\__ \\__\\__|__\\__|\\_\\___\\__|__\\_____/");
    textcolor(WHITE);
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
        imprime_titulo();
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

//A funcao recebe as coordenadas do submarino e as de um obstaculo
//e testa se estao a menos de uma coordenada de distancia
//se estiver, retorna 1
//senao, retorna 0
int colidiu(COORD sub, COORD obstaculo, int tipo){//testa se houve colisao do submarino com alguma outra coisa

    if(tipo == MERGULHADOR){
        //testa se alguma parte do retangulo que representam o tamanho dos obstaculos
        //teve uma interseccao com o retangulo que representa o tamanho do submarino aliado
        if((sub.Y == obstaculo.Y) && ((sub.X + COMPRIMENTOSUBMARINO - 1) >= obstaculo.X) && (sub.X <= (obstaculo.X + COMPRIMENTOMERGULHADOR - 1))){
            return 1;
        }else {
            return 0;
        }
        return 0;
    } else if(tipo == SUBMARINOINIMIGO){
        //testa se alguma parte do retangulo que representam o tamanho dos obstaculos
        //teve uma interseccao com o retangulo que representa o tamanho do submarino aliado
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
    for(i = 0; i < NUMOBSTACULOS; i++){//percorre o array de obstaculos
        colisao = colidiu(submarino->posicao, obstaculos[i].posicao, obstaculos[i].tipo); //testa se houve colisao do sub com algum obstaculo
        if(colisao){ // se houve colisao
            if(obstaculos[i].tipo == SUBMARINOINIMIGO){// se houve colisao e o obstaculo era sub inimigo
                submarino->vidas--;
                //atualiza_vidas(submarino->vidas);
                respawn_submarino(submarino);
                /*
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
                    respawn_submarino(submarino);
                }*/
            }else if(obstaculos[i].tipo == MERGULHADOR){ // se houve colisao e o obstaculo era um mergulhador
                apaga_mergulhador(obstaculos[i]);
                obstaculos[i].tipo = SEMOBSTACULO;//reseta o tipo do obstaculo
                if (submarino->mergulhadores < MERGULHADORESMAXIMOS) {//testa
                    submarino->mergulhadores++;
                    //atualiza_interface_mergulhadores(submarino);
                }
                imprime_submarino(*submarino);
            }
        }
    }
}

//Por enquanto so testa a colisao com subs inimigos
int colidiu_torpedo(COORD torpedo, COORD obstaculo){//testa se houve colisao do submarino com alguma outra coisa

    /*if(tipo == MERGULHADOR){
        //testa se alguma parte do retangulo que representam o tamanho dos obstaculos
        //teve uma interseccao com o retangulo que representa o tamanho do submarino aliado
        if((torpedo.Y == obstaculo.Y) && ((torpedo.X + COMPRIMENTOTORPEDO - 1) >= obstaculo.X) && (torpedo.X <= (obstaculo.X + COMPRIMENTOMERGULHADOR - 1))){
            return 1;
        }else {
            return 0;
        }
        return 0;
    } else if(tipo == SUBMARINOINIMIGO){
        //*/

        //testa se houve colisao do torpedo com um sub inimigo
        //Retorna 1 se houve
        //Retorna 0 se nao houve colisao
        if((torpedo.Y == obstaculo.Y) && ((torpedo.X + COMPRIMENTOTORPEDO - 1) >= obstaculo.X) && (torpedo.X <= (obstaculo.X + COMPRIMENTOSUBMARINO - 1))){
            return 1;
        }else{
            return 0;
        }
    }


void colisao_torpedo(TORPEDO *torpedo, OBSTACULO obstaculos[], SUBMARINO *sub){
    int i;
    for(i = 0; i < NUMOBSTACULOS; i++){//percorre o array de obstaculos
        if(obstaculos[i].tipo == SUBMARINOINIMIGO && (torpedo->status != NAODISPARADO)){
            if(colidiu_torpedo(torpedo->posicao, obstaculos[i].posicao)){//chama a funcao para testar se houve colisao
                torpedo->status = NAODISPARADO;//reseta o status do torpedo para que ele seja apagado e o jogador possa disparar outros
                apaga_submarino_inimigo(obstaculos[i]);//apaga o sub inimigo atingido
                obstaculos[i].tipo = SEMOBSTACULO;
                sub->pontuacao += PONTUACAODESTRUICAOSUBINIMIGO;//aumenta a pontuacao do jogador por ter destruido um sub
            }
        }
    }
}

int main() {
    /*
    OBSTACULO  obstaculos [NUMOBSTACULOS] = {};// inicializa tudo com 0
    // o oxigenio depende do sleep entre os lacos
    // tem 30 segundos de oxigenio mas se o cada laco demora
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
