#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
    char* head = "--config=";
    char* both = malloc(strlen(head) + strlen(argv[1]) + 1);
    strcpy(both, head);
    strcat(both, argv[1]);

    setuid(0);

    char *const args[] = {"fusuma", both, (char*)NULL};
    return execv("/usr/local/bin/fusuma", args);
}
