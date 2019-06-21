#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>



#define NUMOBSTACULOS 8// numero de obstaculos do jogo, um obstaculo pode ser um submarino inimigo ou um mergulhador
#define NUMOBSTACULOSMENU (NUMOBSTACULOS-2) // numero de obstaculos no menu do jogo(no caso numero de submarinos)
#define LINHA1 1// linha inicial da moldura da tela
#define LINHA2 25// linha final da moldura da tela
#define COLUNA1 1// coluna inicial da moldura da tela
#define COLUNA2 80// coluna final da moldura da tela
#define LINHAINICIAL 6// linha inicial do submarino controlado pelo jogador
#define COLUNAINICIAL 36// coluna inicial do submarino controlado pelo jogador
#define COMPRIMENTOMENU 13// 13 linhas para escrever a maior string do menu carregar jogo


#define PORCENTAGEMMERGULHADORES 30// porcentagem de spawn de mergulhadores, a porcentagem de submarinos inimigos eh 100-PORCENTAGEMMERGULHADORES

#define TEMPODELOOP 40//tempo em milissegundos entre as atualizacoes na tela durante o jogo
#define TEMPODELOOPMENU 60//tempo em milissegundos entre as atualizacoes na tela no menu
#define TEMPODEMORTE 80//tempo em milissegundos entre as atualizacoes na tela durante a animacao sem vidas

#define LINHAINTERFACESUPERIOR LINHA1+2// linha que separa a linha inicial da moldura e a parte superior da interface do jogador
#define LINHAINTERFACEINFERIOR LINHA2-2// linha que separa a linha final da moldura e a parte inferior da interface do jogador


#define ALTURASUBMARINO 2// nao seria altura em si mas significa que para escrever um submarino eh necessario 2 linhas
#define COMPRIMENTOSUBMARINO 11// nao seria comprimento em si mas significa que para escrever um submarino eh necessario 11 colunas

#define SUBMARINOINIMIGO 1// 1 eh o numero que indica que o obstaculo eh um submarino inimigo
#define MERGULHADOR 2// 2 indica que o obstaculo eh um mergulhador
#define SEMOBSTACULO 0// 0 indica que nao ha obstaculo na estrutura


#define COMPRIMENTOMERGULHADOR 4 // 4 colunas para escrever mergulhador 

#define COMPRIMENTOTORPEDO 2 //2 colunas para escrever o torpedo

#define DIREITA 1// foi convencionado que orientacao 1 eh direita
#define ESQUERDA 0// foi convencionado que orientacao 0 eh esquerda
#define NAODISPARADO 2// foi convencionado que 2 eh o numero que indica que o torpedo nao foi disparado


#define INICIOAGUA LINHAINICIAL+ALTURASUBMARINO// o submarino pode estar dentro ou fora dagua, se  estiver em INICIOAGUA ou abaixo esta abaixo da agua
#define CAMINHOPORLOOP 1// caminho andado por loop, a cade loop os obstaculos avancam CAMINHOPORLOOP posicoes
#define METADEX COLUNA2 / 2-3// constante usada para printar texto aproximadamente no meio da tela
#define METADEY LINHA2 / 2// constante usada para printar texto aproximadamente no meio da tela

#define SPAWNESQUERDAOBSTACULO (COLUNA1+1)// se o obstaculo der spawn na esquerda seu x sera 2
#define SPAWNDIREITASUBMARINO (COLUNA2-COMPRIMENTOSUBMARINO)// se o submarino inimigo der spawn na direita seu x sera 69
#define SPAWNDIREITAMERGULHADOR (COLUNA2-COMPRIMENTOMERGULHADOR)// se o mergulhador der spawn na direita seu x sera 76

#define BLOCOINICIALSPAWNMENU 2// indica a partir de que bloco da tela os obstaculos podem dar spawn
#define BLOCOINICIALSPAWNJOGO 0// indica a partir de que bloco da tela os obstaculos podem dar spawn

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

#define ESPACOCORACAO 3//colunas ocupadas pelo desenho de um coracao que representa uma vida do jogador
#define INTERFACEOXIGENIO 12// colunas ocupadas para colocar a interface basica do oxigenio

#define COLUNAVIDAS (COLUNA2-(VIDASINICIAIS*ESPACOCORACAO))// coluna em que estarao as vidas do jogador
#define COLUNAOXIGENIO (COLUNA2-(OXIGENIOMAXIMO+INTERFACEOXIGENIO))// coluna em que estara a barra de oxigenio


#define PENALIDADETORPEDO 5// penalidade no oxigenio por torpedo disparado
#define SALVANDOJOGO 100// milissegundos entre tentativas de salvar o jogo
#define MAXSTRINGNOME 9//maior nome possivel com 8 letras e o \0
#define MAXSTRINGARQ (MAXSTRINGNOME+4)//nome do jogador e .bin e \0
#define NUMRECORDES 10 //Numero maximo de pontuacoes que sera mostrada na funcao recorde


// cores do jogo
#define CORAGUA BLUE
#define CORINTERFACE CYAN
#define CORFORADAGUA DARKGRAY
#define CORMENU CYAN
#define CORTEXTO WHITE
#define CORSUPERFICIEAGUA CYAN

// constantes para as opcoes do menu de pausa
#define CONTINUAR 0
#define DESISTIR 1


// constantes para a interface de salvar o jogo
#define SALVAR 2
#define NAOSALVAR 3


#define COLUNARECORDES 15//coluna inicial do titulo recordes
#define LINHAINICIALRECORDES 2// linha inicial do titulo recordes
#define CORRECORDES YELLOW// cor do titulo recordes
#define PORCENTAGEMMERGULHADORESMENU 0// porcentagem de mergulhadores do menu

// constantes de cores dos objetos do jogo
#define CORSUBMARINOJOGADOR YELLOW
#define CORSUBMARINOINIMIGO LIGHTMAGENTA
#define CORMERGULHADOR LIGHTGREEN

