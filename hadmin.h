
#ifndef HADMIN_H
#define HADMIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct days
{
    int mattina;//2
    int pome;//2
    int sera;//2
    int time; // time = giorno pomeriggio e sera infatti maxtime = 3

    int meeting; // meeting per il bot da modificare per una migliore lettura e migliorare codice

}daytime;

daytime day;


typedef struct richieste
{
    int sintomi;
    char nome[30];

}heap;


typedef struct appuntamenti
{
    char nome[30];
    int orario;

    struct appuntamenti *next;

}lista;


// gli input richieste vengono dati con un semplice ffscanf
//gli appuntamenti vengono effetuati solo quando il programma viene chiuso
//lutente quando logga, logga al suo ultimo salvataggio

void admin ( void );

//file work
void file_read( void );
void file_append_storico( void );
void file_write_appuntamenti ( lista *, FILE * );
void file_write_richieste ( heap *, int );


//sostituisci appuntamenti o richieste
lista * change_meeting_list ( lista *  , lista * , lista * , bool );
lista * same_change (lista *  , heap *  , int  , int  , lista * , lista * );
lista * crea_nome_change( lista * ,heap * , int * , int * );
lista * change_meeting(lista * , heap * , int *  );


//aggiungi appuntamenti
bool find_user_richieste( heap * , int  , int * , char * );
lista * add_meeting( lista * , heap * , int *  );


//elimina appuntamenti
lista * delete_meeting( lista * , heap * , int *  );
bool find_user_meeting( lista * , char * );
lista * delete_nodo ( char *  , lista *  );
void add_user_inDynamicA ( heap * , int  *, char * );

//struct appuntamenti
lista * crea_nodo ( lista * );
lista * add_node ( lista *, lista * );
void print_L ( lista *);
lista * crea_user( heap * , lista * , int *  );
void deallocateList ( lista *);


//bot appuntamenti
lista * bot_appuntamenti( lista *, heap *, int* );
lista * set_appuntamenti ( lista *, heap * , int *);
int lenght_lista( lista * );

// daytime
void daytime_file( );
char * what_timeis( int );
void add_orario ( int  );

//richieste tamponi  
heap * build_dynamicA(  );
void free_dynamicA( heap * , int );

//heap tree
void heap_sort ( heap * , int );
void heapify( heap * , int , int  );
void swap( heap * , heap *);
void print_H( heap *, int );
void deleteRoot(heap *  ,lista *, int *, int  );

#endif