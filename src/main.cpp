#include "PIG.h"


typedef struct vetor
{
    double x, y;
} Vetor;

typedef struct corpo
{
    double massa;
    double raio;
    vetor posicao;
    vetor velocidade;
    vetor aceleracao;
} Corpo;

Corpo planeta;
Corpo nave;

int spritePlaneta, spriteNave, spriteCeu;
int timerGeral;
int rotacao;




void configuracoesIniciais()
{
    PIG_criarJanela("simulação nave", 1920,1080 );


    spritePlaneta   = PIG_criarSprite("imagens\\terraNoite.png", 0);
    spriteNave  = PIG_criarSprite("imagens\\Nave.png", 0);
    spriteCeu       = PIG_criarSprite("imagens\\ceu.png", 0);


    //planeta
    planeta.massa = 1000000.0;
    planeta.raio = 350.0;
    planeta.aceleracao.x = 0;
    planeta.aceleracao.y = 0;
    planeta.velocidade.x = 0;
    planeta.velocidade.y = 0;
    planeta.posicao.x = LARG_TELA/2.0;
    planeta.posicao.y = ALT_TELA/2.0;


    //nave
    nave.posicao.x = planeta.posicao.x;
    nave.posicao.y = planeta.posicao.y + planeta.raio + 10;

    nave.velocidade.x = 0;
    nave.velocidade.y = 0;
    nave.aceleracao.x = 0;
    nave.aceleracao.y = 0;
    nave.massa = 1.0;
    nave.raio = 1.0;


    timerGeral = PIG_criarTimer();
}

void desenharnave()
{
    PIG_desenharSprite(spriteNave,
        XVirtualParaReal(nave.posicao.x),
        YVirtualParaReal(nave.posicao.y),
        tamanhoVirtualParaReal(50.0*nave.raio),
        tamanhoVirtualParaReal(50.0*nave.raio),
        rotacao, 1, BRANCO);
}

void desenhar()
{
    char velocidadey[100000];
    char velocidadex[1000];
    char posicaoy[1000];
    char posicaox[1000];

    PIG_iniciarDesenho();

    PIG_desenharSprite(spriteCeu, 0, 0, LARG_TELA, ALT_TELA, 0, 0, BRANCO);

    PIG_desenharSprite(spritePlaneta,
        XVirtualParaReal(planeta.posicao.x),
        YVirtualParaReal(planeta.posicao.y),
        tamanhoVirtualParaReal(2.0*planeta.raio),
        tamanhoVirtualParaReal(2.0*planeta.raio), 0, 1, BRANCO);

    desenharnave();



    sprintf(velocidadey,"velocidadey %.0f", nave.velocidade.y);
    sprintf(velocidadex,"velocidadex %.0f", nave.velocidade.x);
    sprintf(posicaoy,"posicaoy %.0f", nave.posicao.y - 885);
    sprintf(posicaox,"posicaox %.0f", nave.posicao.x - 960);

    PIG_escreverEsquerda(velocidadey, 1, 100, 0);
    PIG_escreverEsquerda(velocidadex, 1, 200, 0);
    PIG_escreverEsquerda(posicaoy, 1, 300, 0);
    PIG_escreverEsquerda(posicaox, 1, 400, 0);






    PIG_encerrarDesenho();
}


// Atualiza a física dos projéteis (gravidade, aceleração, movimento e colisão)

