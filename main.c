//////////////////////////////////////////////////////////////////////////////////
/*  Trabalho feito por:
    Caetano Henrique Muniz Loiola
    Jonas Teixeira
    
    Utilizando a biblioteca gratuita SFML (Simple and Fast Multimedia Library),
    no caso sua versão binding pra C, pois esta se trata de uma biblioteca baseada
    em C++.
    
    Site oficial: https://www.sfml-dev.org/index.php
    Link da binding: https://www.sfml-dev.org/download/csfml/                   */
//////////////////////////////////////////////////////////////////////////////////


//  Bibliotecas utilizadas
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/System.h>
#include <stdbool.h>
#include <stdlib.h>

// Definição de alguns termos que serão utilizados adiante no código
#define larguraTela 512
#define alturaTela  480
#define Clock       sfTime_asSeconds(deltaTempo)
#define movimento   255*Clock*velocidade

// Variáveis globais que serão utilizadas fora do main
float tempoTotal[4]= {0.0,0.0,0.0,0.0};
unsigned int contarSprite[2] = {1,1};

// Protótipo de função que será utilizada para algumas animações
void animarSprite(sfTexture* sprite1, sfTexture* sprite2, sfTexture* sprite3, sfSprite* patoSprite, sfTime* deltaTempo);
void inteiroPraVetor(int inteiro, int vetor[], int tamanho);

