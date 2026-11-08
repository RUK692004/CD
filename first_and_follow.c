#include <stdio.h>
#include <string.h>
#include <ctype.h>

char prod[20][100], first[26][30], follow[26][30];
int n;

int add(char s[], char c)
{
    if (!strchr(s, c))
    {
        int l = strlen(s);
        s[l] = c;
        s[l + 1] = '\0';
        return 1;
    }
    return 0;
}

int main()
{
    printf("Number of productions: ");
    scanf("%d", &n);

    printf("Enter productions, e.g. S=AB (use # for epsilon):\n");

    for (int i = 0; i < n; i++)
        scanf("%99s", prod[i]);

    int changed;

    do
    {
        changed = 0;

        for (int i = 0; i < n; i++)
        {
            int A = prod[i][0] - 'A';
            char *rhs = strchr(prod[i], '=') + 1;
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
                    {
                        changed |= add(first[A], '#');
                        break;
                    }

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

    add(follow[prod[0][0] - 'A'], '$');

    do
    {
        changed = 0;

        for (int i = 0; i < n; i++)
        {
            int A = prod[i][0] - 'A';
            char *rhs = strchr(prod[i], '=') + 1;
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

    puts("FIRST sets:");

    for (int i = 0; i < n; i++)
    {
        int A = prod[i][0] - 'A';

        printf("FIRST(%c)={", prod[i][0]);

        for (int j = 0; first[A][j]; j++)
            printf("%c ", first[A][j]);

        puts("}");
    }

    puts("FOLLOW sets:");

    for (int i = 0; i < n; i++)
    {
        int A = prod[i][0] - 'A';

        printf("FOLLOW(%c)={", prod[i][0]);

        for (int j = 0; follow[A][j]; j++)
            printf("%c ", follow[A][j]);

        puts("}");
    }

    return 0;
}

/*
Sample Input:
Number of productions: 2
Enter productions, e.g. S=AB (use # for epsilon):
S=aA
A=b|#

Sample Output:
FIRST sets:
FIRST(S)={a }
FIRST(A)={b # }
FOLLOW sets:
FOLLOW(S)={$ }
FOLLOW(A)={$ }
*/