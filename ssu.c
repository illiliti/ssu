#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include <inttypes.h>
#include <sys/wait.h>

#ifndef ENV_PATH
#define ENV_PATH "/bin:/sbin:/usr/bin:/usr/sbin"
#endif

#ifndef UID_MAX
#define UID_MAX UINT_MAX
#endif

static const char *argv0;

static uid_t parse_uid(const char *usr)
{
    intmax_t uid;
    char *end;

    errno = 0;
    uid = strtoimax(usr, &end, 10);

    if (end == usr || end[0] != '\0') {
        fprintf(stderr, "%s: uid must be numeric: %s\n", argv0, usr);
        exit(1);
    }

    if (errno && (uid == 0 || uid == INTMAX_MIN || uid == INTMAX_MAX)) {
        perror("strtoimax");
        exit(1);
    }

    if (uid < 0) {
        fprintf(stderr, "%s: uid must be positive: %s\n", argv0, usr);
        exit(1);
    }

    if (uid > UID_MAX - 1) {
        fprintf(stderr, "%s: uid too large: %s\n", argv0, usr);
        exit(1);
    }

    return (uid_t)uid;
}

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
        fprintf(stderr, "execvp %s: %s\n", argv[0], strerror(errno));
        _exit((errno == ENOENT) + 126);
    default:
        waitpid(pid, &status, 0);
        return WIFSIGNALED(status) ? WTERMSIG(status) + 128 : WEXITSTATUS(status);
    }
}

static void print_usage(void)
{
    fprintf(stderr, "usage: %s [-pn] [-u user] -s\n", argv0);
    fprintf(stderr, "       %s [-pn] [-u user] -e file...\n", argv0);
    fprintf(stderr, "       %s [-pn] [-u user] command [args...]\n", argv0);
}

int main(int argc, char **argv)
{
    unsigned pflag = 0, eflag = 0, sflag = 0, nflag = 0;
    const char *term, *user = NULL;
    extern char **environ;
    struct passwd *pw;
    uid_t uid = 0;
    int opt;

    assert((argv0 = argv[0]));

    while ((opt = getopt(argc, argv, "u:hpesn")) != -1) {
        switch (opt) {
        case 'n':
            nflag = 1;
            break;
        case 'p':
            pflag = 1;
            break;
        case 'e':
            eflag = 1;
            break;
        case 's':
            sflag = 2;
            break;
        case 'u':
            user = optarg;
            break;
        case 'h':
            print_usage();
            return 0;
        case '?':
            print_usage();
            return 1;
        }
    }

    argv += optind - sflag - eflag;
    argc -= optind;

    if ((!sflag && !argc) || (sflag && argc) || (eflag && !argc)) {
        print_usage();
        return 2;
    }

    if (getuid() != 0 && geteuid() != 0) {
        fprintf(stderr, "%s: %s\n", argv0, strerror(EPERM));
        return 1;
    }

    if (nflag) {
        uid = user ? parse_uid(user) : 0;
        pw = getpwuid(uid);
    }
    else {
        user = user ? user : "root";
        pw = getpwnam(user);
    }

    if (!pw) {
        if (errno) {
            perror(nflag ? "getpwuid" : "getpwnam");
        }
        else {
            if (nflag) {
                fprintf(stderr, "getpwuid %u: no such uid\n", uid);
            }
            else {
                fprintf(stderr, "getpwnam %s: no such user\n", user);
            }
        }

        return 1;
    }

    if (initgroups(pw->pw_name, pw->pw_gid) == -1) {
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

    if (eflag) {
        argv[0] = getenv("EDITOR");

        if (!argv[0] || argv[0][0] == '\0') {
            argv[0] = "vi";
        }

        return exec_file(argv);
    }

    if (!pflag) {
        term = getenv("TERM");
        environ = NULL;

        if (term && term[0] != '\0') {
            setenv("TERM", term, 0);
        }

        setenv("LOGNAME", pw->pw_name, 0);
        setenv("SHELL", pw->pw_shell, 0);
        setenv("USER", pw->pw_name, 0);
        setenv("HOME", pw->pw_dir, 0);
        setenv("PATH", ENV_PATH, 0);
    }

    if (sflag) {
        argv[1] = NULL;
        argv[0] = getenv("SHELL");

        if (!argv[0] || argv[0][0] == '\0') {
            argv[0] = "/bin/sh";
        }
    }

    return exec_file(argv);
}
