#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

typedef struct {
	char nome[51];
	char telefone[16];
	char empresa[100];
} Contato;

#define LIMITE 10
#define NOME_AQUIVO "contatos.agd"

Contato contatos[LIMITE];
int contador = 0;

void abrirArquivo(){

	FILE *file = fopen(NOME_AQUIVO,"r");
	contador = 0;
	
	if(!file == NULL){
		rewind(file);			
		
		while(!feof(file)){
			fgets(contatos[contador].nome, 50, file);
			if(strlen(contatos[contador].nome) > 1){
				fgets(contatos[contador].telefone, 15, file);
				fgets(contatos[contador].empresa, 100, file);
				contador++;
			}
		}		
	}else{
		printf("Erro ao carregar contatos.\n");
	}
	
	fclose(file);
}

void salvarNoArquivo(Contato contato, FILE *file){
	fputs(contato.nome, file);
	fputs(contato.telefone, file);
	fputs(contato.empresa, file);
}

bool reescreverArquivo(int ignore){
	bool resultado = false;
	Contato arrayTemporario[LIMITE];
	int contadorAux = 0;
	int i;
	
	remove(NOME_AQUIVO);
	FILE *file = fopen(NOME_AQUIVO, "a+");
	
	if(!file == NULL){
		for(i = 0; i < contador; i++){
			if(i != ignore){
				salvarNoArquivo(contatos[i], file);
				arrayTemporario[contadorAux] = contatos[i];
				contadorAux++;
			}
		}
		
		for(i = 0; i < contadorAux; i++){
			contatos[i] = arrayTemporario[i];
		}
		contador = contadorAux;
		resultado = true;
	}
	
	fclose(file);	
	
	return resultado;
}

bool inserirNoArquivo(Contato contato){
	
	bool resultado = false;
	FILE *file = fopen(NOME_AQUIVO, "a+");
	
	if(!file == NULL){
		resultado = true;
		
		salvarNoArquivo(contato, file);
		contatos[contador] = contato;
		contador++;
	}
	
	fclose(file);
	
	return resultado;
}

void cadastrar(){
	printf("\n===========RESULTADO=============\n");
	if(contador < LIMITE){
	
		Contato contato;
		
		printf("\n ** Novo Contato **\n\n");
		printf("Nome: ");
		fgets(contato.nome, 50, stdin);
		fflush(stdin);
		
		printf("Telefone: ");
		fgets(contato.telefone, 15, stdin);
		fflush(stdin);
		
		printf("Empresa: ");
		fgets(contato.empresa, 100, stdin);
		fflush(stdin);
		
		if(inserirNoArquivo(contato)){
			printf("\n ** Contato salvo! **\n");
		}else{
			printf("\nErro ao salvar contato.\n");
		}
	}else{
		printf("\n Perdão! Memória cheia :/\n");
	}	
	printf("\n=================================\n");
}

void alterar(int i){
	
	Contato contato;

	printf("\n ** Alterar (pressione ENTER caso não queira alterar alguma parte) ** \n\n");
	
	printf("Nome: %s",contatos[i].nome);
	fgets(contato.nome, 50, stdin);
	fflush(stdin);
	if(strlen(contato.nome) > 1){
		strcpy(contatos[i].nome, contato.nome);
	}
	
	printf("Telefone: %s",contatos[i].telefone);
	fgets(contato.telefone, 15, stdin);
	fflush(stdin);
	if(strlen(contato.telefone) > 1){
		strcpy(contatos[i].telefone, contato.telefone);
	}
	
	printf("Empresa: %s",contatos[i].empresa);
	fgets(contato.empresa, 100, stdin);
	fflush(stdin);
	if(strlen(contato.empresa) > 1){
		strcpy(contatos[i].empresa, contato.empresa);
	}
	
	if(reescreverArquivo(-1)){ 
		printf("\nContato salvo.\n"); 
	}else{
		printf("\nErro ao salvar as alterações! :/\n");
	}
}

bool confirmar(char *messagem){
	char resposta;
	
	printf("%s (S para SIM, outro para NÃO): ", messagem);
	scanf("%c", &resposta);
	
	if(resposta == 's' || resposta == 'S'){
		return true;
	}else{
		return false;
	}	
}

void deletar(int i){	
	if(confirmar("Deseja realmente excluir este contato?")){
		if(reescreverArquivo(i)){			
			printf("\nContato excluido.\n\n");
		}		
	}
}

