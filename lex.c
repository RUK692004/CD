#include <stdio.h>
#include <ctype.h>
#include <string.h>
int keyword(char s[])
{
    char *k[] = {

        // 1. Basic data types
        "char", "double", "float", "int", "void",

        // 2. Decision making
        "case", "default", "else", "if", "switch",

        // 3. Loops
        "do", "for", "while",

        // 4. Jump statements
        "break", "continue", "goto", "return",

        // 5. Data type modifiers
        "long", "short", "signed", "unsigned",

        // 6. Storage classes
        "auto", "extern", "register", "static",

        // 7. Other keywords
        "const", "enum", "sizeof", "struct",
        "typedef", "union", "volatile"};

    int n = 32, i;

    for (i = 0; i < n; i++)
        if (strcmp(s, k[i]) == 0)
            return 1;

    return 0;
}

int main()
{
    char str[1000], word[100];
    int i = 0, j, ch;

    printf("Enter source code:\n");

    while ((ch = getchar()) != EOF && i < 999)
        str[i++] = ch;

    str[i] = '\0';

    i = 0;

    while (str[i] != '\0')
    {
        // Ignore spaces, tabs and newlines
        if (isspace((unsigned char)str[i]))
        {
            i++;
            continue;
        }

        // Keyword or Identifier
        if (isalpha((unsigned char)str[i]) || str[i] == '_')
        {
            j = 0;

            while (isalnum((unsigned char)str[i]) || str[i] == '_')
            {
                if (j < 99)
                    word[j++] = str[i];

                i++;
            }

            word[j] = '\0';

            if (keyword(word))
                printf("%s : Keyword\n", word);
            else
                printf("%s : Identifier\n", word);
        }

        // Number (integer or decimal)
        else if (isdigit((unsigned char)str[i]))
        {
            j = 0;

            while (isdigit((unsigned char)str[i]) ||
                   (str[i] == '.' && isdigit((unsigned char)str[i + 1])))
            {
                if (j < 99)
                    word[j++] = str[i];

                i++;
            }

            word[j] = '\0';
            printf("%s : Number\n", word);
        }

        // Multi-character and single-character operators
        else if (strchr("+-*/%=<>!&|^", str[i]))
        {
            char op[3] = {str[i], '\0', '\0'};
            i++;

            if ((op[0] == '+' && str[i] == '+') ||
                (op[0] == '-' && str[i] == '-') ||
                (op[0] == '=' && str[i] == '=') ||
                (op[0] == '!' && str[i] == '=') ||
                (op[0] == '<' && str[i] == '=') ||
                (op[0] == '>' && str[i] == '=') ||
                (op[0] == '&' && str[i] == '&') ||
                (op[0] == '|' && str[i] == '|'))
            {
                op[1] = str[i];
                i++;
            }

            printf("%s : Operator\n", op);
        }

        // Special symbols
        else if (strchr(";,(){}[]", str[i]))
        {
            printf("%c : Special Symbol\n", str[i]);
            i++;
        }

        // Ignore other characters
        else
        {
            i++;
        }
    }

    return 0;
}