int main()
{
    srand(time(NULL));

    // Declaração de variáveis
    bool mouseClicado = false,
    correcaoPos = false,
    cachorroAndou = false,
    cachorroPulo = false,
    colisaoInicialX = false,
    colisaoInicialY = false, 
    colisaoDirX, colisaoEsqX, colisaoTopoY, colisaoBaixoY;

    int direcaoInicialX = rand() % 2,
    posInicialX = rand () % 64,
    balas = 3,
    patosMortos = 0,
    rodadas = 1,
    pontos = 0,
    placar[6] = {0, 0, 0, 0, 0, 0},
    rodadasVetor[2] = {0, 0};

    float velocidade = 1.00;

    /*
     enum Etapa é a classe de enumeração que controlará as etapas do jogo:
     'cachorroAndando' e 'cachorroCaindo' são apenas as animações iniciais de quando o jogo inicia
     'cachorroFarejou' é quando o jogo começa de verdade: O pato e o tiro são ativados
     'patoAcertado' representa a etapa após o pato ser acertado
     'patoFugiu' representa a etapa de animação e reinicio de quando o pato foge
    */
    enum Etapa {
        cachorroAndando,
        cachorroCaindo,
        cachorroFarejou,
        patoAcertado,
        patoFugiu
    }; enum Etapa etapa = cachorroAndando;

    // Configura a janela
    sfVideoMode resolucao = {larguraTela,alturaTela, 32};
    sfRenderWindow* janela = sfRenderWindow_create(resolucao, "Duck Hunt", sfClose, NULL);
    sfColor background = sfColor_fromRGB(99, 173, 255);

    // Cria o espaço de memória para o evento, o escutador da janela
    sfEvent event;

    // Cria um retangulo que será usado pra fixar elementos mantendo a proporção, mesmo quando a janela mudar de tamanho
    sfFloatRect view_rectangle = {0,0,0,0};
    sfView* new_view = sfView_createFromRect(view_rectangle);

    // Cria um contador
    sfClock* contador = sfClock_create();
    sfTime deltaTempo = sfTime_Zero;


//////////////////////////////////////////////////////////////////////////////////////////////
/*              Declaração de variáveis do tipo sfIntRect e sfFloatRect                     */
    sfIntRect backgroundRect =  {0,0,512,479};
    sfIntRect cachorroRect[7] = {  {0,0,112,92}
                        ,{0,0,120,115}
                        ,{0,0,107,111}
                        ,{0,0,102,94}
                        ,{0,0,66,84}
                        ,{0,0,66,81}
                        ,{0,0,112,80}};
    sfIntRect patoRect =    {0,0,64,64};
    sfIntRect patoaltRect = {0,0,68,74};
    sfIntRect caindoRect =  {0,0,36,82};
    sfIntRect hitRect =     {0,0,62,84};
    sfIntRect textboxRect = {0,0, 100, 69};
    sfIntRect numRect =     {0,0,16,16};
    sfIntRect balaRect =    {0,0,8,14};
    sfIntRect patoContRect ={0,0,16,14};
    sfFloatRect mouseRect = {0,0,16,16};
//////////////////////////////////////////////////////////////////////////////////////////////
/*                  Declaração de variáveis contendo todas as texturas                      */
    sfTexture* backgroundTextura = sfTexture_createFromFile("assets/background/defaultBackground.png", &backgroundRect);
    sfTexture*           cachorroTextura[12] = {sfTexture_createFromFile("assets/cachorro/c1.png", &cachorroRect[0])
                                    ,sfTexture_createFromFile("assets/cachorro/c2.png", &cachorroRect[0])
                                    ,sfTexture_createFromFile("assets/cachorro/c3.png", &cachorroRect[0])
                                    ,sfTexture_createFromFile("assets/cachorro/c4.png", &cachorroRect[0])
                                    ,sfTexture_createFromFile("assets/cachorro/c5.png", &cachorroRect[0])
                                    ,sfTexture_createFromFile("assets/cachorro/c6.png", &cachorroRect[0])
                                    ,sfTexture_createFromFile("assets/cachorro/c7.png", &cachorroRect[1])
                                    ,sfTexture_createFromFile("assets/cachorro/c8.png", &cachorroRect[2])
                                    ,sfTexture_createFromFile("assets/cachorro/c9.png", &cachorroRect[3])
                                    ,sfTexture_createFromFile("assets/cachorro/cf1.png", &cachorroRect[4])
                                    ,sfTexture_createFromFile("assets/cachorro/cf2.png", &cachorroRect[5])
                                    ,sfTexture_createFromFile("assets/cachorro/cp1.png", &cachorroRect[6])};

    sfTexture*            patoDireitaTextura[3] =  {sfTexture_createFromFile("assets/patos/verde/r1.png", &patoRect)
                                        ,sfTexture_createFromFile("assets/patos/verde/r2.png", &patoRect)
                                        ,sfTexture_createFromFile("assets/patos/verde/r3.png", &patoRect)};
                            
    sfTexture*            patoEsquerdaTextura[3] = {sfTexture_createFromFile("assets/patos/verde/l1.png", &patoRect)
                                        ,sfTexture_createFromFile("assets/patos/verde/l2.png", &patoRect)
                                        ,sfTexture_createFromFile("assets/patos/verde/l3.png", &patoRect)};

    sfTexture*            patoAltEsquerdaTextura[3] =  {sfTexture_createFromFile("assets/patos/verde/l1-alt.png", &patoaltRect)
                                            ,sfTexture_createFromFile("assets/patos/verde/l2-alt.png", &patoaltRect)
                                            ,sfTexture_createFromFile("assets/patos/verde/l3-alt.png", &patoaltRect)};

    sfTexture*            patoCaindoTextura[2] =   {sfTexture_createFromFile("assets/patos/verde/caindo.png", &caindoRect)
                                                    ,sfTexture_createFromFile("assets/patos/verde/caindo2.png", &caindoRect)};

    sfTexture*            numTextura[10] = {sfTexture_createFromFile("assets/ui/numeros/n0.png", &numRect)
                                ,sfTexture_createFromFile("assets/ui/numeros/n1.png", &numRect)
                                ,sfTexture_createFromFile("assets/ui/numeros/n2.png", &numRect)
                                ,sfTexture_createFromFile("assets/ui/numeros/n3.png", &numRect)
                                ,sfTexture_createFromFile("assets/ui/numeros/n4.png", &numRect)
                                ,sfTexture_createFromFile("assets/ui/numeros/n5.png", &numRect)
                                ,sfTexture_createFromFile("assets/ui/numeros/n6.png", &numRect)
                                ,sfTexture_createFromFile("assets/ui/numeros/n7.png", &numRect)
                                ,sfTexture_createFromFile("assets/ui/numeros/n8.png", &numRect)
                                ,sfTexture_createFromFile("assets/ui/numeros/n9.png", &numRect)};

    // Não deu pra armazenar essas texturas num vetor porque a 'struct' é grande demais, e ao compilar,
    // não é carregado corretamente.
    sfTexture*            numV0Textura = sfTexture_createFromFile("assets/ui/numeros/nv0.png", &numRect);
    sfTexture*            numV1Textura = sfTexture_createFromFile("assets/ui/numeros/nv1.png", &numRect);
    sfTexture*            numV2Textura = sfTexture_createFromFile("assets/ui/numeros/nv2.png", &numRect);
    sfTexture*            numV3Textura = sfTexture_createFromFile("assets/ui/numeros/nv3.png", &numRect);
    sfTexture*            numV4Textura = sfTexture_createFromFile("assets/ui/numeros/nv4.png", &numRect);
    sfTexture*            numV5Textura = sfTexture_createFromFile("assets/ui/numeros/nv5.png", &numRect);
    sfTexture*            numV6Textura = sfTexture_createFromFile("assets/ui/numeros/nv6.png", &numRect);   
    sfTexture*            numV7Textura = sfTexture_createFromFile("assets/ui/numeros/nv7.png", &numRect);
    sfTexture*            numV8Textura = sfTexture_createFromFile("assets/ui/numeros/nv8.png", &numRect);
    sfTexture*            numV9Textura = sfTexture_createFromFile("assets/ui/numeros/nv9.png", &numRect);

    sfTexture*            balaTextura =   sfTexture_createFromFile("assets/ui/bala.png", &balaRect);
    sfTexture*            patoContBrancoTextura = sfTexture_createFromFile("assets/ui/patoContadorBranco.png", &patoContRect);
    sfTexture*            patoContVermelhoTextura = sfTexture_createFromFile("assets/ui/patoContVermelho.png", &patoContRect);
    sfTexture*            patoContPretoTextura = sfTexture_createFromFile("assets/ui/patoContPreto.png", &patoContRect);
    sfTexture*            hitTextura =    sfTexture_createFromFile("assets/patos/verde/hit.png", &hitRect);
    sfTexture*            textboxTextura = sfTexture_createFromFile("assets/ui/textbox.png", &textboxRect);

//////////////////////////////////////////////////////////////////////////////////////////////
/*                            Todos as variáveis do tipo sfSprite                         */
    sfSprite*            backgroundSprite = sfSprite_create();
    sfSprite*            textboxSprite = sfSprite_create();
    sfSprite*            numTextboxSprite = sfSprite_create();
    sfSprite*            cachorroSprite = sfSprite_create();
    sfSprite*            patoSprite =   sfSprite_create();
    sfSprite*            mouseSprite = sfSprite_create();
    sfSprite*            rodadaSprite[2] = {sfSprite_create(),sfSprite_create()};
    sfSprite*            balaSprite[3] = {sfSprite_create(), sfSprite_create(), sfSprite_create()};
    sfSprite*            scoreSprite[6] = {sfSprite_create(),sfSprite_create(),sfSprite_create(),sfSprite_create(),sfSprite_create(),sfSprite_create()};
    sfSprite*            patoContSprite[10] = {sfSprite_create(),sfSprite_create(),sfSprite_create(),sfSprite_create(),sfSprite_create(),sfSprite_create(),sfSprite_create(),sfSprite_create(),sfSprite_create(),sfSprite_create()};

//////////////////////////////////////////////////////////////////////////////////////////////
/*                             Todos os vetores (tipo sfVector2f)                         */
    sfVector2f cachorroPos = {-112, 290};
    sfVector2f patoPos = {larguraTela/2,412};
    sfVector2f textboxPos = {(larguraTela/2)-50, 124};
    sfVector2f numTextboxPos = {(larguraTela/2)-8, 160};
    sfVector2f backgroundPos = {0,2};
    sfVector2f mousePos = {0,0};

    sfVector2f rodadaPos[2] = {{80, 384},
                    {96, 384}};

    sfVector2f scorePos[6] = { {384, 416},
                    {384+16, 416},
                    {384+16*2, 416},
                    {384+16*3, 416},
                    {384+16*4, 416},
                    {384+16*5, 416}};

    sfVector2f balaPos[3] = {  {50, 418},
                    {50+16, 418},
                    {50+16*2, 418}};

    sfVector2f patoContPos[10] = {{192,420}, {192+16,420}, {192+16*2,420}, {192+16*3,420}, {192+16*4,420}, {192+16*5,420}, {192+16*6,420}, {192+16*7,420}, {192+16*8,420}, {192+16*9,420}};


//////////////////////////////////////////////////////////////////////////////////////////////
/*                 Todas as atribuições de uma textura para um sprite                      */
    sfSprite_setTexture(backgroundSprite, backgroundTextura, true);
    sfSprite_setTexture(cachorroSprite, cachorroTextura[0], true);
    sfSprite_setTexture(textboxSprite, textboxTextura, true);
    sfSprite_setTexture(scoreSprite[0], numTextura[0], true);
    sfSprite_setTexture(scoreSprite[1], numTextura[0], true);
    sfSprite_setTexture(scoreSprite[2], numTextura[0], true);
    sfSprite_setTexture(scoreSprite[3], numTextura[0], true);
    sfSprite_setTexture(scoreSprite[4], numTextura[0], true);
    sfSprite_setTexture(scoreSprite[5], numTextura[0], true);
    sfSprite_setTexture(balaSprite[0], balaTextura, true);
    sfSprite_setTexture(balaSprite[1], balaTextura, true);
    sfSprite_setTexture(balaSprite[2], balaTextura, true);
    sfSprite_setTexture(patoContSprite[0], patoContBrancoTextura, true);    
    sfSprite_setTexture(patoContSprite[1], patoContBrancoTextura, true);
    sfSprite_setTexture(patoContSprite[2], patoContBrancoTextura, true);
    sfSprite_setTexture(patoContSprite[3], patoContBrancoTextura, true);
    sfSprite_setTexture(patoContSprite[4], patoContBrancoTextura, true);
    sfSprite_setTexture(patoContSprite[5], patoContBrancoTextura, true);
    sfSprite_setTexture(patoContSprite[6], patoContBrancoTextura, true);
    sfSprite_setTexture(patoContSprite[7], patoContBrancoTextura, true);
    sfSprite_setTexture(patoContSprite[8], patoContBrancoTextura, true);
    sfSprite_setTexture(patoContSprite[9], patoContBrancoTextura, true);    

//////////////////////////////////////////////////////////////////////////////////////////////
/*           Todas as atribuições de um vetor(coordenadas) para um sprite                   */
    sfSprite_setPosition(scoreSprite[0], scorePos[0]);
    sfSprite_setPosition(scoreSprite[1], scorePos[1]);    
    sfSprite_setPosition(scoreSprite[2], scorePos[2]);
    sfSprite_setPosition(scoreSprite[3], scorePos[3]);
    sfSprite_setPosition(scoreSprite[4], scorePos[4]);
    sfSprite_setPosition(scoreSprite[5], scorePos[5]);
    sfSprite_setPosition(rodadaSprite[0], rodadaPos[0]);
    sfSprite_setPosition(rodadaSprite[1], rodadaPos[1]);
    sfSprite_setPosition(numTextboxSprite, numTextboxPos);
    sfSprite_setPosition(textboxSprite, textboxPos);
    sfSprite_setPosition(backgroundSprite, backgroundPos);
    sfSprite_setPosition(mouseSprite, mousePos);
    sfSprite_setPosition(balaSprite[0], balaPos[0]);
    sfSprite_setPosition(balaSprite[1], balaPos[1]);
    sfSprite_setPosition(balaSprite[2], balaPos[2]);
    sfSprite_setPosition(patoContSprite[0], patoContPos[0]);
    sfSprite_setPosition(patoContSprite[1], patoContPos[1]);
    sfSprite_setPosition(patoContSprite[2], patoContPos[2]);
    sfSprite_setPosition(patoContSprite[3], patoContPos[3]);
    sfSprite_setPosition(patoContSprite[4], patoContPos[4]);
    sfSprite_setPosition(patoContSprite[5], patoContPos[5]);
    sfSprite_setPosition(patoContSprite[6], patoContPos[6]);
    sfSprite_setPosition(patoContSprite[7], patoContPos[7]);
    sfSprite_setPosition(patoContSprite[8], patoContPos[8]);
    sfSprite_setPosition(patoContSprite[9], patoContPos[9]);   


    // Tudo que ocorre dentro da janela está nesse loop
    while (sfRenderWindow_isOpen(janela))
    {
        deltaTempo = sfClock_restart(contador);
        
        // Escutador de eventos
        while (sfRenderWindow_pollEvent(janela, &event))
        {
            // Fecha a janela
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(janela);
            }   
            // verifica se o botão foi solto
            else if (event.type == sfEvtMouseButtonReleased) {
                if (event.mouseButton.button == sfMouseLeft) {              
                    mouseClicado = true;

                }
            }

            // mostra a posição do mouse
            if (event.type == sfEvtMouseMoved) {
                // pega as coordenadas do mouse
                mousePos.x = event.mouseMove.x;
                mousePos.y = event.mouseMove.y;
                }

        }

        // Animação inicial do cachorro
        if(etapa == cachorroAndando || etapa == cachorroCaindo)
        {
            if(!cachorroAndou)
            {
                if(contarSprite[0]<= 5 && contarSprite[0]> 0)
                    cachorroPos.x += 60 * Clock;


                tempoTotal[0]+= 300 * Clock;
                if (tempoTotal[0]>= 80)
                {
                    switch (contarSprite[0])
                    {
                        case 0:
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[1], true);
                            contarSprite[0]++; 
                            break;
                        case 1:
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[0], true);               
                            contarSprite[0]++;                
                            break;
                        case 2:
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[1], true);                
                            contarSprite[0]++;                
                            break;   
                        case 3:
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[2], true);                
                            contarSprite[0]++;                
                            break; 
                        case 4:
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[3], true);
                            contarSprite[0]++; 
                            break;
                        case 5:
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[4], true);
                            contarSprite[0]++;                
                            break;  
                        case 6:
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[5], true);
                            contarSprite[0]++;                
                            break;   
                        case 7:
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[4], true);
                            contarSprite[0]= 0;                
                            break;                                                                                                                 
                    }
  
                    tempoTotal[0]= 0.0;
                }
            }

            if(cachorroPos.x + 112 >= larguraTela/2 - 24)
                cachorroAndou = true;

            if(cachorroAndou)
            {
                if(!correcaoPos)
                {
                    contarSprite[0]= 7;
                    correcaoPos = true;
                }

                tempoTotal[0]+= 300 * Clock;
                if (tempoTotal[0]>= 200)
                {
                    switch (contarSprite[0])
                    {
                        case 7:
                            cachorroPos.y -= 10;
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[6], true);               
                            contarSprite[0]++;                
                            break;
                        case 8:
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[7], true); 
                            cachorroPulo = true;
                            contarSprite[0]++;
                        case 9:
                            sfSprite_setTexture(cachorroSprite, cachorroTextura[8], true);                             
                            break;                                  
                    }             
                    tempoTotal[0]= 0.0;
                }

                if(cachorroPulo)
                {
                    if(cachorroPos.x <= 180 && etapa == cachorroAndando)
                        cachorroPos.x += 155 * Clock;
                    
                    if(cachorroPos.y > 180 && etapa == cachorroAndando)
                        cachorroPos.y -= 280 * Clock;

                    if(cachorroPos.x > 180 && cachorroPos.y <= 180 && etapa == cachorroAndando)
                        etapa = cachorroCaindo;

                    if(etapa == cachorroCaindo)
                        cachorroPos.y += 255 * Clock;

                    if(cachorroPos.y >= 312)
                        etapa = cachorroFarejou;
                }  
            }
        }

        else if(etapa == cachorroFarejou)
        {
                // Movimentação do pato
                if(!colisaoInicialX && direcaoInicialX == 1)
                {
                    patoPos.x += movimento + posInicialX;
                    animarSprite (patoDireitaTextura[0], patoDireitaTextura[1], patoDireitaTextura[2], patoSprite, &deltaTempo);
                }
                else if (!colisaoInicialX && direcaoInicialX == 0)
                {
                    patoPos.x -= movimento - posInicialX;
                    animarSprite (patoEsquerdaTextura[0], patoEsquerdaTextura[1], patoEsquerdaTextura[2], patoSprite, &deltaTempo);
                }
                if (patoPos.x <= 0 )
                {
                    colisaoEsqX = true;
                    colisaoDirX = false;
                    colisaoInicialX = true;
                }
                if (patoPos.x + 54 >= (float) larguraTela)
                {
                    colisaoDirX = true;
                    colisaoEsqX = false;
                    colisaoInicialX = true;
                }
                // Verificação da colisão vertical
                if (!colisaoInicialY)
                {
                    patoPos.y -= movimento;
                }
                if (patoPos.y <= 0)
                {
                    colisaoTopoY = true;
                    colisaoBaixoY = false;
                    colisaoInicialY = true;
                }
                if (patoPos.y + 66 >= (float) alturaTela - 164)
                {
                    colisaoBaixoY = true;
                    colisaoTopoY = false;
                    colisaoInicialY = true;
                }

                // Movimentação após checar as colisões
                if (colisaoEsqX)
                {
                    patoPos.x += movimento;
                    animarSprite (patoDireitaTextura[0], patoDireitaTextura[1], patoDireitaTextura[2], patoSprite, &deltaTempo);
                }
                if (colisaoDirX)
                {
                    patoPos.x -= movimento;
                    animarSprite (patoEsquerdaTextura[0], patoEsquerdaTextura[1], patoEsquerdaTextura[2], patoSprite, &deltaTempo);
                }
                if (colisaoTopoY)
                {
                    patoPos.y += movimento;
                }
                if (colisaoBaixoY)
                {
                    patoPos.y -= movimento;
                }

                if(mouseClicado && balas != 0)
                    balas -= 1;


                // Se acertar o pato, mudar etapa para 'acertado', senão, mudar etapa para 'fugiu'
                if (mousePos.x >= patoPos.x && mousePos.x < patoPos.x+64 && mousePos.y >= patoPos.y && mousePos.y < patoPos.y+64)
                {
                    if(mouseClicado)
                        etapa = patoAcertado;
                }
                else if (balas == 0)
                    etapa = patoFugiu;

        }

        // Animação do pato cachorroCaindo, do cachorro pegando e prosseguimento da rodada
        else if (etapa == patoAcertado)
        {
            tempoTotal[2] += 300 * Clock;
            if (tempoTotal[2] < 100)            
                sfSprite_setTexture(patoSprite, hitTextura, true);

            else if (tempoTotal[2] >= 100 && patoPos.y < 324)
            {   
                sfSprite_setTexture(cachorroSprite, cachorroTextura[11], true);
                animarSprite (patoCaindoTextura[0], patoCaindoTextura[1], patoCaindoTextura[0], patoSprite, &deltaTempo);
                patoPos.y += 255 * Clock;
            }
            else if (tempoTotal[2] >= 200 && tempoTotal[2] < 1000 && patoPos.y >= 324)
            {
                patoPos.y = 1000;
                cachorroPos.x = patoPos.x - 33;
                if(tempoTotal[2] < 500)
                    cachorroPos.y = 323;
                if(cachorroPos.y >= 239)
                    cachorroPos.y -= 255 * Clock;
            }

            else if (tempoTotal[2] >= 1000 && patoPos.y == 1000 && cachorroPos.y < 323)
            {
                cachorroPos.y += 255 * Clock;            
            }

            else if (tempoTotal[2] >= 1000 && patoPos.y == 1000 && cachorroPos.y >= 323)
            {
                tempoTotal[2] = 0;
                velocidade = velocidade*1.03;

                if(balas == 0)
                    pontos += 500;
                else if (balas == 1)
                    pontos += 1000;
                else if (balas == 2)
                    pontos += 1500;
                
                balas = 3;
                patosMortos++;

                if(patosMortos == 10)
                {
                    sfSprite_setTexture(cachorroSprite, cachorroTextura[0], true);
                    correcaoPos = false;
                    cachorroPulo = false;
                    cachorroAndou = false;
                    cachorroPos.x = -112;
                    cachorroPos.y = 290;
                    patoPos.x = larguraTela/2;
                    patoPos.y = 412;
                    contarSprite[0] = 1;
                    tempoTotal[0] = 0.0;
                    tempoTotal[1] = 0.0;
                    tempoTotal[2] = 0.0;
                    rodadas++;
                    patosMortos = 0;
                    etapa = cachorroAndando;
                }
                else
                    etapa = cachorroFarejou;
            }
        }

        // Animação da fuga do pato e do cachorro zombando do jogador, seguida do reset de algumas informações
        else if (etapa == patoFugiu)
        {
            tempoTotal[3] += 300 * Clock;
            animarSprite(cachorroTextura[9], cachorroTextura[10], cachorroTextura[9], cachorroSprite, &deltaTempo);
            animarSprite (patoAltEsquerdaTextura[0], patoAltEsquerdaTextura[1], patoAltEsquerdaTextura[2], patoSprite, &deltaTempo);
            patoPos.x -= movimento;

            if(tempoTotal[3] < 200)
                cachorroPos.y = 323;

            if (tempoTotal[3] >= 200 && tempoTotal[3] < 1000 && cachorroPos.y > 239)
            {
                cachorroPos.x = larguraTela/2 - 36;

                if(cachorroPos.y >= 239 && patoPos.x <= 220)
                    cachorroPos.y -= 255 * Clock;
            }
            if (tempoTotal[3] >= 1000 && cachorroPos.y < 323 && patoPos.x <= 220)
                cachorroPos.y += 255 * Clock; 

            else if (tempoTotal[3] >= 1500 && cachorroPos.y >= 323 && patoPos.x <= 220)
            {
                patoPos.y = 1000;
                patoPos.x = larguraTela/2 - 36;
                tempoTotal[3] = 0;
                balas = 3;
                patosMortos = 0;
                velocidade = 1.00;
                pontos = 0;
                rodadas = 1;
                for(int i = 0; i < 6; i++)
                    placar[i] = 0;
                etapa = cachorroFarejou;
            }
        }

        sfSprite_setPosition(cachorroSprite, cachorroPos);   
        sfSprite_setPosition(mouseSprite, mousePos);
        sfSprite_setPosition(patoSprite, patoPos); 
        mouseClicado = false; 


