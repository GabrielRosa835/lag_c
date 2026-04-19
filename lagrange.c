/*  ============================================================================
        MÉTODOS NUMÉRICOS - INTERPOLAÇÃO POLINOMIAL VIA MÉTODO DE LAGRANGE
    ============================================================================
    INTEGRANTES:
    - Gabriel Rosa da Silva, RA 237069
    - Rodrigo Avila Ferreira dos Santos Filho, RA 251327
    - Lucas Moraes Mosca, RA 250889
    - Victor Henrique Soares Baleeiro, RA 252366

    DESCRIÇÃO DO PROGRAMA:
    Este programa implementa a Interpolação Polinomial utilizando a Forma de 
    Lagrange para descobrir valores não tabelados de uma função desconhecida. 
    O sistema recebe o grau do polinômio desejado, aloca dinamicamente a memória 
    necessária e coleta os pontos (x, f(x)) inseridos pelo usuário. Em seguida, 
    calcula os coeficientes L_k(x) e o valor final de P(x) para qualquer ponto 
    x solicitado, mantendo um loop de execução até o encerramento manual.

    EXEMPLOS DE POLINÔMIOS PARA TESTE:
    - Exemplo 1 (Grau 1 - Reta simples): 
        Pontos: (1, 2), (3, 4) 
        Testar x = 2 
        Resultado Esperado P(2) = 3
    - Exemplo 2 (Grau 2 - Parábola f(x) = x^2): 
        Pontos: (0, 0), (1, 1), (2, 4) 
        Testar x = 1.5 
        Resultado Esperado P(1.5) = 2.25
    - Exemplo 3 (Grau 3 - Curva de Trajetória/Telemetria): 
        Pontos: (-1, -1), (0, 0), (1, 1), (2, 8) 
        Testar x = 0.5 
        Resultado Esperado P(0.5) = 0.125
    - Exemplo 4 (Grau 2 - Aproximação de f(x) = 1/x): 
        Pontos: (2, 0.5), (2.5, 0.4), (4, 0.25) 
        Testar x = 3 
        Resultado Esperado P(3) = 0.325
    - Exemplo 5 (Grau 3 - Dados empíricos aleatórios): 
        Pontos: (10, 212), (15, 240), (20, 265), (25, 298) 
        Testar x = 18 
        Resultado Esperado P(18) = 254.8
============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    float x;
    float f_x;
} ponto;

typedef struct {
    ponto* valores;
    int tamanho;
} tabela_pontos;

tabela_pontos aquisitar_pontos();
float calcular_L(tabela_pontos* tabela, float valor, int i);
float calcular_P(tabela_pontos* tabela, float valor);

int main()
{
    // Coleta os pontos do usuário e aloca a matriz (vetor de structs) em memória
    tabela_pontos tabela = aquisitar_pontos();
    float valor;
    char resp;

    // Loop de cálculo: permite interpolar vários valores de X com a mesma tabela
    do
    {
        printf("\nDigite o valor de x que deseja interpolar: ");
        scanf("%f", &valor);

        // Chama a função principal de cálculo do polinômio interpolador
        float resultado = calcular_P(&tabela, valor);
        printf("P(%.3f) = %.3f\n", valor, resultado);

        // Loop para garantir formato de entrada válido (s ou n)
        do 
        {
            fflush(stdin);
            printf("\nDeseja calcular outro valor (s/n)? ");
            scanf(" %c", &resp);
        }
        while(resp != 's' && resp != 'n');
    }
    while (resp == 's');
    
    printf("\n");

    // Libera a memória alocada dinamicamente antes de encerrar o programa
    free(tabela.valores);
}

// Função responsável pelo somatório do polinômio de Lagrange: P(x) = Soma( L_i(x) * f(x_i) )
float calcular_P(tabela_pontos* tabela, float valor)
{
    float resultado = 0;

    for(int i = 0; i < tabela->tamanho; i++) 
    {
        // Calcula o coeficiente de Lagrange L_i para o ponto atual
        float L_i = calcular_L(tabela, valor, i);
        float f_x_i = (tabela->valores+i)->f_x;
        
        // Exibe o valor de L_i
        printf("L%i = %.2f\n", i, L_i);
        
        // Multiplica o L_i calculado pelo f(x) correspondente e soma ao resultado total
        resultado += L_i * f_x_i;
    }

    return resultado;
}

// Função responsável pelo produtório do coeficiente de Lagrange L_i(x)
float calcular_L(tabela_pontos* tabela, float valor, int i)
{
    float resultado = 1; // Inicializa com 1 pois trata-se de um produtório
    float valor_i = (tabela->valores+i)->x;

    for(int j = 0; j < tabela->tamanho; j++) 
    {
        // A regra de Lagrange define que ignoramos a iteração onde i == j para evitar divisão por zero
        if (i == j) 
        {
            continue; 
        }
        
        float valor_j = (tabela->valores+j)->x;
        
        // Aplica a fórmula: L_i *= (x - x_j) / (x_i - x_j)
        resultado *= (valor - valor_j) / (valor_i - valor_j);
    }

    return resultado;
}

// Função para receber os dados, instruir o usuário e criar a estrutura dinâmica
tabela_pontos aquisitar_pontos()
{
    int grau;

    // Loop para garantir um valor de 'grau' válido
    do 
    {
        printf("\nDigite o grau do polinomio que deseja obter (grau > 0): ");
        scanf("%i", &grau);
    }
    while (grau <= 0);

    // O número de pontos necessários é sempre o grau do polinômio + 1
    int pontos_necessarios = grau + 1;

    // Alocação dinâmica de memória para os pontos utilizando ponteiros
    tabela_pontos tabela = 
    {
        .valores = (ponto*) malloc(pontos_necessarios * sizeof(ponto)),
        .tamanho = pontos_necessarios,
    };

    printf("\n");
    printf("Insira os pontos tabelados, digitando o valor de xi e f(xi) alternadamente:\n");

    // Aquisita os pontos e os insere no struct da tabela
    for(int i = 0; i < tabela.tamanho; i++)
    {
        if (i != 0) 
        {
            printf("---+-------------\n");
        }

        // Utiliza aritmética de ponteiros para acessar a posição de memória correta
        ponto* p = tabela.valores+i;

        // Capta os dados da tabela de pontos enquanto os exibe de maneira clara
        printf(" %d |  x   = ", i);
        scanf("%f", &(p->x));
        printf("   | f(x) = ");
        scanf("%f", &(p->f_x));
    }

    return tabela;
}