#define CORTITULOJOGO YELLOW// cor do titulo do jogo
#define COLUNATITULO1 22// coluna inicial de yellow
#define COLUNATITULO2 15// coluna inicial de submarine
#define LINHAINICIALTITULO 2// linha inicial do titulo do jogo

//estrutura do submarino do jogador
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
    COLORS cor;// cor do submarino
} SUBMARINO;

// estrutura do obstaculo
typedef struct  {
    int tipo;//1: submarino inimigo 2: mergulhador
    COORD posicao;// posicao do obstaculo
    int orientacao;// orientacao do obstaculo(ESQUERDA ou DIREITA)
    COLORS cor;// cor do obstaculo
} OBSTACULO;

// estrutura da interface do jogo
typedef struct  {
    int vidas;// vidas atuais do jogador
    int mergulhadores;// mergulhadores atuais do jogador
    int oxigenio;// oxigenio atual do jogador
    int pontuacao;// pontuacao atual do jogador
    unsigned long int tempo;// tempo sobrevivido atual do jogador
} INTERFACEJOGO;

// estrutura do torpedo
typedef struct  {
    COORD posicao;// posicao do torpedo
    int status;//0 esquerda, 1 direita, 2 nao disparado
} TORPEDO;

// estrutura utilizada para salvar e carregar jogo
typedef struct{// estrutura utilizada para salvar e carregar jogo
    SUBMARINO submarino;// informacoes do submarino no instante que o jogo foi salvo
    OBSTACULO obstaculos[NUMOBSTACULOS];// informacoes dos obstaculos no instante que o jogo foi salvo
    TORPEDO torpedo;// informacoes do torpedo no instante que o jogo foi salvo
} JOGO;




int guarda_estrutura(SUBMARINO submarino,OBSTACULO obstaculos[],TORPEDO torpedo);
void tenta_guardar_estrutura(SUBMARINO submarino,OBSTACULO obstaculos[],TORPEDO torpedo);
void le_tecla_menu (char *tecla, int *opcao_atual, int lim_superior,int lim_inferior);
void imprime_seta_inicial(int pos);
void imprime_opcoes_menu_salvar();
void preencherArquivo(FILE **arq);
void buscaNomePontuacao(char nomes[][MAXSTRINGNOME], int pontuacao[],FILE **arq, int numero_recordes);


void imprime_moldura_menu();
void creditos();
void imprime_moldura();
int colidiu_torpedo_mergulhador (COORD torpedo,COORD obstaculo);
int colidiu_torpedo_submarino_inimigo (COORD torpedo,COORD obstaculo);
int colidiu_sub_mergulhador(COORD sub, COORD obstaculo );
int colidiu_sub_inimigo (COORD sub, COORD obstaculo );
void menu();
void animacao_menu(int *opcao,OBSTACULO *obstaculos);
void switch_opcao_menu(int opcao);
void mostraTituloRecordes();
void mostraTabelaRecordesAuxiliar();
void imprime_opcoes_menu();
void switch_menu_cor(int opcao_atual);
void imprime_titulo();
void novo_jogo();
void carregar_jogo();
void copia_vetor_obstaculos(OBSTACULO vetor1[],OBSTACULO vetor2[],int tam); 
void imprime_opcoes_menu_salvar();
int teste_inteiro_valido(char *str);
int tam_int_para_char(int num);
void imprime_submarino_controla_agua(SUBMARINO submarino); 
void imprime_interface_pontuacao(); 
void imprime_mergulhadores_vazio();
void imprime_opcoes_menu_pausa();
int letra_ponto_digito_espaco(char leitura);
void dispara_torpedo(SUBMARINO *submarino, TORPEDO *torpedo);
void move_torpedo(TORPEDO *torpedo);
void desenha_torpedo(TORPEDO torpedo);
void apaga_torpedo(TORPEDO torpedo);
void atualiza_torpedo(TORPEDO *torpedo);
void animacao_sem_vidas(SUBMARINO submarino,OBSTACULO *obstaculos); 
void animacao_lados_sem_vidas(SUBMARINO *submarino);
void apaga_submarino(SUBMARINO submarino);
void apaga_obstaculos (OBSTACULO *obstaculo);
void imprime_agua();
void imprime_obstaculos (OBSTACULO *obstaculo);
void apaga_mergulhador(OBSTACULO mergulhador);
void apaga_submarino_inimigo(OBSTACULO submarino_inimigo);
void imprime_mergulhador(COORD posicao,int orientacao,int cor);
void imprime_submarino(COORD posicao,int orientacao,int cor);
void pintar_jogo();
void pintar_fora_agua();
void pintar_agua();
void pintar_tela();
void limpa_tela_moldura();
void limpa_input();

void limpa_input() {// limpa as teclas que restaram no stdin
    while(kbhit()) {
        getch();
    }
}

void limpa_tela_moldura() {// limpa a tela mas mantem a moldura
    clrscr();
    imprime_moldura_menu();
}

void pintar_tela() {// pinta a tela na cor do menu
    int i,j;
    textbackground(CORMENU);
    clrscr();
}

void pintar_agua() {// pinta a agua
    int i,j;
    textbackground(CORAGUA);
    for (i = LINHAINICIAL;i <= LINHAINTERFACEINFERIOR-1; i++) {
        for (j = COLUNA1;j <= COLUNA2; j++) {
            putchxy(j,i,'\0');
        }
    }
}

void pintar_fora_agua() {// pinta a regiao fora da agua que nao eh interface
    int i,j;
    textbackground(CORFORADAGUA);
    for (i = LINHAINTERFACESUPERIOR+1;i < LINHAINICIAL; i++) {
        for (j = COLUNA1;j <= COLUNA2; j++) {
            putchxy(j,i,'\0');
        }
    }
}

