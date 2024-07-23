#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Estrutura para as senhas da fila de atendimento
typedef struct Senha {
    char codigo[10]; // armazena o c�digo da senha (letra + n�mero)
    char tipo[100]; // armazena o tipo de senha
    int numeroConta; // armazena o n�mero da conta associada � senha
    struct Senha* prox; // ponteiro para a pr�xima senha
} Senha;

// Estrutura para as contas banc�rias
typedef struct Conta {
    char nome[50]; // nome do titular
    char cpf[20]; // cpf do titular 
    int numero; // o n�mero da conta banc�ria
    int senha1; // senha de acesso � conta
    double saldo; // saldo banc�rio
    struct Conta* prox; // ponteiro para a pr�xima conta
} Conta;

// Ponteiros para o in�cio e fim da fila de senhas e in�cio da lista de contas
Senha* inicioFila = NULL;
Senha* fimFila = NULL;
Conta* listaContas = NULL;

// Vari�vel para contar o n�mero de senhas geradas
int contadorSenhas = 0;
int numeroContaAtual = 1000; // valor inicial das contas

// Prot�tipos das fun��es
//---------------------------MENU PRINCIPAL---------------------------
void gerarSenha(const char* tipo, int numeroConta);
void atenderSenha();
void visualizarSenhas();
void excluirSenha(const char* codigo);
//------------------------- SERVI�OS--------------------------------------------
void abrirConta(char* nome, char* cpf, double saldo, int senha1);
void encerrarConta(int numeroConta, int senhaConta);
void transferir(int contaOrigem, int contaDestino, double valor);
void depositar(int numeroConta, double valor);
void sacar(int numeroConta, double valor);
void visualizarDadosConta(int numeroConta);
//----------------------------- ARQUIVOS ---------------------------
void salvarSenhas();
void carregarSenhas();
void salvarContas();
void carregarContas();
void salvarNumeroContaAtual();
void carregarNumeroContaAtual();
//--------------------------- PONTEIROS ---------------------------
Senha* buscarSenha(const char* codigo);
Conta* buscarConta(int numero);

