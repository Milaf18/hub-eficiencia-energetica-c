#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ==========================================================================
   ETAPA 1: Bibliotecas, Variaveis, vetores, funções e main
========================================================================== */

#define TOTAL_COMPUTADORES 15
#define LIMITE_INATIVIDADE 20
#define CONSUMO_LIGADO 150
#define CONSUMO_HIBERNANDO 5
#define CONSUMO_DESLIGADO 0
#define CICLO_SIMULACAO 5

typedef struct {
    int id;
    int estado; // 0: Desligada, 1: Ligada, 2: Hibernando
    int tempo_inativo;
    float consumo_watts;
    int prioritaria;
    char historico[300];
} Computador;

// Protótipos das funções
void inicializar_lab (Computador lab[], int n);
void monitorar (Computador lab[], int n);
void registrar_evento (Computador *pc, const char *evento);
void exibir_status (Computador lab[], int n);
float calcular_consumo (Computador lab[], int n);
float calcular_economia (Computador lab[], int n);
void gerar_relatorio (Computador lab[], int n);
void ligar_maquina (Computador lab[], int id);
void definir_prioritaria(Computador lab[], int id);
void simular_atividade (Computador lab[], int n, int id);
void menu (Computador lab[], int n, int ciclo_atual);
int validar_id (int id, int min, int max);
void limpar_tela ();
void aguardar_enter ();
void limpar_buffer ();

int main() {
    Computador lab[TOTAL_COMPUTADORES];
    srand((unsigned int)time(NULL));
    
    inicializar_lab(lab, TOTAL_COMPUTADORES);
    definir_prioritaria(lab, 1);

    int rodando = 1;
    int ciclo = 0;

    limpar_tela();
    printf("==============================================\n");
    printf(" HUB DE EFICIENCIA ENERGETICA - LABORATORIO \n");
    printf("==============================================\n");
    printf(" 15 computadores monitorados | Ciclo: %d min\n", CICLO_SIMULACAO);
    printf(" Limite de inatividade: %d min\n", LIMITE_INATIVIDADE);
    printf("==============================================\n\n");
    printf(" Estado inicial do laboratorio:\n\n");
    
    exibir_status(lab, TOTAL_COMPUTADORES);
    printf("\nPressione ENTER para iniciar a simulacao...");
    limpar_buffer();

    while (rodando) {
        ciclo++;
        monitorar(lab, TOTAL_COMPUTADORES);
        menu(lab, TOTAL_COMPUTADORES, ciclo);

        if (ciclo >= 20) {
            limpar_tela();
            printf("\n==============================================\n");
            printf(" LIMITE DE 20 CICLOS ALCANCADO\n");
            printf("==============================================\n");
            printf("\nPressione ENTER para encerrar a simulacao...");
            limpar_buffer();
            rodando = 0;
        }
    }

    gerar_relatorio(lab, TOTAL_COMPUTADORES);
    return 0;
}

/* ==========================================================================
   ETAPA 2: Criar Funções "iniciar_lab", "monitorar_maquinas" e "registro_de_eventos"
========================================================================== */

void inicializar_lab(Computador lab[], int n) {
    for (int i = 0; i < n; i++) {
        lab[i].id = i + 1;
        lab[i].estado = 1;
        lab[i].tempo_inativo = rand() % 10;
        lab[i].consumo_watts = CONSUMO_LIGADO;
        lab[i].prioritaria = 0;
        lab[i].historico[0] = '\0';
        registrar_evento(&lab[i], "Inicializado");
    }
}

void monitorar(Computador lab[], int n) {
    for (int i = 0; i < n; i++) {
        if (lab[i].estado == 1) {
            lab[i].tempo_inativo += CICLO_SIMULACAO;
            if (lab[i].tempo_inativo > LIMITE_INATIVIDADE && lab[i].prioritaria == 0) {
                lab[i].estado = 2;
                lab[i].consumo_watts = CONSUMO_HIBERNANDO;
                registrar_evento(&lab[i], "Hibernacao automatica");
            }
        }
    }
}

void registrar_evento(Computador *pc, const char *evento) {
    if (strlen(pc->historico) + strlen(evento) + 4 < 300) {
        if (pc->historico[0] != '\0') strcat(pc->historico, " | ");
        strcat(pc->historico, evento);
    }
}

/* ==========================================================================
   ETAPA 3: Criar as funções: "exibir_status", "calcular_consumo", "calcular_economia" e "gerar_relatorio"
========================================================================== */

