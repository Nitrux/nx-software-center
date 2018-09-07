
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *appendPath(const char *varName, const char *path) {
    const char *oldValue = getenv(varName);
    char *newValue = NULL;
    size_t size = 1;
    if (oldValue) {
        size += strlen(oldValue) + strlen(path) + 1;
        newValue = malloc(size);

        sprintf(newValue, "%s:%s", oldValue, path);
    } else {
        size += strlen(path);
        newValue = malloc(size);

        sprintf(newValue, "%s", path);
    }

    return newValue;
}

char *extendEnvironmentVariable(char *varName, const char *appdir, const char *standardPath) {
    size_t newValueSize = strlen(appdir) + strlen(standardPath) + 1;
    char new_value[newValueSize];
    memset(new_value, 0, newValueSize);

    strcat(new_value, appdir);
    strcat(new_value, standardPath);

    return appendPath(varName, new_value);
}

int main(int argc, char **argv, char **envp) {
    int childPid;

    char *appDir = getenv("APPDIR");
    if (appDir == NULL) {
        perror("APPDIR environment variable not set.");
        return -1;
    }

    size_t runnable_str_size = strlen(appDir) + strlen(BINARY_PATH) + 1;
    char *runnable = malloc(runnable_str_size);
    memset(runnable, 0, runnable_str_size);
    strcat(runnable, appDir);
    strcat(runnable, BINARY_PATH);

    puts(runnable);

    setenv("LD_LIBRARY_PATH", extendEnvironmentVariable("LD_LIBRARY_PATH", appDir, "/usr/lib/"), 1);
    setenv("XDG_CONFIG_DIRS", extendEnvironmentVariable("XDG_CONFIG_DIRS", appDir, "/etc/xdg/"), 1);
    setenv("XDG_DATA_DIRS", extendEnvironmentVariable("XDG_DATA_DIRS", appDir, "/usr/share/"), 1);


    if ((childPid = fork()) == -1) {
        perror("can't fork");
        return -1;
    } else if (childPid == 0) {
        argv[0] = runnable;
        return execv(runnable, argv);
    } else
        return 0;
}