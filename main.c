#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define NUMOBSTACULOS 8// numero de obstaculos do jogo, um obstaculo pode ser um submarino inimigo ou um mergulhador
#define NUMOBSTACULOSMENU (NUMOBSTACULOS-2)
#define LINHA1 1// linha inicial da moldura da tela
#define LINHA2 25// linha final da moldura da tela
#define COLUNA1 1// coluna inicial da moldura da tela
#define COLUNA2 80// coluna final da moldura da tela
#define LINHAINICIAL 6// linha inicial do submarino controlado pelo jogador
#define COLUNAINICIAL 36// coluna inicial do submarino controlado pelo jogador
#define COMPRIMENTOMENU 13// 13 linhas para escrever a maior string do menu carregar jogo

// provavelmente apenas uma das de baixo seram uteis pois se sei a porcentagem de um posso usar a de outro
// no codigo so usei porcentagme de mergulhadores
#define PORCENTAGEMMERGULHADORES 30// porcentagem de spawn de mergulhadores
#define PORCENTAGEMSUBMARINOSINIMIGOS 50// porcentagem de spawn dos submarinos inimigos

#define TEMPODELOOP 40//tempo em milissegundos entre as atualizações na tela durante o jogo
#define TEMPODELOOPMENU 60
#define TEMPODEMORTE 80//tempo em milissegundos entre as atualizações na tela durante a animacao sem vidas

#define LINHAINTERFACESUPERIOR LINHA1+2// linha que separa a linha inicial da moldura e a parte superior da interface do jogador
#define LINHAINTERFACEINFERIOR LINHA2-2// linha que separa a linha final da moldura e a parte inferior da interface do jogador

// talvez seja util mudar os valores das constantes abaixo e ajeitar o codigo
// ajeitar para mostrar a altura em si(1) e o comprimento(10)
#define ALTURASUBMARINO 2// nao seria altura em si mas significa que para escrever um submarino eh necessario 2 linhas
#define COMPRIMENTOSUBMARINO 11// nao seria comprimento em si mas significa que para escrever um submarino eh necessario 11 colunas

#define SUBMARINOINIMIGO 1// 1 eh o numero que indica que o obstaculo eh um submarino inimigo
#define MERGULHADOR 2// 2 indica que o obstaculo eh um mergulhador
#define SEMOBSTACULO 0// 0 indica que nao ha obstaculo na estrutura

// 4 colunas para escrever mergulhador talvez seja util trocar para tres pelo mesmo motivo da altura sub
#define COMPRIMENTOMERGULHADOR 4

//2 colunas para escrever o torpedo
#define COMPRIMENTOTORPEDO 2

#define DIREITA 1// foi convencionado que orientacao 1 eh direita
#define ESQUERDA 0// foi convencionado que orientacao 0 eh esquerda
#define NAODISPARADO 2// foi convencionado que 2 eh o numero que indica que o missel nao foi disparado

// so tornei mais generico o inicioagua
#define INICIOAGUA LINHAINICIAL+ALTURASUBMARINO// o submarino pode estar dentro ou fora dagua, se  estiver em INICIOAGUA ou abaixo esta abaixo da agua
#define CAMINHOPORLOOP 1// caminho andado por loop, a cade loop os obstaculos avancam 1 posicao no caso
#define METADEX COLUNA2 / 2-3// constante usada para printar texto aproximadamente no meio da tela
#define METADEY LINHA2 / 2// constante usada para printar texto aproximadamente no meio da tela
#define SPAWNESQUERDAOBSTACULO 2// se o obstaculo der spawn na esquerda seu x sera 2
#define SPAWNDIREITASUBMARINO 69// se o submarino inimigo der spawn na direita seu x sera 69
#define SPAWNDIREITAMERGULHADOR 76// se o mergulhador der spawn na direita seu x sera 76
#define BLOCOINICIALSPAWNMENU 2
#define BLOCOINICIALSPAWNJOGO 0

//Definir a pontuacao para cada evento do jogo
#define SALVAMERGULHADOR 20
#define DESTRUIRSUBINIMIGO 10
#define PONTUACAOPORTEMPOSUBMERGIDO 1
#define PONTUACAODESTRUICAOSUBINIMIGO 10
#define PENALIDADEMORTEMERGULHADOR 5

// constantes para teclado
#define TECLASAUXILIARES -32
#define ENTER 13
#define ESC 27
#define SETABAIXO 80
#define SETACIMA 72
#define SETAESQUERDA 75
#define SETADIREITA 77
#define ESPACO 32
#define BACKSPACE 8

#define SEGUNDO 1000//milissegundos

// constantes para menu
#define NOVOJOGO 0
#define CARREGARJOGO 1
#define RECORDES 2
#define CREDITOS 3
#define SAIR 4

#define MERGULHADORESMAXIMOS 5// numero maximo de mergulhadores que o submarino pode ter
#define VIDASINICIAIS 3// numero maximo de vidas
#define OXIGENIOMAXIMO 30// numero maximo de oxigenio
//#define BLOCOSDEOXIGENIO (1000/TEMPODELOOP)
#define COLUNAVIDAS COLUNA2-9
#define COLUNAOXIGENIO COLUNA2-40-1-1

/*
//#define PONTUACAOSALVARMERGULHADORES 20
#define PONTUACAODESTRUICAOSUBINIMIGO 10
#define PENALIDADEMORTEMERGULHADOR 5
*/

#define PENALIDADETORPEDO 5// penalidade no oxigenio por torpedo disparado
#define SALVANDOJOGO 100// milissegundos entre tentativas de salvar o jogo
#define MAXSTRINGNOME 9//8 letras e o \0
#define MAXSTRINGARQ (MAXSTRINGNOME+4)//nome do jogador e .bin e \0
#define NUMRECORDES 10 //Numero maximo de pontuacoes que sera mostrada na funcao recorde

#define CORAGUA BLUE
#define CORINTERFACE CYAN
#define CORFORADAGUA DARKGRAY
#define CORMENU CYAN

#define CONTINUAR 0
#define DESISTIR 1

#define SALVAR 2
#define NAOSALVAR 3

typedef struct  {
    char nome[MAXSTRINGNOME];// nome do jogador
    COORD posicao;// posicao do submarino
    int orientacao;// orientacao do submarino (ESQUERDA ou DIREITA)
    int vidas;// numero de vidas restantes do jogador
    int oxigenio;// quantidade de oxigenio restante
    int pontuacao;// pontuacao do jogador
    int mergulhadores;// numero de mergulhadores dentro do submarino atualmente
    unsigned long int tempo;// tempo sobrevivido em milissegundos
    unsigned long int tempo_agua;// guarda o tempo quando o submarino entrou na agua
    COLORS cor;
    // n botei a cor
} SUBMARINO;

typedef struct  {
    int tipo;//1: submarino 2: mergulhador
    COORD posicao;// posicao do obstaculo
    int orientacao;// orientacao do obstaculo(ESQUERDA ou DIREITA)
    COLORS cor;
} OBSTACULO;


typedef struct  {
    int vidas;// vidas atuais do jogador
    int mergulhadores;// mergulhadores atuais do jogador
    int oxigenio;// oxigenio atual do jogador
    int pontuacao;// pontuacao atual do jogador
    unsigned long int tempo;// tempo sobrevivido atual do jogador
} INTERFACEJOGO;


typedef struct  {
    COORD posicao;// posicao do torpedo
    int status;//0 esquerda, 1 direita, 2 nao disparado
} TORPEDO;

typedef struct{// estrutura utilizada para salvar e carregar jogo
    SUBMARINO submarino;
    OBSTACULO obstaculos[NUMOBSTACULOS];
    TORPEDO torpedo;
} JOGO;


void menu();
void creditos();
void proMeio();
void testa_colisao_submarino_obstaculos(SUBMARINO* submarino, OBSTACULO obstaculos[]);
void imprime_moldura();
void testa_colisao_torpedo(TORPEDO*, OBSTACULO *, SUBMARINO*);
int colidiu_torpedo_mergulhador (COORD torpedo,COORD obstaculo);
int colidiu_torpedo_submarino_inimigo (COORD torpedo,COORD obstaculo);
int colidiu_sub_mergulhador(COORD sub, COORD obstaculo );
int colidiu_sub_inimigo (COORD sub, COORD obstaculo );
void copia_vetor_obstaculos(OBSTACULO vetor1[],OBSTACULO vetor2[],int tam);
int guarda_estrutura(SUBMARINO submarino,OBSTACULO obstaculos[],TORPEDO torpedo);
void tenta_guardar_estrutura(SUBMARINO submarino,OBSTACULO obstaculos[],TORPEDO torpedo);
void le_tecla_menu (char *tecla, int *opcao_atual, int lim_superior,int lim_inferior);
void imprime_seta_inicial(int pos);
void imprime_opcoes_menu_salvar();
void preencherArquivo(FILE **arq);