void exibir_status(Computador lab[], int n) {
    printf("--- PAINEL DO LABORATORIO ---\n");
    printf("L=Ligada H=Hibernando D=Desligada *=Prioritaria\n\n");
    for (int i = 0; i < n; i++) {
        char simbolo;
        if (lab[i].estado == 1) simbolo = 'L';
        else if (lab[i].estado == 2) simbolo = 'H';
        else simbolo = 'D';

        if (lab[i].prioritaria) printf("[%02d:%c*] ", lab[i].id, simbolo);
        else printf("[%02d:%c ] ", lab[i].id, simbolo);

        if ((i + 1) % 5 == 0) printf("\n");
    }
    printf("\n");
    printf("Consumo atual: %.1f W\n", calcular_consumo(lab, n));
    printf("Economia gerada: %.1f W economizados\n", calcular_economia(lab, n));
}

float calcular_consumo(Computador lab[], int n) {
    float total = 0.0;
    for (int i = 0; i < n; i++) total += lab[i].consumo_watts;
    return total;
}

float calcular_economia(Computador lab[], int n) {
    float consumo_maximo = (float)(n * CONSUMO_LIGADO);
    return consumo_maximo - calcular_consumo(lab, n);
}

void gerar_relatorio(Computador lab[], int n) {
    int ligadas = 0, hibernando = 0, desligadas = 0;
    limpar_tela();
    printf("==========================================\n");
    printf(" RELATORIO FINAL\n");
    printf("==========================================\n");

    for (int i = 0; i < n; i++) {
        if (lab[i].estado == 1) ligadas++;
        else if (lab[i].estado == 2) hibernando++;
        else desligadas++;
    }

    printf("Maquinas ligadas: %d\n", ligadas);
    printf("Maquinas hibernando: %d\n", hibernando);
    printf("Maquinas desligadas: %d\n", desligadas);
    printf("------------------------------------------\n");
    printf("Consumo total atual: %.1f W\n", calcular_consumo(lab, n));
    printf("Economia gerada: %.1f W\n", calcular_economia(lab, n));
    printf("------------------------------------------\n");
    printf("HISTORICO DE EVENTOS POR MAQUINA:\n\n");

    for (int i = 0; i < n; i++) {
        printf(" PC %02d%s: %s\n", lab[i].id,
               lab[i].prioritaria ? " [PRIO]" : " ",
               lab[i].historico[0] ? lab[i].historico : "(sem eventos)");
    }
    printf("==========================================\n");
    printf("\nFim da simulação.\n");
}

/* ==========================================================================
   ETAPA 4: Criar as funções: "ligar_maquina", "definir_prioritario" e "simulacao"
========================================================================== */

void ligar_maquina(Computador lab[], int id) {
    for (int i = 0; i < TOTAL_COMPUTADORES; i++) {
        if (lab[i].id == id) {
            if (lab[i].estado == 1) {
                printf("\n Maquina ja esta ligada.\n");
            } else {
                lab[i].estado = 1;
                lab[i].tempo_inativo = 0;
                lab[i].consumo_watts = CONSUMO_LIGADO;
                registrar_evento(&lab[i], "Ligada manualmente");
                printf("\n [OK] Maquina %02d foi ligada com sucesso.\n", id);
            }
            aguardar_enter();
            return;
        }
    }
}

void definir_prioritaria(Computador lab[], int id) {
    for (int i = 0; i < TOTAL_COMPUTADORES; i++) {
        if (lab[i].id == id) {
            lab[i].prioritaria = 1;
            registrar_evento(&lab[i], "Definida como prioritaria");
            printf("\n [OK] Maquina %02d marcada como prioritaria.\n", id);
            aguardar_enter();
            return;
        }
    }
}

void simular_atividade(Computador lab[], int n, int id) {
    int alvo = -1;
    if (id == 0) alvo = rand() % n;
    else {
        for (int i = 0; i < n; i++) {
            if (lab[i].id == id) { alvo = i; break; }
        }
    }

    if (alvo == -1) return;

    int acao;
    printf("\n --- ACOES PARA MAQUINA %02d ---\n", lab[alvo].id);
    printf(" 1. Simular Atividade (Ligar/Zerar inatividade)\n");
    printf(" 2. Desligar maquina completamente\n");
    printf(" Escolha: ");
    scanf("%d", &acao);
    limpar_buffer();

    if (acao == 1) {
        lab[alvo].estado = 1;
        lab[alvo].tempo_inativo = 0;
        lab[alvo].consumo_watts = CONSUMO_LIGADO;
        registrar_evento(&lab[alvo], "Atividade detectada");
        printf("\n [OK] Maquina %02d agora esta ativa.\n", lab[alvo].id);
    } else if (acao == 2) {
        lab[alvo].estado = 0;
        lab[alvo].tempo_inativo = 0;
        lab[alvo].consumo_watts = CONSUMO_DESLIGADO;
        registrar_evento(&lab[alvo], "Desligada pelo usuario");
        printf("\n [OK] Maquina %02d foi DESLIGADA.\n", lab[alvo].id);
    }
    aguardar_enter();
}

