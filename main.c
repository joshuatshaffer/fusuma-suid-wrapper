#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>
#include <unistd.h>

void addgroup (gid_t group_to_add) {
    int num_groups = getgroups (0, NULL);
    gid_t *list = calloc (num_groups + 1, sizeof(gid_t));
    getgroups (num_groups, list + 1);
    list[0] = group_to_add;
    setgroups (num_groups + 1, list);
}

void exec_pass_args (char *file, int argc, char **argv) {
    // Make a copy of argv including the NULL terminator.
    char **argv_new = calloc (argc + 1, sizeof(char**)); 
    memcpy (argv_new, argv, (argc + 1) * sizeof(char**));
    // Set the program name.
    argv_new[0] = file;
    execvp (file, argv_new);
    perror ("execvp returned");
}

int main (int argc, char *argv[]) {

    // Add input to our groups.
    addgroup (106);

    // Exec Fusuma passing though command line args.
    exec_pass_args ("fusuma", argc, argv);
}
