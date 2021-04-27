#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "hadmin.h"


void admin( )
{
    
    char choice[30] = "null";
    int dim = 0;
    lista * head = NULL;

    file_append_storico ( );

    daytime_file( );

    printf("\n\n\t||Benvenuto nell'aria Admin||\n");

    heap * dynamicA = build_dynamicA( &dim );
    heap_sort(dynamicA , dim);

    while ( strcmp(choice,"exit") )
    {

        if( dim <= 0 ) printf("\n\n\tLista richieste Vuota\n");
        else
        {
            printf("\n\n\tLista richieste\n\tnome\t  sintomi");
            print_H( dynamicA , dim - 1);
        }
        if(!head) printf("\n\n\tLista appuntamenti Vuota\n");
        else
        {
            printf("\n\n\tLista appuntamenti\n\tnome\t  orario");
            print_L( head );
        }



        printf("\n\nInserisci |add| per aggiungere nuovi appuntamenti\n\t |delete| per eliminare appuntamenti\n\t"
                " |change| per sostituire appuntamenti\n\t |bot| per assegnare automaticamente gli appunti\n\t"
                " |storico| per accedere allo storico dei tamponi\n\t |exit| per disconnetterti\n>");
        scanf("%29s",choice);

        if(!strcmp(choice,"add")) head = add_meeting( head, dynamicA, &dim);

        if(!strcmp(choice,"delete")) head = delete_meeting( head, dynamicA, &dim);

        if(!strcmp(choice,"change")) head = change_meeting(head, dynamicA, &dim);

        if(!strcmp(choice,"bot")) head = bot_appuntamenti( head, dynamicA, &dim);

        if(!strcmp(choice,"storico")) file_read( );
    }

    if( dynamicA ) file_write_richieste ( dynamicA , dim-1 );

    if ( head ) file_write_appuntamenti ( head , fopen("appuntamenti.txt","w"));

    free(dynamicA);

    deallocateList( head );

    printf("ao?");

}



//--------------------------------------------------------------------------------

heap * build_dynamicA( int *index)
{

    FILE * fp = fopen( "richieste.txt" , "r");

    char stringa[30];
    int data;         //data come info non come orario

    heap * heap_tree = NULL;

    if(fscanf(fp,"%s %d",stringa,&data) == 2) //controllo se ci siano richieste
    {
        heap_tree = (heap *) malloc (40 *sizeof( heap ));

        heap_tree[0].sintomi = data;
        strcpy(heap_tree[0].nome,stringa);

        *index = *index +1;
        //printf("\n___/|%10s   %d %d",heap_tree[0].nome, heap_tree[0].sintomi, *index);
        
    }else return NULL;


    for( ; fscanf(fp,"%s %d",stringa,&data) == 2 ; *index = *index +1)
    {
        if( *index == 40 ) break;
        //heap_tree = (heap*)realloc(heap_tree, (*index + 1 ) * sizeof(heap));
       
        heap_tree[*index].sintomi = data;
        strcpy(heap_tree[*index].nome,stringa);

        //printf("\n___/|%10s   %d %d",heap_tree[*index].nome, heap_tree[*index].sintomi,*index);
    }

    fclose(fp);

    return heap_tree;
}

void deallocateList(lista * head) 
{

    if( !head ) return;

    lista * temp = head;

    deallocateList( head->next );
    
    free( temp );
}


//------------------------------------------------------------------------------


