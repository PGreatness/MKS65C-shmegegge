#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024  /* make it a 1K shared memory segment */

int main(int argc, char *argv[])
{
    key_t key;
    int shmid;
    char *data;
    char in[200];
    int mode;

    //if (argc > 2) {
    //    fprintf(stderr, "usage: shmdemo [data_to_write]\n");
    //    exit(1);
    //}
    printf("This is case sensitive, please try exactly as shown\n\n");
    /* make the key: */
    if ((key = ftok("test.c", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }

    /* connect to (and possibly create) the segment: */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        printf("Created new key\n");
    }

    /* attach to the segment to get a pointer to it: */
    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    if (strcmp(data, "") == 0) {
	printf("New data\n");
    }else{
	printf("Content of data is: %s\n", data);
    }

    printf("Change the memory?\nChange\nRead\n");
    fgets(in, 200, stdin);
   // printf("This is in: %s", in);
    in[strlen(in)-1] = '\0';
    if (strcmp(in, "Change") == 0) {
	printf("Enter your string: ");
	fgets(in, 200, stdin);
	in[strlen(in)-1] = '\0';
	strcpy(data, in);
    }else{
	printf("This is inside of the memory: \"%s\"\n", data);
    }
    printf("Detach or destroy?\nDetach\nDestroy\n");
    fgets(in, 200, stdin);
    in[strlen(in)-1] = '\0';
    if (strcmp(in, "Detach") == 0) {
    	/* detach from the segment: */
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

