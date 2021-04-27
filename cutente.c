
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hutente.h"



void ospite( char * nome ) 
{

    //aggiungere clear_stringa
    //aggiunger dealloc lista
    
    printf("\n\n\t||Benvenuto nell'aria Ospite||\n");

    incontro * head = read_file_appuntamenti( fopen("appuntamenti.txt","r"));

    char choice[20];

    while ( strcmp(choice,"exit") )
    {

        printf("\n\nInserisci |see| per vedere gli appuntamenti fissati\n\t |set| per richiedere un aappuntamento\n\t"
                " |storico| per accedere allo storico dei tuoi tamponi\n\t |exit| per disconnetterti\n>");
        scanf("%29s",choice);

        if(!strcmp(choice,"see")) head = see_appuntamenti ( head, nome);

        if(!strcmp(choice,"set") )
        {
            if( find_richiesta_file( nome , fopen("richieste.txt","r")) ) set_richiesta ( nome );
            else printf("\nHai gia' effetuato una richiesta aspetta che Il medico ti assegni un appuntamento\n");
        }

        if(!strcmp(choice,"storico")) 
        {
            if( !cliente_storico(nome, fopen("storico.txt","r")) ) printf("\nNon hai ancora effetuato nessun tampone\n"); 
        }

    }


    file_write_appuntamentis ( head , fopen("appuntamenti.txt","w") ); 

}

incontro * read_file_appuntamenti ( FILE * fp )
{
    if(!fp) {   printf("\nErrore scrittura ospite FILE appuntamenti\n"); return NULL;}

    incontro * nodo_t;
    char nome[30], orario[30]; // valori temp

    //caso base
    if(fscanf(fp,"%s %s",nome,orario) != 2)
    {
        fclose ( fp );
        return NULL;
    }
    
    
    nodo_t = (incontro *) malloc(sizeof(incontro));

    strcpy(nodo_t->nome,nome);

    strcpy(nodo_t->orario,orario);

    nodo_t->next = NULL;

    // caso ricorsivo
    nodo_t->next= read_file_appuntamenti(fp);

    return nodo_t;
    

}

incontro * see_appuntamenti( incontro * testa , char * nome)
{
    char choice[4];
    bool find = false;

    if( !testa ) return NULL;

    if(!strcmp( testa->nome, nome) && testa->next == NULL && !find)
    {
        printf("\nHai un appuntamento settato per l %s di domani", testa->orario );

        while( 1 )
        {
            printf("\nVuoi elimninarlo? |si| |no|\n>" );
            scanf("%3s",choice );

            if(!strcmp(choice,"no")) return testa;

            if(!strcmp(choice,"si")) return NULL;
        }
        
    }

    if ( !strcmp( testa->nome, nome) && !find )
    {
        printf("\nHai un appuntamento settato per l %s di domani", testa->orario );
        
        //add_orario ( testa->orario ); //aggiorno il day.time dell'utente eliminato
        while( !find )
        {
            printf("\nVuoi elimninarlo? |si| |no|\n>" );
            scanf("%3s",choice );

            if(!strcmp(choice,"no")) return testa;

            if(!strcmp(choice,"si")) 
            {
                incontro * tmp = testa; 
                testa = testa->next;
                free( tmp );
                find = true;
            }
        }
        
        
    }
    
    
    incontro * nodo_pre = testa; 
    incontro * nodo_curr = testa->next; 


    while( nodo_curr && !find ) 
    {
        if (  !strcmp( nodo_curr->nome, nome) )
        {
            printf("\nHai un appuntamento settato per l %s di domani", testa->orario );
            
            //add_orario ( testa->orario ); //aggiorno il day.time dell'utente eliminato
            while( !find )
            {
                printf("\nVuoi elimninarlo? |si| |no|\n>" );
                scanf("%3s",choice );

                if(!strcmp(choice,"no")) return testa;

                if(!strcmp(choice,"si")) 
                {
                    incontro * tmp = nodo_curr;
                    nodo_pre->next = nodo_curr->next;
                    free ( tmp );
                    find = true;
                }
            }
            
        }
    
        nodo_pre = nodo_curr;
        nodo_curr = nodo_curr->next;
    }   

    if(!find) printf("\n\n\tNon hai appuntamenti\n\n");

    return testa;
}

char * what_sintomiis( int sintomo )
{
    if(sintomo == 1) return "Hai avuto o hai la febbre?";
    else if ( sintomo == 2) return "Ti senti affaticato quando respiri?";
    else if ( sintomo == 3 ) return "Ultimamente tossisci spesso?";
    else return "Hai avuto o hai qualche sintomo di stanchezza?";
}


void set_richiesta ( char * nome )
{
    float count = 0;
    int sintomo = 1;
    char choice[6];

    printf("\n\nCiao, prima di richedere un appuntamento, dovro' farti alcune domande\n"
            "Puoi rispondermi tranquillamente con un |si| |no| |forse|\n\n");

    while( sintomo < 5 )
    {
        printf("\n%s\n>",what_sintomiis(sintomo));
        scanf("%5s",choice);

        if(!strcmp(choice,"si"))
        {
            count = count + 1;
            sintomo++;
        }
        else if(!strcmp(choice,"forse")) 
        {
            count = count + 0.5;
            sintomo++;
        }
        else if(!strcmp(choice,"no")) sintomo ++;
        else printf("\nDevi rispondermi con un |si| |no| |forse|\n\n");

    }

    
    FILE * fp = fopen("richieste.txt","a");

    fprintf(fp,"\n%s %d",nome,(int)count);

    fclose( fp );
}

bool cliente_storico( char * user_id , FILE * fp )
{
    if(!fp) {   printf("\nErrore lettura ospite FILE storico\n"); return true;}

    char nome[30], esito[30];

    if( fscanf(fp,"%s %s",nome, esito) != 2) return false;

    if(!strcmp(user_id,nome))
    {
        printf("\n\nSei risultato %s al tampone\n",esito);
        return true;
    }

    cliente_storico(  user_id , fp );
}


void file_write_appuntamentis ( incontro * head , FILE* fp)
{
    if(!fp) {   printf("\nErrore lettura ospite FILE storico\n"); return;}


    if( !head ) 
    {
        fclose( fp );
        return; 
    }
    fprintf(fp,"%s %s\n",head->nome, head->orario);


    file_write_appuntamentis ( head->next , fp);

}

bool find_richiesta_file ( char * user_id, FILE *fp )
{
    if(!fp) {   printf("\nErrore lettura ospite FILE storico\n"); return true;}

    char nome[30];
    int val;

    if( fscanf(fp,"%s %d",nome, &val) != 2) 
    {
        fclose( fp );
        return true;
    }
    if( !strcmp(user_id,nome) ) return false;

    find_richiesta_file(  user_id , fp );    
}