///////////////////////////////////////////////////////////////////////
/*                          Display da janela                       */



        // Verificação pra saber quais sprites esconder e quais mostrar
        if(etapa == cachorroAndando || etapa == cachorroCaindo)
        {
            if(etapa == cachorroAndando)
            {
                sfRenderWindow_clear(janela, background);
                sfRenderWindow_drawSprite(janela, backgroundSprite, NULL);
                sfRenderWindow_drawSprite(janela, cachorroSprite, NULL);
            }

            if(etapa == cachorroCaindo)
            {
                sfRenderWindow_clear(janela, background);
                sfRenderWindow_drawSprite(janela, cachorroSprite, NULL); 
                sfRenderWindow_drawSprite(janela, backgroundSprite, NULL);
            }

        }
        else if (etapa == cachorroFarejou || etapa == patoAcertado || etapa == patoFugiu)
        {
            sfRenderWindow_clear(janela, background);
            sfRenderWindow_drawSprite(janela, patoSprite, NULL);
            if(etapa == patoAcertado || etapa == patoFugiu)
                sfRenderWindow_drawSprite(janela, cachorroSprite, NULL);
            sfRenderWindow_drawSprite(janela, backgroundSprite, NULL);
        }

        // Nessa parte ele verifica se houve mudança ou não de alguns elementos, e os desenha na tela se forem necessários
        switch(balas)
        {
            case 3:
                sfRenderWindow_drawSprite(janela, balaSprite[0], NULL);
                sfRenderWindow_drawSprite(janela, balaSprite[1], NULL);    
                sfRenderWindow_drawSprite(janela, balaSprite[2], NULL);    
                break;
            case 2:
                sfRenderWindow_drawSprite(janela, balaSprite[0], NULL);
                sfRenderWindow_drawSprite(janela, balaSprite[1], NULL);              
                break;
            case 1:
                sfRenderWindow_drawSprite(janela, balaSprite[0], NULL);
                break;
        }

        switch(patosMortos)
        {
            case 0:
                if(etapa == cachorroFarejou){
                    animarSprite (patoContPretoTextura, patoContBrancoTextura, patoContPretoTextura, patoContSprite[0], &deltaTempo);   
                    sfSprite_setTexture(patoContSprite[1], patoContBrancoTextura, true);
                    sfSprite_setTexture(patoContSprite[2], patoContBrancoTextura, true);
                    sfSprite_setTexture(patoContSprite[3], patoContBrancoTextura, true);
                    sfSprite_setTexture(patoContSprite[4], patoContBrancoTextura, true);
                    sfSprite_setTexture(patoContSprite[5], patoContBrancoTextura, true);
                    sfSprite_setTexture(patoContSprite[6], patoContBrancoTextura, true);
                    sfSprite_setTexture(patoContSprite[7], patoContBrancoTextura, true);
                    sfSprite_setTexture(patoContSprite[8], patoContBrancoTextura, true);
                    sfSprite_setTexture(patoContSprite[9], patoContBrancoTextura, true);                        
                }
                break;
            case 1:
                sfSprite_setTexture(patoContSprite[0], patoContVermelhoTextura, true);    
                animarSprite (patoContPretoTextura, patoContBrancoTextura, patoContPretoTextura, patoContSprite[1], &deltaTempo);
                break;

            case 2:
                sfSprite_setTexture(patoContSprite[1], patoContVermelhoTextura, true);
                animarSprite (patoContPretoTextura, patoContBrancoTextura, patoContPretoTextura, patoContSprite[2], &deltaTempo);
                break;

            case 3:
                sfSprite_setTexture(patoContSprite[2], patoContVermelhoTextura, true);         
                animarSprite (patoContPretoTextura, patoContBrancoTextura, patoContPretoTextura, patoContSprite[3], &deltaTempo);                   
                break;

            case 4:
                sfSprite_setTexture(patoContSprite[3], patoContVermelhoTextura, true);   
                animarSprite (patoContPretoTextura, patoContBrancoTextura, patoContPretoTextura, patoContSprite[4], &deltaTempo);                         
                break;

            case 5:
                sfSprite_setTexture(patoContSprite[4], patoContVermelhoTextura, true);         
                animarSprite (patoContPretoTextura, patoContBrancoTextura, patoContPretoTextura, patoContSprite[5], &deltaTempo);                   
                break;

            case 6:
                sfSprite_setTexture(patoContSprite[5], patoContVermelhoTextura, true);           
                animarSprite (patoContPretoTextura, patoContBrancoTextura, patoContPretoTextura, patoContSprite[6], &deltaTempo);                 
                break;

            case 7:
                sfSprite_setTexture(patoContSprite[6], patoContVermelhoTextura, true);   
                animarSprite (patoContPretoTextura, patoContBrancoTextura, patoContPretoTextura, patoContSprite[7], &deltaTempo);                         
                break;

            case 8:
                sfSprite_setTexture(patoContSprite[7], patoContVermelhoTextura, true);    
                animarSprite (patoContPretoTextura, patoContBrancoTextura, patoContPretoTextura, patoContSprite[8], &deltaTempo);                        
                break;

            case 9:
                animarSprite (patoContPretoTextura, patoContBrancoTextura, patoContPretoTextura, patoContSprite[9], &deltaTempo);            
                sfSprite_setTexture(patoContSprite[8], patoContVermelhoTextura, true);
                break;

            case 10:
                animarSprite (patoContVermelhoTextura, patoContBrancoTextura, patoContVermelhoTextura, patoContSprite[0], &deltaTempo);               
                animarSprite (patoContVermelhoTextura, patoContBrancoTextura, patoContVermelhoTextura, patoContSprite[1], &deltaTempo);   
                animarSprite (patoContVermelhoTextura, patoContBrancoTextura, patoContVermelhoTextura, patoContSprite[2], &deltaTempo);   
                animarSprite (patoContVermelhoTextura, patoContBrancoTextura, patoContVermelhoTextura, patoContSprite[3], &deltaTempo);   
                animarSprite (patoContVermelhoTextura, patoContBrancoTextura, patoContVermelhoTextura, patoContSprite[4], &deltaTempo);   
                animarSprite (patoContVermelhoTextura, patoContBrancoTextura, patoContVermelhoTextura, patoContSprite[5], &deltaTempo);   
                animarSprite (patoContVermelhoTextura, patoContBrancoTextura, patoContVermelhoTextura, patoContSprite[6], &deltaTempo);   
                animarSprite (patoContVermelhoTextura, patoContBrancoTextura, patoContVermelhoTextura, patoContSprite[7], &deltaTempo);   
                animarSprite (patoContVermelhoTextura, patoContBrancoTextura, patoContVermelhoTextura, patoContSprite[8], &deltaTempo);               
                animarSprite (patoContVermelhoTextura, patoContBrancoTextura, patoContVermelhoTextura, patoContSprite[9], &deltaTempo);   
                break;
        }

        // Atualizar as rodadas
        inteiroPraVetor(rodadas, rodadasVetor, 2);
        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j <= 9; j++)
            {
                if(rodadasVetor[i] == j)
                    sfSprite_setTexture(numTextboxSprite, numTextura[j], true);
            }
            
            if (rodadasVetor[i] == 0)
                sfSprite_setTexture(rodadaSprite[i], numV0Textura, true);
            else if (rodadasVetor[i] == 1)
                sfSprite_setTexture(rodadaSprite[i], numV1Textura, true);
            else if (rodadasVetor[i] == 2)
                sfSprite_setTexture(rodadaSprite[i], numV2Textura, true);
            else if (rodadasVetor[i] == 3)
                sfSprite_setTexture(rodadaSprite[i], numV3Textura, true);
            else if (rodadasVetor[i] == 4)
                sfSprite_setTexture(rodadaSprite[i], numV4Textura, true);
            else if (rodadasVetor[i] == 5)
                sfSprite_setTexture(rodadaSprite[i], numV5Textura, true);
            else if (rodadasVetor[i] == 6)
                sfSprite_setTexture(rodadaSprite[i], numV6Textura, true);
            else if (rodadasVetor[i] == 7)
                sfSprite_setTexture(rodadaSprite[i], numV7Textura, true);
            else if (rodadasVetor[i] == 8)
                sfSprite_setTexture(rodadaSprite[i], numV8Textura, true);
            else if (rodadasVetor[i] == 9)
                sfSprite_setTexture(rodadaSprite[i], numV9Textura, true);                                                                                                                                                
        }

        // Atualizar a pontuação
        inteiroPraVetor(pontos, placar, 6);
        for(int i = 0; i < 6; i++)
        {
            for(int j = 0; j <= 9; j++)
            {
                if(placar[i] == j)
                {
                    sfSprite_setTexture(scoreSprite[i], numTextura[j], true);
                }
            }
        }

        // Caixa de texto do início
        if(!(cachorroPos.x >= larguraTela/8) && etapa == cachorroAndando)
        {
            sfRenderWindow_drawSprite(janela, textboxSprite, NULL);
            sfRenderWindow_drawSprite(janela, numTextboxSprite, NULL);
        }

        sfRenderWindow_drawSprite(janela, patoContSprite[0], NULL);
        sfRenderWindow_drawSprite(janela, patoContSprite[1], NULL);
        sfRenderWindow_drawSprite(janela, patoContSprite[2], NULL);
        sfRenderWindow_drawSprite(janela, patoContSprite[3], NULL);
        sfRenderWindow_drawSprite(janela, patoContSprite[4], NULL);
        sfRenderWindow_drawSprite(janela, patoContSprite[5], NULL);
        sfRenderWindow_drawSprite(janela, patoContSprite[6], NULL);
        sfRenderWindow_drawSprite(janela, patoContSprite[7], NULL);
        sfRenderWindow_drawSprite(janela, patoContSprite[8], NULL);
        sfRenderWindow_drawSprite(janela, patoContSprite[9], NULL);
        sfRenderWindow_drawSprite(janela, rodadaSprite[0], NULL);
        sfRenderWindow_drawSprite(janela, rodadaSprite[1], NULL);
        sfRenderWindow_drawSprite(janela, scoreSprite[0], NULL);
        sfRenderWindow_drawSprite(janela, scoreSprite[1], NULL);
        sfRenderWindow_drawSprite(janela, scoreSprite[2], NULL);
        sfRenderWindow_drawSprite(janela, scoreSprite[3], NULL);
        sfRenderWindow_drawSprite(janela, scoreSprite[4], NULL);
        sfRenderWindow_drawSprite(janela, scoreSprite[5], NULL);        


        sfRenderWindow_display(janela);
    }

    // Destroi, limpa da memória
    sfSprite_destroy(backgroundSprite);
    sfSprite_destroy(textboxSprite);
    sfSprite_destroy(numTextboxSprite);
    sfSprite_destroy(cachorroSprite);
    sfSprite_destroy(patoSprite);
    sfSprite_destroy(mouseSprite);
    sfSprite_destroy(rodadaSprite[0]);
    sfSprite_destroy(rodadaSprite[1]);
    sfSprite_destroy(balaSprite[0]);
    sfSprite_destroy(balaSprite[1]);
    sfSprite_destroy(balaSprite[2]);
    for(int i = 0; i < 6; i++)
        sfSprite_destroy(scoreSprite[i]);
    for(int i = 0; i < 10; i++)
        sfSprite_destroy(patoContSprite[i]);
    sfRenderWindow_destroy(janela);
    return 0;
}


void animarSprite(sfTexture* sprite1, sfTexture* sprite2, sfTexture* sprite3, sfSprite* patoSprite, sfTime* deltaTempo)
{
    tempoTotal[1] += 300 * sfTime_asSeconds(*deltaTempo);

    if (tempoTotal[1] >= 30)
    {

        switch (contarSprite[1])
        {
            case 1:
                sfSprite_setTexture(patoSprite, sprite1, true);
                contarSprite[1]++;
                break;
            case 2:
                sfSprite_setTexture(patoSprite, sprite2, true);
                contarSprite[1]++;
                break;   
            case 3:
                sfSprite_setTexture(patoSprite, sprite3, true);
                contarSprite[1] = 1;
                break;                                    
        }
        tempoTotal[1] = 0.0;
    }
}

void inteiroPraVetor(int inteiro, int vetor[], int tamanho) {

    if (inteiro == 0) {
        vetor[0] = 0;
        return;
    }


    // Inicializar um índice para o vetor
    int index = tamanho - 1;

    // Extrair cada dígito do número e armazenar no vetor
    while (inteiro > 0 && index >= 0) {
        int digito = inteiro % 10;
        vetor[index] = digito;
        inteiro /= 10;
        index--;
    }
}
