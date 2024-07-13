#include <stdio.h>
/* Function prototype for factorial */
int factorial(int n);

/**
 * main - Entry point
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	int number;
	int result;

	printf("Enter a number: ");
	scanf("%d", &number);

	result = factorial(number);

	printf("Factorial of %d is %d\n", number, result);

	return (0);
}

/**
 * factorial - Calculates the factorial of a given number
 * @n: The number to calculate the factorial of
 *
 * Return: The factorial of the number
 */
int factorial(int n)
{
	if (n <= 1)
		return (1);
	else
		return (n * factorial(n - 1));
}