void menuSelecionarAlterar(){
	int selecionado = -1;
	int opcao = 0;
	
	printf("Informe o código do contato para selecionar (Ou 0 para voltar ao menu): ");
	scanf("%i", &selecionado);
	fflush(stdin);
	
	if(selecionado > 0){
		if(selecionado <= LIMITE){
			selecionado--;
			printf("\nSelecionado: %s\n", contatos[selecionado].nome);
			printf("Confirme 1 para Editar (Ou outro número para sair): ");
			
			scanf("%i", &opcao);
			fflush(stdin);

			switch(opcao){
				case 1: alterar(selecionado);
				break;
			}	
		} else {
			printf("\nNão existe um contato com esse código!\n");
		}
	}
}

void menuSelecionarDeletar() {
	int selecionado = -1;
	int opcao = 0;
	
	printf("Informe o código do contato para selecionar (Ou 0 para voltar ao menu): ");
	scanf("%i", &selecionado);
	fflush(stdin);
	
	if (selecionado > 0) {
		if (selecionado <= LIMITE) {
			selecionado--;
			printf("\nSelecionado: %s\n", contatos[selecionado].nome);;
			deletar(selecionado);	
		} else {
			printf("\nNão existe um contato com esse código!\n");
		}
	}
}

void exibirContato(int i) {
	printf("[%i]\n", i+1);
	printf("Nome: %s", contatos[i].nome);
	printf("Telefone: %s", contatos[i].telefone);
	printf("Empresa: %s", contatos[i].empresa);
}

void consultar() {
	int i;
	printf("\n===========RESULTADO=============\n");
	printf("\nConsultar: \n");
	
	printf("     ** Lista de Contatos **\n");
	for (i = 0; i < contador; i++) {
		exibirContato(i);
		printf("------------------------\n");
	}
	printf("\n=================================\n");
	
}

bool combinacaoDeLetras(char *a, char *b, int i, int limite) {
	if(a[i] == b[i]) {
		i++;
		if(i<limite){
			return combinacaoDeLetras(a, b, i, limite);
		} else {
			return true;
		}
	} else {
		return false;
	}
}

void buscaContatoAlterar(){
	char nome[51] = {""};
	bool encontrado = false;
	int i;
	
	printf("\n===========RESULTADO=============\n");
	
	printf("      ** Busca contato **\n\n");
	printf("Digite o nome para busca (case sensitive): ");
	fgets(nome, 50, stdin);
	fflush(stdin);
	
	int limite = strlen(nome);
	limite--;
	
	for(i = 0; i < contador; i++){
		if(strlen(contatos[i].nome) >= limite){
			if(combinacaoDeLetras(nome, contatos[i].nome, 0, limite)){
				printf("\n");
				exibirContato(i);	
				printf("----------------------------\n");
				encontrado = true;
			}
		}
	}
	
	if(encontrado){
		menuSelecionarAlterar();
	}
	
	printf("\n=================================\n");
}

void buscaContatoDeletar(){
	char nome[51] = {""};
	bool encontrado = false;
	int i;
	
	printf("\n===========RESULTADO=============\n");
	
	printf("\n      ** Busca contato **\n\n");
	printf("Digite o nome para busca (case sensitive): ");
	fgets(nome, 50, stdin);
	fflush(stdin);
	
	int limite = strlen(nome);
	limite--;
	
	for(i = 0; i < contador; i++){
		if(strlen(contatos[i].nome) >= limite){
			if(combinacaoDeLetras(nome, contatos[i].nome, 0, limite)){
				printf("\n");
				exibirContato(i);	
				printf("----------------------------\n");
				encontrado = true;
			}
		}
	}
	
	if(encontrado){
		menuSelecionarDeletar();
	}
	
	printf("\n=================================\n");
}

void sair() {
	system("cls");
	printf("\n--------------------------------\n");
	printf("\n\t    SAINDO...\n");
	exit(1);
}

int main(){
	setlocale(LC_ALL, "portuguese");
	abrirArquivo();
	int opcao = 0;
	
	while(opcao != 5){
		printf("\n==============MENU===============\n");
	  	printf("|\t1 - Cadastrar\t\t|");
	  	printf("\n|\t2 - Consultar\t\t|");
	  	printf("\n|\t3 - Alterar\t\t|");
	  	printf("\n|\t4 - Deletar\t\t|");
	  	printf("\n|\t5 - Sair\t\t|");
		printf("\n=================================\n");
		
		printf("\nDigite o número da opção desejada: ");
		scanf("%i", &opcao);
		fflush(stdin);
		
		if(opcao){
			switch(opcao){
				case 1: 
					cadastrar();
					break;
				case 2: 
					consultar();
					break;
				case 3: 
					buscaContatoAlterar();
					break;
				case 4: 
					buscaContatoDeletar();
					break;
				case 5: 
					sair();
					break;
				default: 
					printf("\n--------------------------------\n");
					printf("\n\tOPÇÃO INVÁLIDA\n");
					printf("\n--------------------------------\n");
			}
		}
	}
}
