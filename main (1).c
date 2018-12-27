#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <locale.h>
#include <string.h>
#define MAX 100

typedef struct algarismo{
	int valor; //Indica o valor do bit (0 ou 1).
	int posicao; //Indica a que posição, de 0 a n - 1, sendo n o número de bits na sequência, o bit em questão pertence.
	struct algarismo* proximo; //Aponta para o próximo bit do número. 
} ALGARISMO;

void cabecalho(){  //Só por motivos de estética.
	clear();	
	printw("--------------------------------------------------------------------------------");
	printw("      APENAS UMA CALCULADORA INFINITA");
	printw("\n--------------------------------------------------------------------------------\n");
	refresh();
	
	return;
}

void inicializa_lista(ALGARISMO** primeiro){  //Inicializa uma lista vazia, sem nenhum nó.
	*primeiro = NULL;
	
	return;
}

int aloca_bit(ALGARISMO** primeiro, int quantos_bits){
    ALGARISMO* novo;
	int i;
	int bit;
	
	//Alocando a lista:
	for(i = 0; i < quantos_bits; i++){  //Faz isso até que o número de bits alocados seja igual ao número de bits requisitados
	    novo = (ALGARISMO*)malloc(sizeof(ALGARISMO));
	
	    if(novo == NULL){
		    cabecalho();
		    printw("\n##Erro durante a alocação!\n");
		    refresh();
		    getch();
		
		    return 0;  //Retorna 0 se houver algum erro na alocação de memória.
	    }
	
	    //Definindo a posição do bit anteriormente alocado:
		
		novo->posicao = ((quantos_bits - 1) - i);  //Houve edição
				
		//Articulando a lista:
	
	    novo->proximo = *primeiro;
	    *primeiro = novo;
	}
	
	return 1;  //Retorna 1 se a operação for bem-sucedida.
}

void preenche_sequencia(ALGARISMO** primeiro, int quantos_bits){  
	char bit_c[MAX];
	int bit_i[MAX];
	int i;
	ALGARISMO* percorre;

	percorre = *primeiro;
	
	cabecalho();
	printw("\n>>Insira a sequência de bits:\n");
	refresh();
	getnstr(bit_c, MAX);  //Primeiramente, admite-se a sequência de bits como uma string
	
	for(i = 0; i < quantos_bits; i++){
		bit_i[i] = (bit_c[i] - '0');   //Aí, converte-se cada elemento da string no valor correspondente inteiro (Podem existir erros) e insere-se em um vetor paralelo.
	}
		
	i = 0;
	
	while((percorre != NULL) && (i < quantos_bits)){
		percorre->valor = bit_i[i];	
		i++;
		
		percorre = percorre->proximo;
	}
	
	return;
}

void soma(ALGARISMO** primeiro, int maior_tamanho, int* resultado, int tamanho_lista){  //***WIP
	ALGARISMO* percorre;
	ALGARISMO* percorre_aux;
	int qual_posicao; //Indica o número de vezes que deve-se percorrer a lista!
	int i;
	int vaium;
	char* new_result;
	int tamanho;
	
	percorre = *primeiro;
	qual_posicao = 0;
	vaium = 0;
		
	//Preenchendo o vetor "Resultado" com zeros:  //AQUI QUE EU PAREI!!!!!!!!!!!!
	
	for(i = 0; i <= maior_tamanho; i++){
		resultado[i] = 0;
	}
	
	//Sequencias de operações da soma binária:
	
	while(qual_posicao <= maior_tamanho){  //Os números que são permitidos na alocação são sempre um ou zero!
	    while(percorre != NULL){
		    if(percorre->posicao == qual_posicao){ //qual posicao vai variar muito!!
			    resultado[qual_posicao] = (resultado[qual_posicao] + percorre->valor + vaium);  //vaium eh ed
				vaium = 0;  //Pois só precisamos que o "vaium" seja somado uma vez por bit.
		    }
		
		    percorre = percorre->proximo;
	    }
		if(resultado[qual_posicao] == 2){
			resultado[qual_posicao] = 0;
			vaium = 1;
		}
		if(resultado[qual_posicao] == 3){
			resultado[qual_posicao] = 1;
			vaium = 1;
		}
		if((resultado[maior_tamanho] == 0) && (vaium == 1)){  //Algo de errado não está certo aqui...
			resultado[maior_tamanho] = 1;
		}
		else{
			resultado[maior_tamanho] = 0;
		}
		
		qual_posicao++;
		percorre = *primeiro;
    }
	
	//Removendo o zero do último elemento(se houver) e passando o resultado para uma string (facilita a impressão):
	
	if(resultado[maior_tamanho] == 0){
	    new_result = (char*)malloc(sizeof(int)*(maior_tamanho));
		
		for(i = 0; i < maior_tamanho; i++){			
			new_result[i] = (resultado[i] + '0');
		}
		
		free(resultado);
		tamanho = i;
	}
	else{
	    new_result = (char*)malloc(sizeof(int)*(maior_tamanho + 1));		
		
		for(i = 0; i <= maior_tamanho; i++){
			new_result[i] = (resultado[i] + '0');
		}
		
		free(resultado);
	    tamanho = (maior_tamanho + 1);
	}
	
	//Imprime o resultado final:
	
	cabecalho();
	printw("%d %s", tamanho, new_result);
	refresh();
	getch();
	
	return;
}

