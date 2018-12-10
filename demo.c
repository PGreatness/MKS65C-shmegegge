#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define SHM_SIZE 1024  /* make it a 1K shared memory segment */

int main()
{
    key_t key;
    int shmid;
    char *data;
    char * mode;
    char * written;

    //if (argc > 2) {
    //    fprintf(stderr, "usage: shmdemo [data_to_write]\n");
    //    exit(1);
    //}

    /* make the key: */
    if ((key = ftok("demo.c", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }

    /* connect to (and possibly create) the segment: */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    printf("Would you like to write or read?\nRead\nWrite\t(DEFAULT)\n");
    scanf("%s", mode);
    printf("You said %s\n", mode);
    //scanf("%s", mode);
    //printf("Mode is now %s\n", mode);
    /* read or modify the segment, based on the command line: */
    if (strcmp(mode, "Read") != 0) {
        printf("Write stuff:\n");
        scanf("%s", mode);
        printf("You said %s\n", mode);
        printf("writing to segment: \"%s\"\n", mode);
        strcpy(data, mode);
    }else{
        printf("segment contains: \"%s\"\n", data);
    }
    printf("Detach or Destroy?\nDetach\nDestroy\t(DEFAULT)\n");
    scanf("%s", mode);
    /* detach from the segment: */
    if (strcmp(mode, "Destroy") == 0) {
    	if (shmdt(data) == -1) {
        	perror("shmdt");
        	exit(1);
    	}
    }else{
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
	}
    }

    return 0;
}