void limpa_input() {
    while(kbhit()) {
        getch();
    }        
}

void pintar_tela() {
    int i,j;
    textbackground(CORMENU);
    for (i = LINHA1;i <= LINHA2; i++) {
        for (j = COLUNA1;j <= COLUNA2; j++) {
            putchxy(j,i,'\0');
        }
    }
}

void pintar_agua() {
    int i,j;
    textbackground(CORAGUA);
    for (i = LINHAINICIAL+1;i <= LINHAINTERFACEINFERIOR-1; i++) {
        for (j = COLUNA1;j <= COLUNA2; j++) {
            putchxy(j,i,'\0');
        }
    }
}


void pintar_fora_agua() {
    int i,j;
    textbackground(CORFORADAGUA);
    for (i = LINHAINTERFACESUPERIOR+1;i <= LINHAINICIAL; i++) {
        for (j = COLUNA1;j <= COLUNA2; j++) {
            putchxy(j,i,'\0');
        }
    }
}


void pintar_interface() {
    int i,j;
    textbackground(CORINTERFACE);
    for (i = LINHA1;i <= LINHAINTERFACESUPERIOR; i++) {
        for (j = COLUNA1;j <= COLUNA2; j++) {
            putchxy(j,i,'\0');
        }
    }

    for (i = LINHAINTERFACEINFERIOR;i <= LINHA2; i++) {
        for (j = COLUNA1;j <= COLUNA2; j++) {
            putchxy(j,i,'\0');
        }
    }
}

void pintar_jogo() {
    pintar_agua();
    pintar_fora_agua();
    pintar_interface();
    textbackground(CORAGUA);
}

void imprime_submarino(COORD posicao,int orientacao,int cor) {// imprime sub  uso do if pois depende da orientacao
    textcolor(cor);
    if (orientacao) {// eh diferente de 0 logo direita
        if (posicao.Y-1<LINHAINICIAL) {
            textbackground(CORFORADAGUA);
            cputsxy(posicao.X+1,posicao.Y-1,"___|O|___");
            textbackground(CORAGUA);
        } else {
            cputsxy(posicao.X+1,posicao.Y-1,"___|O|___");
        }
        cputsxy(posicao.X,posicao.Y,">\\________)");

    } else {// eh zero logo esquerda
        if (posicao.Y-1<LINHAINICIAL) {
            textbackground(CORFORADAGUA);
            cputsxy(posicao.X+1,posicao.Y-1,"___|O|___");
            textbackground(CORAGUA);
        } else {
            cputsxy(posicao.X+1,posicao.Y-1,"___|O|___");
        }
        cputsxy(posicao.X,posicao.Y,"(________/<");
    }
    textcolor(WHITE);
}

void imprime_mergulhador(COORD posicao,int orientacao,int cor) {// imprime mergulhador uso do if pois depende da orientacao
    textcolor(cor);
    if (orientacao) {// eh diferente de 0 logo direita
        cputsxy(posicao.X,posicao.Y,">->O");
    } else {// eh zero logo esquerda
        cputsxy(posicao.X,posicao.Y,"O<-<");
    }
    textcolor(WHITE);
}

void apaga_submarino_inimigo(OBSTACULO submarino_inimigo) {
    // apaga sub inimigo nao necessita if pois independente da orientacao tem que apagar mesmos espacos
    cputsxy(submarino_inimigo.posicao.X+1,submarino_inimigo.posicao.Y-1,"         ");
    cputsxy(submarino_inimigo.posicao.X,submarino_inimigo.posicao.Y,"           ");
}

void apaga_mergulhador(OBSTACULO mergulhador) {
    // apaga mergulhador nao necessita if pois independente da orientacao tem que apagar mesmos espacos
    cputsxy(mergulhador.posicao.X,mergulhador.posicao.Y,"    ");
}

void imprime_obstaculos (OBSTACULO *obstaculo) {
    // passando um ponteiro para a pos inicial de um vetor de obstaculos imprime os obstaculos dele ate NUMOBSTACULOS
    // utilizacao do else if no final pois foi convencionado que se o tipo for 0 nao representa nenhum nem outro
    // ou seja nao foi inicializado ainda ou terminou sua rota
    int i;
    for (i = 0;i<NUMOBSTACULOS;i++) {
        if (obstaculo[i].tipo==SUBMARINOINIMIGO) {
            imprime_submarino(obstaculo[i].posicao,obstaculo[i].orientacao,obstaculo[i].cor);
        } else if (obstaculo[i].tipo==MERGULHADOR) {
            imprime_mergulhador(obstaculo[i].posicao,obstaculo[i].orientacao,obstaculo[i].cor);
        }
    }
}

void imprime_agua() {// imprime a linha da agua
    int i;
    textcolor (CYAN);
    for (i = COLUNA1+1;i<COLUNA2;i++) {
        putchxy(i,LINHAINICIAL,'~');
    }
    textcolor (WHITE);
}


void apaga_obstaculos (OBSTACULO *obstaculo) {
    // passando um ponteiro para a pos inicial de um vetor de obstaculos apaga os obstaculos dele ate NUMOBSTACULOS
    // utilizacao do else if no final pois foi convencionado que se o tipo for 0 nao representa nenhum nem outro
    // ou seja nao foi inicializado ainda ou terminou sua rota, logo nao precisa ser apagado
    int i;
    for (i = 0;i<NUMOBSTACULOS;i++) {
        if (obstaculo[i].tipo==SUBMARINOINIMIGO) {
            apaga_submarino_inimigo(obstaculo[i]);
        } else if (obstaculo[i].tipo==MERGULHADOR){
            apaga_mergulhador(obstaculo[i]);
        }
    }
}


void atualiza_obstaculo (OBSTACULO *obstaculo) {//FUNCIONA PARA UM OBSTACULO INDIVIDUAL
    // atualiza a pos de um obstaculo ou seja se esta virado para esquerda anda o caminhoporloop para esquerda
    // se esta virado para a direita anda o caminhoporllop para direita
    if (obstaculo->orientacao) {// eh direita
        obstaculo->posicao.X += CAMINHOPORLOOP;
    } else {// se n direita eh esquerda
        obstaculo->posicao.X -= CAMINHOPORLOOP;
    }
}

void atualiza_obstaculos(OBSTACULO *obstaculo) {// atualiza a posicao dos obstaculos,FUNCIONA PARA O VETOR DE OBSTACULOS
    // atualiza a pos dos obstaculos ou seja se esta virado para esquerda anda o caminhoporloop para esquerda
    // se esta virado para a direita anda o caminhoporllop para direita
    int i;
    for(i = 0; i<NUMOBSTACULOS;i++) {
         if (obstaculo[i].tipo==SUBMARINOINIMIGO || obstaculo[i].tipo==MERGULHADOR) {
            atualiza_obstaculo(&obstaculo[i]);
        }
    }
}

void apaga_submarino(SUBMARINO submarino) {// posicoes que devem ser apagadas n dependem da orientacao logo sem if
    // funcao que apaga sub
    if (submarino.posicao.Y-1<LINHAINICIAL) {
        textbackground(CORFORADAGUA);
        cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"         ");
        textbackground(CORAGUA);
    } else {
        cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"         ");
    }
    cputsxy(submarino.posicao.X,submarino.posicao.Y,"           ");

}

void animacao_sem_vidas(SUBMARINO *submarino,OBSTACULO *obstaculos) {// faz uma animacao quando o jogador perde
    int i;
    apaga_obstaculos(obstaculos);// tira os obstaculos da tela
    //while(submarino->posicao.Y<LINHA2-1) {// vai ate a linha antes da moldura
    while(submarino->posicao.Y<LINHAINTERFACEINFERIOR-1) {// vai ate a linha antes da moldura
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
            imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);
            Sleep(TEMPODEMORTE);
            apaga_submarino(*submarino);
        }
        submarino->posicao.Y+=ALTURASUBMARINO;
        MessageBeep(MB_ICONWARNING);
    }
    
    limpa_input();
    pintar_tela();
    clrscr();
    cputsxy(METADEX,METADEY,"GAME OVER");
    getch();
}


