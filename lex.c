
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int keyword(char s[])
{
    char *k[] = {"char", "double", "float", "int", "void", "case", "default",
                 "else", "if", "switch", "do", "for", "while", "break", "continue",
                 "goto", "return", "long", "short", "signed", "unsigned", "auto",
                 "extern", "register", "static", "const", "enum", "sizeof",
                 "struct", "typedef", "union", "volatile"};
    for (int i = 0; i < 32; i++)
        if (!strcmp(s, k[i]))
            return 1;
    return 0;
}

int main()
{
    char str[1000], word[100], op[3];
    int i = 0, j, ch;

    printf("Enter source code:\n");
    while ((ch = getchar()) != EOF && i < 999)
        str[i++] = ch;
    str[i] = 0;

    for (i = 0; str[i];)
    {
        if (isspace(str[i]))
        {
            i++;
            continue;
        }

        if (isalpha(str[i]) || str[i] == '_')
        {
            j = 0;
            while (isalnum(str[i]) || str[i] == '_')
            {
                if (j < 99)
                    word[j++] = str[i];
                i++;
            }
            word[j] = 0;
            printf("%s : %s\n", word, keyword(word) ? "Keyword" : "Identifier");
        }
        else if (isdigit(str[i]))
        {
            j = 0;
            while (isdigit(str[i]) || (str[i] == '.' && isdigit(str[i + 1])))
            {
                if (j < 99)
                    word[j++] = str[i];
                i++;
            }
            word[j] = 0;
            printf("%s : Number\n", word);
        }
        else if (strchr("+-*/%=<>!&|^", str[i]))
        {
            op[0] = str[i++];
            op[1] = op[2] = 0;
            if ((strchr("+-=<>!", op[0]) && str[i] == '=') ||
                (op[0] == '+' && str[i] == '+') ||
                (op[0] == '-' && str[i] == '-') ||
                (op[0] == '&' && str[i] == '&') ||
                (op[0] == '|' && str[i] == '|'))
                op[1] = str[i++];
            printf("%s : Operator\n", op);
        }
        else if (strchr(";,(){}[]", str[i]))
            printf("%c : Special Symbol\n", str[i++]);
        else
            i++;
    }
    return 0;
}

/*
int a = 10;
float b = 2.5;
if(a >= b)
    a++;
int : Keyword
a : Identifier
= : Operator
10 : Number
; : Special Symbol
float : Keyword
b : Identifier
= : Operator
2.5 : Number
; : Special Symbol
if : Keyword
( : Special Symbol
a : Identifier
>= : Operator
b : Identifier
) : Special Symbol
a : Identifier
++ : Operator
; : Special Symbol
*/