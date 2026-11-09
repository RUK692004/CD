
#include <stdio.h>
#define MAX 20

int n, m, nfa[20][20][20], count[20][20];
int states[100][20], dfa[100][20], final[20], dfinal[100], total = 1;

int find(int a[])
{
    for (int i = 0; i < total; i++)
    {
        int j;
        for (j = 0; j < n; j++)
            if (states[i][j] != a[j])
                break;
        if (j == n)
            return i;
    }
    return -1;
}

int main()
{
    int i, j, k, s, x, nf;

    printf("Enter number of states and symbols: ");
    scanf("%d%d", &n, &m);

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
        {
            printf("d(q%d,%d) count: ", i, j);
            scanf("%d", &count[i][j]);
            for (k = 0; k < count[i][j]; k++)
                scanf("%d", &nfa[i][j][k]);
        }

    printf("Enter number of final states: ");
    scanf("%d", &nf);
    printf("Enter final states: ");
    while (nf--)
    {
        scanf("%d", &s);
        final[s] = 1;
    }

    states[0][0] = 1;

    for (i = 0; i < total; i++)
    {
        for (s = 0; s < n; s++)
            if (states[i][s] && final[s])
                dfinal[i] = 1;

        for (j = 0; j < m; j++)
        {
            int next[MAX] = {0};

            for (s = 0; s < n; s++)
                if (states[i][s])
                    for (k = 0; k < count[s][j]; k++)
                        next[nfa[s][j][k]] = 1;

            x = find(next);
            if (x == -1)
            {
                x = total++;
                for (s = 0; s < n; s++)
                    states[x][s] = next[s];
            }
            dfa[i][j] = x;
        }
    }

    printf("\nDFA TRANSITION TABLE\n");
    printf("State\tNFA States\t");
    for (j = 0; j < m; j++)
        printf("%c\t", 'a' + j);
    printf("Final\n");

    for (i = 0; i < total; i++)
    {
        printf("%d\t{", i);
        for (s = 0; s < n; s++)
            if (states[i][s])
                printf("q%d ", s);
        printf("}\t\t");

        for (j = 0; j < m; j++)
            printf("%d\t", dfa[i][j]);
        printf("%s\n", dfinal[i] ? "Yes" : "No");
    }

    printf("\nFinal DFA states: { ");
    for (i = 0; i < total; i++)
        if (dfinal[i])
            printf("%d ", i);
    printf("}\n");

    return 0;
}
