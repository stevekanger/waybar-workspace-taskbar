#include "cmd.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/unistd.h>

static gboolean sig_initialized = FALSE;

/**
 * Runs a command in its own process.
 *
 * @param cmd The command to run
 * @return The process id or -1
 */
int cmd_fork_exec(const char *cmd) {
    if(!cmd || cmd[0] == '\0') {
        return -1;
    }

    if(!sig_initialized) {
        signal(SIGCHLD, SIG_IGN);
        sig_initialized = TRUE;
    }

    pid_t pid = fork();

    if(pid < 0) {
        perror("fork failed");
        return pid;
    }

    if(pid == 0) {
        sigset_t mask;
        sigfillset(&mask);
        int err = pthread_sigmask(SIG_UNBLOCK, &mask, NULL);

        if(err != 0) {
            g_warning("pthread_sigmask failed: %s", strerror(err));
        }

        setpgid(0, 0);
        prctl(PR_SET_PDEATHSIG, SIGTERM);
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);

        perror("execl failed");
        _exit(127);
    }

    return pid;
}

/**
 * Send a command on the same process no output
 *
 * @param cmd The command to send
 * @return The return value of system()
 */
int cmd_send(const char *cmd) {
    return system(cmd);
}

/**
 * Send a command and get the output
 *
 * @param cmd The command to send
 * @return (transfer full): The output
 */
char *cmd_output(const char *cmd) {
    FILE *fp = popen(cmd, "r");

    if(!fp) {
        return NULL;
    }

    size_t size = 4096;
    char *buf = g_malloc(size);
    size_t len = 0;
    char tmp[256];

    while(fgets(tmp, sizeof(tmp), fp)) {
        size_t chunk = strlen(tmp);

        if(len + chunk + 1 > size) {
            size *= 2;
            buf = g_realloc(buf, size);
        }

        memcpy(buf + len, tmp, chunk);
        len += chunk;
    }

    buf[len] = '\0';
    pclose(fp);

    return buf;
}
