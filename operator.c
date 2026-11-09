
#include <stdio.h>
#include <string.h>

char stack[100], input[100];
int top = 0, ip = 0;

int prec(char c)
{
    if (c == '+' || c == '-')
        return 1;
    if (c == '*' || c == '/')
        return 2;
    return 0;
}

void display()
{
    for (int i = 0; i <= top; i++)
        printf("%c", stack[i]);
    printf("\t\t%s\n", input + ip);
}

int reduce()
{
    if (stack[top] == 'i')
        stack[top] = 'E';
    else if (top >= 3 && stack[top] == 'E' &&
             stack[top - 2] == 'E' && prec(stack[top - 1]))
    {
        top -= 2;
        stack[top] = 'E';
    }
    else
        return 0;
    display();
    return 1;
}

int main()
{
    int valid = 1, expect = 1;
    char c;

    printf("Enter expression (use i for id): ");
    scanf("%98s", input);
    strcat(input, "$");
    stack[0] = '$';

    printf("\nSTACK\t\tINPUT\n");
    display();

    while ((c = input[ip]) != '$')
    {
        if (c == 'i' || c == '(')
        {
            if (!expect)
            {
                valid = 0;
                break;
            }
            stack[++top] = c;
            ip++;
            display();
            if (c == 'i')
            {
                reduce();
                expect = 0;
            }
        }
        else if (c == ')')
        {
            if (expect)
            {
                valid = 0;
                break;
            }
            while (top >= 2 && !(stack[top - 1] == '(' && stack[top] == 'E'))
                if (!reduce())
                    break;
            if (top < 2 || stack[top - 1] != '(' || stack[top] != 'E')
            {
                valid = 0;
                break;
            }
            stack[--top] = 'E';
            ip++;
            display();
        }
        else if (prec(c))
        {
            if (expect)
            {
                valid = 0;
                break;
            }
            while (top >= 3 && stack[top] == 'E' &&
                   prec(stack[top - 1]) >= prec(c))
                if (!reduce())
                    break;
            stack[++top] = c;
            ip++;
            display();
            expect = 1;
        }
        else
        {
            valid = 0;
            break;
        }
    }

    if (expect)
        valid = 0;
    while (valid && top > 1)
        if (!reduce())
        {
            valid = 0;
            break;
        }

    printf("\n%s\n", valid && top == 1 && stack[1] == 'E'
                         ? "Accepted"
                         : "Rejected");
    return 0;
}

// Enter expression (use i for id): i*(i+i)
// STACK           INPUT
// $               i*(i+i)$
// $i              *(i+i)$
// $E              *(i+i)$
// $E*             (i+i)$
// $E*(            i+i)$
// $E*(i           +i)$
// $E*(E           +i)$
// $E*(E+          i)$
// $E*(E+i         )$
// $E*(E+E         )$
// $E*(E           )$
// $E*E            $
// $E              $

// Accepted