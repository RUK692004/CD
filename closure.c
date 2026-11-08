#include <stdio.h>
#define MAX 20

int n, epsilon[MAX][MAX], visited[MAX];

void findEpsilonClosure(int state)
{
    visited[state] = 1;

    for (int i = 0; i < n; i++)
    {
        if (epsilon[state][i] == 1 && !visited[i])
        {
            findEpsilonClosure(i);
        }
    }
}

int main()
{
    printf("Enter the number of states: ");
    scanf("%d", &n);

    printf("Enter the epsilon transition matrix (%d x %d):\n", n, n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &epsilon[i][j]);
        }
    }

    for (int s = 0; s < n; s++)
    {
        for (int i = 0; i < n; i++)
        {
            visited[i] = 0;
        }

        findEpsilonClosure(s);

        printf("Epsilon-closure of state %d: { ", s);

        for (int i = 0; i < n; i++)
        {
            if (visited[i])
            {
                printf("%d ", i);
            }
        }

        printf("}\n");
    }

    return 0;
}