// funcao que gera obstaculos
//rand()%100+1 retorna um numero entre 1 e 100 logo pode ser usado para porcentagem (poderia tirar o +1 mais eh mais intuivo)
// pensar com um numero entre 1 e 100 do que com 0 e 99 em termos de porcentagem
//na versao atual da funcao n precisa rand()%100+1 pois foi utilizado 50 por cento para mergulhadores e 50 para sub inimigos
// mas esta ai caso queira fazer um numero mais quebrado
void gera_obstaculos(OBSTACULO *obstaculo, int num_obstaculos,int porcentagem_mergulhadores, int bloco_inicial,int cor_sub,int cor_mergulhador) {
    int i;
    int probabilidade_tipo;
    int probabilidade_gerar;
    int orientacao;
    srand(time(0));
    for(i = 0; i<num_obstaculos;i++) {
        probabilidade_tipo = rand()%100+1;
         if (obstaculo[i].tipo==SEMOBSTACULO) {
            probabilidade_gerar = rand()%2;
            if (probabilidade_gerar) {
                if (probabilidade_tipo<=porcentagem_mergulhadores) {
                    orientacao = rand()%2;
                    obstaculo[i].tipo = MERGULHADOR;
                    obstaculo[i].orientacao = orientacao;
                    obstaculo[i].posicao.Y = INICIOAGUA+(i+bloco_inicial)*ALTURASUBMARINO;
                    obstaculo[i].cor = cor_mergulhador;
                    if (orientacao) {
                       obstaculo[i].posicao.X = SPAWNESQUERDAOBSTACULO;
                    } else {
                        obstaculo[i].posicao.X = SPAWNDIREITAMERGULHADOR;
                    }
                } else if (probabilidade_tipo>porcentagem_mergulhadores) {
                    orientacao = rand()%2;
                    obstaculo[i].tipo = SUBMARINOINIMIGO;
                    obstaculo[i].orientacao = orientacao;
                    obstaculo[i].posicao.Y = INICIOAGUA+(i+bloco_inicial)*ALTURASUBMARINO;
                    obstaculo[i].cor = cor_sub;
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
void testa_colisao(SUBMARINO *submarino,OBSTACULO *obstaculos, TORPEDO *torpedo) {
    testa_colisao_obstaculos_tela(obstaculos);
    testa_colisao_torpedo(torpedo, obstaculos, submarino);
    testa_colisao_submarino_obstaculos(submarino,obstaculos);
}

void atualiza_torpedo(TORPEDO *torpedo, SUBMARINO sub){// atualiza a posicao do torpedo e testa se atingiu as paredes
    if(torpedo->status == DIREITA){
        torpedo->posicao.X += CAMINHOPORLOOP;//adiciona a quantidade que o torpedo deve andar
    }else if(torpedo->status == ESQUERDA){
        torpedo->posicao.X -= CAMINHOPORLOOP;//subtrai a quantidade que o torpedo deve andar
    }
    if(torpedo->posicao.X <= COLUNA1 || torpedo->posicao.X + COMPRIMENTOTORPEDO - 1 >= COLUNA2){ // atingiu as bordas
        torpedo->status = NAODISPARADO; //seta o status para "Nao disparado" assim o jogador pode disparar outro torpedo
        //torpedo->posicao = sub.posicao;//reseta a posicao do torpedo
    }
}

void apaga_torpedo(TORPEDO *torpedo){//apaga o torpedo
    cputsxy(torpedo->posicao.X, torpedo->posicao.Y, "  ");
}

void desenha_torpedo(TORPEDO *torpedo, SUBMARINO sub){// desenha o torpedo
        if(torpedo->status == DIREITA){
            apaga_torpedo(torpedo);//apaga o torpedo
            atualiza_torpedo(torpedo, sub);//atualiza a posicao do torpedo
            if(torpedo->status == DIREITA){//checa se ainda eh possivel desenhar o torpedo
                cputsxy(torpedo->posicao.X, torpedo->posicao.Y, "->");//desenha o torpedo
            }
        }else if(torpedo->status == ESQUERDA){
            apaga_torpedo(torpedo);//apaga o torpedo
            atualiza_torpedo(torpedo, sub);//atualiza a posicao do torpedo
            if(torpedo->status == ESQUERDA){//checa se ainda eh possivel desenhar o torpedo
                cputsxy(torpedo->posicao.X, torpedo->posicao.Y, "<-");//desenha o torpedo em direcao a esquerda
            }
        }
}


// funcao que eh chamada quando o torpedo eh disparado pelo jogador
void dispara_torpedo(SUBMARINO *submarino, TORPEDO *torpedo){

    //testa se esta dentro da agua e possui o oxigenio necessario para realizar o disparo
    if(submarino->posicao.Y >= INICIOAGUA && submarino->oxigenio > PENALIDADETORPEDO){
            //testa se a posicao do sub permite disparar o torpedo
        if(submarino->orientacao == DIREITA && torpedo->status == NAODISPARADO && submarino->posicao.X+COMPRIMENTOSUBMARINO-1+COMPRIMENTOTORPEDO<COLUNA2){//x123456789ABC
            torpedo->posicao = submarino->posicao;//torpedo esta na mesma posicao do submarino
            torpedo->posicao.X += COMPRIMENTOSUBMARINO;//o comprimento do submarino eh adicionado para que parte do submarino nao seja apagada
            torpedo->status = DIREITA;//seta a direcao para a qual o torpedo sera disparado
            submarino->oxigenio -= PENALIDADETORPEDO;// penalidade por usar o torpedo
            desenha_torpedo(torpedo, *submarino);//chamada da funcao que desenha o torpedo

            //testa se a posicao do sub permite disparar o torpedo
        }else if(torpedo->status == NAODISPARADO && submarino->orientacao == ESQUERDA && submarino->posicao.X-COMPRIMENTOTORPEDO>COLUNA1){
            torpedo->posicao = submarino->posicao;//torpedo esta na mesma posicao do submarino
            torpedo->posicao.X -= 2;//duas posicoes sao subtraidas da posicao para que parte do submarino nao seja apagada
            torpedo->status = ESQUERDA;//seta a direcao para a qual o torpedo sera disparado
            submarino->oxigenio -= PENALIDADETORPEDO;// penalidade por usar o torpedo
            desenha_torpedo(torpedo, *submarino);//chamada da funcao que desenha o torpedo
        }
    }
}

// le nome do jogador limita o numero de chars na tela para MAXSTRINGNOME - 1
void le_nome_jogador(char *nome) {
    //char intermed[MAXSTRINGNOME];
    int i = 0;
    char leitura;
    gotoxy(METADEX,METADEY+1);
    do {
        leitura = getch();
        if (leitura != ENTER && leitura != BACKSPACE && i != MAXSTRINGNOME -1) {
            nome[i] = leitura;
            putchxy(METADEX+i,METADEY+1,leitura);
            i++;
        } else if (leitura == BACKSPACE && i != 0) {
            i--;
            putchxy(METADEX+i,METADEY+1,'\0');
        }
    } while(i==0 || leitura!=ENTER);
    nome[i] = '\0';
}
void imprime_opcoes_menu_pausa() {
    cputsxy(METADEX, METADEY, "Continuar");
    cputsxy(METADEX, METADEY + 1, "Sair");
}

void switch_menu_pausa_cor (int opcao_atual) {
    textcolor(YELLOW);
    if (opcao_atual==CONTINUAR) {
        cputsxy(METADEX, METADEY, "Continuar");
    } else if (opcao_atual==DESISTIR) {
        cputsxy(METADEX, METADEY+1, "Sair");
    }
    textcolor(WHITE);
}

void switch_menu_salvar_cor (int opcao_atual) {
    textcolor(YELLOW);
    if (opcao_atual==SALVAR) {
        cputsxy(METADEX,METADEY+2,"Sim");
    } else if (opcao_atual==NAOSALVAR) {
        cputsxy(METADEX,METADEY+3,"Nao");
    }
    textcolor(WHITE);
}

void imprime_interface_salvar(int *opcao) {
    char leitura;
    imprime_opcoes_menu_salvar();
    switch_menu_salvar_cor (*opcao);
    imprime_seta_inicial(2);
    do {
        le_tecla_menu (&leitura, opcao,SALVAR,NAOSALVAR);
        imprime_opcoes_menu_salvar();
        switch_menu_salvar_cor (*opcao);
    } while(leitura!=ENTER);

}


void imprime_menu_pausa(int *sair) {
    char leitura;
    int opcao = 0;
    imprime_opcoes_menu_pausa();
    switch_menu_pausa_cor (opcao);
    imprime_seta_inicial(0);
    do {
        le_tecla_menu (&leitura, &opcao,CONTINUAR,DESISTIR);
        imprime_opcoes_menu_pausa();
        switch_menu_pausa_cor (opcao);
    } while(leitura!=ENTER && leitura!=ESC);

    cputsxy(METADEX-1, METADEY, "          ");
    cputsxy(METADEX-1, METADEY + 1, "     ");

    if (opcao==DESISTIR && leitura!=ESC) {
        *sair = 1;
    }
}


void switch_game_loop(char *a, SUBMARINO *submarino,OBSTACULO *obstaculos, TORPEDO *torpedo, int *sair) {

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
                        imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);
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
                        imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);
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
                        imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);
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
                        imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);
                        if (submarino->posicao.Y==LINHAINICIAL) {
                            imprime_agua();
                        }
                    }
                    //x -=1;
                    break;
            }
        } else if(*a == ESPACO){
            dispara_torpedo(submarino, torpedo);
        } else if(*a == ESC) {
            imprime_menu_pausa(sair);
        }
    }
}

