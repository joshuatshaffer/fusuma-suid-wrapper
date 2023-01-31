#include <errno.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

gid_t get_gid (char *name) {
    errno = 0;
    struct group *group_info = getgrnam (name);
    if (group_info == NULL) {
        if (errno == 0) {
            fprintf (stderr, "The group \"%s\" could not be found.\n", name);
        } else {
            perror ("Could not get group info");
        }
        exit (1);
    }
    // The return value of getgrnam (group_info) should not be freed.
    return group_info->gr_gid;
}

void join_group (gid_t group_to_add) {
    // Get the groups we are currently in.
    int num_groups = getgroups (0, NULL);
    gid_t *groups = calloc (num_groups + 1, sizeof (gid_t));
    getgroups (num_groups, groups + 1);

    // Add the new group.
    groups[0] = group_to_add;

    setgroups (num_groups + 1, groups);
    if (errno != 0) {
        perror ("Could not join group");
        exit (1);
    }
    free (groups);
}

/**
 * Drop "sudo" privileges by setting this process's effective user id to its
 * real user id.
 */
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
    exit (1);
}

int main (int argc, char *argv[]) {
    // Add input to our groups.
    gid_t input_gid = get_gid ("input");
    join_group (input_gid);

    // Drop sudo privileges.
    // They are no longer needed and should not be passed on to fusuma.
    drop_sudo ();

    // Exec Fusuma and pass along the command line args.
    exec_pass_args ("fusuma", argc, argv);
}
