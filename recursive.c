// RECURSIVE DESCENT PARSER
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
/*
Grammar:
E -> T E'
E' -> + T E' | - T E' | #
T -> F T'
T' -> * F T' | / F T' | #
F -> ( E ) | id
*/
char input[50];
int pos = 0;
char lookahead()
{
	return input[pos];
}
void match(char c)
{
	if (input[pos] == c)
		pos++;
	else
	{
		printf("Error at position %d: expected '%c', got '%c'\n", pos, c, input[pos]);
		exit(1);
	}
}
void E();
void EP();
void T();
void TP();
void F();
void E()
{
	printf("E -> T E'\n");
	T();
	EP();
}
void EP()
{
	if (lookahead() == '+')
	{
		printf("E' -> + T E'\n");
		match('+');
		T();
		EP();
	}
	else if (lookahead() == '-')
	{
		printf("E' -> - T E'\n");
		match('-');
		T();
		EP();
	}
	else
	{
		printf("E' -> #\n");
	}
}
void T()
{
	printf("T -> F T'\n");
	F();
	TP();
}
void TP()
{
	if (lookahead() == '*')
	{
		printf("T' -> * F T'\n");
		match('*');
		F();
		TP();
	}
	else if (lookahead() == '/')
	{
		printf("T' -> / F T'\n");
		match('/');
		F();
		TP();
	}
	else
	{
		printf("T' -> #\n");
	}
}
void F()
{
	if (lookahead() == '(')
	{
		printf("F -> ( E )\n");
		match('(');
		E();
		match(')');
	}
	else if (isalnum(lookahead()))
	{
		printf("F -> %c\n", lookahead());
		pos++;
	}
	else
	{
		printf("Error at position %d: unexpected '%c'\n", pos, lookahead());
		exit(1);
	}
}
int main()
{
	printf("Enter expression (end with $): ");
	scanf("%s", input);
	E();
	if (input[pos] == '$')
		printf("\nParsing successful!\n");
	else
		printf("\nError: unexpected character '%c'\n", input[pos]);
	return 0;
}
/*
Sample Input:
Enter expression (end with $): a+b*c$

Sample Output:
Enter expression (end with $): a+b*c$
E -> T E'
T -> F T'
F -> a
T' -> #
E' -> + T E'
T -> F T'
F -> b
T' -> * F T'
F -> c
T' -> #
E' -> #

Parsing successful!
*/