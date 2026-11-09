
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char p[20][100], first[26][30], follow[26][30];
int n;

int add(char s[], char c)
{
    if (strchr(s, c))
        return 0;
    int l = strlen(s);
    s[l] = c;
    s[l + 1] = 0;
    return 1;
}

int main()
{
    int i, j, k, A, B, C, change;
    char *r, c;

    printf("Enter number of productions: ");
    scanf("%d", &n);
    printf("Enter productions:\n");
    for (i = 0; i < n; i++)
        scanf("%99s", p[i]);

    do
    {
        change = 0;
        for (i = 0; i < n; i++)
        {
            A = p[i][0] - 'A';
            r = p[i] + 2;
            int nullable = 1;
            for (j = 0;; j++)
            {
                c = r[j];
                if (c == '|' || c == 0)
                {
                    if (nullable)
                        change |= add(first[A], '#');
                    nullable = 1;
                    if (!c)
                        break;
                }
                else if (c == '#')
                    change |= add(first[A], '#');
                else if (!isupper(c))
                {
                    if (nullable)
                        change |= add(first[A], c);
                    nullable = 0;
                }
                else if (nullable)
                {
                    B = c - 'A';
                    for (k = 0; first[B][k]; k++)
                        if (first[B][k] != '#')
                            change |= add(first[A], first[B][k]);
                    if (!strchr(first[B], '#'))
                        nullable = 0;
                }
            }
        }
    } while (change);

    add(follow[p[0][0] - 'A'], '$');

    do
    {
        change = 0;
        for (i = 0; i < n; i++)
        {
            A = p[i][0] - 'A';
            r = p[i] + 2;
            for (j = 0; r[j]; j++)
            {
                if (!isupper(r[j]))
                    continue;
                B = r[j] - 'A';
                int nullable = 1;

                for (k = j + 1; r[k] && r[k] != '|'; k++)
                {
                    c = r[k];
                    if (c == '#')
                        continue;
                    if (!isupper(c))
                    {
                        change |= add(follow[B], c);
                        nullable = 0;
                        break;
                    }
                    C = c - 'A';
                    for (int t = 0; first[C][t]; t++)
                        if (first[C][t] != '#')
                            change |= add(follow[B], first[C][t]);
                    if (!strchr(first[C], '#'))
                    {
                        nullable = 0;
                        break;
                    }
                }
                if (nullable)
                    for (k = 0; follow[A][k]; k++)
                        change |= add(follow[B], follow[A][k]);
            }
        }
    } while (change);

    printf("\nFIRST sets:\n");
    for (i = 0; i < n; i++)
    {
        A = p[i][0] - 'A';
        printf("FIRST(%c)={%s}\n", p[i][0], first[A]);
    }

    printf("\nFOLLOW sets:\n");
    for (i = 0; i < n; i++)
    {
        A = p[i][0] - 'A';
        printf("FOLLOW(%c)={%s}\n", p[i][0], follow[A]);
    }
    return 0;
}
/*
Enter number of productions: 2
Enter productions:
S=aA
A=b|#

FIRST sets:
FIRST(S)={a}
FIRST(A)={b#}

FOLLOW sets:
FOLLOW(S)={$}
FOLLOW(A)={$}
*/