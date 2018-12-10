#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>

int main() {
    key_t key;
    int shmid;
    char * data;
    char * mode;
    size_t size = 1024;

    FILE * f = fopen("./testfile", "r");
    if (f == NULL) {
        printf("This file doesn't exist, it will be created shortly.\n");
        f = fopen("./testfile", "w");
    }else{
        char c = fgetc(f);
        while (c != EOF) {
            printf("%c", c);
            c = fgetc(f);
        }
        printf("\n");
    }
    fclose(f);

    if ((key = ftok("./testfile", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }

    printf("This is key: %i\n", key);

    if ((shmid = shmget(key, size, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }
    printf("This is shmid: %i\n", shmid);
    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    // data = shmat(shmid, (void *)0, 0);
    // if (data == (char *) -1) {
    //     perror("shmat");
    //     exit(1);
    // }

    printf("Would you like to read or write to the file?\nRead\nWrite\t(DEFAULT)\n");
    scanf("%s", mode);
    //mode[strcspn(mode, "\n")] = '\0';

    if (strcmp(mode, "Read") == 0 || strcmp(mode, "read") == 0) {
        printf("Reading from the file...\n");
        printf("Segment contains: \"%s\"\n", data);
    }else{
        char * written;
        printf("Writing to file...");
        printf("Please write something now:");
        fgets(written, 200, stdin);
        written[strcspn(written, "\n")] = '\0';
        strcpy(data, written);
    }

    char * dnd;
    printf("Would you like to disconnect or destroy?\nDisconnect\t(DEFAULT)\nDestroy\n");
    fgets(dnd, 20, stdin);
    //dnd[strcspn(dnd, "\n")] = '\0';

    if (strcmp(dnd, "Destroy") == 0 || strcmp(dnd, "destroy") == 0) {
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
        }
    }else{
        if (shmdt(data) == -1) {
            perror("shmdt");
            exit(1);
        }
    }
    return 0;
}
