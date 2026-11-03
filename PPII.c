#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 50
#define MAX_TENTATIVAS 3


typedef struct {
    char nome[50];
    char telefone[20];
    int cadastrado;
} Cliente;

typedef struct {
    char nome[50];
    char especie[20];
    char raca[30];
    char porte[20];
    int porte_codigo;
    int cadastrado;
} Pet;

typedef struct {
    char desc[60];
    int qtd;
    float val;
} Item;

typedef struct {
    Cliente cliente;
    Pet pet;
    Item itens[MAX_ITENS];
    int total_itens;
    float total_geral;
    char funcionario[30];
} Atendimento;


char nomes_categorias[3][20] = {"ALIMENTOS", "ACESSORIOS", "HIGIENE"};
char nomes_produtos[3][3][30];
float precos_produtos[3][3];


int lerInteiroSeguro() {
    int valor, status;
    do {
        status = scanf("%d", &valor);
        while (getchar() != '\n');
        if (status != 1) {
            printf("[ERRO] Entrada invalida! Digite um numero: ");
        }
    } while (status != 1);
    return valor;
}


void limparAtendimento(Atendimento *a) {
    a->cliente.cadastrado = 0;
    a->pet.cadastrado = 0;
    a->total_itens = 0;
    a->total_geral = 0.0;
}


void garantirArquivoUsuarios() {
    FILE *f = fopen("usuarios.txt", "r");
    if (!f) {
        f = fopen("usuarios.txt", "w");
        fprintf(f, "admin 1234\n");
    }
    if (f) fclose(f);
}

int login(Atendimento *a) {
    char usuario[30], senha[30];
    char u_arq[30], s_arq[30];
    int tentativas = 0, autenticado = 0;

    garantirArquivoUsuarios();

    while (tentativas < MAX_TENTATIVAS) {
        printf("\n===== LOGIN DO FUNCIONARIO =====\n");
        printf("Usuario: ");
        scanf(" %29[^\n]", usuario);
        printf("Senha: ");
        scanf(" %29[^\n]", senha);
        while (getchar() != '\n');

        FILE *f = fopen("usuarios.txt", "r");
        if (f) {
            while (fscanf(f, "%s %s", u_arq, s_arq) != EOF) {
                if (strcmp(usuario, u_arq) == 0 && strcmp(senha, s_arq) == 0) {
                    strcpy(a->funcionario, usuario);
                    autenticado = 1;
                    break;
                }
            }
            fclose(f);
        }

        if (autenticado) {
            printf("\nBem-vindo(a), %s!\n", a->funcionario);
            return 1;
        }
        
        tentativas++;
        printf("[ERRO] Usuario ou senha incorretos! (%d/%d)\n", tentativas, MAX_TENTATIVAS);
    }
    printf("\nNumero maximo de tentativas atingido. Encerrando...\n");
    return 0;
}


void carregarProdutos() {
    FILE *f = fopen("produtos.txt", "r");
    if (!f) {
        f = fopen("produtos.txt", "w");
        
        fprintf(f, "Racao_cao_10kg 120.00 Racao_gato_3kg 65.00 Petisco 15.00\n");
        fprintf(f, "Coleira 25.00 Guia 30.00 Caminha 80.00\n");
        fprintf(f, "Shampoo 18.00 Tapete_higienico 45.00 Escova 22.00\n");
        fclose(f);
        f = fopen("produtos.txt", "r");
    }
    
    for (int cat = 0; cat < 3; cat++) {
        for (int p = 0; p < 3; p++) {
            fscanf(f, "%29s %f", nomes_produtos[cat][p], &precos_produtos[cat][p]);
            
            for(int i = 0; nomes_produtos[cat][p][i]; i++) {
                if(nomes_produtos[cat][p][i] == '_') nomes_produtos[cat][p][i] = ' ';
            }
        }
    }
    fclose(f);
}


int registrarItem(Atendimento *a, char descricao[], int quantidade, float valor) {
    if (a->total_itens >= MAX_ITENS) {
        printf("\n[ERRO] Limite de itens do atendimento atingido!\n");
        return 0;
    }
    strcpy(a->itens[a->total_itens].desc, descricao);
    a->itens[a->total_itens].qtd = quantidade;
    a->itens[a->total_itens].val = valor;
    a->total_itens++;
    a->total_geral += valor;
    return 1;
}

void menuCadastros(Atendimento *a) {
    int opcao;
    do {
        printf("\n--------- CADASTROS ----------\n");
        printf(" 1 - Cadastrar cliente\n");
        printf(" 2 - Cadastrar pet\n");
        printf(" 0 - Voltar\n");
        printf("Escolha: ");
        opcao = lerInteiroSeguro();

        switch (opcao) {
            case 1:
                printf("\nNome do cliente: ");
                scanf(" %49[^\n]", a->cliente.nome);
                printf("Telefone: ");
                scanf(" %19[^\n]", a->cliente.telefone);
                a->cliente.cadastrado = 1;
                printf("=> Cliente cadastrado!\n");
                break;
            case 2:
                if (!a->cliente.cadastrado) {
                    printf("\n[ERRO] Cadastre o cliente primeiro!\n");
                } else {
                    int escolha;
                    printf("\nNome do pet: ");
                    scanf(" %49[^\n]", a->pet.nome);

                    do {
                        printf("Especie (1 - Cao, 2 - Gato): ");
                        escolha = lerInteiroSeguro();
                    } while (escolha != 1 && escolha != 2);
                    strcpy(a->pet.especie, (escolha == 1) ? "Cao" : "Gato");

                    printf("Raca: ");
                    scanf(" %29[^\n]", a->pet.raca);

                    do {
                        printf("Porte (1 - Pequeno, 2 - Medio, 3 - Grande): ");
                        a->pet.porte_codigo = lerInteiroSeguro();
                    } while (a->pet.porte_codigo < 1 || a->pet.porte_codigo > 3);
                    
                    if (a->pet.porte_codigo == 1) strcpy(a->pet.porte, "Pequeno");
                    else if (a->pet.porte_codigo == 2) strcpy(a->pet.porte, "Medio");
                    else strcpy(a->pet.porte, "Grande");

                    a->pet.cadastrado = 1;
                    printf("=> Pet cadastrado!\n");
                }
                break;
        }
    } while (opcao != 0);
}
