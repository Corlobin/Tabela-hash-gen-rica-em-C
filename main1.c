/*
* Tabela Hash 
* Autor: Antonio Ricardo
**/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
* Estrutura de uma pessoa
*/
typedef struct pessoa {
	unsigned rg;
	unsigned cpf;	
} Pessoa;

Pessoa* inicializa_pessoa() {
	return NULL;
}
Pessoa* cria_pessoa(unsigned rg, unsigned cpf) {	
	Pessoa* p = (Pessoa*) malloc(sizeof(Pessoa));
	p->rg = rg;
	p->cpf = cpf;
	return p;
}
// Minha funcao que compara duas pessoas
int compara_pessoas(Pessoa* p1, Pessoa* p2) {
	printf("\tEntrou no compara_pessoas\n");
	printf("\tRG da pessoa 1: %d RG da pessoa 2: %d\n", p1->rg, p2->rg);
	return (p1->rg - p2->rg);
}

/*
* Estrutura da lista
*/ 
typedef struct lista {
    void* info; 
    struct lista *prox; // proximo
} Listagen;


Listagen* insere_lista(Listagen* p, void* nova) {
	Listagen* lst = (Listagen*) malloc(sizeof(Listagen));
    lst->info = nova;
    lst->prox = p;
    return lst;
}

void libera_lista(Listagen* p) { // Libera a lsita da memoria
    Listagen* l = p;
    while(l->prox != NULL) {
        Listagen* t = l->prox;
        free(l);
        l = t;
    }
}

Listagen* inicializa_lista(void) { // Inicialzia com null
    return NULL;
}

/*
* Estrutura da tabela Hash
*/ 

typedef struct hash {
	int limite;
	int (*compara)(); 
	Listagen **tabela;
} Hash;

Hash* inicializa(int limite, int (*comparador)() )
{
	int i;
	Hash* h = malloc(sizeof(Hash));
	h->limite = limite;
	h->compara = comparador;
	h->tabela = (Listagen**) malloc(sizeof(Listagen*) * limite);
	for ( i = 0 ; i < limite ; i++)
		h->tabela[i] = NULL;
	return h;
}


int hash(Hash* h, char* obj) {
	int k,i,tam;
    tam = strlen(obj);
    for( k = i = 0 ; i < tam ; i++)
      k += obj[i];
    return k % h->limite;
}

void insere(Hash* h, char* chave, void* estrutura) {
	int valor = hash(h, chave);
	printf("Inserindo um novo valor na tabela Hash ( chave: %s ):\n\tValor da hash: %d\n", chave, valor);
	if(h->tabela[valor] != NULL) {
		printf("\tColisao ao inserir, chave %d\n", valor);
	} else {
		h->tabela[valor] = inicializa_lista();
	}
	h->tabela[valor] = insere_lista(h->tabela[valor], estrutura);
}


void* busca(Hash* h, char* chave, void* estrutura) {
	int valor = hash(h, chave);
	printf("Buscando um valor na tabela hash (chave: %s): %d\n", chave, valor);
	if(h->tabela[valor] != NULL) {
		printf("\tExiste uma lista com esse valor na tabela, verificando..\n");
		Listagen* aux;
		for(aux = h->tabela[valor]; aux != NULL; aux = aux->prox){
			if(h->compara(aux->info, estrutura) == 0) {
				printf("\tEncontrado\n");
				return aux->info;
			}
		}
		
	} else {
		printf("\tOcorreu um erro: o indice do valor e nulo.\n");
	}
	return NULL;
	
}

void libera(Hash *h)
{
	int i;
	for ( i = 0 ; i < h->limite; i++) {
		if (h->tabela[i] != NULL) {
			Listagen* l = h->tabela[i];
			libera_lista(l);
            //free(h->tabela[i]);
		}
	}
	free(h->tabela);
}





int compara_inteiros(int *a, int *b) {
	printf("\tEntrou na funcao de compara inteiros n1: %d n2: %d\n", *a, *b);	
	return (*a - *b);
}

// Programa principal
int main(int argc, char *argv[])
{	
    void* objeto;	
	int valor1 = 156;
	int valor2 = 188;	
	Hash* willy;
	Pessoa* p_ricardo;
	Pessoa* p_antonio;
	Hash* deInteiros;
	
	p_ricardo = inicializa_pessoa();
	p_antonio = inicializa_pessoa(); 
	
	p_ricardo = cria_pessoa(304050, 13640043);
	p_antonio = cria_pessoa(306050, 13740012);
	
	willy = inicializa(100, &compara_pessoas);
	
	insere(willy, "RicardoChave", p_antonio);
	insere(willy, "RicardoChave", p_ricardo);
	
	objeto = busca(willy, "RicardoChave", p_antonio);
	
	if(objeto == NULL) {
		printf("Objeto nao encontrado!\n");
	} else {
		Pessoa* pessoa = (Pessoa*) objeto;
		printf("RG com cast: %d\n", pessoa->rg);
		printf("CPF com cast: %d\n", pessoa->cpf);
	}
		
	printf(" \n");
	printf("Criando uma tabela hash agora de inteiros..\n");
	printf(" \n");
	deInteiros = inicializa(100, compara_inteiros);
	insere(deInteiros, "Inteiro", (void*)(&valor1));
	insere(deInteiros, "Inteiro", (void*)(&valor2));
	objeto = busca(deInteiros, "Inteiro", (void*)(&valor1));
	if( objeto == NULL ) {
		printf("Objeto encontrado\n");
	} else {
		int* int_ = (int*)objeto;
		printf("Inteiro: %d\n", *int_);
	}
	objeto = busca(deInteiros, "Inteiro", (void*)(&valor1));
	if(objeto == NULL)
		printf("Ele nao existe na tabela!");
	
		
	
	libera(willy);
	free(willy);
	libera(deInteiros);
	free(deInteiros);	
	return 0;
}