void atualizarnave()
{
    Vetor forca;

    double CONSTANTE_GRAVITACIONAL = 0.005;
    double CONSTANTE_RESTITUICAO = 0.9;


    //definindo alnguns parametros

    double xNave = nave.posicao.x;
    double yNave = nave.posicao.y;
    double massaNave = nave.massa;
    double raioNave = nave.raio;

    double xPlaneta = planeta.posicao.x;
    double yPlaneta = planeta.posicao.y;
    double massaPlaneta = planeta.massa;
    double raioPlaneta = planeta.raio;


    double distancia = distanciaEntrePontos(xNave, yNave, xPlaneta, yPlaneta);
    //if(distancia < 1) distancia = 1; // Evita divisão por zero

        // Cálculo da força gravitacional

    double produtoMassas = massaNave * massaPlaneta;
    double produtoDistancia = (distancia * distancia);
    double forcaGravitacional = CONSTANTE_GRAVITACIONAL * produtoMassas / produtoDistancia;

        // Cálculo dos vetores de força
    double cos = (xPlaneta - xNave) / distancia;
    double sen = (yPlaneta - yNave) / distancia;

    forca.x = forcaGravitacional * cos;
    forca.y = forcaGravitacional * sen;

    nave.aceleracao.x = forca.x / massaNave;
    nave.aceleracao.y = forca.y / massaNave;

    nave.velocidade.x += nave.aceleracao.x;
    nave.velocidade.y += nave.aceleracao.y;

    nave.posicao.x += nave.velocidade.x;
    nave.posicao.y += nave.velocidade.y;

        if(distanciaEntrePontos(nave.posicao.x, nave.posicao.y,
                                xPlaneta, yPlaneta) < raioNave + raioPlaneta*0.982)
        {
            // Reposiciona o projétil para evitar "enterrar" no planeta
            nave.posicao.x = xNave;
            nave.posicao.y = yNave;

            // Aplica amortecimento na colisão
            nave.velocidade.x *= CONSTANTE_RESTITUICAO;
            nave.velocidade.y *= CONSTANTE_RESTITUICAO;

        }

        double motor = 0.2;


        if(PIG_tecla == TECLA_BAIXO) {
            nave.velocidade.y -= motor;
        }
        if(PIG_tecla == TECLA_CIMA) {
            nave.velocidade.y += motor;
        }
        if (PIG_tecla == TECLA_DIREITA) {
            nave.velocidade.x += motor;
        }
        if (PIG_tecla == TECLA_ESQUERDA) {
            nave.velocidade.x -= motor;

        }
}





// Função principal
int main(int argc, char* args[])
{
    int prossiga;
    printf("=====================================\n");
    printf("      SIMULADOR DE ORBITA ESPACIAL   \n");
    printf("=====================================\n");
    printf("OBJETIVO\n");
    printf("Este jogo tem como objetivo colocar a nave espacial em orbita\n");
    printf("Ao apertar as teclas de aceleracao voce ira deslocar a nave\n");
    printf("ate o espaco mas a gravidade ira puxar ela de volta\n");
    printf("Seu desafio e equilibrar a velocidade e a direcao para que\n");
    printf("a nave entre em orbita estavel ao redor do planeta\n");
    printf("-------------------------------------\n");
    printf("CONTROLES DA NAVE\n");
    printf("  seta para cima - acelerar para cima\n");
    printf("  seta para baixo - acelerar para baixo\n");
    printf("  seta para esquerda - acelerar para esquerda\n");
    printf("  seta para direita - acelerar para direita\n");
    printf("\n");
    printf("CONTROLES DA CAMERA\n");
    printf("  W A S D  movimenta a camera\n");
    printf("  Q        aumentar zoom\n");
    printf("  E        diminuir zoom\n");
    printf("-------------------------------------\n");
    printf("DICA Se a nave cair de volta tente ajustar a velocidade\n");
    printf("lateral para equilibrar a forca da gravidade\n");
    printf("=====================================\n");
        printf("aperte 1 para comecar\n");

    scanf("%prossiga", &prossiga);
    configuracoesIniciais();

    while(PIG_jogoRodando() == 1)
    {
        if(PIG_tempoDecorrido(timerGeral) >= 0.01) // Atualiza a cada 0.01s
        {
            PIG_reiniciarTimer(timerGeral);

            PIG_atualizarJanela();
            atualizarnave();

            atualizarCamera();

            desenhar();
        }
    }

    PIG_finalizarJanela();
    return 0;
}
