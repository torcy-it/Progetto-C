

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "hlogin.h"
#include "hadmin.h"
#include "hplatform.h"
#include "hutente.h"


int main ( )
{
    platform( );

    list_users user = login_signin( );

    if( user.userid == 'a') admin();
    else ospite( user.username );

    
    return 0;

}