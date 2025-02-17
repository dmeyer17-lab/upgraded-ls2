#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"

//function definitions for ls2

void mode1(const char *path, int indent);
void mode2(const char *path, const char *emp, int indent, stack_t *stk);

void mode1(const char *path, int indent)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        perror("Error using opendir");
        return;
    }

    struct dirent *ent;
    struct stat stats;
    char full_path[1024];

    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, ent->d_name);

        if (stat(full_path, &stats) != 0)
        {
            perror("Error using stat");
            return;
        }

        if (S_ISREG(stats.st_mode))
        {
            for (int i = 0; i < indent; i++)
            {
                printf(INDENT);
            }
            printf("%s (%lld bytes)\n", ent->d_name, (long long)stats.st_size);
        }
    }

    rewinddir(dir); //reset

    
    while ((ent = readdir(dir)) != NULL) //print and recurse
    {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        {
            continue; //skip . and ..
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, ent->d_name);

        if (stat(full_path, &stats) != 0)
        {
            perror("Error using stat");
            return;
        }

        if (S_ISDIR(stats.st_mode))
        {
            for (int i = 0; i < indent; i++)
            {
                printf(INDENT);
            }
            printf("%s/ (directory)\n", ent->d_name);
            mode1(full_path, indent + 1);
        }
    }

    closedir(dir);
}

void mode2(const char *path, const char *emp, int indent, stack_t *stk)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        perror("Error using opendir");
        return;
    }

    struct dirent *ent;
    struct stat stats;
    char full_path[1024];

    while ((ent = readdir(dir)) != NULL) //print & recurse
    {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
        {
            continue; // skip . and ..
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, ent->d_name);
        if (lstat(full_path, &stats) != 0)
        {
            perror("Error using lstat");
            return;
        }

        // check if current file is an emp
        if (S_ISREG(stats.st_mode) && strcmp(ent->d_name, emp) == 0)
        {
            char *file_ent = malloc(1024);
            snprintf(file_ent, 1024, "%*s%s (%lld bytes)", indent * 4, "", ent->d_name, (long long)stats.st_size);
            push(stk, file_ent);
        }

        // check subdirs w/recursions
        if (S_ISDIR(stats.st_mode))
        {
            stack_t *subdir_stk = initstack();
            mode2(full_path, emp, indent + 1, subdir_stk);

            // push emp subdirs onto the stack
            if (subdir_stk->size > 0)
            {
                char *dir_ent = malloc(1024);
                snprintf(dir_ent, 1024, "%*s%s/ (directory)", indent * 4, "", ent->d_name);
                push(stk, dir_ent);

                while (subdir_stk->size > 0)
                {
                    char *subdir_ent = pop(subdir_stk);
                    push(stk, subdir_ent);
                }
            }

            freestack(subdir_stk);
        }
    }

    closedir(dir);
}