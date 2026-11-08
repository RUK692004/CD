
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 1024
#define TOK 128
#define QUADS 512

typedef struct
{
    char op[8], a[TOK], b[TOK], result[TOK];
} Quad;

Quad quad[QUADS];
int nq = 0, temp = 0, error = 0;
char input[MAX], token[TOK];
size_t pos = 0;
int kind;

enum
{
    END = 0,
    IDENT = 256,
    NUMBER,
    BAD
};

void fail(const char *s)
{
    if (!error)
        fprintf(stderr, "Invalid expression: %s at position %zu\n", s, pos);
    error = 1;
}

void next(void)
{
    while (isspace((unsigned char)input[pos]))
        pos++;

    size_t i = 0;
    unsigned char c = input[pos];

    if (!c)
    {
        kind = END;
        token[0] = 0;
        return;
    }

    if (isalpha(c) || c == '_')
    {
        while (isalnum((unsigned char)input[pos]) || input[pos] == '_')
        {
            if (i < TOK - 1)
                token[i++] = input[pos];
            else
                error = 1;
            pos++;
        }
        kind = IDENT;
    }
    else if (isdigit(c) || (c == '.' && isdigit((unsigned char)input[pos + 1])))
    {
        int dots = 0;
        while (isdigit((unsigned char)input[pos]) || input[pos] == '.')
        {
            if (input[pos] == '.')
                dots++;
            if (i < TOK - 1)
                token[i++] = input[pos];
            else
                error = 1;
            pos++;
        }
        kind = dots <= 1 ? NUMBER : BAD;
    }
    else
    {
        kind = c;
        token[i++] = c;
        pos++;
        if (!strchr("+-*/%()=", c))
            kind = BAD;
    }
    token[i] = '\0';
}

char *copy(const char *s)
{
    char *p = malloc(strlen(s) + 1);
    if (!p)
    {
        perror("malloc");
        exit(1);
    }
    strcpy(p, s);
    return p;
}

char *emit(const char *op, const char *a, const char *b)
{
    if (nq >= QUADS)
    {
        fail("too many instructions");
        return copy("?");
    }

    Quad *q = &quad[nq++];
    snprintf(q->op, sizeof(q->op), "%s", op);
    snprintf(q->a, TOK, "%s", a);
    snprintf(q->b, TOK, "%s", b);
    snprintf(q->result, TOK, "t%d", ++temp);

    return copy(q->result);
}

char *expression(void);

char *primary(void)
{
    if (error)
        return copy("?");

    if (kind == IDENT || kind == NUMBER)
    {
        char *v = copy(token);
        next();
        return v;
    }

    if (kind == '(')
    {
        next();
        char *v = expression();
        if (kind != ')')
            fail("missing closing parenthesis");
        else
            next();
        return v;
    }

    fail("expected operand");
    return copy("?");
}

char *unary(void)
{
    if (kind == '+' || kind == '-')
    {
        int op = kind;
        next();
        char *v = unary();

        if (op == '-')
        {
            char *t = emit("neg", v, "");
            free(v);
            return t;
        }
        return v;
    }
    return primary();
}

char *term(void)
{
    char *a = unary();

    while (!error && (kind == '*' || kind == '/' || kind == '%'))
    {
        char op[2] = {(char)kind, '\0'};
        next();

        char *b = unary();
        if (error)
        {
            free(b);
            break;
        }

        char *t = emit(op, a, b);
        free(a);
        free(b);
        a = t;
    }
    return a;
}

char *expression(void)
{
    char *a = term();

    while (!error && (kind == '+' || kind == '-'))
    {
        char op[2] = {(char)kind, '\0'};
        next();

        char *b = term();
        if (error)
        {
            free(b);
            break;
        }

        char *t = emit(op, a, b);
        free(a);
        free(b);
        a = t;
    }
    return a;
}

int main(void)
{
    printf("Enter expression: ");
    if (!fgets(input, sizeof(input), stdin))
        return 1;

    next();
    char dest[TOK] = "";

    if (kind == IDENT)
    {
        char first[TOK];
        strcpy(first, token);
        next();

        if (kind == '=')
        {
            strcpy(dest, first);
            next();
        }
        else
        {
            pos = 0;
            next();
        }
    }

    char *result = expression();

    if (!error && kind != END)
        fail("unexpected token");

    if (error)
    {
        free(result);
        return 1;
    }

    if (dest[0])
    {
        if (nq >= QUADS)
        {
            fail("too many instructions");
            free(result);
            return 1;
        }

        Quad *q = &quad[nq++];
        strcpy(q->op, "=");
        strcpy(q->a, result);
        q->b[0] = '\0';
        strcpy(q->result, dest);
    }

    printf("\nThree Address Code:\n");

    for (int i = 0; i < nq; i++)
    {
        Quad *q = &quad[i];

        if (!strcmp(q->op, "="))
            printf("%s = %s\n", q->result, q->a);
        else if (!strcmp(q->op, "neg"))
            printf("%s = -%s\n", q->result, q->a);
        else
            printf("%s = %s %s %s\n",
                   q->result, q->a, q->op, q->b);
    }

    if (!nq)
        printf("(no intermediate operations)\n");

    printf("Result stored in %s\n", dest[0] ? dest : result);

    printf("\nQuadruples:\n");
    printf("Index\tOp\tArg1\tArg2\tResult\n");

    for (int i = 0; i < nq; i++)
    {
        printf("%d\t%s\t%s\t%s\t%s\n",
               i, quad[i].op, quad[i].a,
               quad[i].b, quad[i].result);
    }

    free(result);
    return 0;
}

/*
sample Input:
total = -(a + 12.5) * num2 + 7 % 3

Output:

Three Address Code:
t1 = a + 12.5
t2 = -t1
t3 = t2 * num2
t4 = 7 % 3
t5 = t3 + t4
total = t5
Result stored in total

Quadruples:
Index   Op    Arg1   Arg2   Result
0       +     a      12.5   t1
1       neg   t1            t2
2       *     t2     num2   t3
3       %     7      3      t4
4       +     t3     t4     t5
5       =     t5            total



*/