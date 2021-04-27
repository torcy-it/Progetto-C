#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hlogin.h"

list_users login_signin( )
{
    printf("\n\t\t******   Benvenuto     ******\n\n\n");

    list_users user;

    user.userid = 'f';

    list_users * head =  readfile_list_users ( fopen("lista_utenti.txt","r") ); 

    while( user.userid == 'f' )
    {
        head = main_login ( head, &user  );
    }

    //ho creato user affinche possa capire chi si sia loggato o regisstrato

    recursively_fw( head, fopen("lista_utenti.txt","w") );

    dealloc_list_users ( head );   

    return user;
}

list_users * add_nodes( list_users* head, list_users * user)
{

    if (head == NULL) return crea_node(user->username, user->key, user->userid); //se la list_usersa e' vuota creo un nodo

    else head->next = add_nodes(head->next, user);// Se non siamo arrivati alla fine scorro la list_usersa

    return head;
}

list_users * readfile_list_users ( FILE * fpi )
{
    list_users * nodo_t;
    char nome[30], key[30], id;

    if(!fpi) {   printf("\nErrore lettura login FILE list_usersa_utenti \n"); return NULL ;}

    //caso base
    if(fscanf(fpi,"%s %s %c",nome, key, &id) != 3) 
    {
        fclose( fpi );
        return NULL;
    }
    printf("ao??");
    nodo_t = (list_users *) malloc(sizeof(list_users));  //alloco la dimensione della struttura list_users

    strcpy(nodo_t->username,nome);

    strcpy(nodo_t->key,key);
    
    nodo_t->userid = id;

    nodo_t->next = NULL;

    nodo_t->next = readfile_list_users(fpi);

    return nodo_t; 
    

   
}

//questa e' una semplice funzione sulla creazione di un nodo di una list_usersa
//quindi volendo in futuro potrei modificarla per aggiungerci altri dati come una stringa in piu blabla
list_users * crea_node ( char s1[], char s2[] , char id)
{
    list_users * nodo = (list_users *)malloc (sizeof (list_users));

    strcpy(nodo->username,s1);
    strcpy(nodo->key,s2);

    nodo->userid = id; 

    nodo->next = NULL;

    return nodo;
}

void dealloc_list_users (list_users * head)
{
   if (!head) return; //checks if head is null 

   list_users * next = head->next; //faccio una copia del next cosi da non perdere il riferimento dopo aver deallocato

   free(head);

   dealloc_list_users(head->next);
}

//-----------------------------------------------------------------------------------------------------------------------------


//funzione madre
list_users * main_login( list_users * head , list_users * user)
{
    char choice[30];


    if( !head  )
    {

        first_timeSignin( head, user );//signin

        head = add_nodes ( head, user);//aggiungo un nodo alla list_usersa

        return head;
    }
    else
    {
        
        printf("\nVuoi registrarti o accedere ? \n\tscrivere |log| per accedere |signin| per iscriverti\n>");
        scanf("%29s",choice);
    
        change_string( choice );//read description up on the definition of the function

        if(!back_command( choice ))
        {
            user->userid = 'f';
            return head;
        }

        if (!strcmp(choice,"signin"))//entro nella fase signin
        {
            
            signin( head , user);//signin


            if(!back_command( user->username ) ) 
            {
                return head;
                user->userid = 'f';
            }        

            head = add_nodes ( head, user);//aggiungo un nodo alla list_usersa


        }
        else if( !strcmp(choice,"log"))//entro nella fase login
        {

            login(head, user );//login 

            if( !back_command( user->username ) )
            {
                user->userid = 'f';
                return head;//controllo se la parola inserita e' back
            }
            

        }
        else printf ("\n||  Inserisci |LOG| per loggare O  |SIGNIN| per registrarti   ||\n\n");
        
        
    }

    return head;
}


void first_timeSignin ( list_users * head, list_users * user )
{
    printf("\n| Benvenuto nel primo signin |\n");
    
    char key[60];
    char name[60];

    //al primo signin non dovrei fare il check
    puts("\nInserisci l'id\n>");
    scanf("%29s",name);
    
    change_string( name );//read description up on the definition of the function

    while ( !back_command( name ) )
    {
        printf("\n|| Nome Utente non disponibile, Riprova ||\n");
        
        puts("\nInserisci l'id\n>");
        scanf("%29s",name);
        
        change_string( name );//read description up on the definition of the function
    }

    strcpy( user->username ,name);
    
    user->userid = 'a';//carattere $
 
    double_password( key );//funzione per creare la password
    
    strcpy( user->key, key);

}


// devo assolutamente migliorarla
void login (  list_users * head, list_users * user )
{
    printf("\n| Benvenuto nel login |\n");

    int  tentativi = 0 ;

    bool check = false; // variabile booleana per inserire prima l'utente e successivamente la password

    while ( tentativi != 3 )// while per controllare id e passeword
    {
        if (!check )//check false so i can go for the id
        {
            string_alloc( user->username, true);//create a ID

            if( !back_command ( user->username ) )
            {
                user->userid = 'f';
                return; 
            }
        
            check = find_ID( head, user->username ); // if user exist check = true so i can continue to password
        }

        if ( check )// check = true go for the password
        {   
            string_alloc( user->key, false);//create password

            if( !back_command ( user->key ) )
            {
                user->userid = 'f';
                return; 
            }

            if ( find_Key( head, user)) 
            {
                //end function go to your account
                printf("\nlogin succ\n");                              
                
                return;             
            }
            
            tentativi++; 
            printf("\nPASSWORD ERRATA, tentativi rimasti : %d\n",3-tentativi);      
                
        }
        else printf("\nUTENTE NON TROVATO\n");// if !check allora significa che non ho trovato l'utente repeat 

    }

    //end of the function go to main_login or forgot password
    printf("\nlogin failed\n");

    strcpy(user->username,"LOGINFAILEDsus");
    
}

