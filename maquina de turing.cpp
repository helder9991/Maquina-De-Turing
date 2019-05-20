#include <iostream>
#include <stdlib.h>
#include <string.h>

#define MAX 40
using namespace std;

FILE *arq;

// Função caso não exista o arquivo txt
void Ajuda(){
	printf("---- Como as informacoes ficam no arquivo ----\n");
	printf("1 Linha: Alfabeto\n");
	printf("2 Linha: Alfabeto Auxiliar\n");
	printf("3 Linha: Palavra\n");
	printf("Demais linhas: Transicoes (Estado Atual,Leitura,Escrita,Direcao,Estado Destino)\n");
	printf("OBS: E necessario colocar ':' no final das 3 primeiras linhas");
	exit(0);
}

// Funções da Lista
	struct TipoItem{
		int estado_atual;
		int estado_destino;
		char leitura;
		char escrita;
		char direcao;
	};

	typedef struct TipoCelula *TipoApontador;

	struct TipoCelula{
		TipoItem estado; 
		TipoApontador prox;
	};

	struct TipoLista{
		TipoApontador primeiro,ultimo;
	};

	void FListaVazia(TipoLista *lista){
		lista->primeiro = (TipoApontador) malloc(sizeof(TipoCelula));
		lista->ultimo = lista->primeiro;
		lista->primeiro->prox = NULL;
	}

	void Insere(TipoItem x,TipoLista *lista){
		lista->ultimo->prox = (TipoApontador) malloc(sizeof(TipoCelula));
		lista->ultimo = lista->ultimo->prox;
		lista->ultimo->estado = x;
		lista->ultimo->prox = NULL;
		
	}

	void Mostra(TipoLista lista){
		TipoApontador aux;
		aux = lista.primeiro->prox;
		while (aux!=NULL){
			printf("Estado Atual:%d (%c,%c,%c) -> Estado Destino:%d\n",aux->estado.estado_atual,
				aux->estado.leitura,aux->estado.escrita,aux->estado.direcao,aux->estado.estado_destino);
			aux=aux->prox;
		}
	}

// Fim Funções da Lista

// Verifica se o usuario escreveu os caracteres de transicao conforme o alfabeto/alfabeto auxiliar
int Verifica_Simbolo(char simbolo, char alfabeto[MAX], char alfabeto_aux[MAX]){

	if(simbolo == '#' || simbolo == '_' || simbolo == '*')
		return 0;

	for(int i = 0; i<strlen(alfabeto); i++){
		if(simbolo == alfabeto[i]){
			return 0;
		}
	}
	
	for(int i = 0; i<strlen(alfabeto_aux); i++){
		if(simbolo == alfabeto_aux[i]){
			return 0;
		}
	}

	return 1;
}
// Carrega conteudo do arquivo

	void LeTransicoes(TipoLista *maquina,char alfabeto[MAX], char alfabeto_aux[MAX]){

		while (!feof(arq)){

			int count = 0, erro;
			char linha[MAX];
			TipoItem estado;
			estado.estado_atual = 0;
			estado.estado_destino = 0;
			
			fgets(linha,MAX,arq);

			for(int i=0; i<MAX; i++){
				if (linha[i] == ','){
					count++;
				}else if(linha[i] == '\n' || linha[i] == '\000'){
					break;
				}else{
					switch(count){
						case 0:
							estado.estado_atual *= 10;
							estado.estado_atual += linha[i] - 48;
							break;
						case 1:
							erro  = Verifica_Simbolo(linha[i],alfabeto,alfabeto_aux);

							if (erro == 1){
								printf("Erro na linha:%s",linha);
								exit(0);
							}

							estado.leitura = linha[i];
							
							break;
						case 2:
							erro = Verifica_Simbolo(linha[i],alfabeto,alfabeto_aux);

							if (erro == 1){
								printf("Erro na linha:%s",linha);
								exit(0);
							}

							estado.escrita = linha[i];

							break;
						case 3:

							if(linha[i] != '>' && linha[i] != '<'){
								printf("Erro na linha:%s",linha);
								exit(0);
							}

							estado.direcao = linha[i];
							
							break;
						case 4:
							estado.estado_destino *=10;
							estado.estado_destino += linha[i] - 48;
							break;
					}
				}
			}
			Insere(estado,maquina);
		}
	}

	void CortaString(char linha[MAX], char linha_aux[MAX]){
		for(int i=0; i<MAX; i++){
			if (linha[i] == ':'){		
				for(int j=0; j<i; j++){
					linha_aux[j] = linha[j];
				}
				linha_aux[i] = '\0';
				return;
			}
		}
	}

	void Carrega_Alfabeto(char alfabeto[MAX],char alfabeto_aux[MAX],char palavra[MAX]){
		char linha[MAX];

		// Carrega Alfabeto
		fgets(linha,MAX,arq);
		CortaString(linha,alfabeto);

		if(strlen(alfabeto) == 0){
			printf("Erro: Alfabeto nao declarado");
			exit(0);
		}

		// Carrega Alfabeto aux
		fgets(linha,MAX,arq);
		CortaString(linha,alfabeto_aux);

		// Carrega Palavra
		fgets(linha,MAX,arq);
		CortaString(linha,palavra);
	}