void pintar_jogo() {// pinta a tela de jogo com as cores adequadas
    pintar_fora_agua();
    pintar_agua();
    // como a maioria do que tem q ser printado na tela estara em baixo dagua
    // se colocou a cor do background para CORAGUA e se trocara a cor do background
    // quando for necessario imprimir em outra regiao da tela
}

void imprime_submarino(COORD posicao,int orientacao,int cor) {// imprime sub  uso do if pois depende da orientacao
    textcolor(cor);
    if (orientacao) {// eh diferente de 0 logo direita
        if (posicao.Y-1<LINHAINICIAL) {// se uma posicao do sub esta fora dagua
            textbackground(CORFORADAGUA);
            cputsxy(posicao.X+1,posicao.Y-1,"___|O|___");
            textbackground(CORAGUA);
        } else {
            cputsxy(posicao.X+1,posicao.Y-1,"___|O|___");
        }
        cputsxy(posicao.X,posicao.Y,">\\________)");

    } else {// eh zero logo esquerda
        if (posicao.Y-1<LINHAINICIAL) {// se uma posicao do sub esta fora dagua
            textbackground(CORFORADAGUA);
            cputsxy(posicao.X+1,posicao.Y-1,"___|O|___");
            textbackground(CORAGUA);
        } else {
            cputsxy(posicao.X+1,posicao.Y-1,"___|O|___");
        }
        cputsxy(posicao.X,posicao.Y,"(________/<");
    }
    textcolor(CORTEXTO);
}

void imprime_mergulhador(COORD posicao,int orientacao,int cor) {// imprime mergulhador uso do if pois depende da orientacao
    textcolor(cor);
    if (orientacao) {// eh diferente de 0 logo direita
        cputsxy(posicao.X,posicao.Y,">->O");
    } else {// eh zero logo esquerda
        cputsxy(posicao.X,posicao.Y,"O<-<");
    }
    textcolor(CORTEXTO);
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
    textcolor (CORSUPERFICIEAGUA);
    for (i = COLUNA1+1;i<COLUNA2;i++) {
        putchxy(i,LINHAINICIAL,'~');
    }
    textcolor (CORTEXTO);
}

void apaga_obstaculos (OBSTACULO *obstaculo) {
    // passando um ponteiro para a pos inicial de um vetor de obstaculos apaga os obstaculos dele ate NUMOBSTACULOS
    // utilizacao do else if no final pois foi convencionado que se o tipo for 0 representa obstaculo vazio
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

void apaga_submarino(SUBMARINO submarino) {// funcao que apaga sub
    // posicoes que devem ser apagadas n dependem da orientacao logo sem if
    if (submarino.posicao.Y-1<LINHAINICIAL) {// se uma posicao do sub esta fora dagua
        textbackground(CORFORADAGUA);
        cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"         ");
        textbackground(CORAGUA);
    } else {
        cputsxy(submarino.posicao.X+1,submarino.posicao.Y-1,"         ");
    }
    cputsxy(submarino.posicao.X,submarino.posicao.Y,"           ");

}

void animacao_lados_sem_vidas(SUBMARINO *submarino) {// parte da animacao de morte(movimento para os lados)
    int i;
    for (i = 0;i<4;i++) {
        if (i%3) {
            if (submarino->posicao.X-1>COLUNA1) {
                submarino->posicao.X--;
            }
        } else {
            if (submarino->posicao.X+COMPRIMENTOSUBMARINO<COLUNA2) {
                submarino->posicao.X++;
            }
        }
        imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);
        Sleep(TEMPODEMORTE);
        apaga_submarino(*submarino);
    }
}

void animacao_sem_vidas(SUBMARINO submarino,OBSTACULO *obstaculos) {// faz uma animacao quando o jogador perde
    apaga_obstaculos(obstaculos);// tira os obstaculos da tela
    while(submarino.posicao.Y<LINHAINTERFACEINFERIOR) {// vai ate a linha antes da moldura
        animacao_lados_sem_vidas(&submarino);
        submarino.posicao.Y+=ALTURASUBMARINO;
        MessageBeep(MB_ICONWARNING);
    }
    limpa_input();// para caso o jogador apertar botoes durante a animacao nao seja pulada a tela de game over
    pintar_tela();
    imprime_moldura_menu();
    cputsxy(METADEX,METADEY,"GAME OVER");
    getch();
}


// funcao que gera obstaculos
//rand()%100+1 retorna um numero entre 1 e 100 logo pode ser usado para porcentagem
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


void atualiza_torpedo(TORPEDO *torpedo){// atualiza a posicao do torpedo e testa se atingiu as paredes
    if(torpedo->status == DIREITA){
        torpedo->posicao.X += CAMINHOPORLOOP;//adiciona a quantidade que o torpedo deve andar
    }else {
        torpedo->posicao.X -= CAMINHOPORLOOP;//subtrai a quantidade que o torpedo deve andar
    }
    if(torpedo->posicao.X <= COLUNA1 || torpedo->posicao.X + COMPRIMENTOTORPEDO - 1 >= COLUNA2){ // atingiu as bordas
        torpedo->status = NAODISPARADO; //seta o status para "Nao disparado" assim o jogador pode disparar outro torpedo
    }
}

void apaga_torpedo(TORPEDO torpedo){//apaga o torpedo
    cputsxy(torpedo.posicao.X, torpedo.posicao.Y, "  ");
}

void desenha_torpedo(TORPEDO torpedo) {// desenha o torpedo
    //uso do else if pois o torpedo pode nao ter sido disparado
   if(torpedo.status == DIREITA){
        cputsxy(torpedo.posicao.X, torpedo.posicao.Y, "->");
    } else if(torpedo.status == ESQUERDA){
        cputsxy(torpedo.posicao.X, torpedo.posicao.Y, "<-");
    }
}

