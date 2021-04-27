

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "hplatform.h"

void platform (  )
{
    storico * head = read_storico_double_del( fopen("storico.txt","r") ); //leggo tutto lo storico
    head = find_double_node ( head ); // me lo salvo in una lista per poi modificarlo e metterlo di nuovo in storico.txt
    
    if(!head ) 
    {
        printf("\n\n\tSTORICO vuoto\n\n");
    }
    else
    {
        printf("\n\tSTORICO\t nome \t esito\n\n");
        file_write_storico(head, fopen("storico.txt","w"));
    }
    
    
    dealloc_storico( head );
    
}

void file_write_storico( storico * head,  FILE * fp)
{
    if(!fp) {   printf("\nErrore lettura ospite FILE storico\n"); return;}


    if( !head ) 
    {
        fclose( fp );
        return; 
    }

    if( !strcmp( head->esito,"positivo") ) printf("\n---| %20s %s\n",head->nome,head->esito);

    fprintf(fp,"%s %s\n",head->nome, head->esito);


    file_write_storico ( head->next , fp);
}


void dealloc_storico ( storico *  head )
{
   if (!head) return; //checks if head is null 

   storico * next = head->next; //faccio una copia del next cosi da non perdere il riferimento dopo aver deallocato

   free(head);

   dealloc_storico(head->next);   
}

storico * read_storico_double_del( FILE * fp )
{
    if(!fp) {   printf("\nErrore scrittura ospite FILE appuntamenti\n"); return NULL;}

    storico * nodo_t;
    char nome[30], esito[30]; // valori temp

    //caso base
    if(fscanf(fp,"%s %s",nome,esito) != 2) 
    {
        fclose( fp );
        return NULL;
    }
     
    
    nodo_t = (storico *) malloc(sizeof(storico));

    strcpy(nodo_t->nome,nome);

    strcpy(nodo_t->esito,esito);

    nodo_t->tag = true;

    nodo_t->next = NULL;
    
    // caso ricorsivo
    nodo_t->next = read_storico_double_del(fp);

    return nodo_t;
    
}


storico * find_double_node ( storico * head)
{
    
    storico * nodo_t;

    //caso base
    if( !head ) return NULL;
    else // caso ricorsivo
    {
        int esito = 0;
        nodo_t = (storico *) malloc(sizeof(storico));

        while( !head->tag ) 
        {
            if( head->next && head ) head = head->next;
            else return NULL;
        }

        strcpy(nodo_t->nome,head->nome);

        head = del_double_storico( head , &esito,  head->nome );

        if( esito == 1 )strcpy(nodo_t->esito,"positivo");
        else strcpy(nodo_t->esito,"negativo");
        
        nodo_t->next = NULL;
        
        nodo_t->next = find_double_node(head->next);
        
        return nodo_t;
    }
}


storico * del_double_storico( storico * head , int * esito, char * nome)
{
    if( !head ) return NULL;

    if(!strcmp(head->nome, nome))
    {
        int esiti = 0;
        head->tag = false;
        
        if (!strcmp(head->esito,"negativo")) 
        {
            * esito = 0;
        }
        else { *esito = 1; }

    }

    head->next = del_double_storico( head->next ,esito, nome);

    return head;
}