/* ==========================================================================
   ETAPA 5: Interface, menu, e Validação do id
========================================================================== */

// Valida se o ID digitado pelo usuário existe (entre 1 e 15) para evitar que o programa trave
int validar_id(int id, int min, int max) {
    if (id < min || id > max) {
        printf("\n [ERRO] ID %d invalido! Digite um numero entre %d e %d.\n", id, min, max);
        return 0;
    }
    return 1;
}

void menu(Computador lab[], int n, int ciclo_atual) {
    int opcao, id;

    limpar_tela();
    printf("==============================================\n");
    printf(" HUB DE EFICIENCIA ENERGETICA - LABORATORIO \n");
    printf(" CICLO ATUAL: %d\n", ciclo_atual);
    printf("==============================================\n\n");
    printf(" 1. Ver painel de controle\n");
    printf(" 2. Simular atividade de usuario/Desligar\n");
    printf(" 3. Ligar uma maquina\n");
    printf(" 4. Definir maquina como prioritaria\n");
    printf(" 5. Ver historico de uma maquina\n");
    printf(" 6. Avancar ciclo\n");
    printf(" 0. Encerrar simulacao\n\n");
    printf(" Escolha: ");
    
    if (scanf("%d", &opcao) != 1) { limpar_buffer(); return; }
    limpar_buffer();

    switch (opcao) {
        case 1: limpar_tela(); exibir_status(lab, n); aguardar_enter(); break;
        case 2: {
            int escolha, id_alvo;
            limpar_tela();
            printf(" Simular atividade em qual maquina?\n\n");
            printf(" 1. Maquina aleatoria\n");
            printf(" 2. Escolher uma especifica\n\n");
            printf(" Opcao: ");
            scanf("%d", &escolha); limpar_buffer();
            if (escolha == 2) {
                printf(" ID da maquina (1-%d): ", TOTAL_COMPUTADORES);
                scanf("%d", &id_alvo); limpar_buffer();
                if (validar_id(id_alvo, 1, TOTAL_COMPUTADORES)) simular_atividade(lab, n, id_alvo);
                else aguardar_enter();
            } else simular_atividade(lab, n, 0);
            break;
        }
        case 3:
            limpar_tela();
            printf(" ID da maquina (1-%d): ", TOTAL_COMPUTADORES);
            scanf("%d", &id); limpar_buffer();
            if (validar_id(id, 1, TOTAL_COMPUTADORES)) ligar_maquina(lab, id);
            else aguardar_enter();
            break;
        case 4:
            limpar_tela();
            printf(" ID da maquina prioritaria (1-%d): ", TOTAL_COMPUTADORES);
            scanf("%d", &id); limpar_buffer();
            if (validar_id(id, 1, TOTAL_COMPUTADORES)) definir_prioritaria(lab, id);
            else aguardar_enter();
            break;
        case 5:
            limpar_tela();
            printf(" ID da maquina (1-%d): ", TOTAL_COMPUTADORES);
            scanf("%d", &id); limpar_buffer();
            if (validar_id(id, 1, TOTAL_COMPUTADORES)) {
                printf("\n Historico PC %02d:\n %s\n", id, lab[id-1].historico);
            }
            aguardar_enter();
            break;
        case 6:
            printf("\n [OK] Ciclo %d processado com sucesso!\n", ciclo_atual);
            printf(" As maquinas estao ha mais %d min inativas.\n", CICLO_SIMULACAO);
            aguardar_enter();
            break;
        case 0: gerar_relatorio(lab, n); exit(0);
        default: printf("\n [ERRO] Opcao invalida.\n"); aguardar_enter(); break;
    }
}

/* ==========================================================================
   FUNÇÕES ADICIONAIS (Suporte e Sistema)
========================================================================== */

// Limpa o console para a interface ficar organizada (funciona no Windows e Linux)
void limpar_tela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Remove restos do teclado (como o 'Enter' invisível) para o programa não pular perguntas
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Pausa a tela para dar tempo de o usuário ler o que aconteceu antes de voltar ao menu
void aguardar_enter() {
    printf("\nPressione ENTER para voltar ao menu...");
    limpar_buffer();
}