void imprime_mergulhadores_vazio() {//apaga mergulhadores da interface do jogador
    int j;
    for (j = 0; j<MERGULHADORESMAXIMOS;j++) {
       cputsxy(COLUNA1+1+(COMPRIMENTOMERGULHADOR+1)*j,LINHAINTERFACEINFERIOR+1,"     ");
    }
}


// atualiza num de mergulhadores na interface do jogador
void atualiza_interface_mergulhadores(SUBMARINO *submarino,INTERFACEJOGO *interface_jogo) {
    if (submarino->posicao.Y==LINHAINICIAL) {// deleta todos mergulhadores da interface
        imprime_mergulhadores_vazio();
        //submarino->mergulhadores = 0;
    } else {// adiciona um mergulhador na interface
        cputsxy(COLUNA1+1+(COMPRIMENTOMERGULHADOR+1)*interface_jogo->mergulhadores,LINHAINTERFACEINFERIOR+1,">->O ");
    }
}

//imprime interface inicial da pontuacao
void imprime_interface_pontuacao() {
    gotoxy(COLUNA1+1,LINHAINTERFACESUPERIOR-1);
    cprintf("Pontos: ");
}

// imprime a pontuacao atual do jogador na posicao adequada,apagando antes a pontuacao que estava no lugar
void atualiza_pontuacao(INTERFACEJOGO *interface_jogo) {
    cputsxy(COLUNA1+9,LINHAINTERFACESUPERIOR-1,"               ");
    gotoxy(COLUNA1+9,LINHAINTERFACESUPERIOR-1);
    cprintf("%d",interface_jogo->pontuacao);
}

// verifica se os mergulhadores no sub foram resgatados, e caso tenham sido aumenta a posicao do sub
void resgatou_mergulhadores(SUBMARINO *submarino) {
    if (submarino->mergulhadores && submarino->posicao.Y==LINHAINICIAL) {
        submarino->pontuacao += SALVAMERGULHADOR*submarino->mergulhadores;
        submarino->mergulhadores = 0;
    }
}

// atualiza o numero de vidas na interface do jogo
void atualiza_vidas(INTERFACEJOGO *interface_jogo) {
    cputsxy(COLUNAVIDAS+3*(interface_jogo->vidas-1),LINHAINTERFACESUPERIOR-1,"  ");
}

// atualiza o oxigenio na interface do jogo
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
}

// quando o submarino morre, recoloca ele na posicao inicial
void respawn_submarino(SUBMARINO *submarino) {
    MessageBeep(MB_ICONWARNING);
    if (submarino->vidas>0) {// caso nao tenha mais vidas, nao é necessario recolocar na pos inicial
        apaga_submarino(*submarino);
        submarino->posicao.X = COLUNAINICIAL;
        submarino->posicao.Y = LINHAINICIAL;
        submarino->orientacao = DIREITA;
        submarino->oxigenio = OXIGENIOMAXIMO;
        submarino->mergulhadores = 0;
        //printf("\a");
        imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);
        imprime_agua();
    }
}

// verifica se o submarino morreu por falta de oxigenio
void testa_morte_oxigenio (SUBMARINO *submarino) {
    if (submarino->oxigenio==0 && submarino->vidas>0) {
        submarino->vidas--;
        respawn_submarino(submarino);
    }
}


// atualiza o oxigenio do submarino caso seja necessario
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
            //submarino->tempo_agua==0;
            submarino->tempo_agua = 0;// estava como == mas na real essa parte do codigo pode tira eu acho
            // pq n precisa reseta pra zero mas vou deixar assim
        }
        submarino->oxigenio = OXIGENIOMAXIMO;
    }/*
    if (submarino->posicao.Y!=LINHAINICIAL) {
        submarino->oxigenio--;
    } else if (submarino->oxigenio<OXIGENIOMAXIMO) {
        submarino->oxigenio++;
    }*/
}

// atualiza a pontuacao do jogador se ele estiver submerso
void atualiza_pontuacao_submerso(SUBMARINO *submarino) {
    if (submarino->posicao.Y!=LINHAINICIAL) {
        //if ((submarino->tempo-submarino->tempo_agua)%SEGUNDO==0 && submarino->tempo_agua!=0 && submarino->tempo_agua!=submarino->tempo) {
       if ((submarino->tempo-submarino->tempo_agua)%SEGUNDO<TEMPODELOOP && submarino->tempo_agua!=0 && submarino->tempo_agua!=submarino->tempo) {
            submarino->pontuacao++;
       }
    }
}

// atualiza tempo de jogo
void atualiza_tempo (SUBMARINO *submarino ) {
    submarino->tempo += TEMPODELOOP;
}

// imprime a quantidade de vidas inicial na tela
void imprime_vidas(INTERFACEJOGO *interface_jogo) {
    int i;
    //int j;
    textcolor(LIGHTRED);
    for (i = 0; i<interface_jogo->vidas;i++) {
       cputsxy(COLUNAVIDAS+3*i,LINHAINTERFACESUPERIOR-1,"<3 ");
    }/*
    for (j = i; j<VIDASINICIAIS;j++) {
       cputsxy(COLUNAVIDAS+3*j,LINHAINTERFACESUPERIOR-1,"  ");
    }*/
    textcolor(WHITE);
}

// imprime o oxigenio inicial na tela
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

// atualiza o tempo de jogo na interface
void atualiza_interface_tempo (INTERFACEJOGO *interface_jogo) {
    gotoxy(38,2);// acho q nas horas n vai da problema
    cprintf("%02d:%02d:%02d",interface_jogo->tempo/1000/60/60,interface_jogo->tempo/1000/60,interface_jogo->tempo/1000%60);
}

// faz verificacoes entre os valores dos campos do sub e da interface para saber se eh necessario
// atualizar a interface e se for faz a atualizacao
void atualiza_interface(SUBMARINO *submarino,INTERFACEJOGO *interface_jogo) {
    /*
    atualiza_oxigenio(submarino);
    if (submarino->posicao.Y==LINHAINICIAL) {
        atualiza_pontuacao(submarino);
        atualiza_interface_mergulhadores(submarino);
    }*/textbackground(CORINTERFACE);
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
    textbackground(CORAGUA);
}
/*
void imprime_interface_auxiliar() {
    putchxy(COLUNA2-1,LINHAINTERFACEINFERIOR+1,']');
    cputsxy(COLUNAOXIGENIO,LINHAINTERFACEINFERIOR+1,"Oxigenio: [");
    gotoxy(COLUNA1+1,LINHAINTERFACESUPERIOR-1);
    printf("Pontos: 0");
}*/

// imprime a interface inicial
void imprime_interface(INTERFACEJOGO *interface_jogo) {
    //imprime_interface_auxiliar();
    textbackground(CORINTERFACE);
    imprime_interface_pontuacao();
    atualiza_pontuacao(interface_jogo);//
    imprime_oxigenio(interface_jogo);// imprime completo
    imprime_vidas(interface_jogo);
    atualiza_interface_tempo(interface_jogo);
    //imprime_interface_mergulhadores();
    textbackground(CORAGUA);
}

void atualiza_obstaculo_individual(COORD *posicao, int orientacao) {
    // atualiza a pos de um obstaculo ou seja se esta virado para esquerda anda o caminhoporloop para esquerda
    // se esta virado para a direita anda o caminhoporllop para direita
    if (orientacao==ESQUERDA) {
        posicao->X -= CAMINHOPORLOOP;
    } else {
        posicao->X += CAMINHOPORLOOP;
    }
}

void testa_colisao_tela_inimigo(OBSTACULO *obstaculo) {// testa a colisao do sub inimigo com a tela
    if (obstaculo->posicao.X+COMPRIMENTOSUBMARINO-1>=COLUNA2 || obstaculo->posicao.X<=COLUNA1) {
        obstaculo->tipo = 0;
    } //else {
    //    imprime_submarino_inimigo(*obstaculo);
    //}
}
void testa_colisao_tela_mergulhador(OBSTACULO *obstaculo) {// testa a colisao do mergulhador com a tela
    if (obstaculo->posicao.X+COMPRIMENTOMERGULHADOR-1>=COLUNA2 || obstaculo->posicao.X<=COLUNA1) {
        obstaculo->tipo = 0;
    }// else {
     //   imprime_mergulhador(*obstaculo);
    //}
}

