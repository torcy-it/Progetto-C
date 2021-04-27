

#ifndef HPLATFORM_H
#define HPLATFORM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct esiti
{
    char nome[30];
    char esito[30];
    bool tag;

    struct esiti * next;

}storico;

void platform (  );


//modifica lista storico elima doppioni e aggiorna esiti
storico * read_storico_double_del( FILE * );
storico * del_double_storico( storico *  , int *, char * );
storico * find_double_node ( storico * );
void dealloc_storico ( storico * );
void file_write_storico( storico * ,  FILE * );

#endif