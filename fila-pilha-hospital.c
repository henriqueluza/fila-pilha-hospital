#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define MAX_PACIENTES 100
#define MAX_NOME 50

// Definição da estrutura de paciente
typedef struct {
    char nome[MAX_NOME];
    int idade;
    int prioridade; // 1-alta, 2-média, 3-baixa (normal)
} Paciente;

// Fila de atendimento
typedef struct {
    Paciente pacientes[MAX_PACIENTES];
    int inicio;
    int fim;
    int tamanho;
} FilaAtendimento;

// Inicializar a fila vazia
void inicializarFila(FilaAtendimento *fila) {
    fila->inicio = 0;
    fila->fim = -1;
    fila->tamanho = 0;
}

// Verificar se a fila está vazia
int filaVazia(FilaAtendimento *fila) {
    return (fila->tamanho == 0);
}

// Verificar se a fila está cheia
int filaCheia(FilaAtendimento *fila) {
    return (fila->tamanho == MAX_PACIENTES);
}

// Adicionar paciente com prioridade (no início da fila)
void adicionarPacientePrioritario(FilaAtendimento *fila, Paciente novoPaciente) {
    int i;
    
    if (filaCheia(fila)) {
        printf("\nERRO: Fila de atendimento cheia!\n");
        return;
    }
    
    // Desloca todos os pacientes uma posição para a direita
    for (i = fila->tamanho; i > 0; i--) {
        fila->pacientes[(fila->inicio + i) % MAX_PACIENTES] = 
            fila->pacientes[(fila->inicio + i - 1) % MAX_PACIENTES];
    }
    
    // Insere o novo paciente no início
    fila->pacientes[fila->inicio] = novoPaciente;
    
    // Atualiza o fim da fila
    fila->fim = (fila->fim + 1) % MAX_PACIENTES;
    fila->tamanho++;
    
    printf("\nPaciente %s adicionado com prioridade!\n", novoPaciente.nome);
}

// Adicionar paciente normal (no final da fila)
void adicionarPacienteNormal(FilaAtendimento *fila, Paciente novoPaciente) {
    if (filaCheia(fila)) {
        printf("\nERRO: Fila de atendimento cheia!\n");
        return;
    }
    
    // Atualiza o fim da fila
    fila->fim = (fila->fim + 1) % MAX_PACIENTES;
    fila->pacientes[fila->fim] = novoPaciente;
    fila->tamanho++;
    
    printf("\nPaciente %s adicionado ao final da fila!\n", novoPaciente.nome);
}

// Remover paciente (próximo a ser atendido)
Paciente atenderProximoPaciente(FilaAtendimento *fila) {
    Paciente pacienteAtendido;
    
    if (filaVazia(fila)) {
        printf("\nERRO: Fila de atendimento vazia!\n");
        strcpy(pacienteAtendido.nome, ""); // Paciente inválido
        return pacienteAtendido;
    }
    
    // Remove o paciente do início da fila
    pacienteAtendido = fila->pacientes[fila->inicio];
    fila->inicio = (fila->inicio + 1) % MAX_PACIENTES;
    fila->tamanho--;
    
    return pacienteAtendido;
}

// Mostrar todos os pacientes na fila
void mostrarFila(FilaAtendimento *fila) {
    int i, posicao;
    
    if (filaVazia(fila)) {
        printf("\nFila de atendimento vazia!\n");
        return;
    }
    
    printf("\n===== FILA DE ATENDIMENTO =====\n");
    printf("Total de pacientes: %d\n\n", fila->tamanho);
    
    for (i = 0; i < fila->tamanho; i++) {
        posicao = (fila->inicio + i) % MAX_PACIENTES;
        
        printf("Posição %d:\n", i+1);
        printf("  Nome: %s\n", fila->pacientes[posicao].nome);
        printf("  Idade: %d\n", fila->pacientes[posicao].idade);
        
        printf("  Prioridade: ");
        switch (fila->pacientes[posicao].prioridade) {
            case 1: printf("ALTA\n"); break;
            case 2: printf("MÉDIA\n"); break;
            case 3: printf("NORMAL\n"); break;
        }
        printf("\n");
    }
    printf("==============================\n");
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função principal
int main() {
    // Configuração de localização para português
    setlocale(LC_ALL, "Portuguese");
    
    FilaAtendimento fila;
    inicializarFila(&fila);
    
    Paciente novoPaciente, pacienteAtendido;
    int opcao, prioridade;
    
    do {
        // Menu principal
        printf("\n===== SISTEMA DE ATENDIMENTO HOSPITALAR =====\n");
        printf("1. Adicionar novo paciente\n");
        printf("2. Atender proximo paciente\n");
        printf("3. Visualizar fila de atendimento\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        switch (opcao) {
            case 1: // Adicionar paciente
                printf("\n----- Cadastro de Paciente -----\n");
                printf("Nome: ");
                fgets(novoPaciente.nome, MAX_NOME, stdin);
                novoPaciente.nome[strcspn(novoPaciente.nome, "\n")] = '\0'; // Remove o \n
                
                printf("Idade: ");
                scanf("%d", &novoPaciente.idade);
                limparBuffer();
                
                printf("Prioridade (1-Alta, 2-Média, 3-Normal): ");
                scanf("%d", &prioridade);
                limparBuffer();
                
                if (prioridade < 1 || prioridade > 3) {
                    printf("Prioridade invalida! Definindo como normal (3).\n");
                    prioridade = 3;
                }
                
                novoPaciente.prioridade = prioridade;
                
                // Adiciona o paciente de acordo com a prioridade
                if (prioridade == 1 || prioridade == 2) {
                    adicionarPacientePrioritario(&fila, novoPaciente);
                } else {
                    adicionarPacienteNormal(&fila, novoPaciente);
                }
                break;
                
            case 2: // Atender próximo paciente
                pacienteAtendido = atenderProximoPaciente(&fila);
                
                if (strcmp(pacienteAtendido.nome, "") != 0) {
                    printf("\nPaciente atendido:\n");
                    printf("Nome: %s\n", pacienteAtendido.nome);
                    printf("Idade: %d\n", pacienteAtendido.idade);
                    
                    printf("Prioridade: ");
                    switch (pacienteAtendido.prioridade) {
                        case 1: printf("ALTA\n"); break;
                        case 2: printf("MÉDIA\n"); break;
                        case 3: printf("NORMAL\n"); break;
                    }
                }
                break;
                
            case 3: // Visualizar fila
                mostrarFila(&fila);
                break;
                
            case 4: // Sair
                printf("\nEncerrando o sistema...\n");
                break;
                
            default:
                printf("\nOpcao invalida!\n");
        }
        
    } while (opcao != 4);
    
    return 0;
}