// testa a colisao do torpedo com um obstaculo e nao um vetor de obstaculos
// caso colidam faz as atualizacoes necessarias na pontuacao do sub,no jogador e no obstaculo
void testa_colisao_torpedo_individual (TORPEDO *torpedo, OBSTACULO *obstaculo, SUBMARINO *sub){
    //int i;
    //for(i = 0; i < NUMOBSTACULOS; i++){//percorre o array de obstaculos
        if(torpedo->status != NAODISPARADO){
            if(obstaculo->tipo == SUBMARINOINIMIGO && colidiu_torpedo_submarino_inimigo (torpedo->posicao,obstaculo->posicao)){//chama a funcao para testar se houve colisao
                torpedo->status = NAODISPARADO;//reseta o status do torpedo para que ele seja apagado e o jogador possa disparar outros
                apaga_torpedo(torpedo);
                apaga_submarino_inimigo(*obstaculo);//apaga o sub inimigo atingido
                obstaculo->tipo = SEMOBSTACULO;
                sub->pontuacao += PONTUACAODESTRUICAOSUBINIMIGO;//aumenta a pontuacao do jogador por ter destruido um sub
            }else if(obstaculo->tipo == MERGULHADOR && colidiu_torpedo_mergulhador (torpedo->posicao,obstaculo->posicao)){
                torpedo->status = NAODISPARADO;//reseta o status do torpedo para que ele seja apagado e o jogador possa disparar outros
                apaga_torpedo(torpedo);
                apaga_mergulhador(*obstaculo);
                obstaculo->tipo = SEMOBSTACULO;
                if(sub->pontuacao >= PENALIDADEMORTEMERGULHADOR){
                    sub->pontuacao -= PENALIDADEMORTEMERGULHADOR;
                } else {
                    sub->pontuacao = 0;
                }
            }
        }
    //}
}

// verifica se um obstaculo eh sub inimigo ou mergulhador e imprime ele
void imprime_obstaculo_individual(OBSTACULO obstaculo) {
    if (obstaculo.tipo==SUBMARINOINIMIGO) {
         imprime_submarino(obstaculo.posicao,obstaculo.orientacao,obstaculo.cor);
    } else if(obstaculo.tipo==MERGULHADOR) {
        imprime_mergulhador(obstaculo.posicao,obstaculo.orientacao,obstaculo.cor);
    }
}



// testa a colisao entre o submarino e um sub inimigo,caso colidam faz as atualizacoes necessarias
void testa_colisao_submarino_inimigo(SUBMARINO *submarino, OBSTACULO *obstaculo) {
    if (colidiu_sub_inimigo(submarino->posicao,obstaculo->posicao) && obstaculo->tipo==SUBMARINOINIMIGO) {
        submarino->vidas--;
        respawn_submarino(submarino);
    }
}

// testa a colisao entre o submarino e um mergulhador,caso colidam faz as atualizacoes necessarias
void testa_colisao_submarino_mergulhador(SUBMARINO *submarino, OBSTACULO *obstaculo) {
    if (colidiu_sub_mergulhador(submarino->posicao,obstaculo->posicao) && obstaculo->tipo==MERGULHADOR) {
        apaga_mergulhador(*obstaculo);
        obstaculo->tipo = SEMOBSTACULO;//reseta o tipo do obstaculo
        if (submarino->mergulhadores < MERGULHADORESMAXIMOS) {//testa
           submarino->mergulhadores++;
          //atualiza_interface_mergulhadores(submarino);
        }
        imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);
    }
}

// funcao que percorre o vetor de obstaculos e faz as atualizacoes e verificacoes necessarias nos obstaculos
void percorre_vetor_obstaculos(OBSTACULO *obstaculos, TORPEDO *torpedo,SUBMARINO *submarino) {
    int i;
    for (i = 0;i<NUMOBSTACULOS;i++) {
        if (obstaculos[i].tipo==SUBMARINOINIMIGO) {
            apaga_submarino_inimigo(obstaculos[i]);
            atualiza_obstaculo_individual(&obstaculos[i].posicao,obstaculos[i].orientacao);
            testa_colisao_tela_inimigo(&obstaculos[i]);
            testa_colisao_torpedo(torpedo, obstaculos, submarino);
            testa_colisao_submarino_inimigo(submarino,&obstaculos[i]);
            imprime_obstaculo_individual(obstaculos[i]);
        } else if (obstaculos[i].tipo==MERGULHADOR){
            apaga_mergulhador(obstaculos[i]);
            atualiza_obstaculo_individual(&obstaculos[i].posicao,obstaculos[i].orientacao);
            testa_colisao_tela_mergulhador(&obstaculos[i]);
            testa_colisao_torpedo(torpedo, obstaculos, submarino);
            testa_colisao_submarino_mergulhador(submarino,&obstaculos[i]);
            imprime_obstaculo_individual(obstaculos[i]);
        }
    }
}

int colisao_menu_submarino(OBSTACULO obstaculo) {
    int verifica_y, verifica_x1, verifica_x2, verifica_x3;
    verifica_y = obstaculo.posicao.Y - (ALTURASUBMARINO-1)<=METADEY+4 && obstaculo.posicao.Y>=METADEY;
    verifica_x1 = obstaculo.posicao.X <= METADEX-1 && obstaculo.posicao.X + COMPRIMENTOSUBMARINO - 1 >= METADEX-1;
    verifica_x2 = obstaculo.posicao.X <= METADEX + COMPRIMENTOMENU -1 && obstaculo.posicao.X + COMPRIMENTOSUBMARINO - 1>= METADEX + COMPRIMENTOMENU -1;
    verifica_x3 = obstaculo.posicao.X <= METADEX + COMPRIMENTOMENU -1 && obstaculo.posicao.X + COMPRIMENTOMERGULHADOR - 1>= METADEX-1;
    if (verifica_y && (verifica_x1 || verifica_x2 || verifica_x3)) {
        return 1;
    } else {
        return 0;
    }
}

int colisao_menu_mergulhador(OBSTACULO obstaculo) {
    int verifica_y, verifica_x1, verifica_x2;
    verifica_y = obstaculo.posicao.Y <=METADEY+4 && obstaculo.posicao.Y>=METADEY;
    verifica_x1 = obstaculo.posicao.X <= METADEX-1 && obstaculo.posicao.X + COMPRIMENTOMERGULHADOR - 1<= METADEX-1;
    verifica_x2 = obstaculo.posicao.X <= METADEX + COMPRIMENTOMENU -1 && obstaculo.posicao.X + COMPRIMENTOMERGULHADOR - 1<= METADEX + COMPRIMENTOMENU -1;
    if (verifica_y && (verifica_x1 || verifica_x2)) {
        return 1;
    } else {
        return 0;
    }
}

void percorre_vetor_obstaculos_menu (OBSTACULO *obstaculos) {
    int i;
    for (i = 0;i<NUMOBSTACULOSMENU;i++) {
        if (obstaculos[i].tipo==SUBMARINOINIMIGO) {
            if (!colisao_menu_submarino(obstaculos[i])) {
                apaga_submarino_inimigo(obstaculos[i]);
            }
            atualiza_obstaculo_individual(&obstaculos[i].posicao,obstaculos[i].orientacao);
            if (!colisao_menu_submarino(obstaculos[i])) {
                testa_colisao_tela_inimigo(&obstaculos[i]);
                //imprime_obstaculo_individual_cor(obstaculos[i],YELLOW);
                //verifica_inimigo_individual_cor(obstaculos[i],YELLOW);
                imprime_obstaculo_individual(obstaculos[i]);
            }
        }
    }
}

// imprime o submarino quando nao estiver na posicao inicial
// usada porque percorremos o vetor de obstaculos em uma funcao so
// e eh possivel apagar um pedaco do sub dessa maneira
// mas isso so acontece quando o sub estiver dentro dagua dai para evitar
// um if para ver se se colidiu com todos obstaculos e caso colida imprimir
// imprimi-se o sub sempre quando o sub estiver fora dagua
void imprime_submarino_controla_agua(SUBMARINO submarino) {//
    //imprime_submarino(submarino);
    /*
    if (submarino.posicao.Y==LINHAINICIAL) {
        imprime_agua();
    } else {
        imprime_submarino(submarino);
    }*/

    if (submarino.posicao.Y!=LINHAINICIAL) {
        imprime_submarino(submarino.posicao,submarino.orientacao,submarino.cor);
    }
}

