
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

char input[50];
int pos = 0;

void E(), EP(), T(), TP(), F();

void match(char c)
{
	if (input[pos] == c)
		pos++;
	else
	{
		printf("Error at position %d\n", pos);
		exit(1);
	}
}

void E()
{
	printf("E -> T E'\n");
	T();
	EP();
}

void EP()
{
	char c = input[pos];
	if (c == '+' || c == '-')
	{
		printf("E' -> %c T E'\n", c);
		match(c);
		T();
		EP();
	}
	else
		printf("E' -> #\n");
}

void T()
{
	printf("T -> F T'\n");
	F();
	TP();
}

void TP()
{
	char c = input[pos];
	if (c == '*' || c == '/')
	{
		printf("T' -> %c F T'\n", c);
		match(c);
		F();
		TP();
	}
	else
		printf("T' -> #\n");
}

void F()
{
	char c = input[pos];
	if (c == '(')
	{
		printf("F -> ( E )\n");
		match('(');
		E();
		match(')');
	}
	else if (isalnum(c))
	{
		printf("F -> %c\n", c);
		pos++;
	}
	else
	{
		printf("Invalid expression\n");
		exit(1);
	}
}

int main()
{
	printf("Enter expression (end with $): ");
	scanf("%49s", input);
	E();
	if (input[pos] == '$' && input[pos + 1] == '\0')
		printf("\nParsing successful!\n");
	else
		printf("\nInvalid expression\n");
	return 0;
}

// a+b*c$
// E -> T E'
// T -> F T'
// F -> a
// T' -> #
// E' -> + T E'
// T -> F T'
// F -> b
// T' -> * F T'
// F -> c
// T' -> #
// E' -> #

// Parsing successful!