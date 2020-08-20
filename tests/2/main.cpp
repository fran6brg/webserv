#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int
main(void)
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    /* Surveiller stdin (fd 0) en attente d'entrées */
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    /* Pendant 5 secondes maxi */
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(1, &rfds, NULL, NULL, &tv);
    /* Considérer tv comme indéfini maintenant ! */

    if (retval == -1)
        perror("select()");
    else if (retval)
        printf("Des données sont disponibles maintenant\n");
        /* FD_ISSET(0, &rfds) est vrai */
    else
        printf("Aucune données durant les 5 secondes\n");

    exit(EXIT_SUCCESS);
}

// http://manpagesfr.free.fr/man/man2/select.2.html