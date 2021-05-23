#include <stdio.h>
#include <string.h>

int main() {
	char a[] = "abcde\0";
	char b[] = "fghij\0";
	printf("%s%s\n", a, b);
	return 0;
}
