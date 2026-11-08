
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20
#define SIZE 100

char prod[MAX][SIZE], first[26][SIZE], follow[26][SIZE];
int n;

int add(char s[], char c)
{
    if (!strchr(s, c))
    {
        int l = strlen(s);
        if (l >= SIZE - 1)
            return 0;

        s[l] = c;
        s[l + 1] = '\0';
        return 1;
    }
    return 0;
}

int main()
{
    printf("Number of productions: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
    {
        printf("Invalid number of productions\n");
        return 1;
    }

    printf("Enter productions (e.g. S=AB, # for epsilon):\n");

    for (int i = 0; i < n; i++)
    {
        if (scanf("%99s", prod[i]) != 1 ||
            !isupper((unsigned char)prod[i][0]) ||
            prod[i][1] != '=' || prod[i][2] == '\0')
        {
            printf("Invalid production\n");
            return 1;
        }

        char *rhs = prod[i] + 2;
        int start = 0;

        while (rhs[start])
        {
            int end = start;

            while (rhs[end] && rhs[end] != '|')
                end++;

            if (end == start)
            {
                printf("Invalid production\n");
                return 1;
            }

            for (int j = start; j < end; j++)
            {
                if (rhs[j] == '#' && (end - start != 1))
                {
                    printf("Invalid epsilon production\n");
                    return 1;
                }
            }

            if (rhs[end] == '|' && rhs[end + 1] == '\0')
            {
                printf("Invalid production\n");
                return 1;
            }

            start = end;
            if (rhs[start] == '|')
                start++;
        }
    }

    int changed;

    // Calculate FIRST
    do
    {
        changed = 0;

        for (int i = 0; i < n; i++)
        {
            int A = prod[i][0] - 'A';
            char *rhs = prod[i] + 2;
            int start = 0;

            while (rhs[start])
            {
                int end = start;

                while (rhs[end] && rhs[end] != '|')
                    end++;

                int nullable = 1;

                for (int j = start; j < end; j++)
                {
                    char c = rhs[j];

                    if (c == '#')
                        break;

                    if (!isupper((unsigned char)c))
                    {
                        changed |= add(first[A], c);
                        nullable = 0;
                        break;
                    }

                    int B = c - 'A';

                    for (int k = 0; first[B][k]; k++)
                        if (first[B][k] != '#')
                            changed |= add(first[A], first[B][k]);

                    if (!strchr(first[B], '#'))
                    {
                        nullable = 0;
                        break;
                    }
                }

                if (nullable)
                    changed |= add(first[A], '#');

                start = end;
                if (rhs[start] == '|')
                    start++;
            }
        }
    } while (changed);

    // Start symbol
    add(follow[prod[0][0] - 'A'], '$');

    // Calculate FOLLOW
    do
    {
        changed = 0;

        for (int i = 0; i < n; i++)
        {
            int A = prod[i][0] - 'A';
            char *rhs = prod[i] + 2;
            int start = 0;

            while (rhs[start])
            {
                int end = start;

                while (rhs[end] && rhs[end] != '|')
                    end++;

                for (int j = start; j < end; j++)
                {
                    if (!isupper((unsigned char)rhs[j]))
                        continue;

                    int B = rhs[j] - 'A';
                    int nullable = 1;

                    for (int k = j + 1; k < end; k++)
                    {
                        char c = rhs[k];

                        if (c == '#')
                            continue;

                        if (!isupper((unsigned char)c))
                        {
                            changed |= add(follow[B], c);
                            nullable = 0;
                            break;
                        }

                        int C = c - 'A';

                        for (int t = 0; first[C][t]; t++)
                            if (first[C][t] != '#')
                                changed |= add(follow[B], first[C][t]);

                        if (!strchr(first[C], '#'))
                        {
                            nullable = 0;
                            break;
                        }
                    }

                    if (nullable)
                        for (int k = 0; follow[A][k]; k++)
                            changed |= add(follow[B], follow[A][k]);
                }

                start = end;
                if (rhs[start] == '|')
                    start++;
            }
        }
    } while (changed);

    // Display FIRST
    printf("\nFIRST sets:\n");

    for (int i = 0; i < n; i++)
    {
        int A = prod[i][0] - 'A';

        printf("FIRST(%c) = { ", prod[i][0]);

        for (int j = 0; first[A][j]; j++)
            printf("%c ", first[A][j]);

        printf("}\n");
    }

    // Display FOLLOW
    printf("\nFOLLOW sets:\n");

    for (int i = 0; i < n; i++)
    {
        int A = prod[i][0] - 'A';

        printf("FOLLOW(%c) = { ", prod[i][0]);

        for (int j = 0; follow[A][j]; j++)
            printf("%c ", follow[A][j]);

        printf("}\n");
    }

    return 0;
}

/*
    Sample Input:
    Number of productions: 3
Enter productions (e.g. S=AB, # for epsilon):
S=AB
A=a|#
B=b|#

Sample Output:
FIRST sets:
FIRST(S) = { a b # }
FIRST(A) = { a # }
FIRST(B) = { b # }

FOLLOW sets:
FOLLOW(S) = { $ }
FOLLOW(A) = { b $ }
FOLLOW(B) = { $ }
*/