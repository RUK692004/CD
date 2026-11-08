
#include <stdio.h>
#define MAX 10

int n, sym, trans[MAX][MAX], final[MAX];
int mark[MAX][MAX], parent[MAX], reach[MAX];

int find(int x)
{
    if (parent[x] != x)
        parent[x] = find(parent[x]);
    return parent[x];
}

void unite(int x, int y)
{
    x = find(x);
    y = find(y);
    if (x != y)
        parent[y] = x;
}

int main()
{
    int i, j, k, x, y, start, changed;
    int queue[MAX], front = 0, rear = 0;

    printf("Enter number of states: ");
    scanf("%d", &n);

    printf("Enter number of symbols: ");
    scanf("%d", &sym);

    if (n < 1 || n > MAX || sym < 1 || sym > MAX)
    {
        printf("Invalid input\n");
        return 1;
    }

    printf("Enter transition table:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < sym; j++)
        {
            scanf("%d", &trans[i][j]);
            if (trans[i][j] < 0 || trans[i][j] >= n)
            {
                printf("Invalid transition\n");
                return 1;
            }
        }

    printf("Enter initial state: ");
    scanf("%d", &start);

    if (start < 0 || start >= n)
    {
        printf("Invalid initial state\n");
        return 1;
    }

    printf("Enter final states (0 or 1):\n");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &final[i]);
        if (final[i] != 0 && final[i] != 1)
        {
            printf("Invalid final-state value\n");
            return 1;
        }
    }

    // Step 1: Find reachable states using BFS
    queue[rear++] = start;
    reach[start] = 1;

    while (front < rear)
    {
        x = queue[front++];

        for (j = 0; j < sym; j++)
        {
            y = trans[x][j];

            if (!reach[y])
            {
                reach[y] = 1;
                queue[rear++] = y;
            }
        }
    }

    // Step 2: Initially mark final/non-final pairs
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (reach[i] && reach[j] &&
                final[i] != final[j])
                mark[i][j] = 1;

    // Step 3: Iteratively mark distinguishable pairs
    do
    {
        changed = 0;

        for (i = 0; i < n; i++)
        {
            if (!reach[i])
                continue;

            for (j = i + 1; j < n; j++)
            {
                if (!reach[j] || mark[i][j])
                    continue;

                for (k = 0; k < sym; k++)
                {
                    x = trans[i][k];
                    y = trans[j][k];

                    if (x > y)
                    {
                        int temp = x;
                        x = y;
                        y = temp;
                    }

                    if (mark[x][y])
                    {
                        mark[i][j] = 1;
                        changed = 1;
                        break;
                    }
                }
            }
        }
    } while (changed);

    // Step 4: Initialize DSU
    for (i = 0; i < n; i++)
        parent[i] = i;

    // Step 5: Merge equivalent reachable states
    printf("\nEquivalent State Pairs:\n");

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (reach[i] && reach[j] && !mark[i][j])
            {
                printf("(%d,%d)\n", i, j);
                unite(i, j);
            }

    // Step 6: Assign minimized state numbers
    int group[MAX], rep[MAX], count = 0;
    int minFinal[MAX], minTrans[MAX][MAX];

    for (i = 0; i < n; i++)
        group[i] = -1;

    for (i = 0; i < n; i++)
    {
        if (!reach[i])
            continue;

        x = find(i);

        if (group[x] == -1)
        {
            group[x] = count;
            rep[count] = i;
            minFinal[count] = final[i];
            count++;
        }
    }

    for (i = 0; i < n; i++)
        if (reach[i])
            group[i] = group[find(i)];

    // Step 7: Construct minimized transition table
    for (i = 0; i < count; i++)
        for (j = 0; j < sym; j++)
            minTrans[i][j] =
                group[trans[rep[i]][j]];

    // Step 8: Display minimized DFA
    printf("\nMinimized DFA States:\n");

    for (i = 0; i < count; i++)
    {
        printf("A%d = { ", i);

        for (j = 0; j < n; j++)
            if (reach[j] && group[j] == i)
                printf("q%d ", j);

        printf("}\n");
    }

    printf("\nInitial State: A%d\n", group[start]);

    printf("Final States: ");
    for (i = 0; i < count; i++)
        if (minFinal[i])
            printf("A%d ", i);

    printf("\n\nMinimized DFA Transition Table:\n");

    printf("State\t");
    for (j = 0; j < sym; j++)
        printf("%c\t", 'a' + j);
    printf("\n");

    for (i = 0; i < count; i++)
    {
        if (i == group[start])
            printf("->");
        if (minFinal[i])
            printf("*");

        printf("A%d\t", i);

        for (j = 0; j < sym; j++)
            printf("A%d\t", minTrans[i][j]);

        printf("\n");
    }

    return 0;
}
