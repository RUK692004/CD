#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#define MAXLINES 100
#define MAXVARS 200
#define MAXLABELS 100
#define LEN 128
char vars[MAXVARS][LEN], labels[MAXLABELS][LEN];
int nv = 0, nl = 0, failed = 0;
void die(int line, const char *msg)
{
    fprintf(stderr, "Error on TAC line %d: %s\n", line, msg);
    failed = 1;
}
void trim(char *s)
{
    char *p = s;
    while (isspace((unsigned char)*p))
        p++;
    memmove(s, p, strlen(p) + 1);
    size_t n = strlen(s);
    while (n && isspace((unsigned char)s[n - 1]))
        s[--n] = 0;
}
int identifier(const char *s)
{
    if (!isalpha((unsigned char)*s) && *s != '_')
        return 0;
    for (s++; *s; s++)
        if (!isalnum((unsigned char)*s) && *s != '_')
            return 0;
    return 1;
}
int number(const char *s)
{
    if (*s == '+' || *s == '-')
        s++;
    if (!isdigit((unsigned char)*s))
        return 0;
    long v = 0;
    while (isdigit((unsigned char)*s))
    {
        v = v * 10 + (*s++ - '0');
        if (v > 32768)
            return 0;
    }
    return !*s && v <= 32768;
}
int operand(const char *s) { return identifier(s) || number(s); }
void addvar(const char *s, int line)
{
    if (number(s))
        return;
    if (!identifier(s))
    {
        die(line, "invalid variable");
        return;
    }
    for (int i = 0; i < nv; i++)
        if (!strcmp(vars[i], s))
            return;
    if (nv == MAXVARS)
    {
        die(line, "too many variables");
        return;
    }
    strcpy(vars[nv++], s);
}
void addlabel(const char *s, int line)
{
    if (!identifier(s))
    {
        die(line, "invalid label");
        return;
    }
    for (int i = 0; i < nl; i++)
        if (!strcmp(labels[i], s))
        {
            die(line, "duplicate label");
            return;
        }
    if (nl == MAXLABELS)
    {
        die(line, "too many labels");
        return;
    }
    strcpy(labels[nl++], s);
}
int haslabel(const char *s)
{
    for (int i = 0; i < nl; i++)
        if (!strcmp(labels[i], s))
            return 1;
    return 0;
}
void compact(char *dst, const char *src)
{
    while (*src)
    {
        if (!isspace((unsigned char)*src))
            *dst++ = *src;
        src++;
    }
    *dst = 0;
}
int splitop(const char *s, char *a, char *op, char *b)
{
    int k = -1;
    for (int i = 1; s[i]; i++)
        if (strchr("+-*/%", s[i]))
        {
            k = i;
            break;
        }
    if (k < 0)
        return 0;
    size_t la = k, lb = strlen(s + k + 1);
    if (!la || !lb || la >= LEN || lb >= LEN)
        return 0;
    memcpy(a, s, la);
    a[la] = 0;
    strcpy(b, s + k + 1);
    op[0] = s[k];
    op[1] = 0;
    return operand(a) && operand(b);
}
int parse(int line, char *src, int output)
{
    char s[512], a[LEN], b[LEN], r[LEN], op[4], lbl[LEN];
    compact(s, src);
    if (!*s)
        return 1;
    size_t z = strlen(s);
    if (s[z - 1] == ':')
    {
        s[z - 1] = 0;
        if (!identifier(s))
        {
            die(line, "invalid label");
            return 0;
        }
        if (!output)
            addlabel(s, line);
        else
            printf("%s:\n", s);
        return 1;
    }
    if (!strncmp(s, "goto", 4))
    {
        if (!identifier(s + 4))
        {
            die(line, "invalid goto label");
            return 0;
        }
        if (output)
            printf("    JMP %s\n", s + 4);
        return 1;
    }
    if (!strncmp(s, "if", 2))
    {
        char *p = s + 2, *g = strstr(p, "goto");
        if (!g)
        {
            die(line, "expected goto in condition");
            return 0;
        }
        if (strlen(g + 4) >= LEN || !identifier(g + 4))
        {
            die(line, "invalid jump label");
            return 0;
        }
        strcpy(lbl, g + 4);
        *g = 0;
        char *q = NULL;
        const char *rels[] = {"<=", ">=", "==", "!=", "<", ">"};
        const char *jmps[] = {"JLE", "JGE", "JE", "JNE", "JL", "JG"};
        int ri = -1;
        for (int i = 0; i < 6; i++)
        {
            q = strstr(p, rels[i]);
            if (q)
            {
                ri = i;
                break;
            }
        }
        if (ri < 0)
        {
            die(line, "invalid relational operator");
            return 0;
        }
        size_t len = q - p;
        if (!len || len >= LEN || strlen(q + strlen(rels[ri])) >= LEN)
        {
            die(line, "invalid condition operands");
            return 0;
        }
        memcpy(a, p, len);
        a[len] = 0;
        strcpy(b, q + strlen(rels[ri]));
        if (!operand(a) || !operand(b))
        {
            die(line, "invalid condition operands");
            return 0;
        }
        if (!output)
        {
            addvar(a, line);
            addvar(b, line);
        }
        else
        {
            printf("    MOV AX, %s\n    CMP AX, %s\n    %s %s\n", a, b, jmps[ri], lbl);
        }
        return 1;
    }
    char *eq = strchr(s, '=');
    if (!eq || strchr(eq + 1, '='))
    {
        die(line, "expected assignment");
        return 0;
    }
    *eq = 0;
    if (strlen(s) >= LEN || !identifier(s))
    {
        die(line, "invalid assignment target");
        return 0;
    }
    strcpy(r, s);
    char *rhs = eq + 1;
    if (!*rhs)
    {
        die(line, "missing expression");
        return 0;
    }
    if (operand(rhs))
    {
        if (!output)
        {
            addvar(r, line);
            addvar(rhs, line);
        }
        else
            printf("    MOV AX, %s\n    MOV %s, AX\n", rhs, r);
        return 1;
    }
    if ((*rhs == '-' || *rhs == '+') && operand(rhs + 1))
    {
        if (!output)
        {
            addvar(r, line);
            addvar(rhs + 1, line);
        }
        else
        {
            printf("    MOV AX, %s\n", rhs + 1);
            if (*rhs == '-')
                puts("    NEG AX");
            printf("    MOV %s, AX\n", r);
        }
        return 1;
    }
    if (!splitop(rhs, a, op, b))
    {
        die(line, "unsupported arithmetic expression (use TAC temporaries)");
        return 0;
    }
    if (!output)
    {
        addvar(r, line);
        addvar(a, line);
        addvar(b, line);
    }
    else
    {
        printf("    MOV AX, %s\n", a);
        switch (*op)
        {
        case '+':
            printf("    ADD AX, %s\n", b);
            break;
        case '-':
            printf("    SUB AX, %s\n", b);
            break;
        case '*':
            printf("    MOV BX, %s\n    IMUL BX\n", b);
            break;
        case '/':
        case '%':
            printf("    CWD\n    MOV BX, %s\n    IDIV BX\n", b);
            break;
        }
        printf("    MOV %s, %s\n", r, *op == '%' ? "DX" : "AX");
    }
    return 1;
}
int main(void)
{
    char lines[MAXLINES][512], buf[64];
    int n;
    printf("Number of TAC statements: ");
    if (!fgets(buf, sizeof(buf), stdin) || sscanf(buf, "%d", &n) != 1 || n < 1 || n > MAXLINES)
    {
        puts("Invalid statement count");
        return 1;
    }
    puts("Enter TAC statements:");
    for (int i = 0; i < n; i++)
    {
        if (!fgets(lines[i], sizeof(lines[i]), stdin))
        {
            puts("Missing TAC line");
            return 1;
        }
        trim(lines[i]);
    }
    for (int i = 0; i < n; i++)
        if (!parse(i + 1, lines[i], 0))
            failed = 1;
    for (int i = 0; i < n; i++)
    {
        char s[512];
        compact(s, lines[i]);
        char *target = NULL;
        if (!strncmp(s, "goto", 4))
            target = s + 4;
        else if (!strncmp(s, "if", 2))
        {
            char *g = strstr(s, "goto");
            if (g)
                target = g + 4;
        }
        if (target && !haslabel(target))
            die(i + 1, "undefined jump label");
    }
    if (failed)
        return 1;
    puts("\nDATA SEGMENT");
    for (int i = 0; i < nv; i++)
        printf("    %s DW ?\n", vars[i]);
    puts("DATA ENDS\nCODE SEGMENT\n    ASSUME CS:CODE, DS:DATA\nSTART:\n    MOV AX, DATA\n    MOV DS, AX");
    for (int i = 0; i < n; i++)
        parse(i + 1, lines[i], 1);
    puts("    MOV AX, 4C00H\n    INT 21H\nCODE ENDS\nEND START");
    return 0;
}

/*
Sample Input:
Number of TAC statements: 8
Enter TAC statements:
a = 10
b = -3
t1 = a + b
if t1 >= 5 goto L1
t2 = t1 % 2
goto L2
L1:
L2:

Output:
DATA SEGMENT
    a DW ?
    b DW ?
    t1 DW ?
    t2 DW ?
DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX
    MOV AX, 10
    MOV a, AX
    MOV AX, -3
    MOV b, AX
    MOV AX, a
    ADD AX, b
    MOV t1, AX
    MOV AX, t1
    CMP AX, 5
    JGE L1
    MOV AX, t1
    CWD
    MOV BX, 2
    IDIV BX
    MOV t2, DX
    JMP L2
L1:
L2:
    MOV AX, 4C00H
    INT 21H
CODE ENDS
END START



*/