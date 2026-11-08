//SHIFT REDUCE PARSER

#include <stdio.h>
#include<string.h>

#define MAX 100

char stack[MAX], input[MAX];
int top = 0, ip = 0;

int precedence(char c)
{
    if(c == '+') return 1;
    if(c == '*') return 2;
    return 0;
}

void display(char *action)
{
    for(int i = 0; i <= top; i++)
        printf("%c", stack[i]);

    printf("\t\t%s\t%s\n", input + ip, action);
}

int reduce()
{
    char look = input[ip];

    // i -> E
    if(stack[top] == 'i')
    {
        stack[top] = 'E';
        display("REDUCE E->i");
        return 1;
    }

    // (E) -> E
    if(top >= 3 &&
       stack[top-2] == '(' &&
       stack[top-1] == 'E' &&
       stack[top] == ')')
    {
        top -= 2;
        stack[top] = 'E';
        display("REDUCE E->(E)");
        return 1;
    }

    // E+E -> E or E*E -> E
    if(top >= 3 &&
       stack[top] == 'E' &&
       stack[top-2] == 'E' &&
       (stack[top-1] == '+' || stack[top-1] == '*'))
    {
        char op = stack[top-1];

        if(look == '$' || look == ')' || ((look == '+' || look == '*') && precedence(op) >= precedence(look)))
        {
            top -= 2;
            stack[top] = 'E';

            if(op == '+')
                display("REDUCE E->E+E");
            else
                display("REDUCE E->E*E");

            return 1;
        }
    }

    return 0;
}

int main()
{
    printf("Enter expression (use i for identifier): ");
    scanf("%s", input);
    
    strcat(input, "$");
    
    stack[0] = '$';

    printf("\nSTACK\t\tINPUT\t\tACTION\n");
    display("INITIAL");

    while(input[ip] != '$')
    {
        while(reduce());

        stack[++top] = input[ip++];
        display("SHIFT");
    }

    // Final reductions
    while(reduce());

    if(top == 1 && stack[top] == 'E')
        printf("\nAccepted\n");
    else
        printf("\nRejected\n");

    return 0;
}
