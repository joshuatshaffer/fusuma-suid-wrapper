#include <errno.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fsuid.h>
#include <sys/types.h>
#include <unistd.h>

gid_t get_gid (char *name) {
    errno = 0;
    struct group *ginfo = getgrnam (name);
    if (ginfo == NULL) {
        if (errno == 0) {
            fprintf (stderr, "The group \"%s\" could not be found.\n", name);
            exit (1);
        } else {
            perror ("Could not get group info");
        }
    }
    return ginfo->gr_gid;
}

void join_group (gid_t group_to_add) {
    int num_groups = getgroups (0, NULL);
    gid_t *list = calloc (num_groups + 1, sizeof (gid_t));
    getgroups (num_groups, list + 1);
    list[0] = group_to_add;
    setgroups (num_groups + 1, list);
}

void drop_sudo () {
    uid_t uid = getuid ();
    seteuid (uid);
}

void exec_pass_args (char *file, int argc, char **argv) {
    // Make a copy of argv including the NULL terminator.
    char **argv_new = calloc (argc + 1, sizeof (char **));
    memcpy (argv_new, argv, (argc + 1) * sizeof (char **));
    // Set the program name arg.
    argv_new[0] = file;
    execvp (file, argv_new);
    perror ("execvp returned");
}

int main (int argc, char *argv[]) {

    gid_t input_gid = get_gid ("input");

    // Add input to our groups.
    join_group (input_gid);

    // This in needed to fix "Insecure operation - spawn (SecurityError)"
    // which was being thrown by read_libinput in fusuma.
    // I don't know why this is needed.
    drop_sudo ();

    // Exec Fusuma passing though command line args.
    exec_pass_args ("fusuma", argc, argv);
}
