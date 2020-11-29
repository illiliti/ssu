#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#ifndef ENV_PATH
#define ENV_PATH "/bin:/sbin:/usr/bin:/usr/sbin"
#endif

static int exec_file(char **argv)
{
    int status;
    pid_t pid;

    switch ((pid = fork())) {
    case -1:
        perror("fork");
        return 1;
    case 0:
        execvp(argv[0], argv);
        fprintf(stderr, "execvp: %s: %s\n", argv[0], strerror(errno));
        _exit((errno == ENOENT) + 126);
    default:
        waitpid(pid, &status, 0);
        return WIFSIGNALED(status) ? WTERMSIG(status) + 128 : WEXITSTATUS(status);
    }
}

static void print_usage(const char *argv0)
{
    fprintf(stderr, "usage: %s [-p] [-u user] -s\n", argv0);
    fprintf(stderr, "usage: %s [-p] [-u user] -e file ...\n", argv0);
    fprintf(stderr, "usage: %s [-p] [-u user] command [args ...]\n", argv0);

    fputc('\n', stderr);

    // TODO move to man page
    fprintf(stderr, "-p do not reset environment\n");
    fprintf(stderr, "-u execute command as user\n");
    fprintf(stderr, "-s run interactive shell\n");
    fprintf(stderr, "-e edit file\n");
}

int main(int argc, char **argv)
{
    int pflag = 0, eflag = 0, sflag = 0;
    char *argv0, *term, *user = "root";
    char *shell[2], **editor;
    extern char **environ;
    struct passwd *pw;
    int i, opt;

    argv0 = strrchr(argv[0], '/');

    if (!argv0) {
        argv0 = argv[0];
    }
    else {
        argv0++;
    }

    while ((opt = getopt(argc, argv, "u:hpes")) != -1) {
        switch (opt) {
        case 'p':
            pflag = 1;
            break;
        case 'e':
            eflag = 1;
            break;
        case 's':
            sflag = 1;
            break;
        case 'u':
            user = optarg;
            break;
        case 'h':
            print_usage(argv0);
            return 0;
        case '?':
            print_usage(argv0);
            return 1;
        }
    }

    argv += optind;
    argc -= optind;

    if (!sflag && !argc) {
        print_usage(argv0);
        return 1;
    }

    if (getuid() != 0 && (geteuid() != 0 || getgid() != getegid())) {
        fprintf(stderr, "%s: %s\n", argv0, strerror(EPERM));
        return 1;
    }

    pw = getpwnam(user);

    if (!pw) {
        if (errno) {
            perror("getpwnam");
        }
        else {
            fprintf(stderr, "getpwnam: %s: No such user\n", user);
        }

        return 1;
    }

    if (initgroups(user, pw->pw_gid) == -1) {
        perror("initgroups");
        return 1;
    }

    if (setgid(pw->pw_gid) == -1) {
        perror("setgid");
        return 1;
    }

    if (setuid(pw->pw_uid) == -1) {
        perror("setuid");
        return 1;
    }

    if (!pflag && !eflag) {
        term = getenv("TERM");
        environ = NULL;

        if (term) {
            setenv("TERM", term, 0);
        }

        setenv("LOGNAME", pw->pw_name, 0);
        setenv("SHELL", pw->pw_shell, 0);
        setenv("USER", pw->pw_name, 0);
        setenv("HOME", pw->pw_dir, 0);
        setenv("PATH", ENV_PATH, 0);
    }

    if (eflag) {
        editor = malloc(++argc);

        if (!editor) {
            perror("malloc");
            return 1;
        }

        editor[argc] = NULL;
        editor[0] = getenv("EDITOR");

        if (!editor[0] || editor[0][0] == '\0') {
            editor[0] = "vi";
        }

        for (i = 0; argv[i]; i++) {
            editor[i + 1] = argv[i];
        }

        argv = editor;
    }

    if (sflag) {
        shell[1] = NULL;
        shell[0] = getenv("SHELL");

        if (!shell[0] || shell[0][0] == '\0') {
            shell[0] = "/bin/sh";
        }

        argv = shell;
    }

    return exec_file(argv);
}
