#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"
#include <sys/stat.h>

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char* argv[]) {
	if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: %s <path> [exact-match-pattern]\n", argv[0]);
        return FALSE;
    }
    
	const char *path = argv[1];
    struct stat stats;

    if (stat(path, &stats) != 0 || !S_ISDIR(stats.st_mode)) {
        fprintf(stderr, "%s is not a directory path! Exiting...\n", path);
        return FALSE;
    }

    if (argc == 2)
    {
        mode1(path, 0);
    }

    else
    {
        stack_t *stk = initstack();
        mode2(path, argv[2], 0, stk);

		printstack(stk);

		freestack(stk);
    }

    return TRUE;
}
