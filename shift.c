
#include <stdio.h>
#include <string.h>
#define MAX 100

char stack[MAX], input[MAX];
int top = 0, ip = 0;

int prec(char c)
{
    if (c == '+')
        return 1;
    if (c == '*')
        return 2;
    return 0;
}

void display(char *action)
{
    for (int i = 0; i <= top; i++)
        printf("%c", stack[i]);
    printf("\t\t%s\t%s\n", input + ip, action);
}

int reduce()
{
    char look = input[ip], op;

    if (stack[top] == 'i')
    {
        stack[top] = 'E';
        display("REDUCE E->i");
        return 1;
    }

    if (top >= 3 && stack[top - 2] == '(' &&
        stack[top - 1] == 'E' && stack[top] == ')')
    {
        top -= 2;
        stack[top] = 'E';
        display("REDUCE E->(E)");
        return 1;
    }

    if (top >= 3 && stack[top] == 'E' && stack[top - 2] == 'E' && prec(stack[top - 1]) &&
        (look == '$' || look == ')' || prec(stack[top - 1]) >= prec(look)))
    {
        op = stack[top - 1];
        top -= 2;
        stack[top] = 'E';
        display(op == '+' ? "REDUCE E->E+E" : "REDUCE E->E*E");
        return 1;
    }
    return 0;
}

int main()
{
    printf("Enter expression (use i for identifier): ");
    scanf("%98s", input);
    strcat(input, "$");
    stack[0] = '$';

    printf("\nSTACK\t\tINPUT\t\tACTION\n");
    display("INITIAL");

    while (input[ip] != '$')
    {
        while (reduce())
            ;
        stack[++top] = input[ip++];
        display("SHIFT");
    }

    while (reduce())
        ;
    printf("\n%s\n", top == 1 && stack[top] == 'E' ? "Accepted" : "Rejected");
    return 0;
}

// Enter expression (use i for identifier): i+i*i
// STACK           INPUT           ACTION
// $               i+i*i$          INITIAL
// $i              +i*i$           SHIFT
// $E              +i*i$           REDUCE E->i
// $E+             i*i$            SHIFT
// $E+i            *i$             SHIFT
// $E+E            *i$             REDUCE E->i
// $E+E*           i$              SHIFT
// $E+E*i          $               SHIFT
// $E+E*E          $               REDUCE E->i
// $E+E            $               REDUCE E->E*E
// $E              $               REDUCE E->E+E

// Accepted