void swap(heap * a, heap* b)
{
    heap temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(heap * tree, int n, int i)
{
    int largest = i; // Initialize largest as root 
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2
 
    // If left child is larger than root
    if (l < n && tree[l].sintomi > tree[largest].sintomi)
        largest = l;
 
    // If right child is larger than largest so far
    if (r < n && tree[r].sintomi > tree[largest].sintomi)
        largest = r;
 
    // If largest is not root
    if (largest != i) {
        swap(&tree[i], &tree[largest]);
 
        // Recursively heapify the affected sub-tree
        heapify(tree, n, largest);
    }
}

void heap_sort(heap * heap_tree, int n)
{
    int i;
    for (i = (n / 2) -1 ; i >= 0; i--)
        heapify(heap_tree, n, i);

    for (i = n - 1; i >= 0; i--) 
    {
        swap(&heap_tree[0], &heap_tree[i]); //Move the largest element at root to the end

        heapify(heap_tree, i, 0); //Apply heapify to reduced heap
    }

}

void print_H ( heap * heap_tree , int dim)
{
    if( dim < 0) return;

    print_H ( heap_tree, dim-1 );

    printf("\n___/|%10s     %d",heap_tree[dim].nome, heap_tree[dim].sintomi);
     
}

//-------------------------------------------------------------------------------

void daytime_file( )
{
    FILE * fp = fopen("daytime.txt","r");
    
    fscanf(fp,"%d %d %d %d %d",&day.mattina, &day.pome, &day.sera,&day.time,&day.meeting);

    fclose( fp );
}

char * what_timeis( int daytime)
{

    if(daytime == 1) return "mattina";
    else if ( daytime == 2) return "pomeriggio";
    else return "sera";

}

bool check_orario( char * choice , int * orario )
{
    
    if(!strcmp(choice,"mattina") || *orario == 1)
    {
        //faccio questo controllo come a tutte le altre fasi orarie
        if(day.mattina == 0)//affinche l'utente non possa inserire piu di due appuntamenti
        {
            printf("\nNon posso inserire appuntamenti in questa fase oraria :P\n");
            *orario = -1; //return -1 e true cosi che nella funzione che ha chiamato
            return false; // check orario posso sapere se sono stati gia assegnati 2 meeting
        }

        *orario = 1;//imposto orario a 1 per identificare che e' mattina e cosi via
        day.mattina--;// diminuisco le disponibilita di meeting nella fase scelta
        if( !day.mattina )//rifaccio un cotrollo qui per assicurarmi di diminuire le fasi        
        {   
            day.meeting = 2;//devo aggiornarlo per il bot
            day.time--;
        } 

        return true;
    }
    else if(!strcmp(choice,"pomeriggio") || *orario == 2 )
    {

        if(day.pome == 0)
        {
            printf("\nNon posso inserire appuntamenti in questa fase oraria :P\n");
            *orario = -1; 
            return false; 
        }

        *orario = 2;
        day.pome--;
        if( !day.pome )
        {   
            day.meeting = 2;//devo aggiornarlo per il bot
            day.time--;
        }
        return true;
    }
    else if(!strcmp(choice,"sera") || *orario == 3 )
    {

        if(day.sera == 0)
        {
            printf("\nNon posso inserire appuntamenti in questa fase oraria :P\n");
            *orario = -1; 
            return false; 
        }

        *orario = 3;
        day.sera--;
        if( !day.sera )        
        {   
            day.meeting = 2;//devo aggiornarlo per il bot
            day.time--;
        }

        return true;
    }
    else 
    {
        printf("\n||INSERISCI 'POMERIGGIO' 'SERA' O 'MATTINA'||\n");

        return false;
    }

}

void add_orario ( int orario )
{
    if(orario == 1) {   day.mattina++ ; day.time++; }
    else if ( orario == 2) {   day.pome++ ; day.time++; }
    else if ( orario == 3) {   day.sera++ ; day.time++; }
    
}
//------------------------------------------------------------------------------------


//gli appuntamenti vengono fatti per il giorno dopo
//passato un giorno non e' piu possibile assegnare appuntamenti 
lista * bot_appuntamenti ( lista * head , heap * dynamicA , int *dim)
{

    int dimension = *dim;
    int lenght_list = lenght_lista( head );

    if( lenght_list == 6 )
    {
        printf("\n\nMi dispiace non posso asseganre appuntamenti, hai occupato tutti i posti disponibili"
                "\nTorna domani oppure modifica i tuoi appuntamenti nella sezione |change| o |delete|\n\n");
        system("pause");
        return head;
    }

    while( lenght_list < 6 )
    {
        if( dimension <= 0 )
        {
            printf("\n\nNon hai richieste --- Potresti assegnare gli appuntamenti manualmente , io non posso esserti di aiuto.\n\n");
            
            *dim = dimension;
            return head;
        }

        head = set_appuntamenti ( head , dynamicA, &dimension );

        lenght_list++;

        *dim = dimension;

    }

    return head;
}

lista * set_appuntamenti( lista * head, heap * heap_tree, int * dim )
{
    
    lista * nodo = (lista *) malloc ( sizeof ( lista ));

    deleteRoot( heap_tree, nodo, dim , 0); //cancello le root dell ' heap tree

    head = add_node ( head , nodo);

    return head;
}

//mi dissocio da questa funzione ma e' lunica cosa che ho pensato al momento forse la riscriverò
int lenght_lista ( lista * head )
{
    int val = 0; 

    if( !head ) return 0;

    val = lenght_lista( head->next );

    return ( val +1 );
}

void deleteRoot(heap * heap_tree , lista * nodo ,int * dim, int pos)
{
    // Get the last element
    heap * tmp =  (heap_tree+(*dim - 1));

    //save the first element for the list meeting
    if( nodo ) //ho messo questo controllo cosi che io possa deallocare array of struct per il bot
    {
        int orario;
        //se trovate nodo = NULL nelle nella chiamata di questa fuinzione e perche' non ho bisogno di deallcoare l'array
        strcpy(nodo->nome, heap_tree[pos].nome);

        if( day.mattina != 0 ) check_orario ( "mattina", &orario );//aggiorno l'orario per il bot
        else if( day.pome != 0 ) check_orario ( "pomeriggio", &orario );
        else if ( day.sera != 0) check_orario ( "sera", &orario );
        
        
        nodo->orario = orario;

        nodo->next = NULL;
    }
    
    //printf("\n|%s|--->|%s|\n", heap_tree[0].nome,nodo->nome);
    // Replace root with first element
    *( heap_tree + pos ) = *tmp;

    // Decrease size of heap by 1
    *dim = *dim - 1;

    //realloc array of stru
    //heap_tree = (heap*)realloc(heap_tree, (*dim ) * sizeof(heap));

    
    // heapify the root node
    heap_sort(heap_tree,*dim);

    free( tmp );
    //free(  );
    
}    

void file_write_appuntamenti ( lista * head , FILE * fp)
{
    if(!fp) {   printf("\nErrore scrittura FILE appuntamenti\n"); return;}
    if( head == NULL ) 
    {
        fclose( fp );
        return;
    }
    fprintf(fp,"%s %s\n",head->nome,what_timeis( head->orario));

    file_write_appuntamenti ( head->next , fp);

}

void file_write_richieste ( heap * tree, int  dim )
{
    if( dim == 0 ) return;

    FILE * fp  = fopen ("richieste.txt","w");

    if(!fp) {   printf("\nErrore scrittura FILE richieste\n"); return;}

    while( dim >= 0 ) 
    {
        fprintf(fp,"%s %d\n",tree[dim].nome, tree[dim].sintomi);
        dim--;
    }

    fclose( fp );
}



void file_append_storico ( )
{
    char nome[30];
    char trash[30];


    time_t t;
    int count = 0;

    srand((unsigned) time(&t));

    FILE * fp_storico = fopen ("storico.txt","a");
    if(!fp_storico) {   printf("\nErrore scrittura FILE storico\n"); return;}

    FILE * fp_appuntamenti = fopen ("appuntamenti.txt","r");
    if(!fp_appuntamenti) {   printf("\nErrore lettura FILE appuntamenti\n"); return;}

    

    while( count < 6 ) 
    {
        if(fscanf(fp_appuntamenti,"%s %s",nome,trash) != 2 ) 
        {
            fclose(fp_storico);
            fclose(fp_appuntamenti);
            return ;
        }

        fprintf(fp_storico,"%s ",nome);

         

        if(  rand() % 5 <= 2  ) fprintf(fp_storico,"%s\n","positivo");
        else fprintf(fp_storico,"%s\n","negativo");

        count++;
    }

    fclose(fp_storico);
    fclose(fp_appuntamenti);
}

void file_read( )
{
    FILE * fp = fopen ("storico.txt","r");
    if(!fp) {   printf("\nErrore lettura FILE storico\n"); return;}

    char l_s[60]; 

    if((fgets(l_s,60,fp) == NULL)) printf("\n\tStorico Vuoto\n");
    else printf("%s",l_s);

    while(fgets(l_s,60,fp) != NULL)
        printf("%s",l_s);
    
    fclose(fp);
}

//--------------------------------------------------------------------------------------------------------------

lista* add_node(lista *head, lista * user){

	lista*ptr;

	lista *temp = (lista *)malloc(sizeof(lista));

    strcpy(temp->nome,user->nome);

	temp->orario = user->orario;
	temp->next = NULL;
	
	if(head==NULL)
    {
		// this section runs if linked list is empty
		// we simply set head to temp
		head = temp;	
	}
	else if(temp->orario >= head->orario)
    {
		// this section runs if the new val is smaller then
		// the first value in the linked list
		// we simple insert temp at the beginning of the linked list
		temp->next = head;
		head = temp;
	}
	else
    {
		// we are iterating each element of the linked list
		// to find the node whose value is just smaller than val
        ptr = head;
        while(ptr->next!=NULL&&ptr->next->orario > temp->orario)
        {
            ptr = ptr->next;
        }
		// ptr now points to the node whose value is just less than val
		// we simply insert temp between ptr and ptr->next
		temp->next = ptr->next;
		ptr->next = temp;
	}

	return head;
	
}

lista * crea_nodo ( lista * user )
{
    lista * nodo = (lista *)malloc( sizeof ( lista ));

    strcpy(nodo->nome,user->nome);
    nodo->orario = user->orario;

    //dovrei aggiungere data instead of sintomi
    nodo->next = NULL;

    return nodo;
}

void print_L( lista * head )
{
    if( !head ) 
    {
        printf("\n\n ");
        return;
    }

    printf("\n\t%s %s", head->nome,what_timeis(head->orario));


    print_L( head->next );
}

//-----------------------------------------------------------------

lista * delete_meeting( lista * head, heap * dynamicA, int * dim )
{
    char nome[30] = "easteregg";

    printf("\n| Benvenuto nel delete meeting |\n");

    //cerco l'utente nella lista
    while( !find_user_meeting( head, nome ) )
    {
        printf("\nInserisci un Utente presente nella lista appuntamenti  |back| per tornare indietro\n>");
        scanf("%29s",nome);

        if(!strcmp(nome,"back")) return head;
    }
    
    //elimino lutente dalla lista appuntamenti
    head = delete_nodo( nome, head );

    //aggiungo utente nella lista richieste
    add_user_inDynamicA( dynamicA, dim, nome);

    free( nome );
    
    return head;
}

bool find_user_meeting( lista * head, char *nome )
{
    if( head == NULL ) return false;

    if( !strcmp( head->nome, nome )) return true;

    find_user_meeting( head->next, nome);
}

lista *delete_nodo ( char *nome, lista * testa )
{

    if(!strcmp( testa->nome, nome) && testa->next == NULL ) return NULL;


    if ( !strcmp( testa->nome, nome))
    {
        add_orario ( testa->orario ); //aggiorno il day.time dell'utente eliminato
        lista * tmp = testa; 
        testa = testa->next;
        free( tmp );
    }
    
    
    lista * nodo_pre = testa; 
    lista * nodo_curr = testa->next; 


    while( nodo_curr ) 
    {
        if (  !strcmp( nodo_curr->nome, nome) )
        {
            add_orario ( nodo_curr->orario ); //aggiorno il day.time dell'utente eliminato
            lista * tmp = nodo_curr;
            nodo_pre->next = nodo_curr->next;
            free ( tmp );
        }
    
        nodo_pre = nodo_curr;
        nodo_curr = nodo_curr->next;
    }   

    return testa;
}


void add_user_inDynamicA ( heap * dynamicA, int *dim , char * nome)
{
    //dim = dim + 1;
    
    //heap * new_dynamicA = realloc (dynamicA, dim* sizeof( heap ));
    //if( !new_dynamicA ) printf("------------------ERRORE");

    //for( int i = 0 ; i < dim; i++ )
    //{
    //    strcpy(new_dynamicA[i].nome,dynamicA[i].nome);
    //    new_dynamicA[i].sintomi = dynamicA[i].sintomi;


    //    if( i == dim-1 )
    //    {
            strcpy(dynamicA[*dim].nome,nome);
            dynamicA[*dim].sintomi = 0;
    //    }
    //}

    //dynamicA = new_dynamicA;

    * dim = *dim +1;

    heap_sort(dynamicA , *dim);
}

//----------------------------------------------------------------------

lista * change_meeting(lista * head, heap * dynamicA, int * dim )
{
    int dimension = *dim;
    int pos_meeting_nome1 = 0;
    int pos_meeting_nome2 = 0;

    printf("\nInserisci il primo utente |back| per tornare indietro\n>");
    lista * user1 = crea_nome_change( head, dynamicA, dim , &pos_meeting_nome1);

    if( !user1 ) return head;

    printf("\nInserisci il secondo utente |back| per tornare indietro\n>");
    lista * user2 = crea_nome_change( head, dynamicA, dim , &pos_meeting_nome2 );

    if( !user2 ) return head;

    if( user1->orario == user2->orario ) return same_change( head , dynamicA, pos_meeting_nome1 , pos_meeting_nome2 , user1, user2);

    if (  (user2->orario == 1 && user1->orario == 2) || ( user1->orario == 1 && user2->orario == 2 ) )
    {
        if( user2->orario == 1) //significa che l'user2 va in appuntamenti else il contrario
        {
            //user in richieste va in appunatamenti
            head = change_meeting_list( head, user1, user2, false);

            //user in appuntamenti deve spostasrsi in rinchieste
            
            strcpy(dynamicA[pos_meeting_nome2].nome, user1->nome);
            dynamicA[pos_meeting_nome2].sintomi = 0;


            //same as below but change the name
        }
        else
        {
            head = change_meeting_list( head, user2, user1, false);

            strcpy(dynamicA[pos_meeting_nome1].nome, user2->nome); 
            dynamicA[pos_meeting_nome1].sintomi = 0;       
        }

    }
    else if( (user1->orario == 1 && user2->orario == 3) || (user2->orario == 1 && user1->orario == 3) )// from now on this control is for new users
    {
        //new user go in richieste
        if( user2->orario == 3 )//significa che l'user2 va in richieste else il contrario
        {

            add_user_inDynamicA ( dynamicA, &dimension , dynamicA[pos_meeting_nome1].nome);
            strcpy( dynamicA[pos_meeting_nome1].nome, user2->nome);
            dynamicA[pos_meeting_nome1].sintomi = 0;
        }
        else
        {
            add_user_inDynamicA ( dynamicA, &dimension , dynamicA[pos_meeting_nome2].nome);

            strcpy( dynamicA[pos_meeting_nome2].nome, user1->nome);
            dynamicA[pos_meeting_nome2].sintomi = 0;

        }

    }
    else if( (user2->orario == 2 && user1->orario == 3) ||  (user1->orario == 2 && user2->orario == 3) )
    {
        //new user go in meeting

        if( user2->orario == 3 ) //significa che l'user2 va in appuntamenti else il contrario
        {
            head = change_meeting_list( head, user1, user2, false);
            
            add_user_inDynamicA(dynamicA, &dimension, user1->nome);
    
        }
        else
        {
            head = change_meeting_list( head, user2, user1, false);
            
            add_user_inDynamicA(dynamicA, &dimension, user2->nome);
        }
        
        
    }

    * dim = dimension;

    return head; 
}



lista * crea_nome_change( lista * head,heap * dynamicA, int * dim, int *pos_meeting )// ho usato la struct lista poerche mi scocciava di creare una nuova struct
{
    lista * user = ( lista * )malloc( sizeof ( lista ));

    scanf("%29s",user->nome);

    if(!strcmp(user->nome,"back")) return NULL;
    
    if ( find_user_richieste( dynamicA, *dim , pos_meeting, user->nome  ) )
    {
        user->orario = 1; //al posto di un orario ci ho messo un valore che significa che sarà un utente preso da richieste
        
    }
    else if( find_user_meeting( head, user->nome) )
    {
        user->orario = 2; //significa che sarà un utente preso da meeting
        
    }
    else
    {
        printf("\nHai inserito un utente nuovo quindi te lo aggiungero' alla lista che hai scelto\n\n");
        user->orario = 3; //significa che sarà un utente nuovo
        
    }

    

    return user;
}

lista * same_change (lista * head , heap * dynamicA , int  pos_meeting_nome1, int pos_meeting_nome2 , lista * user1, lista * user2)
{
    if( user1->orario == 3 )// is the same, if make a control of user1 ill make a control of user2 because they re the same
    {
        printf("\n\n||NON PUOI SOSTITUIRE DUE NUOVI UTENTI||\n");
        return head;
    }

    if( user1->orario == 2 )
    {
        //change pos meeting
        head = change_meeting_list(head , user1, user2, true);
    }


    //change pos richieste
    if( dynamicA[pos_meeting_nome1].sintomi == dynamicA[pos_meeting_nome2].sintomi )
    {
 
        strcpy(dynamicA[pos_meeting_nome1].nome,user2->nome);
        strcpy(dynamicA[pos_meeting_nome2].nome,user1->nome);

    }
    else printf("\n||Puoi sostituire solo utenti che hanno sintomi uguali||");

    return head;
}

lista * change_meeting_list ( lista * head , lista * user_meeting, lista * user_richieste, bool check)
{
    if (head == NULL ) return NULL;

 

    if( !strcmp(head->nome,user_meeting->nome ) && user_meeting->orario > 0 )
    {
        strcpy( head->nome,user_richieste->nome);//questo controllo lo utilizzo sempre per sostituire il nome della richiesta con quello del meeting
        user_meeting->orario = 0;// ho utilizzato la variabile orario che non mi serviva piu per fare un controllo per evitare di riutilizzarla
        change_meeting_list (head->next , user_meeting, user_richieste , check);

    }

    if( !strcmp(head->nome,user_richieste->nome ) && user_richieste->orario > 0 && check ) 
    {
        strcpy( head->nome,user_meeting->nome); // questo controllo lo uso solo quando entrambi sono uguali, check "same_change" function
        user_richieste->orario = 0;

        change_meeting_list (head->next , user_meeting, user_richieste , check);
    }

    change_meeting_list (head->next , user_meeting, user_richieste, check );

    return head;

}


//---------------------------------------------------------------------------------------

lista * add_meeting(lista * head, heap * dynamicA, int * dim )
{
    int pos_meeting = 0;
    char nome[30] = "null";
    int orario = 0;
    lista * user_meeting = ( lista * ) malloc ( sizeof ( lista ));
    bool check = false;

    printf("\n| Benvenuto nel add meeting |\n");

    if( dim == 0 )
    {
        printf("\nLista richieste vuota, Puoi solo creare appuntamenti |back| per tornare indietro\n>");
        scanf("%29s",nome);

        if(!strcmp(nome,"back")) return head;        
    }
    else
    {
        while( strcmp( nome ,"no"))
        {
            printf("\nVuoi aggiungere un Utente che non e' presente nella lista richieste?\n\n\tEnter |si| |no| or |back| per tornare indietro\n>");
            scanf("%29s",nome);

            if(!strcmp(nome,"back")) return head;

            if(!strcmp(nome,"si")) return crea_user( dynamicA, head, dim );

        }

        while( !check ) 
        {
            printf("\nInserisci un Utente presente nella lista richieste |back| per tornare indietro\n>");
            scanf("%29s",nome);

            if(!strcmp(nome,"back")) return head;

            check = find_user_richieste( dynamicA, *dim , &pos_meeting, nome  );
        }
    }

    strcpy(user_meeting->nome, nome);

    while( !check_orario(nome, &orario))
    {
        printf("\nA che orario vuoi inserire l'utente ? |back| per tornare indietro\n>");
        scanf("%12s",nome);

        if(!strcmp(nome,"back")) return head;          
    }

    deleteRoot( dynamicA , NULL , dim, pos_meeting);

    
    user_meeting->orario = orario;
    user_meeting->next = NULL;

    head = add_node( head,  user_meeting );

    free ( user_meeting );

    return head;
}


lista * crea_user( heap * dynamicA, lista * head, int * dim )
{

    int pos_meeting = 0;
    char nome[30] = "null";
    int orario = 0;

    lista * user_meeting = ( lista * ) malloc ( sizeof ( lista ));
    
    printf("\nInserisci un Utente che e' non presente nella lista richieste o appuntamenti |back| per tornare indietro\n>");
    scanf("%29s",nome);

    if(!strcmp(nome,"back")) return head;
    
    while( find_user_richieste( dynamicA, *dim , &pos_meeting, nome  ) ||  find_user_meeting( head, nome) ) 
    {
        printf("\nInserisci un Utente che e' non presente nella lista richieste o appuntamenti  |back| per tornare indietro \n>");
        scanf("%29s",nome);

        if(!strcmp(nome,"back")) return head;

    }

    strcpy(user_meeting->nome, nome);

    while( !check_orario(nome, &orario))
    {
        printf("\nA che orario vuoi inserire l'utente ? |back| per tornare indietro\n>");
        scanf("%12s",nome);

        if(!strcmp(nome,"back")) return head;          
    }

    user_meeting->orario = orario;
    user_meeting->next = NULL;

    head = add_node( head, user_meeting);

    free( user_meeting );

    return head;

}



bool find_user_richieste( heap * dynamicA, int dim , int * pos, char * nome) 
{
    for( int i = 0; i< dim ; i++)
    {
        if(!strcmp(dynamicA[i].nome, nome )) 
        {
            * pos = i;// mi salvo la posizione dell'utente nella lista meeting
            return true;
            
        }

    }

    return false;
}