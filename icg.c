
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char in[100], post[100], ops[100];
char val[100][20], quad[100][4][20];
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
    int i, k = 0, sp = -1, t = 1, q = 0;
    char c, a[20], b[20], tmp[20];

    printf("Enter expression: ");
    scanf("%99s", in);

    for (i = 0; in[i]; i++)
    {
        c = in[i];
        if (isalnum(c))
            post[k++] = c;
        else if (c == '(')
            ops[++top] = c;
        else if (c == ')')
        {
            while (top >= 0 && ops[top] != '(')
                post[k++] = ops[top--];
            top--;
        }
        else
        {
            while (top >= 0 && ops[top] != '(' && prec(ops[top]) >= prec(c))
                post[k++] = ops[top--];
            ops[++top] = c;
        }
    }

    while (top >= 0)
        post[k++] = ops[top--];
    post[k] = 0;

    printf("Postfix: %s\nThree Address Code:\n", post);

    for (i = 0; post[i]; i++)
    {
        c = post[i];
        if (isalnum(c))
        {
            val[++sp][0] = c;
            val[sp][1] = 0;
        }
        else
        {
            strcpy(b, val[sp--]);
            strcpy(a, val[sp--]);
            sprintf(tmp, "t%d", t++);
            printf("%s = %s %c %s\n", tmp, a, c, b);

            sprintf(quad[q][0], "%c", c);
            strcpy(quad[q][1], a);
            strcpy(quad[q][2], b);
            strcpy(quad[q][3], tmp);
            q++;

            strcpy(val[++sp], tmp);
        }
    }

    printf("\nQuadruples:\nIndex\tOp\tArg1\tArg2\tResult\n");
    for (i = 0; i < q; i++)
        printf("%d\t%s\t%s\t%s\t%s\n",
               i, quad[i][0], quad[i][1], quad[i][2], quad[i][3]);

    return 0;
}

/*
Enter expression: (a+b)*c

Postfix: ab+c*
Three Address Code:
t1 = a + b
t2 = t1 * c

Quadruples:
Index   Op   Arg1   Arg2   Result
0       +    a      b      t1
1       *    t1     c      t2
*/