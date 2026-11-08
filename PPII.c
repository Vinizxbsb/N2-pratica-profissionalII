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

char nomes_produtos[3][3][30] = {
    {"Racao cao 10kg", "Racao gato 3kg", "Petisco"},
    {"Coleira", "Guia", "Caminha"},
    {"Shampoo", "Tapete higienico", "Escova"}
};

float precos_produtos[3][3] = {
    {120.00, 65.00, 15.00},
    {25.00, 30.00, 80.00},
    {18.00, 45.00, 22.00}
};

char nomes_servicos[3][30] = {"Banho", "Tosa", "Consulta"};

float precos_servicos[3] = {50.00, 80.00, 100.00};

float calcularServicos(Atendimento *a) {
    float total_servicos = 0.0;
    for (int i = 0; i < 3; i++) {
        if (a->itens[i].qtd > 0) {
            total_servicos += a->itens[i].val;
        }
    }
    return total_servicos;
}
void limparAtendimento(Atendimento *a) {
    a->cliente.cadastrado = 0;
    a->pet.cadastrado = 0;
    a->total_itens = 0;
    a->total_geral = 0.0;
}

int logar(char usuario[]){
    int senha[10];
    int tentativa;
    for(tentativa = 1; tentativa <= 3; tentativa++){
        printf("Usuario: \n");
        scanf("%s[^\n]", usuario);
        printf("Senha: \n");
        scanf("%d", senha);
        if(strcmp(usuario, "admin") == 0 && strcmp(senha, 1234) == 0){
            return 1;
        
        }
        printf("Dados inválidos! Tentativas restantes: %d\n", 3 - tentativa);
    return 0;
    }    
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
        scanf("%d", &opcao);

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
                        scanf("%d", &escolha);
                    } while (escolha != 1 && escolha != 2);
                    strcpy(a->pet.especie, (escolha == 1) ? "Cao" : "Gato");

                    printf("Raca: ");
                    scanf(" %29[^\n]", a->pet.raca);

                    do {
                        printf("Porte (1 - Pequeno, 2 - Medio, 3 - Grande): ");
                        scanf("%d", &a->pet.porte_codigo);
                    } while (a->pet.porte_codigo < 1 || a->pet.porte_codigo > 3);

                    if (a->pet.porte_codigo == 1) strcpy(a->pet.porte, "Pequeno");
                    else if (a->pet.porte_codigo == 2) strcpy(a->pet.porte, "Medio");
                    else strcpy(a->pet.porte, "Grande");

                    a->pet.cadastrado = 1;
                    printf("=> Pet cadastrado!\n");
                }
                break;
            case 0:
                printf("Voltando...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);
}

void serviços(Atendimento *a) {
    int servico;
    printf("\n--------- SERVICOS ----------\n");
    for (int i = 0; i < 3; i++) {
        printf("%d - %s (R$ %.2f)\n", i + 1, nomes_servicos[i], precos_servicos[i]);
    }
    printf("Escolha o servico: ");
    scanf("%d", &servico);
    if (servico < 1 || servico > 3) {
        printf("[ERRO] Servico invalido!\n");
        return;
    }
    else {
        servico--;
        float valor_total = precos_servicos[servico];
        if (registrarItem(a, nomes_servicos[servico], 1, valor_total)) {
            printf("=> Servico registrado com sucesso!\n");
        }
    }
}

void produtos(Atendimento *a) {
    int categoria, produto, quantidade;
    printf("\n--------- PRODUTOS ----------\n");
    for (int i = 0; i < 3; i++) {
        printf("%d - %s\n", i + 1, nomes_categorias[i]);
    }
    printf("Escolha a categoria: ");
    scanf("%d", &categoria);
    if (categoria < 1 || categoria > 3) {
        printf("[ERRO] Categoria invalida!\n");
        return;
    }
    categoria--;

    printf("\nProdutos da categoria %s:\n", nomes_categorias[categoria]);
    for (int j = 0; j < 3; j++) {
        printf("%d - %s (R$ %.2f)\n", j + 1, nomes_produtos[categoria][j], precos_produtos[categoria][j]);
    }
    printf("Escolha o produto: ");
    scanf("%d", &produto);
    if (produto < 1 || produto > 3) {
        printf("[ERRO] Produto invalido!\n");
        return;
    }
    produto--;

    printf("Quantidade: ");
    scanf("%d", &quantidade);
    if (quantidade <= 0) {
        printf("[ERRO] Quantidade invalida!\n");
        return;
    }

    float valor_total = precos_produtos[categoria][produto] * quantidade;
    if (registrarItem(a, nomes_produtos[categoria][produto], quantidade, valor_total)) {
        printf("=> Produto registrado com sucesso!\n");
    }
}
void menuPrincipal(Atendimento *a) {
    int opcao;
    do {
        printf("\n--------- MENU PRINCIPAL ----------\n");
        printf(" 1 - Cadastros\n");
        printf(" 2 - Serviços\n");
        printf(" 3 - Produtos\n");
        printf(" 4 - Comprovante Final\n");
        printf(" 0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                menuCadastros(a);
                break;
            case 2:
                serviços(a);
                break;
            case 3:
                produtos(a);
                break;
            case 4:
                // Chamar função para gerar comprovante final
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);
}