// Fun��o principal
int main() {
    setlocale(LC_ALL, "Portuguese");

    int opcao;
    int subOpcao;
    char codigoSenha[10];

	carregarNumeroContaAtual(); // Manter as contas em arquivo
    carregarSenhas();  // Carregar senhas do arquivo ao iniciar o programa
    carregarContas();  // Carregar contas do arquivo ao iniciar o programa

    do { //op��es do menu principal
        system("cls");
        printf("\tSENAIBANK\n");
        printf("\n");
        printf("MENU PRINCIPAL:\n");
        printf("1. Gerar Senha\n");
        printf("2. Atendimento\n");
        printf("3. Controle de Senha\n");
        printf("4. Exclus�o de Senha\n");
        printf("5. Sair\n");
        printf("Escolha uma op��o: ");
        fflush(stdin);
        scanf("%i", &opcao);

        switch (opcao) { /*----------------- MENU SECUND�RIO PARA SERVI�OS-----------------------------------------------------------------------*/
            case 1: //gerar senha 
                system("cls");
                printf("Selecione o tipo de servi�o:\n");
                printf("1. Abertura de Conta\n");
                printf("2. Encerramento de Conta\n");
                printf("3. Dep�sito\n");
                printf("4. Saque\n");
                printf("5. Transfer�ncia\n");
                printf("6. Saldo\n");
                printf("Escolha uma op��o: ");
                fflush(stdin);
                scanf("%i", &subOpcao);

                switch (subOpcao) { //op��es do menu secund�rio dentro da op��o de gerar senha
                	
                    case 1: //abrir conta
                        gerarSenha("Abertura de Conta", 0);
                        system("pause");
                        break;
                        
                    case 2: //encerrar conta
                        gerarSenha("Encerramento de Conta", 0);
                        system("pause");
                        break;
                        
                    case 3: //dep�sito 
                    	gerarSenha("Dep�sito", 0);
                        system("pause");
                        break;
                        
                    case 4: //saque
						gerarSenha("Saque", 0);
                        system("pause");
                        break;
                        
                    case 5: //transfer�ncia de valores
                        gerarSenha("Transfer�ncias", 0);
                        system("pause");
                        break;
                        
                    case 6: // mostrar saldo
                        gerarSenha("Saldo", 0);
                        system("pause");
                        break;
                        
                    default:
                        printf("Op��o inv�lida. Tente novamente.\n");
                }
                break;
                
// --------------------------------------------  CONTINUA��O DO MENU PRINCIPAL -------------------------------------------------------------------------    
            
			case 2: //atendimento
                if (inicioFila == NULL) { //caso a fila de senhar estiver vazia
                    printf("Nenhuma senha gerada anteriormente. Por favor, gere uma senha antes de solicitar atendimento.\n");
                } else {
                    atenderSenha(); //invoca a fun��o de atender senha
                }
                system("pause");
                break;
                
            case 3: // controle de senha de atendimento
                visualizarSenhas(); //invoca a fun��o de visualizar senha
                system("pause");
                break;
                
            case 4: //exclus�o da senha de atendimento
                printf("Digite o c�digo da senha a excluir: ");
                fflush(stdin);
                scanf("%s", codigoSenha);
                excluirSenha(codigoSenha); //invoca a fun��o de excluir a senha, passando a senha informada pelo user como par�metro para exclus�o 
                system("pause");
                break;
                
            case 5: //sair
                printf("Saindo...");
                salvarNumeroContaAtual();
                break;
                
            default:
                printf("Op��o inv�lida. Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}

// Fun��es de gerenciamento de senha
void gerarSenha(const char* tipo, int numeroConta) {
    Senha* novaSenha = (Senha*)malloc(sizeof(Senha)); //fun��o malloc aloca espa�o na mem�ria
    char letraInicial; //especifica o atendimento na senha
    char prioridade; //string que confirma ou n�o se � uma senha priorit�ria 

    // Determinar a letra inicial com base no tipo de opera��o
    if (strcmp(tipo, "Abertura de Conta") == 0) {
        letraInicial = 'A';
    } else if (strcmp(tipo, "Encerramento de Conta") == 0) {
        letraInicial = 'E';
    } else if (strcmp(tipo, "Transfer�ncias") == 0) {
        letraInicial = 'T';
    } else if (strcmp(tipo, "Dep�sito") == 0) {
        letraInicial = 'D';
    } else if (strcmp(tipo, "Saque") == 0) {
        letraInicial = 'R';
    } else if (strcmp(tipo, "Saldo") == 0) {
        letraInicial = 'S';
    } else {
        letraInicial = 'X'; // Para poss�veis erros
    }

    // Perguntar se � uma senha priorit�ria
    system("cls");
	printf("Conforme o artigo 1� da Lei 10.048, as pessoas com defici�ncia, as pessoas com transtorno do espectro autista, as pessoas idosas com idade igual ou superior a 60 (sessenta) anos, as gestantes, as lactantes, as pessoas com crian�a de colo, os obesos, as pessoas com mobilidade reduzida e os doadores de sangue ter�o atendimento priorit�rio.\n");
    printf("\nA senha � priorit�ria? (S/N): ");
    fflush(stdin);
    scanf("%c", &prioridade);
    
    if (prioridade == 'S' || prioridade == 's') {
        snprintf(novaSenha->codigo, sizeof(novaSenha->codigo), "P%c%d", letraInicial, ++contadorSenhas);
    } else {
        snprintf(novaSenha->codigo, sizeof(novaSenha->codigo), "%c%d", letraInicial, ++contadorSenhas);
    }
    strcpy(novaSenha->tipo, tipo);
    novaSenha->numeroConta = numeroConta; // Armazenar o n�mero da conta na senha
    novaSenha->prox = NULL;

    // Inserir a nova senha na fila
    if (inicioFila == NULL) {
        inicioFila = novaSenha;
        fimFila = novaSenha;
    } else if (prioridade == 'S' || prioridade == 's') {
        // Inserir a senha priorit�ria mantendo a ordem de chegada
        Senha* atual = inicioFila;
        Senha* anterior = NULL;
        while (atual != NULL && atual->codigo[0] == 'P') {
            anterior = atual;
            atual = atual->prox;
        }
        if (anterior == NULL) {
            novaSenha->prox = inicioFila;
            inicioFila = novaSenha;
        } else {
            novaSenha->prox = anterior->prox;
            anterior->prox = novaSenha;
        }
        if (novaSenha->prox == NULL) {
            fimFila = novaSenha;
        }
    } else {
        // Inserir no fim da fila
        fimFila->prox = novaSenha;
        fimFila = novaSenha;
    }

    printf("Senha gerada: %s - Tipo: %s\n", novaSenha->codigo, novaSenha->tipo);
    salvarSenhas();
}

//fun��o para visualizar as contas, tanto ao abrir uma nova, quanto para consultar o saldo
void visualizarDadosConta(int numeroConta) {
    Conta* conta = buscarConta(numeroConta);
    
    if (conta != NULL) {
        printf("\tDados da conta:\n");
        printf("Nome do titular: %s\n", conta->nome);
        printf("CPF do titular: %s\n", conta->cpf);
		printf("N�mero da conta: %d\n", conta->numero);
        printf("Saldo: %.2f\n", conta->saldo);
    } else {
        printf("Conta n�o encontrada.\n");
    }
}

//OP��O 2 DO MENU PRINCIPAL - ATENDIMENTO
//fun��o para atender a senha de atendimento gerada
void atenderSenha() {
    if (inicioFila == NULL) {
        printf("Nenhuma senha na fila de atendimento.\n");
        return;
    }
    
    Senha* senhaAtendida = inicioFila;
    inicioFila = inicioFila->prox;
    if (inicioFila == NULL) {
        fimFila = NULL;
    }
    printf("Atendendo senha: %s - Tipo: %s\n", senhaAtendida->codigo, senhaAtendida->tipo);
        
        //caso usu�rio tenha gerado senha de abertura de conta
        if (strcmp(senhaAtendida->tipo, "Abertura de Conta") == 0) {
        char nome[50], cpf[15];
        double saldo;
        int senha1;
	
		system("cls");
		printf("\tSetor de Abertura de Conta\n");
        printf("Cadastre o nome do titular: ");
        fflush(stdin);
        gets(nome);
        
        int validacao_cpf;
		do {
		    printf("Cadastre o CPF do titular, preenchendo todos os 14 caracteres, considerando ponto e h�fen, no formato XXX.XXX.XXX-XX: ");
		    fflush(stdin);
		    fgets(cpf, sizeof(cpf), stdin);
		    cpf[strcspn(cpf, "\n")] = '\0'; // Remove o caractere de nova linha do final
		    validacao_cpf = (strlen(cpf) == 14); // verifica se o CPF tem 14 caracteres
		
		    if (!validacao_cpf || cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-') {
		        validacao_cpf = 0; 
		    }
		
		    if (!validacao_cpf) {
		        printf("CPF inv�lido! O CPF deve ter exatamente 14 caracteres no formato XXX.XXX.XXX-XX. Tente novamente!\n");
		    }
		} while (!validacao_cpf);

        // Entrada da senha num�rica com valida��o de 6 d�gitos
        do {
            printf("Cadastre uma senha de acesso num�rica de 6 d�gitos (n�o utilize caracteres especiais): ");
            fflush(stdin);
            scanf("%d", &senha1);
            if (senha1 < 100000 || senha1 > 999999) {
                printf("Senha inv�lida! A senha deve ter exatamente 6 d�gitos. Tente novamente!\n");
            }
        } while (senha1 < 100000 || senha1 > 999999);
        printf("Informe o valor que deseja depositar: ");
        scanf("%lf", &saldo);

        if (saldo < 0) {
            printf("Valor inv�lido. O dep�sito deve ser maior ou igual a zero.\n");
        } else {
        system("cls");
        abrirConta(nome, cpf, saldo, senha1); //invoca a fun��o de abrir a conta
        visualizarDadosConta(numeroContaAtual - 1); //ap�s abrir a conta, invoca a fun��o que permite visualizar os dados da conta gerada
		printf("Conta aberta com sucesso!\n");
	}
	//caso usu�rio tenha gerado senha de encerramento	
    } else if (strcmp(senhaAtendida->tipo, "Encerramento de Conta") == 0) {
        int numeroConta;
        int senhaConta;
        char resposta;

        system("cls");
        printf("\tSetor de Encerramento de Conta\n");
        printf("Digite o n�mero da conta que deseja encerrar: ");
        fflush(stdin);
        scanf("%d", &numeroConta);
        printf("Digite a senha de acesso � conta: ");
        fflush(stdin);
        scanf("%d", &senhaConta);
                
        printf("Ciente de que o encerramento da conta � definitivo, deseja continuar? (S/N): ");
        fflush(stdin);
        scanf(" %c", &resposta);
        
    if (resposta == 'S' || resposta == 's') {
        Conta* conta = buscarConta(numeroConta);
        if (conta->saldo != 0) {
        printf("A conta n�o pode ser encerrada porque o seu saldo n�o est� zerado.\n");
        return;
    } else if (conta != NULL && conta->senha1 == senhaConta) {
            encerrarConta(numeroConta, senhaConta); 
        } else {
            printf("N�mero da conta ou senha incorretos. N�o � poss�vel encerrar a conta.\n");
        }
    } else if (resposta == 'N' || resposta == 'n') {
        printf("Opera��o de encerramento de conta cancelada.\n");
        return; 
    } else {
        printf("Op��o inv�lida. Opera��o de encerramento de conta cancelada.\n");
        return; 
    }
	
	//caso o usu�rio tenha gerado senha de dep�sito de valores
    } else if (strcmp(senhaAtendida->tipo, "Dep�sito") == 0) {
        int numeroConta;
        double valor;
        int senhaConta;
        
        system("cls");
		printf("\tSetor de Dep�sito\n");
        printf("Digite o n�mero da conta para dep�sito: ");
        fflush(stdin);
        scanf("%d", &numeroConta);
        printf("Digite a senha de acesso � conta: ");
        fflush(stdin);
        scanf("%d", &senhaConta);
        printf("Digite o valor que deseja depositar: ");
        fflush(stdin);
        scanf("%lf", &valor);
        
        if (valor <= 0) {
            printf("Valor inv�lido. O dep�sito deve ser maior que zero.\n");
        } else {
        Conta* conta = buscarConta(numeroConta);
        if (conta != NULL && conta->senha1 == senhaConta) {
        	depositar(numeroConta, valor); //invoca a fun��o de depositar
        } else {
            printf("N�mero da conta ou senha incorretos. N�o � poss�vel realizar o dep�sito.\n");
        }
	}
	//caso o usu�rio tenha gerado senha para realizar saque
    } else if (strcmp(senhaAtendida->tipo, "Saque") == 0) {
        int numeroConta;
        double valor;
        int senhaConta;
        
        system("cls");
		printf("\tSetor de Saque\n");
        printf("Digite o n�mero da conta para saque: ");
        fflush(stdin);
        scanf("%d", &numeroConta);
        printf("Digite a senha de acesso � conta: ");
        fflush(stdin);
        scanf("%d", &senhaConta);
        printf("Digite o valor que deseja sacar: ");
        fflush(stdin);
        scanf("%lf", &valor);
        
        Conta* conta = buscarConta(numeroConta);
         if (conta != NULL && conta->senha1 == senhaConta) {
            if (valor <= 0) {
                printf("Opera��o inv�lida. O valor do saque deve ser maior que zero.\n");
            } else if (valor > conta->saldo) {
                printf("Saldo insuficiente. Voc� possui apenas R$%.2f na conta.\n", conta->saldo);
            } else {
                sacar(numeroConta, valor);
            }
        } else {
            printf("N�mero da conta ou senha incorretos. N�o � poss�vel realizar o saque.\n");
        }
	//caso o usu�rio tenha gerado senha para transferir valores
    } else if (strcmp(senhaAtendida->tipo, "Transfer�ncias") == 0) {
        int contaOrigem, contaDestino;
        double valor;
        int senhaConta;
        
		system("cls");
		printf("\tSetor de Transfer�ncia de Valores\n");
        printf("Digite o n�mero da conta de origem: ");
        fflush(stdin);
        scanf("%d", &contaOrigem);
        printf("Digite a senha da conta de origem: ");
        fflush(stdin);
        scanf("%d", &senhaConta);
		printf("Digite o n�mero da conta de destino: ");
        fflush(stdin);
        scanf("%d", &contaDestino);
        printf("Digite o valor a ser transferido: ");
        fflush(stdin);
        scanf("%lf", &valor);

        Conta* conta = buscarConta(contaOrigem);
        if (conta != NULL && conta->senha1 == senhaConta) {
            if (valor <= 0) {
                printf("Opera��o inv�lida. O valor da transfer�ncia deve ser maior que zero.\n");
            } else if (valor > conta->saldo) {
                printf("Saldo insuficiente. Voc� possui apenas R$%.2f na conta.\n", conta->saldo);
            } else {
                transferir(contaOrigem, contaDestino, valor);
            }
        } else {
            printf("N�mero da conta ou senha incorretos. N�o � poss�vel realizar a transfer�ncia.\n");
        }
	//caso o usu�rio tenha gerado senha para consultar o saldo
   } else if (strcmp(senhaAtendida->tipo, "Saldo") == 0) {
        int numeroConta;
        int senhaConta;

		system("cls");
		printf("\tSetor de Consulta de Saldo\n");
        printf("Digite o n�mero da conta: ");
        scanf("%d", &numeroConta);
        printf("Digite a senha de acesso � conta: ");
        scanf("%d", &senhaConta);

        Conta* conta = buscarConta(numeroConta);
        if (conta != NULL && conta->senha1 == senhaConta) {
            visualizarDadosConta(numeroConta);
        } else {
            printf("N�mero da conta ou senha incorretos. N�o � poss�vel exibir o saldo.\n");
        }
        
    } else {
        printf("Tipo de opera��o desconhecido.\n");
    }

    free(senhaAtendida);

    salvarSenhas();
}

//controle de senha de atendimento
void visualizarSenhas() {
    Senha* atual = inicioFila;

    if (atual == NULL) {
        printf("Nenhuma senha de atendimento na fila.\n");
        return;
    }

    printf("Senhas na fila de atendimento:\n");
    while (atual != NULL) {
        printf("C�digo: %s - Tipo: %s\n", atual->codigo, atual->tipo);
        atual = atual->prox;
    }
}

//exclus�o de senha de atendimento
void excluirSenha(const char* codigo) {
    Senha* atual = inicioFila;
    Senha* anterior = NULL;

    while (atual != NULL && strcmp(atual->codigo, codigo) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Senha n�o encontrada.\n");
        return;
    }

    if (anterior == NULL) {
        inicioFila = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    if (atual == fimFila) {
        fimFila = anterior;
    }

    free(atual);
    printf("Senha exclu�da com sucesso.\n");

    salvarSenhas(); // Salvar a fila de senhas ap�s a exclus�o
}

// Fun��es de gerenciamento de contas banc�rias - menu de servi�os
//abertura de conta
void abrirConta(char* nome, char* cpf, double saldo, int senha1) {
    Conta* novaConta = (Conta*)malloc(sizeof(Conta));
    strcpy(novaConta->nome, nome);
    strcpy(novaConta->cpf, cpf);
    novaConta->numero = numeroContaAtual++;
    novaConta->senha1 = senha1;
    novaConta->saldo = saldo;
    novaConta->prox = listaContas;
    listaContas = novaConta;

    salvarContas();
    salvarNumeroContaAtual(); // Salvar o n�mero da conta atual ap�s a abertura de uma nova conta
}

//encerramento de conta
void encerrarConta(int numeroConta, int senhaConta) {
    Conta* atual = listaContas;
    Conta* anterior = NULL;

    while (atual != NULL && atual->numero != numeroConta) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Conta n�o encontrada.\n");
        return;
    }

    if (atual->senha1 != senhaConta) {
        printf("Senha incorreta.\n");
        return;
    }

    if (anterior == NULL) {
        listaContas = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    printf("Conta %d encerrada com sucesso.\n", numeroConta);

    salvarContas(); // Salvar a lista de contas ap�s o encerramento
}

//transfer�ncia de valores
void transferir(int contaOrigem, int contaDestino, double valor) {
    Conta* origem = buscarConta(contaOrigem);
    Conta* destino = buscarConta(contaDestino);

    if (origem == NULL) {
        printf("Conta de origem n�o encontrada.\n");
        return;
    }

    if (destino == NULL) {
        printf("Conta de destino n�o encontrada.\n");
        return;
    }

    if (origem->saldo < valor) {
        printf("Saldo insuficiente na conta de origem.\n");
        return;
    }

    origem->saldo -= valor;
    destino->saldo += valor;

    printf("Transfer�ncia realizada com sucesso! Novo saldo da conta de origem: %.2lf\n", origem->saldo);
    
    salvarContas(); // Salvar a lista de contas ap�s a transfer�ncia
}

//dep�sito de valores
void depositar(int numeroConta, double valor) {
    Conta* conta = buscarConta(numeroConta);
    if (conta == NULL) {
        printf("Conta n�o encontrada.\n");
        return;
    }

    conta->saldo += valor;
    printf("Dep�sito realizado com sucesso! Novo saldo: %.2lf\n", conta->saldo);

    salvarContas(); // Salvar a lista de contas ap�s o dep�sito
}

//saque de valores
void sacar(int numeroConta, double valor) {
    Conta* conta = buscarConta(numeroConta);
    if (conta == NULL) {
        printf("Conta n�o encontrada.\n");
        return;
    }

    if (conta->saldo < valor) {
        printf("Saldo insuficiente.\n");
        return;
    }

    conta->saldo -= valor;
    printf("Saque realizado com sucesso! Novo saldo: %.2lf\n", conta->saldo);

    salvarContas(); // Salvar a lista de contas ap�s o saque
}

// Fun��es de manipula��o de arquivos
//------------------- SENHAS-------------------------------------------
void salvarSenhas() {
    FILE* arquivo = fopen("senhas.dat", "wb"); 
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar as senhas.\n");
        return;
    }

    Senha* atual = inicioFila;
    while (atual != NULL) {
        fwrite(atual, sizeof(Senha), 1, arquivo);
        atual = atual->prox;
    }

    fclose(arquivo);
}

void carregarSenhas() {
    FILE* arquivo = fopen("senhas.dat", "rb"); 
    if (arquivo == NULL) {
        printf("Nenhuma senha salva anteriormente.\n");
        return;
    }

    Senha senhaTemp;
    while (fread(&senhaTemp, sizeof(Senha), 1, arquivo) == 1) {
        Senha* novaSenha = (Senha*)malloc(sizeof(Senha));
        if (novaSenha == NULL) {
            printf("Erro ao alocar mem�ria para a nova senha.\n");
            fclose(arquivo);
            return;
        }
        *novaSenha = senhaTemp;
        novaSenha->prox = NULL;

        if (inicioFila == NULL) {
            inicioFila = novaSenha;
            fimFila = novaSenha;
        } else {
            fimFila->prox = novaSenha;
            fimFila = novaSenha;
        }
    }

    fclose(arquivo);
}

//-------------------------- CONTAS----------------------------

void salvarContas() {
    FILE* arquivo = fopen("contas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de contas.\n");
        return;
    }

    Conta* atual = listaContas;
    while (atual != NULL) {
        fprintf(arquivo, "%s %s %d %d %.2lf\n", atual->nome, atual->cpf, atual->numero, atual->senha1, atual->saldo);
        atual = atual->prox;
    }

    fclose(arquivo);
}

void carregarContas() {
    FILE* arquivo = fopen("contas.txt", "r");
    if (arquivo == NULL) {
        return;
    }

    while (!feof(arquivo)) {
        Conta* novaConta = (Conta*)malloc(sizeof(Conta));
        if (novaConta == NULL) {
            printf("Erro ao alocar mem�ria para a nova conta.\n");
            fclose(arquivo);
            return;
        }
        if (fscanf(arquivo, "%s %s %d %d %lf\n", novaConta->nome, novaConta->cpf, &novaConta->numero, &novaConta->senha1, &novaConta->saldo) != 5) {
            free(novaConta); 
            break;
        }
        novaConta->prox = listaContas;
        listaContas = novaConta;
    }

    fclose(arquivo);
}

void salvarNumeroContaAtual() {
    FILE* arquivo = fopen("numero_conta_atual.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de n�mero de conta atual.\n");
        return;
    }

    fprintf(arquivo, "%d\n", numeroContaAtual);

    fclose(arquivo);
}

void carregarNumeroContaAtual() {
    FILE* arquivo = fopen("numero_conta_atual.txt", "r");
    if (arquivo == NULL) {
        return;
    }

    fscanf(arquivo, "%d", &numeroContaAtual);

    fclose(arquivo);
}

Conta* buscarConta(int numero) {
    Conta* atual = listaContas;
    while (atual != NULL && atual->numero != numero) {
        atual = atual->prox;
    }
    return atual;
}