void signin ( list_users * head , list_users * user)
{
    printf("\n| Benvenuto nel signin |\n");
    
    string_alloc(user->username, true);

    if( !back_command ( user->username ) )
    {
        user->userid = 'f';
        return; //controllo se il nome inserito non e' back
    }

    while ( find_ID( head, user->username ) )
    {
        printf("\n|| Nome Utente gia' usato, Riprova ||\n");
        
        string_alloc(user->username, true);

        if( !back_command ( user->username ) )
        {
            user->userid = 'f';
            return; //controllo se il nome inserito non e' back
        }

    }

    double_password( user->key );//funzione per creare la password

    if(!strcmp(user->key,"back")) user->userid = 'f';

    else user->userid = 'u' ; 

}

void double_password( char key[])
{
    char * tmp = ( char * )malloc(30*  sizeof ( char ));

    string_alloc( tmp , false);

    if( !back_command ( tmp ) )
    {
        strcpy(key,"back");
        return; 
    }

    string_alloc( key, false);

    if( !back_command ( key ) )
    {
        return; 
    }


    if (!strcmp ( tmp, key) ) return;
    else
    {
        printf("\nLe password !%s! !%s! non combaciano riprova",tmp, key);
        free( tmp );
    } 

    double_password( key );

    free (tmp);

}
                                                        
//-----------------------------------------------------------------------------------------------------------------------------------------



void recursively_fw( list_users * head, FILE *fpo )  //recursively file written 
{
    if(!fpo) {   printf("\nErrore scrittura login FILE list_usersa_utenti \n"); return ;}
    
    if( !head )
    {
        fclose( fpo );
        return;
    }
    fprintf(fpo,"%s %s %c\n",head->username,head->key,head->userid);

    recursively_fw( head->next, fpo );
}


//------------------------------------------------------------------------------------------------------------------------------------------


//ho utilizzato questa funzione per non scrivere sempre il puts o printf
void string_alloc( char string[], bool binary ) //leggere i commenti di bool find_Keynid per sapere di piu su binary
{
    if( binary )
    {

        puts("\n   >Se vuoi tornare indietro inserisci il comando BACK<\nInserisci l'id\n>");
        scanf("%29s",string);
        
        change_string( string );//read description up on the definition of the function

    }
    else
    {
        puts("\nInserisci la password \n> ");
        scanf("%29s",string);
    }
}

void change_string ( char * str )//funzione madre per tolower e nospace_string
{
    to_lower( str );//Una funzione tolower ma fatta a mano, alternativa #include <ctype.h> char = tolower( char )

    no_space( str, str );//ho dovuto inserire la stessa stringa perchè il secondo paramentro della funzione e' di appoggio
}

void no_space(char *str , char * app_str ) //ho utilizzato l'aritmetica dei puntatori per copiare la app_str
{                                           //che potrebbe contenere spazi o caratteri di escape come \n 
    if ( *app_str == '\0')                  // nella str senza quei caratteri.
    {                                       
        *str ='\0';                     //------> caso base in cui app_str ha il carattere teminatore
        return ;                               
    }
    
    if( *app_str == ' ' || *app_str == '\n') no_space( str , app_str+1 ); // caso ricorsivo in cui i caratteri devo essere
    else                                                                  //saltati senza aumentare 
    {
        *str = *app_str;                    
                                                        
        no_space ( str+1 , app_str+1);                                // devo effetuare la copia
    }

}

void to_lower(char *str ) //una tolower scritta a mano
{
    if (*str == '\0') return;
        
    else if (*str >= 'A' && *str <= 'Z') *str = *str - 'A' + 'a'; 
        
    to_lower(str + 1);
}


bool back_command ( char string [])
{
    // ho fatto questo controllo cosi che l'uente una volta aver sbagliato 3 volte la pasword oppure se inserisce il comando back
    if( !strcmp( string,"loginfailedsus" ) || !strcmp( string,"back" ) ) return false;//posso mandarlo nella fase scelta login signin
    
    else return true;  //se no posso continuare tranquillamente 
}



bool find_ID ( list_users * head , char * string  ) //stringa e' una funzione anonima cosi che io possa utilizzarla per vari ccontrolli
{           
    if( !back_command( string ) ) return false; //controllo se l'utente non abbia inserito un comando

    if( head == NULL ) return false; 
                                                    
    else if ( !strcmp( head->username , string ) ) return true;// se trovo l'utente return true
    
    else find_ID( head->next, string );                                                                               

}  

bool find_Key( list_users * head, list_users * user )
{
    if( head == NULL ) return false;// list_usersa vuota oppure non ho trovato l'utente inserito con la passwoprd inserita 

    else if ( !strcmp( head->username, user->username ) && !strcmp( head->key, user->key ) ) //trovo utente e password inserita prev
    {
        user->userid = head->userid; //dopo che password e utente == true posso inserire di che tipologia e' l'utente (admin o paziente)

        return true;
    }
    else find_Key( head->next, user );        

}