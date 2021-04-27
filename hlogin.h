#ifndef HLOGIN_H
#define HLOGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct users
{
    char userid; //userid per capire se e' un paziente o admin----- admin a, paziente u, non valido f
    char username[30];
    char key[30];

    struct users * next;

}list_users;

// go to the definition to understand functions


list_users login_signin( );

//file
void recursively_fw( list_users *, FILE *);
void write_file ( list_users *);


// list_userse
list_users * readfile_list_users ( FILE * );
list_users * crea_node ( char[], char[], char );
list_users * add_nodes( list_users * , list_users* );
void dealloc_list_users ( list_users * );

//funzioni per migliorare codice 
void string_alloc( char [], bool);
void change_string ( char *);
void no_space ( char *, char *);
void to_lower( char * ); 
bool back_command( char []);



// login e signin
list_users * main_login( list_users * , list_users *);
bool find_ID ( list_users *  , char * ); 
bool find_Key ( list_users * , list_users *);
void first_timeSignin (list_users *, list_users * );
void  login ( list_users *, list_users *);
void signin( list_users * , list_users*);
void double_password( char []);



#endif