int main(){
	//DECLARAÇÕES INICIAIS:
	ALGARISMO* primeiro;  //Aponta para o primeiro elemento 
	int quantos_bits; //Indica quantos bits existem na sequência atual.
	int quantas_sequencias; //Indica quantas sequências o usuário deseja somar. 
	int i;
	int j;
	int flag;
	int* guarda_tamanho; //Guarda dinamicamente o tamanho de cada sequencia.
	int bit_max; //Guarda o número máximo de bits que uma soma de binários pode resultar.
	int* resultado;  //Vetor dinâmico que guarda o resultado da soma.
	int o_maior; //Armazena o maior elemento de um certo vetor.
	int tamanho_lista;
	
	//ATRIBUIÇÕES INICIAIS:
	
	tamanho_lista = 0;
	
	//FUNÇÕES INICIAIS:
	
	setlocale(LC_ALL, "Portuguese");
	initscr();
	cbreak();
	inicializa_lista(&primeiro);
	
	//SAUDAÇÃO INICIAL:
	
	cabecalho();
	printw("\n~Pressione qualquer tecla para começar~\n");
	refresh();
	getch();
	
	//O PROGRAMA EM SI:
	
	
	//PARTE 1 - colher informações básicas do usuário e, então, realizar a alocação:
	cabecalho();
	printw("\n>>Digite o número de sequências que deseja somar:\n");
	refresh();
	scanw("%d", &quantas_sequencias);
	
	guarda_tamanho = (int*)malloc(sizeof(int)*quantas_sequencias);
	
	for(i = 0; i < quantas_sequencias; i++){
		setbuf(stdin, NULL);  //Para limpar o buffer.
		
		cabecalho();
		printw("\n>>Digite o número de bits da %dª sequência:\n", (i+1));
		refresh();
		scanw("%d", &quantos_bits);
		
		tamanho_lista = tamanho_lista + quantos_bits;
		
		guarda_tamanho[i] = quantos_bits;
		
		flag = aloca_bit(&primeiro, quantos_bits);
			
        preenche_sequencia(&primeiro, quantos_bits);
		
		if(flag == 0){  //Ocorreu erro de alocação de memória.
			return 0;
		}
	}
	
	
	//PARTE 2 - Realizar as operações:
	
	//Achar o maior elemento do vetor que guarda os tamanhos das sequências:
		
	o_maior = guarda_tamanho[0];
	
	for (i = 1; i < quantas_sequencias; i++){
	    if (o_maior < guarda_tamanho[i]){
		    o_maior = guarda_tamanho[i];
	    }
	}
	
	bit_max = (o_maior + 1);
	
	//Cria o vetor "resultado":
	
	resultado = (int*)malloc(sizeof(int)*bit_max);
	
	//Realiza a soma e exibe o resultado final:
	
	soma(&primeiro, o_maior, resultado, tamanho_lista);  //WIP
	
	//Desaloca tudo:
	
	
	
	//SAUDAÇÃO FINAL:
	
	cabecalho();
	printw("\n>>FEITO! Obrigado por usar :D\n");
	getch();
	refresh();
	
	endwin();
	
	return 0;	
}
