
#ifndef HUTENTE_H
#define HUTENTE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


typedef struct incontri
{
    char orario[30];
    char nome[30];

    struct incontri * next;

}incontro;

void ospite ( char * );


incontro * see_appuntamenti( incontro *  , char * );
incontro * read_file_appuntamenti ( FILE *  );



void file_write_appuntamentis ( incontro * head , FILE* fp);
bool find_richiesta_file ( char * , FILE * );
bool cliente_storico( char *, FILE *);


void set_richiesta ( char * );
char * what_sintomiis( int );

#endif