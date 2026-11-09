#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int num_states, num_symbols;
int nfa[20][20][20];
int nfa_trans_count[20][20];
int dfa[100][20];
int dfa_states[100][20];
int dfa_state_count = 0;

int final[20], dfa_final[100];

int is_equal(int a[], int b[], int n)
{
    for (int i = 0; i < n; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

int find_dfa_state(int state[], int n)
{
    for (int i = 0; i < dfa_state_count; i++)
    {
        if (is_equal(dfa_states[i], state, n))
            return i;
    }
    return -1;
}

int main()
{
    printf("Enter number of NFA states: ");
    scanf("%d", &num_states);

    printf("Enter number of input symbols: ");
    scanf("%d", &num_symbols);

    for (int i = 0; i < num_states; i++)
    {
        for (int j = 0; j < num_symbols; j++)
        {
            printf("Number of transitions from q%d on symbol %d: ", i, j);
            scanf("%d", &nfa_trans_count[i][j]);

            if (nfa_trans_count[i][j] > 0)
            {
                printf("Enter the next states: ");
                for (int k = 0; k < nfa_trans_count[i][j]; k++)
                {
                    scanf("%d", &nfa[i][j][k]);
                }
            }
        }
    }

    // Read final states of NFA
    int nf, state;

    printf("Enter number of final states: ");
    scanf("%d", &nf);

    printf("Enter final states: ");
    for (int i = 0; i < nf; i++)
    {
        scanf("%d", &state);
        final[state] = 1;
    }

    int start_state[20] = {0};
    start_state[0] = 1;

    for (int i = 0; i < num_states; i++)
        dfa_states[0][i] = start_state[i];

    dfa_state_count = 1;

    for (int i = 0; i < dfa_state_count; i++)
    {

        // Check whether DFA state is final
        for (int st = 0; st < num_states; st++)
        {
            if (dfa_states[i][st] && final[st])
                dfa_final[i] = 1;
        }

        for (int sym = 0; sym < num_symbols; sym++)
        {

            int new_state[20] = {0};

            for (int st = 0; st < num_states; st++)
            {
                if (dfa_states[i][st] == 1)
                {
                    for (int k = 0; k < nfa_trans_count[st][sym]; k++)
                    {
                        int target = nfa[st][sym][k];
                        new_state[target] = 1;
                    }
                }
            }

            int exists = find_dfa_state(new_state, num_states);

            if (exists == -1)
            {
                for (int st = 0; st < num_states; st++)
                {
                    dfa_states[dfa_state_count][st] = new_state[st];
                }

                dfa[i][sym] = dfa_state_count;
                dfa_state_count++;
            }
            else
            {
                dfa[i][sym] = exists;
            }
        }
    }

    printf("\nDFA TRANSITION TABLE\n\n");

    printf("%-15s %-22s", "DFA State", "NFA States");

    for (int j = 0; j < num_symbols; j++)
        printf("%-15c", 'a' + j);

    printf("%-12s\n", "Final");

    for (int i = 0; i < dfa_state_count; i++)
    {

        char str[100] = "{ ";

        for (int st = 0; st < num_states; st++)
        {
            if (dfa_states[i][st])
            {
                char temp[10];
                sprintf(temp, "q%d ", st);
                strcat(str, temp);
            }
        }

        strcat(str, "}");

        printf("%-15d %-22s", i, str);

        for (int j = 0; j < num_symbols; j++)
            printf("%-15d", dfa[i][j]);

        printf("%-12s\n", dfa_final[i] ? "Yes" : "No");
    }

    printf("\nFinal states of DFA: { ");

    for (int i = 0; i < dfa_state_count; i++)
        if (dfa_final[i])
            printf("%d ", i);

    printf("}\n");

    return 0;
}
