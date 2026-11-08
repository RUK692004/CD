// OPERATOR PRECEDENCE PARSER
#include <stdio.h>
#include <string.h>

char stack[100], input[100];
int top = 0, ip = 0;

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;

    if (op == '*' || op == '/')
        return 2;

    return 0;
}

void display()
{
    for (int i = 0; i <= top; i++)
        printf("%c", stack[i]);

    printf("\t\t%s\n", input + ip);
}

void reduce()
{
    // i -> E
    if (stack[top] == 'i')
    {
        stack[top] = 'E';
        display();
        return;
    }

    // E op E -> E
    if (top >= 3 &&
        stack[top] == 'E' &&
        stack[top - 2] == 'E' &&
        precedence(stack[top - 1]) > 0)
    {
        top -= 2;
        stack[top] = 'E';
        display();
    }
}

int main()
{
    int valid = 1, expectOperand = 1;

    printf("Enter expression (use i for id): ");
    scanf("%99s", input);

    strcat(input, "$");

    stack[0] = '$';

    printf("\nSTACK\t\tINPUT\n");
    display();

    while (input[ip] != '$')
    {
        char c = input[ip];

        // Identifier
        if (c == 'i')
        {
            if (!expectOperand)
            {
                valid = 0;
                break;
            }

            stack[++top] = c;
            ip++;
            display();

            reduce();
            expectOperand = 0;
        }

        // Opening parenthesis
        else if (c == '(')
        {
            if (!expectOperand)
            {
                valid = 0;
                break;
            }

            stack[++top] = c;
            ip++;
            display();

            expectOperand = 1;
        }

        // Closing parenthesis

        // Closing parenthesis
        else if (c == ')')
        {
            if (expectOperand)
            {
                valid = 0;
                break;
            }

            // Reduce until (E is found
            while (!(top >= 1 &&
                     stack[top - 1] == '(' &&
                     stack[top] == 'E'))
            {
                int oldTop = top;
                reduce();

                if (top == oldTop)
                {
                    valid = 0;
                    break;
                }
            }

            if (!valid)
                break;

            // Replace (E with E
            stack[top - 1] = 'E';
            top--;

            ip++;
            display();

            expectOperand = 0;
        }

        // Arithmetic operators
        else if (c == '+' || c == '-' ||
                 c == '*' || c == '/')
        {
            if (expectOperand)
            {
                valid = 0;
                break;
            }

            // Reduce operators of higher or equal precedence
            while (top >= 3 &&
                   stack[top] == 'E' &&
                   precedence(stack[top - 1]) >= precedence(c))
            {
                int oldTop = top;
                reduce();

                if (top == oldTop)
                {
                    valid = 0;
                    break;
                }
            }

            if (!valid)
                break;

            stack[++top] = c;
            ip++;
            display();

            expectOperand = 1;
        }

        else
        {
            valid = 0;
            break;
        }
    }

    if (expectOperand)
        valid = 0;

    // Perform remaining reductions
    while (valid && top > 1)
    {
        int oldTop = top;
        reduce();

        if (top == oldTop)
        {
            valid = 0;
            break;
        }
    }

    // Final acceptance condition
    if (valid && top == 1 && stack[top] == 'E')
    {
        printf("\nAccepted\n");
    }
    else
    {
        printf("\nRejected\n");
    }

    return 0;
}

/*
Sample Input:
Enter expression (use i for id): i*(i+i)

Sample Output:
STACK           INPUT
$               i*(i+i)$
$i              *(i+i)$
$E              *(i+i)$
$E*             (i+i)$
$E*(            i+i)$
$E*(i           +i)$
$E*(E           +i)$
$E*(E+          i)$
$E*(E+i         )$
$E*(E+E         )$
$E*(E           )$
$E*E            $
$E              $

Accepted
*/