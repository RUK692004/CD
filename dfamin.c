
#include <stdio.h>
#define MAX 10

int n, m, a[MAX][MAX], final[MAX], mark[MAX][MAX];
int reach[MAX], parent[MAX];

int find(int x)
{
    if (parent[x] != x)
        parent[x] = find(parent[x]);
    return parent[x];
}

int main()
{
    int i, j, k, x, y, s, change;
    int q[MAX], front = 0, rear = 0;
    int group[MAX], rep[MAX], count = 0;

    printf("Enter states and symbols: ");
    scanf("%d%d", &n, &m);

    printf("Enter transition table:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &a[i][j]);

    printf("Enter initial state: ");
    scanf("%d", &s);

    printf("Enter final states (0/1): ");
    for (i = 0; i < n; i++)
        scanf("%d", &final[i]);

    q[rear++] = s;
    reach[s] = 1;
    while (front < rear)
    {
        x = q[front++];
        for (j = 0; j < m; j++)
        {
            y = a[x][j];
            if (!reach[y])
            {
                reach[y] = 1;
                q[rear++] = y;
            }
        }
    }

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (final[i] != final[j])
                mark[i][j] = 1;

    do
    {
        change = 0;
        for (i = 0; i < n; i++)
        {
            if (!reach[i])
                continue;
            for (j = i + 1; j < n; j++)
            {
                if (!reach[j] || mark[i][j])
                    continue;
                for (k = 0; k < m; k++)
                {
                    x = a[i][k];
                    y = a[j][k];
                    if (x > y)
                    {
                        int t = x;
                        x = y;
                        y = t;
                    }
                    if (mark[x][y])
                    {
                        mark[i][j] = change = 1;
                        break;
                    }
                }
            }
        }
    } while (change);

    for (i = 0; i < n; i++)
        parent[i] = i;

    printf("\nEquivalent pairs:\n");
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (reach[i] && reach[j] && !mark[i][j])
            {
                printf("(%d,%d)\n", i, j);
                parent[find(j)] = find(i);
            }

    for (i = 0; i < n; i++)
        group[i] = -1;
    for (i = 0; i < n; i++)
        if (reach[i])
        {
            x = find(i);
            if (group[x] == -1)
            {
                group[x] = count;
                rep[count++] = i;
            }
        }

    for (i = 0; i < n; i++)
        if (reach[i])
            group[i] = group[find(i)];

    printf("\nMinimized DFA States:\n");
    for (i = 0; i < count; i++)
    {
        printf("A%d = { ", i);
        for (j = 0; j < n; j++)
            if (reach[j] && group[j] == i)
                printf("q%d ", j);
        printf("}\n");
    }

    printf("\nInitial State: A%d\n", group[s]);
    printf("Final States: ");
    for (i = 0; i < count; i++)
        if (final[rep[i]])
            printf("A%d ", i);

    printf("\n\nMinimized DFA Transition Table:\nState\t");
    for (j = 0; j < m; j++)
        printf("%c\t", 'a' + j);
    printf("\n");

    for (i = 0; i < count; i++)
    {
        if (i == group[s])
            printf("->");
        if (final[rep[i]])
            printf("*");
        printf("A%d\t", i);
        for (j = 0; j < m; j++)
            printf("A%d\t", group[a[rep[i]][j]]);
        printf("\n");
    }
    return 0;
}
