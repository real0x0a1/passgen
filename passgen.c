// -*- Author: real0x0a1 (Ali) -*-
// -*- info: Simple Passwrod list generator written in C -*-

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

char def_charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$^&*()_+-=[]{};:'\",.<>/?`~";

unsigned int inc[128];

int finished(char *block, char *charset, char *templ)
{
    unsigned int i;

    if (templ[0] == 0)
    {
        for (i = 0; i < strlen(block); i++)
            if (inc[i] < strlen(charset) - 1)
                return FALSE;
    }
    else
    {
        for (i = 0; i < strlen(block); i++)
            if (templ[i] == '@' && (inc[i] < strlen(charset) - 1))
                return FALSE;
    }

    return TRUE;
}

void increment(char *block, int len, char *charset, char *templ)
{
    int i;

    for (i = strlen(block) - 1; i > -1; i--)
    {
        if (templ[0] == 0 || templ[i] == '@')
        {
            if (inc[i] < strlen(charset) - 1)
            {
                inc[i]++;
                block[i] = charset[inc[i]];
                i = -1;
            }
            else
            {
                block[i] = charset[0];
                inc[i] = 0;
            }
        }
        else
        {
        }
    }
}

void chunk(int start, int end, char *charset, char *templ, char *startblock, const char *outputFile)
{
    int i, j, k, t;
    char block[128];

    if (end - start < 0)
        return;

    if (templ[0] == 0)
        t = 0;
    else
        t = 1;

    FILE *file = fopen(outputFile, "w");

    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    long long int total_size = 0;
    long long int line_count = 0;

    for (i = start; i <= end; i++)
    {
        memset(block, 0, sizeof(block));

        for (j = 0; j < i; j++)
        {
            if (startblock[0] == 0)
            {
                if (t == 0)
                {
                    block[j] = charset[0];
                    inc[j] = 0;
                }
                else
                {
                    if (templ[j] == '@')
                    {
                        block[j] = charset[0];
                        inc[j] = 0;
                    }
                    else
                        block[j] = templ[j];
                }
            }
            else
            {
                block[j] = startblock[j];

                for (k = 0; k < strlen(charset); k++)
                    if (block[j] == charset[k])
                        inc[j] = k;
            }
        }

        fprintf(file, "%s\n", block);
        line_count++;
        total_size += strlen(block) + 1; // Adding 1 for the newline character
        while (!finished(block, charset, templ))
        {
            increment(block, i, charset, templ);
            fprintf(file, "%s\n", block);
            line_count++;
            total_size += strlen(block) + 1; // Adding 1 for the newline character
        }
    }

    fclose(file);

    int B = 1024;
    int MB = 1024 * 1024;
    int GB = 1024 * 1024 * 1024;
    long long int TB = 1024LL * 1024LL * 1024LL * 1024LL;
    long long int PB = 1024LL * 1024LL * 1024LL * 1024LL * 1024LL;

    printf("passgen generated the following amount of data : \n");
    printf("%lld Bytes\n", total_size / B);
    printf("%lld MB\n", total_size / MB);
    printf("%lld GB\n", total_size / GB);
    printf("%lld TB\n", total_size / TB);
    printf("%lld PB\n", total_size / PB);
    printf("passgen generated the following number of lines: %lld\n", line_count);
}

int main(int argc, char *argv[])
{
    printf("      _____  _______ _______ _______  ______ _______ __   _\n");
    printf("     |_____] |_____| |______ |______ |  ____ |______ | \\  |\n");
    printf("     |       |     | ______| ______| |_____| |______ |  \\_|\n");

    printf("\n\nVersion: 0.0.1\nDeveloper: real0x0a1 (Ali)\n");

    int i, j, flag, min, max;
    char charset[256];
    char templ[256];
    char startblock[256];
    const char *outputFile = "output.txt";

    if (argc < 3)
    {
        printf("usage: <from-len> <to-len> [charset] [-t [FIXED]@@@@] [-s startblock] -o <output_file>\n");
        return 0;
    }

    memset(charset, 0, sizeof(charset));
    memset(templ, 0, sizeof(templ));
    memset(startblock, 0, sizeof(startblock));

    /* remove duplicate characters from charset */
    if (argc > 3)
    {
        for (i = 0; i < strlen(argv[3]); i++)
        {
            flag = 0;
            for (j = 0; j < strlen(charset); j++)
                if (argv[3][i] == charset[j])
                    flag = 1;

            if (flag == 0)
                strncat(charset, &argv[3][i], 1);
        }
    }
    else
        strncpy(charset, def_charset, strlen(def_charset));

    min = atoi(argv[1]);
    max = atoi(argv[2]);

    for (i = 1; i < argc - 1; i++)
    {
        if (strncmp(argv[i], "-t", 2) == 0)
        {
            strncpy(templ, argv[i + 1], strlen(argv[i + 1]));
        }

        if (strncmp(argv[i], "-s", 2) == 0)
        {
            if (strlen(argv[i + 1]) > min)
            {
                printf("Warning: starting length increased to %lu.\n", (unsigned long)strlen(argv[i + 1]));
                min = strlen(argv[i + 1]);
            }
            if (strlen(argv[i + 1]) < min)
            {
                printf("Warning: minimum length decreased to %lu.\n", (unsigned long)strlen(argv[i + 1]));
                min = strlen(argv[i + 1]);
            }

            strncpy(startblock, argv[i + 1], strlen(argv[i + 1]));
        }

        if (strncmp(argv[i], "-o", 2) == 0)
        {
            outputFile = argv[i + 1];
        }
    }

    chunk(min, max, charset, templ, startblock, outputFile);

    return 0;
}