void move_torpedo(TORPEDO *torpedo){// atualiza o torpedo
        if(torpedo->status == DIREITA || torpedo->status == ESQUERDA){
            apaga_torpedo(*torpedo);//apaga o torpedo
            atualiza_torpedo(torpedo);//atualiza a posicao do torpedo
        }
}

void dispara_torpedo(SUBMARINO *submarino, TORPEDO *torpedo){// funcao que inicializa o torpedo quando ele eh disparado pelo jogador

    //testa se esta dentro da agua e possui o oxigenio necessario para realizar o disparo
    if(submarino->posicao.Y >= INICIOAGUA && submarino->oxigenio > PENALIDADETORPEDO){
            //testa se a posicao do sub permite disparar o torpedo
        if(torpedo->status == NAODISPARADO && submarino->orientacao == DIREITA &&  submarino->posicao.X+COMPRIMENTOSUBMARINO-1+COMPRIMENTOTORPEDO<COLUNA2){
            torpedo->posicao = submarino->posicao;//torpedo esta na mesma posicao do submarino
            torpedo->posicao.X += COMPRIMENTOSUBMARINO;//o comprimento do submarino eh adicionado para colocar o torpedo na frente do submarino
            torpedo->status = DIREITA;//seta a direcao para a qual o torpedo sera disparado
            submarino->oxigenio -= PENALIDADETORPEDO;// penalidade por usar o torpedo

            //testa se a posicao do sub permite disparar o torpedo
        }else if(torpedo->status == NAODISPARADO && submarino->orientacao == ESQUERDA && submarino->posicao.X-COMPRIMENTOTORPEDO>COLUNA1){
            torpedo->posicao = submarino->posicao;//torpedo esta na mesma posicao do submarino
            torpedo->posicao.X -= COMPRIMENTOTORPEDO;//duas posicoes sao subtraidas da posicao para colocar o torpedo na frente do submarino
            torpedo->status = ESQUERDA;//seta a direcao para a qual o torpedo sera disparado
            submarino->oxigenio -= PENALIDADETORPEDO;// penalidade por usar o torpedo
        }
    }
}

int letra_ponto_digito_espaco(char leitura) {// verifica se o char leitura eh uma letra ,um numero, ou um ponto
    if ((leitura>='A' && leitura<='Z')|| (leitura>='a' && leitura<='z') || (leitura>='0' && leitura<='9') || leitura=='.' || leitura==ESPACO){
        return 1;
    } else {
        return 0;
    }
}

// le nome do jogador limita o numero de chars na tela para MAXSTRINGNOME - 1
int le_nome_jogador(char *nome) {
    //char intermed[MAXSTRINGNOME];
    int i = 0;
    char leitura;
    gotoxy(METADEX,METADEY+1);
    do {
        leitura = getch();
        //if (leitura != ENTER && leitura != BACKSPACE && leitura != ESC && i != MAXSTRINGNOME -1) {
        if (letra_ponto_digito_espaco(leitura) && i != MAXSTRINGNOME -1) {
            nome[i] = leitura;
            putchxy(METADEX+i,METADEY+1,leitura);
            i++;
        } else if (leitura == BACKSPACE && i != 0) {
            i--;
            putchxy(METADEX+i,METADEY+1,'\0');
        } else if(leitura == ESC) {
            return 0;
        }
    } while(i==0 || leitura!=ENTER);
    nome[i] = '\0';
    return 1;
}