void buscaNomesEPontuacao(FILE **arq, char nomes[NUMRECORDES+1][MAXSTRINGNOME], int pontuacao[]){
    //FILE *arq;
    int i = 0;
    rewind(*arq);
    while(!feof(*arq)){
        fscanf(*arq,"%s", nomes[i]);
        fscanf(*arq, "%d", &pontuacao[i]);
        i++;
    }
    //fclose(*arq);
}
void gravarRecordes(char nomes[][MAXSTRINGNOME], int pontuacao[]){
    int i;
    FILE *arq;
    arq = fopen("recordes.txt", "w");
    if(arq){
        for(i = 0; i < NUMRECORDES; i++){
            fprintf(arq, "%s %d\n", nomes[i], pontuacao[i]);
        }
        fflush(arq);
        fclose(arq);
    }else{
        cputsxy(METADEX, METADEY, "ALGO DEU ERRADO");
    }

}

void bubblesort(FILE **arq){
    //FILE *arq;
    int pontuacao[NUMRECORDES + 1];//no arquivo de texto, o maximo de registro possivel é o NUMRECORDES + 1(que eh o novo recorde recem adicionado)
    char nomes[NUMRECORDES + 1][MAXSTRINGNOME];
    int i;
    int ind, fim, sinal, aux;
    char auxStr[MAXSTRINGNOME];
    fim = NUMRECORDES;
    buscaNomesEPontuacao(arq, nomes, pontuacao);
    do{
        sinal = 0;
        for(ind = 0; ind < fim; ind++){
            if(pontuacao[ind] < pontuacao[ind + 1]){
                aux = pontuacao[ind];
                strcpy(auxStr, nomes[ind]);//trocando de posicao os nomes
                pontuacao[ind] = pontuacao[ind + 1];
                strcpy(nomes[ind], nomes[ind + 1]);
                pontuacao[ind + 1] = aux;
                strcpy(nomes[ind + 1], auxStr);
                sinal = 1;
            }
        }
        fim--;
    }while(sinal == 1 && fim > 0);

    //rewind(*arq);
    //fclose(*arq);
    gravarRecordes(nomes, pontuacao);

}
int testaIntegridade(FILE *arq){
    //FILE *arq;
    int i = 0;
    int tentativas = 0;
    char tmp[256];
    rewind(arq);
    while(!feof(arq)){
        fgets(tmp, sizeof(tmp), arq);
        i++;
    }
    if(i == NUMRECORDES + 1){
        return 1;
    }else{
        return 0;
    }
}

void guarda_pontuacao(SUBMARINO sub){
    FILE *arq;
    int tentativas = 0;
    while((arq = fopen("recordes.txt", "a+")) == NULL && tentativas < 10){
        arq = fopen("recordes.txt", "a+");
        tentativas++;
    }
    if(tentativas < 10){
        if(testaIntegridade(arq)){
            fprintf(arq, "%s %d\n", sub.nome, sub.pontuacao);
            fflush(arq);
            bubblesort(&arq);
            fclose(arq);
         }else{
            fclose(arq);
            arq = fopen("recordes.txt", "w+");
            if(arq){
                preencherArquivo(&arq);
                fprintf(arq, "%s %d\n", sub.nome, sub.pontuacao);
                fflush(arq);
                bubblesort(&arq);
                fclose(arq);
            }else{
                cputsxy(METADEX, METADEY, "ALGO DEU ERRADO");
            }
        }
    }else{
        cputsxy(METADEX, METADEY, "ALGO DEU ERRADO!");
    }



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
    int sair;
    INTERFACEJOGO interface_jogo = {submarino->vidas, submarino->mergulhadores, submarino->oxigenio, submarino->pontuacao,submarino->tempo};
    pintar_jogo();
    imprime_moldura();
    imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);// imprime o submarino inicialmente
    imprime_obstaculos(obstaculos);//para caso tenha carregado o jogo imprima os obstaculos do load
    imprime_agua();
    imprime_interface(&interface_jogo);
    do {
        Sleep(TEMPODELOOP);// para dar um tempo entre loops
        atualiza_tempo (submarino);
        gera_obstaculos(obstaculos,NUMOBSTACULOS,PORCENTAGEMMERGULHADORES,BLOCOINICIALSPAWNJOGO,LIGHTMAGENTA,GREEN);// funcao que gera obstaculos nas posicoes que n tem obstaculos
        //apaga_obstaculos(obstaculos);// apaga os os obstaculos
        switch_game_loop(&a,submarino,obstaculos, torpedo,&sair );
        //
        //atualiza_interface(submarino,&interface_jogo);
        // depois que faz o comando do submarino apaga os obstaculos na tela e atualiza as posicoes dos obstaculos
        // dai testa se alguma dessas novas posicoes bate na tela, ou seja se o obstaculo atravessou a tela
        // se esse for o caso esse obstaculo desaparece e imprime os obstaculos que ainda nao atravessaram
        /*
        apaga_obstaculos(obstaculos);
        imprime_submarino(*submarino);// gambiarra pro caso de um escapar do outro
        */
        //colisao_torpedo(torpedo, obstaculos, submarino);
        //atualiza_torpedo(torpedo);
        //
        /*
        apaga_obstaculos(obstaculos);
        atualiza_obstaculos(obstaculos);
        testa_colisao_obstaculos_tela(obstaculos);
        testa_colisao_torpedo(torpedo, obstaculos, submarino);
        testa_colisao_submarino_obstaculos(submarino,obstaculos);
        //desenha_torpedo(torpedo, *submarino);
        imprime_obstaculos(obstaculos);
        */
        percorre_vetor_obstaculos(obstaculos,torpedo,submarino);
        imprime_submarino_controla_agua(*submarino);
        desenha_torpedo(torpedo, *submarino);
        resgatou_mergulhadores(submarino);
        atualiza_oxigenio_submarino(submarino);
        atualiza_pontuacao_submerso(submarino);
        //atualiza_interface(submarino,&interface_jogo);
        testa_morte_oxigenio (submarino);
        atualiza_interface(submarino,&interface_jogo);
    } while(sair!=1 && submarino->vidas>0);


    if (submarino->vidas==0) {
        animacao_sem_vidas(submarino,obstaculos);
        guarda_pontuacao(*submarino);
    } else {
        textbackground(CORMENU);
        //guarda_estrutura(*submarino);
        //tenta_guardar_estrutura(*submarino);
        tenta_guardar_estrutura(*submarino,obstaculos,*torpedo);
    }
    //textbackground(CORMENU);
    clrscr();
}

void imprime_moldura() {// imprime a moldura do jogo
    int x,y;
    textbackground(CORINTERFACE);
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
    textbackground(CORAGUA);

}

// imprime a moldura do menu
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

void imprime_opcoes_menu_salvar() {
    cputsxy(METADEX,METADEY+2,"Sim");
    cputsxy(METADEX,METADEY+3,"Nao");
}

// pergunta ao usuario se deseja salvar o jogo e caso queira tenta salvar o jogo
// caso nao consiga salvar pergunta ao usuario se quer tentar novamente ou desistir
// ate que o usuario desista ou o jogo seja salvo com sucesso
void tenta_guardar_estrutura(SUBMARINO submarino,OBSTACULO obstaculos[],TORPEDO torpedo) {
    int gravou = 0;
    int opcao = SALVAR;
    char arquivo[MAXSTRINGARQ];

    clrscr();
    cputsxy(METADEX,METADEY+1,"Deseja salvar o jogo:");
    imprime_interface_salvar(&opcao);

    while(opcao==SALVAR && gravou==0) {
        Sleep(SALVANDOJOGO);
        gravou = guarda_estrutura(submarino,obstaculos,torpedo);
        if (!gravou) {
            cputsxy(METADEX,METADEY+1,"Deseja tentar salvar novamente:");
            imprime_interface_salvar(&opcao);
            clrscr();
        } else {
            strcpy(arquivo,submarino.nome);
            strcat(arquivo,".bin");
            clrscr();
            cputsxy(METADEX,METADEY,"JOGO SALVO EM");
            cputsxy(METADEX,METADEY+1,arquivo);
            getch();
        }
    }
}

// funcao que salva o jogo em um arquivo bin
int guarda_estrutura(SUBMARINO submarino,OBSTACULO obstaculos[],TORPEDO torpedo) {
    FILE *arq;
    JOGO jogo;
    char nome[MAXSTRINGARQ];
    strcpy(nome,submarino.nome);
    strcat(nome,".bin");
    arq = fopen(nome,"wb");
    clrscr();
    if (arq) {
    //if (fwrite(&submarino.nome,sizeof(submarino.nome),1,arq) == 1 && fwrite(&submarino.vidas,sizeof(submarino.vidas),1,arq) == 1 && fwrite(&submarino.pontuacao,sizeof(submarino.pontuacao),1,arq) == 1 && fwrite(&submarino.tempo,sizeof(submarino.tempo),1,arq) == 1) {
    //if (fwrite(&submarino,sizeof(SUBMARINO),1,arq) == 1 && fwrite(obstaculos,sizeof(OBSTACULO),NUMOBSTACULOS,arq) == NUMOBSTACULOS && fwrite(&torpedo,sizeof(TORPEDO),1,arq) == 1) {
        jogo.submarino = submarino;
        jogo.torpedo = torpedo;
        copia_vetor_obstaculos(jogo.obstaculos,obstaculos,NUMOBSTACULOS);
        if (fwrite(&jogo,sizeof(JOGO),1,arq) == 1) {
            //printf("DALE");
        fclose(arq);
        return 1;
    } else {
        cputsxy(METADEX,METADEY,"Erro ao salvar o jogo");
        fclose(arq);
        //getch();
        return 0;
    }
    } else {
        cputsxy(METADEX,METADEY,"Erro ao salvar o jogo");
        //getch();
        return 0;
    }
}

