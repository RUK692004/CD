#include <stdio.h>
#include <string.h>
#include <ctype.h>

char infix[100], postfix[100], ops[100];
int top = -1;

int prec(char c)
{
    if (c == '*' || c == '/')
        return 2;

    if (c == '+' || c == '-')
        return 1;

    return 0;
}

int main()
{
    printf("Enter expression: ");
    scanf("%99s", infix);

    int k = 0;

    for (int i = 0; infix[i]; i++)
    {
        char c = infix[i];

        if (isalnum((unsigned char)c))
            postfix[k++] = c;

        else if (c == '(')
            ops[++top] = c;

        else if (c == ')')
        {
            while (top >= 0 && ops[top] != '(')
                postfix[k++] = ops[top--];

            if (top < 0)
            {
                puts("Invalid expression");
                return 0;
            }

            top--;
        }
        else if (c == '+' || c == '-' ||
                 c == '*' || c == '/')
        {
            while (top >= 0 && ops[top] != '(' &&
                   prec(ops[top]) >= prec(c))
                postfix[k++] = ops[top--];

            ops[++top] = c;
        }
        else
        {
            puts("Invalid symbol");
            return 0;
        }
    }

    while (top >= 0)
    {
        if (ops[top] == '(')
        {
            puts("Invalid expression");
            return 0;
        }

        postfix[k++] = ops[top--];
    }

    postfix[k] = '\0';

    char val[100][20];
    char quad[100][4][20];

    int sp = -1, t = 1, q = 0;

    printf("Postfix: %s\n", postfix);
    puts("Three Address Code:");

    for (int i = 0; postfix[i]; i++)
    {
        char c = postfix[i];

        if (isalnum((unsigned char)c))
        {
            val[++sp][0] = c;
            val[sp][1] = '\0';
        }
        else
        {
            if (sp < 1)
            {
                puts("Invalid expression");
                return 0;
            }

            char b[20], a[20], tmp[20];

            strcpy(b, val[sp--]);
            strcpy(a, val[sp--]);

            sprintf(tmp, "t%d", t++);

            printf("%s = %s %c %s\n", tmp, a, c, b);

            /* Store quadruple */
            strcpy(quad[q][0], (char[]){c, '\0'});
            strcpy(quad[q][1], a);
            strcpy(quad[q][2], b);
            strcpy(quad[q][3], tmp);
            q++;

            strcpy(val[++sp], tmp);
        }
    }

    /* Print quadruples */
    puts("\nQuadruples:");
    printf("Index\tOp\tArg1\tArg2\tResult\n");

    for (int i = 0; i < q; i++)
    {
        printf("%d\t%s\t%s\t%s\t%s\n",
               i, quad[i][0], quad[i][1],
               quad[i][2], quad[i][3]);
    }

    return 0;
}

/*
Sample Input:
Enter expression: (a+b)*c

Sample Output:
Postfix: ab+c*
Three Address Code:
t1 = a + b
t2 = t1 * c

Quadruples:
Index	Op	Arg1	Arg2	Result
0	+	a	b	t1
1	*	t1	c	t2
*/