// Fim Carrega conteudo do arquivo
void IniciaFita(char fita[3*MAX],char palavra[MAX]){
	
	//Limpa Fita	
	for(int i=0; i<3*MAX; i++){
		fita[i] = '_';
	}

	// Coloca palavra na fita
	fita[MAX-1] = '#';
	for(int i=0; i<strlen(palavra); i++){
		fita[MAX+i] = palavra[i];
	}
	fita[MAX+strlen(palavra)] = '*';
}

void MostraFita(char fita[3*MAX], int cabecote,int tamPalavra){
	printf("\n");
	for(int i=0; i<3*MAX; i++){
		if(fita[i] != '_')
			printf("%c",fita[i]);
	}

	printf("\t:Fita\n");

	for(int i=0; i<3*MAX; i++){
		if(fita[i] != '_' || i-MAX+1 == cabecote){
			if (i-MAX+1 != cabecote)
				printf(" ");
			else
				printf("^");
		}
	}
	printf("\t:Cabecote");
}

void Rotina(TipoLista maquina, char alfabeto[MAX], char alfabeto_aux[MAX],char palavra[MAX]){
	char fita[3*MAX];
	IniciaFita(fita,palavra);

	if (maquina.primeiro->prox != NULL){
		int estado_atual = maquina.primeiro->prox->estado.estado_atual, tamPalavra = strlen(palavra);
		int cabecote = estado_atual;
		while(true){

			TipoApontador aux;
			aux = maquina.primeiro->prox;
			
			MostraFita(fita,cabecote,tamPalavra);
			
			while(aux != NULL){
				if (estado_atual == aux->estado.estado_atual && aux->estado.leitura == fita[MAX-1+cabecote]){
					fita[MAX-1+cabecote] = aux->estado.escrita;
					estado_atual = aux->estado.estado_destino;
					if(aux->estado.direcao == '<')
						cabecote--;
					else
						cabecote++;
						
					break;	
				}
				aux = aux->prox;
			}
			
			if (aux == NULL){
				printf("\nMaquina Finalizada\n");
				exit(0);
			}			
		}
	}
}

main(){	
	arq = fopen("entrada2.txt","r");
	
	if (arq == NULL){
		printf("-> Erro ao tentar ler o arquivo.\n\n");
		Ajuda();
	}else{
		// Inicia A lista
		TipoLista maquina;
		FListaVazia(&maquina);
			
		// Carrega Alfabetos
		char alfabeto[MAX],alfabeto_aux[MAX],palavra[MAX];
		Carrega_Alfabeto(alfabeto,alfabeto_aux,palavra);
		
		// Carrega Transições
		LeTransicoes(&maquina,alfabeto,alfabeto_aux);

		Mostra(maquina);
		Rotina(maquina,alfabeto,alfabeto_aux,palavra);
	}	
}