// copia de um vetor2 para um vetor1 cada obstaculo do vetor
void copia_vetor_obstaculos(OBSTACULO vetor1[],OBSTACULO vetor2[],int tam) {// vetor1 eh destino
    int i;
    for (i = 0; i<tam; i++) {
        vetor1[i] = vetor2[i];
    }
}

// le de um arquivo bin os dados salvos do jogo
int le_estrutura(SUBMARINO *submarino,OBSTACULO obstaculos[],TORPEDO *torpedo) {
    FILE *arq;
    JOGO jogo;
    char nome_arq[MAXSTRINGARQ];
    cputsxy(METADEX,METADEY,"Digite o nome do arquivo: ");
    gotoxy(METADEX,METADEY+1);
    gets(nome_arq);
    cputsxy(METADEX,METADEY,"                          ");
    cputsxy(METADEX,METADEY+1,"                          ");
    arq = fopen(nome_arq,"rb");
    if (arq) {
        //if (fread(&submarino->nome,sizeof(submarino->nome),1,arq) == 1 && fread(&submarino->vidas,sizeof(submarino->vidas),1,arq) == 1 && fread(&submarino->pontuacao,sizeof(submarino->pontuacao),1,arq) == 1 && fread(&submarino->tempo,sizeof(submarino->tempo),1,arq) == 1) {
        if (fread(&jogo,sizeof(JOGO),1,arq) == 1) {
                /*printf("Nome: %s\n",buffer.Nome);
                printf("Idade: %d\n",buffer.Idade);
                printf("Altura: %.2f\n\n",buffer.Altura);*/
            *submarino = jogo.submarino;
            *torpedo = jogo.torpedo;
            copia_vetor_obstaculos(obstaculos,jogo.obstaculos,NUMOBSTACULOS);
            return 1;

        } else{
            cputsxy(METADEX,METADEY,"ERRO");
            getch();
            return 0;
            //fclose(arq); //fecha
        }
        fclose(arq);
    } else {
        cputsxy(METADEX,METADEY,"ERRO");
        getch();
        return 0;
    }
    //} // Fim do while
    //fecha
} // Fim da função

// carrega o jogo
void carregar_jogo() {
    OBSTACULO  obstaculos [NUMOBSTACULOS] = {};
    SUBMARINO sub = {"",{COLUNAINICIAL,LINHAINICIAL},DIREITA,VIDASINICIAIS,OXIGENIOMAXIMO,0,0,0,0,YELLOW};
    TORPEDO torpedo = {sub.posicao, NAODISPARADO};
    if (le_estrutura(&sub,obstaculos,&torpedo)) {
        clrscr();
        game_loop(&sub,obstaculos, &torpedo);
    }
}

// comeca um novo jogo
void novo_jogo() {
    OBSTACULO  obstaculos [NUMOBSTACULOS] = {};
    SUBMARINO sub = {"",{COLUNAINICIAL,LINHAINICIAL},DIREITA,VIDASINICIAIS,OXIGENIOMAXIMO,0,0,0,0,YELLOW};
    TORPEDO torpedo = {sub.posicao, NAODISPARADO};
    cputsxy(METADEX,METADEY,"Digite seu nome:");
    gotoxy(METADEX,METADEY+1);
    //scanf("%8s",sub.nome);
    le_nome_jogador(sub.nome);
    //fflush(stdin);
    //fgets(sub.nome,MAXSTRING,stdin);// bota o \0 se digitar menos que 8 caracteres?
    //cputsxy(METADEX,METADEY+2,sub.nome);
    clrscr();
    game_loop(&sub,obstaculos, &torpedo);
}


//funcao que colocar o cursor no meio da tela:
//o parametro linha serve para deslocar o cursor
// para baixo pelo numero de vezes que for passado
void proMeio(int linha){
    gotoxy(COLUNA2/ 2, (LINHA2 / 2) + linha);
}
//funcao menu: desenha o menu e chama a funcao de acordo com o
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
            //printf("Espero que volte logo...");
            cputsxy(METADEX,METADEY,"Espero que volte logo...");
            getch();
            exit(0);
            break;

    }
}