void imprime_opcoes_menu_pausa() {// imprime opcoes do menu de pausa
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
        *a = getch();
        if (*a==TECLASAUXILIARES) {
            *a = getch();
            switch(*a) {
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
                    break;
                case SETADIREITA:
                    if (submarino->posicao.X+COMPRIMENTOSUBMARINO+CAMINHOPORLOOP-1<COLUNA2) {
                        apaga_submarino(*submarino);
                        if (submarino->orientacao==DIREITA) {
                            submarino->posicao.X +=CAMINHOPORLOOP;
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
                    if (submarino->posicao.X-CAMINHOPORLOOP>COLUNA1) {
                        apaga_submarino(*submarino);
                        if (submarino->orientacao==ESQUERDA) {
                            submarino->posicao.X -=CAMINHOPORLOOP;
                        } else {
                            submarino->orientacao = ESQUERDA;
                        }
                        imprime_submarino(submarino->posicao,submarino->orientacao,submarino->cor);
                        if (submarino->posicao.Y==LINHAINICIAL) {
                            imprime_agua();
                        }
                    }
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


void imprime_interface_pontuacao() {//imprime interface inicial da pontuacao
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
    if (submarino->vidas>0) {// caso nao tenha mais vidas, nao � necessario recolocar na pos inicial
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
    }
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
    textcolor(LIGHTRED);
    for (i = 0; i<interface_jogo->vidas;i++) {
       cputsxy(COLUNAVIDAS+3*i,LINHAINTERFACESUPERIOR-1,"<3 ");
    }
    textcolor(WHITE);
}

// imprime o oxigenio inicial na tela
void imprime_oxigenio(INTERFACEJOGO *interface_jogo) {
    int i;
    //int oxigenio = 30;
    putchxy(COLUNA2-1,LINHAINTERFACEINFERIOR+1,']');
    cputsxy(COLUNAOXIGENIO,LINHAINTERFACEINFERIOR+1,"Oxigenio: [");
    for (i = 0; i<interface_jogo->oxigenio;i++) {
       cputsxy(COLUNAOXIGENIO+11+i,LINHAINTERFACEINFERIOR+1,"|");
    }
}

// atualiza o tempo de jogo na interface
void atualiza_interface_tempo (INTERFACEJOGO *interface_jogo) {
    gotoxy(38,2);// acho q nas horas n vai da problema
    cprintf("%02d:%02d:%02d",interface_jogo->tempo/1000/60/60,interface_jogo->tempo/1000/60,interface_jogo->tempo/1000%60);
}

// faz verificacoes entre os valores dos campos do sub e da interface para saber se eh necessario
// atualizar a interface e se for faz a atualizacao
void atualiza_interface(SUBMARINO *submarino,INTERFACEJOGO *interface_jogo) {
    textbackground(CORINTERFACE);
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
    if ((submarino->tempo-interface_jogo->tempo)%SEGUNDO<TEMPODELOOP) {// a diferen�a sempre sera !=0
        interface_jogo->tempo = submarino->tempo;
        atualiza_interface_tempo(interface_jogo);
    }
    textbackground(CORAGUA);
}

void imprime_interface_mergulhadores(INTERFACEJOGO *interface_jogo) {
    int i;
    for (i = 0; i<interface_jogo->mergulhadores;i++) {
        cputsxy(COLUNA1+1+(COMPRIMENTOMERGULHADOR+1)*i,LINHAINTERFACEINFERIOR+1,">->O ");
    }
}


void imprime_interface(INTERFACEJOGO *interface_jogo) {// imprime a interface inicial
    textbackground(CORINTERFACE);
    imprime_interface_pontuacao();
    atualiza_pontuacao(interface_jogo);//
    imprime_oxigenio(interface_jogo);// imprime completo
    imprime_vidas(interface_jogo);
    atualiza_interface_tempo(interface_jogo);
    imprime_interface_mergulhadores(interface_jogo);
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
                apaga_torpedo(*torpedo);
                apaga_submarino_inimigo(*obstaculo);//apaga o sub inimigo atingido
                obstaculo->tipo = SEMOBSTACULO;
                sub->pontuacao += PONTUACAODESTRUICAOSUBINIMIGO;//aumenta a pontuacao do jogador por ter destruido um sub
            }else if(obstaculo->tipo == MERGULHADOR && colidiu_torpedo_mergulhador (torpedo->posicao,obstaculo->posicao)){
                torpedo->status = NAODISPARADO;//reseta o status do torpedo para que ele seja apagado e o jogador possa disparar outros
                apaga_torpedo(*torpedo);
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
            testa_colisao_torpedo_individual(torpedo, &obstaculos[i], submarino);
            testa_colisao_submarino_inimigo(submarino,&obstaculos[i]);
            imprime_obstaculo_individual(obstaculos[i]);
        } else if (obstaculos[i].tipo==MERGULHADOR){
            apaga_mergulhador(obstaculos[i]);
            atualiza_obstaculo_individual(&obstaculos[i].posicao,obstaculos[i].orientacao);
            testa_colisao_tela_mergulhador(&obstaculos[i]);
            testa_colisao_torpedo_individual(torpedo, &obstaculos[i], submarino);
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
// um if para ver se se colidiu com todos obstaculos e caso colida imprimir novamente
// imprimi-se o sub sempre quando o sub estiver fora dagua
void imprime_submarino_controla_agua(SUBMARINO submarino) {
    if (submarino.posicao.Y!=LINHAINICIAL) {
        imprime_submarino(submarino.posicao,submarino.orientacao,submarino.cor);
    }
}

void gravarRecordes(char nomes[][MAXSTRINGNOME], int pontuacao[]){
    int i;
    FILE *arq;
    arq = fopen("recordes.txt", "w");
    if(arq){
        for(i = 0; i < NUMRECORDES; i++){
            fprintf(arq, "%s;%d;\n", nomes[i], pontuacao[i]);
        }
        fflush(arq);
        fclose(arq);
    }else{
        cputsxy(METADEX, METADEY, "ALGO DEU ERRADO");
    }

}

void bubblesort(FILE **arq){
    //FILE *arq;
    int pontuacao[NUMRECORDES + 1];//no arquivo de texto, o maximo de registro possivel � o NUMRECORDES + 1(que eh o novo recorde recem adicionado)
    char nomes[NUMRECORDES + 1][MAXSTRINGNOME];
    int i;
    int ind, fim, sinal, aux;
    char auxStr[MAXSTRINGNOME];
    fim = NUMRECORDES;
    //buscaNomesEPontuacao(arq, nomes, pontuacao);
    buscaNomePontuacao(nomes, pontuacao, arq,NUMRECORDES+1);

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
    fclose(*arq);
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


int tam_int_para_char(int num) {// retorna o numero de chars necessarios para armazenar um inteiro positivo
    int i = 1;
    while(num>=10) {
        num = num/10;
        i++;
    }
    return i;
}

int teste_inteiro_valido(char *str) {// testa se o valor em str eh um inteiro valido
    //obs: funcionara apenas se o valor da string puder ser convertido para long long
    long long valor_str = strtoll(str,NULL,0);
    return valor_str<=(long long)INT_MAX;
}

// obs: troquei para so poder botar no nome digitos letras e .
// a funcao funciona mas so verifica se o numero eh um int valido so verifica se tem o num
// de digitos validos e tambem n verifica se o nome so tem caracteres que sao . digitos ou letras
// obs a funcao recordes vai dar problema quando altera o arquivo pq mesmo q tenha a estrutura certa
// vai precisar reordenar ele pois pode estar com a ordem errada
// essas alteracoes a gente pode fazer na aula
int testaIntegridade2(FILE *arq){
    //FILE *arq;
    int i = 0;
    int posicao_branco;
    int erro = 0;
    int tentativas = 0;
    int separador_encontrado = 0;
    char letra;
    int int_max = tam_int_para_char(INT_MAX);
    int num_letras = 0;
    int tam_nome;
    char tmp2[MAXSTRINGNOME];
    char tmp[int_max+1];

    tmp[int_max] = '\0';

    rewind(arq);
    while(!feof(arq) && !erro && i != NUMRECORDES){
        // ideia: ler ate o ;(proibir jogador de colocar ;)
        // se leu no max 8 entao parte para testar numero
        // numero pode ter ate int_max

        tam_nome = 0;//
        separador_encontrado = 0;
        while(!separador_encontrado && !erro && tam_nome<MAXSTRINGNOME) {//msn e n msn-1 pq vai pegar o ;
        //while(!separador_encontrado && !erro) {

            letra = getc(arq);
            tam_nome++;
            if (letra == '\n' || letra == EOF) {
                erro = 1;
            } else if (letra == ';') {// eof para o ultimo caso
                separador_encontrado = 1;
            } else if(tam_nome==MAXSTRINGNOME) {
                erro = 1;
            } else if (!letra_ponto_digito_espaco(letra)) {
                erro = 1;
            }
        }
        // se n achou separador, ou achou eof,\n ou estourou o tamanho mas em todos esses casos erro = 1 logo nem entrara no while
        // achou separador logo tem q verificar ate \n( no caso do separador encontrado se o ; se for o EOF nao entra no while)

        //ou encontrara o \n ou o eof ou ; unico caso certo eh o ; com o tamanho adequado
        // tem q ler tudo ate o ; e quando achar se tiver o tam certo e o proximo for \n esta certo
        num_letras = 0;
        separador_encontrado = 0;
        //while(!erro && letra!='\n' && letra!=EOF && num_letras<=int_max){
        while(!erro && !separador_encontrado && num_letras<=int_max){// pode stourar num letras
            letra = getc(arq);
            if (letra<=57 && letra>=48) {// numeros na tabela ascii
                tmp[num_letras] = letra;
                num_letras++;
            } else if(letra=='\n' || letra==EOF) {
                erro = 1;
            } else if (letra==';') {
                separador_encontrado = 1;
            } else {
                erro = 1;
            }
        }
        if (num_letras>=int_max && !erro) {
            if (!teste_inteiro_valido(tmp)) {
                erro = 1;
            }
        }

        // tem que ter encontrado o separador
        if (!separador_encontrado) {
            erro = 1;
        } else {
            letra = getc(arq);
            if (letra!='\n') {
                erro = 1;
            }
        }
        /*
        if (num_letras>int_max) {//&& getc(arq)=='\n'
            erro = 1;
        }*/




        i++;
    }

    letra = getc(arq);
    if (letra!=EOF) {
        erro = 1;
    }

    if(erro == 0){
    //if(i == NUMRECORDES + 1 && erro == 0){
        return 1;
    }else{
        return 0;
    }
}


void guarda_pontuacao(SUBMARINO sub){
    FILE *arq;
    int tentativas = 0;
    while((arq = fopen("recordes.txt", "r+")) == NULL && tentativas < 10){
        arq = fopen("recordes.txt", "r+");
        tentativas++;
    }
    if(tentativas < 10){
        if(testaIntegridade2(arq)){
            fprintf(arq, "%s;%d;\n", sub.nome, sub.pontuacao);
            fflush(arq);
            bubblesort(&arq);
            fclose(arq);
         }else{
            fclose(arq);
            arq = fopen("recordes.txt", "w+");
            if(arq){
                preencherArquivo(&arq);
                fprintf(arq, "%s;%d;\n", sub.nome, sub.pontuacao);
                fflush(arq);
                bubblesort(&arq);
                fclose(arq);
            }else{
                cputsxy(METADEX, METADEY, "ALGO DEU ERRADO");
            }
        }
    }else{
        arq = fopen("recordes.txt", "w+");
            if(arq){
                preencherArquivo(&arq);
                fprintf(arq, "%s;%d;\n", sub.nome, sub.pontuacao);
                fflush(arq);
                bubblesort(&arq);
                fclose(arq);
            }else{
                cputsxy(METADEX, METADEY, "ALGO DEU ERRADO");
            }
    }



}

// funcao move o submarino e os obstaculos
// n vou comentar individualmente as condicoes para ver se o submarino atravessou a borda mas o raciocinio eh semelhante ao usado
// para ver se o submarino inimigo fez a mesma coisa
void game_loop(SUBMARINO *submarino, OBSTACULO *obstaculos, TORPEDO *torpedo){// deixei ainda com dois switch por que vai ter 3 comandos esc espaco e setas
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
        gera_obstaculos(obstaculos,NUMOBSTACULOS,PORCENTAGEMMERGULHADORES,BLOCOINICIALSPAWNJOGO,CORSUBMARINOINIMIGO,CORMERGULHADOR);// funcao que gera obstaculos nas posicoes que n tem obstaculos
        switch_game_loop(&a,submarino,obstaculos, torpedo,&sair );
        // depois que faz o comando do submarino apaga os obstaculos na tela e atualiza as posicoes dos obstaculos
        // dai testa se alguma dessas novas posicoes bate na tela, ou seja se o obstaculo atravessou a tela
        // se esse for o caso esse obstaculo desaparece e imprime os obstaculos que ainda nao atravessaram
        percorre_vetor_obstaculos(obstaculos,torpedo,submarino);
        imprime_submarino_controla_agua(*submarino);
        move_torpedo(torpedo);
        desenha_torpedo(*torpedo);
        resgatou_mergulhadores(submarino);
        atualiza_oxigenio_submarino(submarino);
        atualiza_pontuacao_submerso(submarino);
        testa_morte_oxigenio (submarino);
        atualiza_interface(submarino,&interface_jogo);
    } while(sair!=1 && submarino->vidas>0);


    if (submarino->vidas==0) {
        animacao_sem_vidas(*submarino,obstaculos);
        guarda_pontuacao(*submarino);
    } else {
        tenta_guardar_estrutura(*submarino,obstaculos,*torpedo);
    }
    //textbackground(CORMENU);
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
    textbackground(CORAGUA);

}


void imprime_moldura_menu() {// imprime a moldura do menu
    int x,y;
    for(x = COLUNA1+1;x<COLUNA2;x++) {
            putchxy(x,LINHA1,'-');
            putchxy(x,LINHA2,'-');
    }
    for(y = LINHA1+1;y<LINHA2;y++) {
            putchxy(COLUNA1,y,'|');
            putchxy(COLUNA2,y,'|');
    }
}

void imprime_opcoes_menu_salvar() {// imprime opcoes menu salvar
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
    pintar_tela();
    imprime_moldura_menu();
    cputsxy(METADEX,METADEY+1,"Deseja salvar o jogo:");
    imprime_interface_salvar(&opcao);

    while(opcao==SALVAR && gravou==0) {
        gravou = guarda_estrutura(submarino,obstaculos,torpedo);
        if (!gravou) {
            cputsxy(METADEX,METADEY+1,"Deseja tentar salvar novamente:");
            imprime_interface_salvar(&opcao);
        } else {
            strcpy(arquivo,submarino.nome);
            strcat(arquivo,".bin");
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
    limpa_tela_moldura();
    Sleep(SALVANDOJOGO);
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

// copia de um vetor2 para um vetor1 cada obstaculo do vetor, tam eh o tamanho do vetor2
// vetor1 tem que ter tamanho maior ou igual ao tamanho de vetor2
void copia_vetor_obstaculos(OBSTACULO vetor1[],OBSTACULO vetor2[],int tam) {
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
    //gets(nome_arq);
    //le_nome_jogador(nome_arq);;// so para n deixar escrever mais do que precisa
    if (le_nome_jogador(nome_arq)) {
        strcat(nome_arq,".bin");
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
    }
    //} // Fim do while
    //fecha
} // Fim da fun��o


void carregar_jogo() {// carrega o jogo
    OBSTACULO  obstaculos [NUMOBSTACULOS];
    SUBMARINO sub;
    TORPEDO torpedo;
    if (le_estrutura(&sub,obstaculos,&torpedo)) {// se o usuario nao voltou para o menu e nao deu erro
        clrscr();
        game_loop(&sub,obstaculos, &torpedo);
    }
}


void novo_jogo() {// comeca um novo jogo
    OBSTACULO  obstaculos [NUMOBSTACULOS] = {};
    SUBMARINO sub = {"",{COLUNAINICIAL,LINHAINICIAL},DIREITA,VIDASINICIAIS,OXIGENIOMAXIMO,0,0,0,0,YELLOW};
    TORPEDO torpedo = {sub.posicao, NAODISPARADO};
    cputsxy(METADEX,METADEY,"Digite seu nome:");
    gotoxy(METADEX,METADEY+1);
    if (le_nome_jogador(sub.nome)) {// se o usuario nao voltou para o menu
        clrscr();
        game_loop(&sub,obstaculos, &torpedo);
    }
}

void imprime_titulo() {// imprime o titulo do jogo
    textcolor(CORTITULOJOGO);
    cputsxy(COLUNATITULO1,LINHAINICIALTITULO, "___ ________ ____  ____  _____ __  __ ");
    cputsxy(COLUNATITULO1,LINHAINICIALTITULO+1, "\\  |  /   __/  _/ /  _/ /  _  /   /  \\  ");
    cputsxy(COLUNATITULO1,LINHAINICIALTITULO+2, " |   ||   __|  |--|  |--|  |  |  /\\  |  ");
    cputsxy(COLUNATITULO1,LINHAINICIALTITULO+3, " \\___/\\_____\\_____\\_____\\_____\\__/\\__/");

    cputsxy(COLUNATITULO2,LINHAINICIALTITULO+4, " _____ __ __ _____ __  __ _____ _____ ___ _____ _____ ");
    cputsxy(COLUNATITULO2,LINHAINICIALTITULO+5, "/  ___/  |  /  _  /  \\/  /  _  /  _  /___/  _  /   __\\");
    cputsxy(COLUNATITULO2,LINHAINICIALTITULO+6, "|___  |  |  |  _  |  \\/  |  _  |  _  |   |  |  |   __|");
    cputsxy(COLUNATITULO2,LINHAINICIALTITULO+7, "<_____\\_____\\_____\\__ \\__\\__|__\\__|\\_\\___\\__|__\\_____/");
    textcolor(CORTEXTO);
}

// le a tecla digitada no menu e faz alteracoes correspondentes ao que foi pressionado
void le_tecla_menu (char *tecla, int *opcao_atual, int lim_superior,int lim_inferior){// funcao que deve ser usada com menu
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


void imprime_opcoes_menu() {// imprime as opcoes do menu
    cputsxy(METADEX, METADEY, "Novo Jogo");
    cputsxy(METADEX, METADEY + 1, "Carregar Jogo");
    cputsxy(METADEX, METADEY + 2, "Recordes");
    cputsxy(METADEX, METADEY + 3, "Creditos");
    cputsxy(METADEX, METADEY + 4, "Sair");
}


void imprime_seta_inicial(int pos) {// imprime a seta inicial que marca a opcao do menu
    textcolor(YELLOW);
    putchxy(METADEX-1,METADEY+pos,'>');
    textcolor(WHITE);
}

void buscaNomePontuacao(char nomes[][MAXSTRINGNOME], int pontuacao[],FILE **arq, int numero_recordes){
    int i = 0;
    char intermed[90];
    rewind(*arq);
        while(i < numero_recordes && !feof(*arq)){
            fgets(intermed,90,*arq);
            strcpy(nomes[i],strtok(intermed,";"));
            pontuacao[i] = atoi(strtok(NULL,";"));
            i++;
        }
}

void mostraTabelaRecordesAuxiliar() {// mostra parte da tabela dos recordes
    textcolor(CORRECORDES);
    cputsxy(METADEX, METADEY-1, "JOGADORES");
    cputsxy(METADEX+10, METADEY-1, "PONTOS");
    textcolor(CORTEXTO);
}

void mostraTabelaRecordes(char nomes[NUMRECORDES][MAXSTRINGNOME], int pontuacoes[]){
    int i;
    mostraTabelaRecordesAuxiliar();
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
        fprintf(*arq, "%s;%d;\n", nome, pontuacao);
    }
    fflush(*arq);
    //fclose(*arq);
}

void mostraTituloRecordes(){// imprime titulo recordes
    textcolor(CORRECORDES);
    cputsxy(COLUNARECORDES,LINHAINICIALRECORDES, " _____  _____  _____  _____  _____  _____  _____  _____ ");
    cputsxy(COLUNARECORDES,LINHAINICIALRECORDES+1, "/  _  \\/   __\\/     \\/  _  \\/  _  \\|  _  \\/   __\\/  ___>");
    cputsxy(COLUNARECORDES,LINHAINICIALRECORDES+2, "|  _  <|   __||  |--||  |  ||  _  <|  |  ||   __||___  |");
    cputsxy(COLUNARECORDES,LINHAINICIALRECORDES+3, "\\__|\\_/\\_____/\\_____/\\_____/\\__|\\_/|_____/\\_____/<_____/");
    textcolor(CORTEXTO);
}

void recordes(){
    int resp;
    char nomes[NUMRECORDES][MAXSTRINGNOME];
    int pontuacoes[NUMRECORDES];
    FILE *arq;
    arq = fopen("recordes.txt", "r+"); //so teste para verificar se o arquivo existe
    if(arq != NULL && testaIntegridade2(arq)){
        bubblesort(&arq);
        if (arq = fopen("recordes.txt", "r")) {
            buscaNomePontuacao(nomes, pontuacoes, &arq,NUMRECORDES);
            fclose(arq);
            mostraTituloRecordes();
            mostraTabelaRecordes(nomes, pontuacoes);
        } else {
            printf("ERRO AO CARREGAR O ARQUIVO");
        }
    }else{
        arq = fopen("recordes.txt", "w+");
        if(arq == NULL){
            printf("ERRO AO CARREGAR O ARQUIVO");
        }else{
            preencherArquivo(&arq);
            buscaNomePontuacao(nomes, pontuacoes, &arq,NUMRECORDES);
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

void switch_opcao_menu(int opcao) {// switch para as opcoes do menu principal do jogo
    limpa_tela_moldura();
    switch(opcao){// ve qual a opcao que deu enter
        case NOVOJOGO:
            novo_jogo();
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
            cputsxy(METADEX,METADEY,"Espero que volte logo...");
            getch();
    }
}

void animacao_menu(int *opcao,OBSTACULO *obstaculos) {// animacao das opcoes do jogador e dos submarinos da tela
    char resp = '\0';// tecla pressionada pelo jogador
    do {// fica atualizando posicao ate que de enter
        Sleep(TEMPODELOOPMENU);// para dar um tempo entre loops
        gera_obstaculos(obstaculos,NUMOBSTACULOSMENU,PORCENTAGEMMERGULHADORESMENU,BLOCOINICIALSPAWNMENU,CORSUBMARINOJOGADOR,CORMERGULHADOR);
        if (kbhit()) {
            le_tecla_menu(&resp,opcao,NOVOJOGO,SAIR);
            imprime_opcoes_menu();// imprime menu novamente para colocar a cor do texto nas opcoes nao selecionadas
            switch_menu_cor(*opcao);
        }
        percorre_vetor_obstaculos_menu (obstaculos);
    } while(resp!=ENTER);
}

void menu(){// menu do jogo
    OBSTACULO  obstaculos [NUMOBSTACULOSMENU] = {};// obstaculos da animacao da tela
    int opcao;// opcao do jogador
    pintar_tela();
    do {
        opcao = 0;
        limpa_tela_moldura();
        imprime_titulo();
        imprime_opcoes_menu();
        switch_menu_cor(opcao);
        imprime_seta_inicial(NOVOJOGO);// imprime marcacao da primeira opcao
        animacao_menu(&opcao,obstaculos);
        switch_opcao_menu(opcao);
    } while (opcao!=SAIR);
}

void creditos(){// creditos do jogo
    char voltar;
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
}

int colidiu_sub_mergulhador(COORD sub, COORD obstaculo ) {// testa se um submarino colidiu com um mergulhador utilizando as coordenadas
    if((sub.Y == obstaculo.Y) && ((sub.X + COMPRIMENTOSUBMARINO - 1) >= obstaculo.X) && (sub.X <= (obstaculo.X + COMPRIMENTOMERGULHADOR - 1))){
            return 1;
        }else {
            return 0;
        }
}

int colidiu_sub_inimigo (COORD sub, COORD obstaculo ) {// testa se um submarino colidiu com um submarino inimigo utilizando as coordenadas
    if((sub.Y == obstaculo.Y) && ((sub.X + COMPRIMENTOSUBMARINO - 1) >= obstaculo.X) && (sub.X <= (obstaculo.X + COMPRIMENTOSUBMARINO - 1))){
            return 1;
        }else{
            return 0;
        }
}

int colidiu_torpedo_mergulhador (COORD torpedo,COORD obstaculo) {// testa se um torpedo colidiu com um mergulhador utilizando as coordenadas
    if((torpedo.Y == obstaculo.Y) && ((torpedo.X + COMPRIMENTOTORPEDO - 1) >= obstaculo.X) && (torpedo.X <= (obstaculo.X + COMPRIMENTOMERGULHADOR - 1))){
            return 1;
        }else {
            return 0;
        }
}

int colidiu_torpedo_submarino_inimigo (COORD torpedo,COORD obstaculo) {// testa se um torpedo colidiu com um submarino inimigo utilizando as coordenadas
    if((torpedo.Y == obstaculo.Y) && ((torpedo.X + COMPRIMENTOTORPEDO - 1) >= obstaculo.X) && (torpedo.X <= (obstaculo.X + COMPRIMENTOSUBMARINO - 1))){
            return 1;
        }else{
            return 0;
        }
}

int main() {
    menu();// inicia o menu do jogo
    return 0;
}