void imprime_titulo() {// imprime o titulo do jogo
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

// le a tecla digitada no menu e faz alteracoes correspondentes ao que foi pressionado
void le_tecla_menu (char *tecla, int *opcao_atual, int lim_superior,int lim_inferior){// funcao que deve ser usada com menu2
    *tecla = getch();
    textcolor(YELLOW);
    if(*tecla==TECLASAUXILIARES) {
        *tecla = getch();
        if (*tecla==SETACIMA) {// se for cima e tiver como ir para cima vai para cima
            if(*opcao_atual!=lim_superior) {
                putchxy(METADEX-1,METADEY+*opcao_atual,'\0');
                (*opcao_atual)--;
                putchxy(METADEX-1,METADEY+*opcao_atual,'>');
            }
        } else if (*tecla==SETABAIXO) {// se for baixo e tiver como ir para baixo vai para baixo
            if(*opcao_atual!=lim_inferior) {
                putchxy(METADEX-1,METADEY+*opcao_atual,'\0');
                (*opcao_atual)++;
                putchxy(METADEX-1,METADEY+*opcao_atual,'>');
            }
        }
    }
    textcolor(WHITE);
}

// funcao para imprimir novamente a opcao do menu que o usuario tem selecionado
// pois a opcao selecionada tem cor diferente
void switch_menu_cor(int opcao_atual) {
    textcolor(YELLOW);
    switch(opcao_atual) {
        case NOVOJOGO:
                cputsxy(METADEX, METADEY, "Novo Jogo");
                break;
        case CARREGARJOGO:
                cputsxy(METADEX, METADEY + 1, "Carregar Jogo");
                break;
        case RECORDES:
                cputsxy(METADEX, METADEY + 2, "Recordes");
                break;
        case CREDITOS:
                cputsxy(METADEX, METADEY + 3, "Creditos");
                break;
        case SAIR:
                cputsxy(METADEX, METADEY + 4, "Sair");
    }
    textcolor(WHITE);
}

// imprime as opcoes do menu
void imprime_opcoes_menu() {
    cputsxy(METADEX, METADEY, "Novo Jogo");
    cputsxy(METADEX, METADEY + 1, "Carregar Jogo");
    cputsxy(METADEX, METADEY + 2, "Recordes");
    cputsxy(METADEX, METADEY + 3, "Creditos");
    cputsxy(METADEX, METADEY + 4, "Sair");
}

// imprime a seta inicial que marca a opcao do menu
void imprime_seta_inicial(int pos) {
    textcolor(YELLOW);
    putchxy(METADEX-1,METADEY+pos,'>');
    textcolor(WHITE);
}

void buscaNomePontuacao(char nomes[NUMRECORDES][MAXSTRINGNOME], int pontuacao[],FILE **arq){
    int i = 0;
    //int tentativas = 0;
    /*
    while((*arq = fopen("recordes.txt", "r")) == NULL && tentativas < 10){
        arq = fopen("recordes.txt", "r");
        tentativas++;
    }*/
    //if(tentativas < 10){
    rewind(*arq);
       // while(!feof(*arq)){
        i = 0;
        while(i < NUMRECORDES && !feof(*arq)){
            fscanf(*arq, "%s", nomes[i]);
            fscanf(*arq, "%d", &pontuacao[i]);
            i++;
        }
        //fclose(arq);
        fflush(*arq);
    //}else{
    //    cputsxy(METADEX, METADEY, "ALGO DEU ERRADO!");
    //}
}

void mostraTabelaRecordes(char nomes[NUMRECORDES][MAXSTRINGNOME], int pontuacoes[]){
    int i;
    for(i = 0; i < NUMRECORDES; i++){
        cputsxy(METADEX, METADEY+i, nomes[i]);
    }
    for(i = 0; i < NUMRECORDES; i++){
        gotoxy(METADEX+10, METADEY+i);
        cprintf("%d", pontuacoes[i]);
    }
}

void preencherArquivo(FILE **arq){
    char nome[] = "null";
    int pontuacao = 0;
    int i;
    rewind(*arq);
    for(i = 0; i < NUMRECORDES; i++){
        fprintf(*arq, "%s %d\n", nome, pontuacao);
    }
    fflush(*arq);
    //fclose(*arq);
}

void mostraTituloRecordes(){
    int i = 13;
    int j = 13;
    textcolor(YELLOW);
    cputsxy(2+i,2, " _____  _____  _____  _____  _____  _____  _____  _____ ");
    cputsxy(2+i,3, "/  _  \\/   __\\/     \\/  _  \\/  _  \\|  _  \\/   __\\/  ___>");
    cputsxy(2+i,4, "|  _  <|   __||  |--||  |  ||  _  <|  |  ||   __||___  |");
    cputsxy(2+i,5, "\\__|\\_/\\_____/\\_____/\\_____/\\__|\\_/|_____/\\_____/<_____/");
    textcolor(WHITE);
}

void recordes(){
    int resp;
    char nomes[NUMRECORDES][MAXSTRINGNOME];
    int pontuacoes[NUMRECORDES];
    FILE *arq;
    arq = fopen("recordes.txt", "r"); //so teste para verificar se o arquivo existe
    if(arq != NULL){
        buscaNomePontuacao(nomes, pontuacoes, &arq);
        fclose(arq);
        mostraTituloRecordes();
        mostraTabelaRecordes(nomes, pontuacoes);
    }else{
        arq = fopen("recordes.txt", "w+");
        if(arq == NULL){
            printf("ERRO AO CARREGAR O ARQUIVO");
        }else{
            preencherArquivo(&arq);
            buscaNomePontuacao(nomes, pontuacoes, &arq);
            fclose(arq);
            mostraTituloRecordes();
            mostraTabelaRecordes(nomes, pontuacoes);
        }
    }
    //printf("NOME %s", nomes[0]);
    //printf("PONTUACAO: %d", pontuacoes[0]);
    do{
        resp = getch();
    }while(resp != ESC);
}

// menu do jogo
void menu2(){// outro menu
    OBSTACULO  obstaculos [NUMOBSTACULOSMENU] = {};
    char resp;
    int opcao;// novo jogo
    pintar_tela();
    do {
        opcao = 0;
        clrscr();
        imprime_moldura_menu();
        //imprime_agua();// sei la achei q fico massa
        imprime_titulo();
        imprime_opcoes_menu();
        switch_menu_cor(opcao);
        imprime_seta_inicial(0);
            // print inicial
        resp = '\0';// para resetar o valor de resp na iteracao

        do {// fica atualizando posicao ate que de enter
            Sleep(TEMPODELOOPMENU);// para dar um tempo entre loops
            gera_obstaculos(obstaculos,NUMOBSTACULOSMENU,0,BLOCOINICIALSPAWNMENU,YELLOW,GREEN);
            if (kbhit()) {
                le_tecla_menu(&resp,&opcao,NOVOJOGO,SAIR);
                imprime_opcoes_menu();
                switch_menu_cor(opcao);
            }
            percorre_vetor_obstaculos_menu (obstaculos);
        } while(resp!=ENTER);
        //cputsxy(METADEX, METADEY + 1, "Carregar Jogo");


        clrscr();// todas que chamar tem q limpar a tela
        switch(opcao){// ve qual a opcao que deu enter
            case NOVOJOGO:
                novo_jogo();
                //imprime_moldura();
                //move_sub(&sub);
                break;
            case CARREGARJOGO:
                carregar_jogo();
                break;
            case RECORDES:
                recordes();
                break;
            case CREDITOS:
                creditos();
                break;
            case SAIR:
                //cprintf("Espero que volte logo...");
                cputsxy(METADEX,METADEY,"Espero que volte logo...");
                getch();
                exit(0);
                break;

        }
    } while (opcao!=SAIR);
}

void creditos(){// creditos do jogo
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
        //return 0;
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

int colidiu_sub_mergulhador(COORD sub, COORD obstaculo ) {
    if((sub.Y == obstaculo.Y) && ((sub.X + COMPRIMENTOSUBMARINO - 1) >= obstaculo.X) && (sub.X <= (obstaculo.X + COMPRIMENTOMERGULHADOR - 1))){
            return 1;
        }else {
            return 0;
        }
}

int colidiu_sub_inimigo (COORD sub, COORD obstaculo ) {
    if((sub.Y == obstaculo.Y) && ((sub.X + COMPRIMENTOSUBMARINO - 1) >= obstaculo.X) && (sub.X <= (obstaculo.X + COMPRIMENTOSUBMARINO - 1))){
            return 1;
        }else{
            return 0;
        }
}


void testa_colisao_submarino_obstaculos(SUBMARINO* submarino, OBSTACULO obstaculos[]){
    int i;

    for(i = 0; i < NUMOBSTACULOS; i++){//percorre o array de obstaculos
        //testa se houve colisao do sub com algum obstaculo
            if(obstaculos[i].tipo == SUBMARINOINIMIGO && colidiu_sub_inimigo(submarino->posicao, obstaculos[i].posicao)){// se houve colisao e o obstaculo era sub inimigo
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
            }else if(obstaculos[i].tipo == MERGULHADOR && colidiu_sub_mergulhador (submarino->posicao, obstaculos[i].posicao)){ // se houve colisao e o obstaculo era um mergulhador
                apaga_mergulhador(obstaculos[i]);
                obstaculos[i].tipo = SEMOBSTACULO;//reseta o tipo do obstaculo
                if (submarino->mergulhadores < MERGULHADORESMAXIMOS) {//testa
                    submarino->mergulhadores++;
                    //atualiza_interface_mergulhadores(submarino);
                }
                imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);
            }
        }
}

//Por enquanto so testa a colisao com subs inimigos
int colidiu_torpedo(COORD torpedo, COORD obstaculo, int tipo){//testa se houve colisao do submarino com alguma outra coisa

    if(tipo == MERGULHADOR){
        //testa se alguma parte do retangulo que representam o tamanho dos obstaculos
        //teve uma interseccao com o retangulo que representa o tamanho do torpedo
        if((torpedo.Y == obstaculo.Y) && ((torpedo.X + COMPRIMENTOTORPEDO - 1) >= obstaculo.X) && (torpedo.X <= (obstaculo.X + COMPRIMENTOMERGULHADOR - 1))){
            return 1;
        }else {
            return 0;
        }

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
}

int colidiu_torpedo_mergulhador (COORD torpedo,COORD obstaculo) {
    if((torpedo.Y == obstaculo.Y) && ((torpedo.X + COMPRIMENTOTORPEDO - 1) >= obstaculo.X) && (torpedo.X <= (obstaculo.X + COMPRIMENTOMERGULHADOR - 1))){
            return 1;
        }else {
            return 0;
        }
}

int colidiu_torpedo_submarino_inimigo (COORD torpedo,COORD obstaculo) {
    if((torpedo.Y == obstaculo.Y) && ((torpedo.X + COMPRIMENTOTORPEDO - 1) >= obstaculo.X) && (torpedo.X <= (obstaculo.X + COMPRIMENTOSUBMARINO - 1))){
            return 1;
        }else{
            return 0;
        }
}


void testa_colisao_torpedo(TORPEDO *torpedo, OBSTACULO obstaculos[], SUBMARINO *sub){
    int i;
    for(i = 0; i < NUMOBSTACULOS; i++){//percorre o array de obstaculos
        if(torpedo->status != NAODISPARADO){
                if(obstaculos[i].tipo == SUBMARINOINIMIGO && colidiu_torpedo_submarino_inimigo (torpedo->posicao,obstaculos[i].posicao)){//chama a funcao para testar se houve colisao
                    torpedo->status = NAODISPARADO;//reseta o status do torpedo para que ele seja apagado e o jogador possa disparar outros
                    apaga_torpedo(torpedo);
                    apaga_submarino_inimigo(obstaculos[i]);//apaga o sub inimigo atingido
                    obstaculos[i].tipo = SEMOBSTACULO;
                    sub->pontuacao += PONTUACAODESTRUICAOSUBINIMIGO;//aumenta a pontuacao do jogador por ter destruido um sub
                }else if(obstaculos[i].tipo == MERGULHADOR && colidiu_torpedo_mergulhador (torpedo->posicao,obstaculos[i].posicao)){
                    torpedo->status = NAODISPARADO;//reseta o status do torpedo para que ele seja apagado e o jogador possa disparar outros
                    apaga_torpedo(torpedo);
                    apaga_mergulhador(obstaculos[i]);
                    obstaculos[i].tipo = SEMOBSTACULO;
                    if(sub->pontuacao > PENALIDADEMORTEMERGULHADOR){
                        sub->pontuacao -= PENALIDADEMORTEMERGULHADOR;
                    }
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
