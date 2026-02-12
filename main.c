#include <stdio.h>

int main(int args, char *argv[]) {
    int input;
    printf("Passed %d arguments to %s\n", args, argv[0]);

    printf("Pick a number from 0-10: ");
    scanf("%d", &input);
    printf("You picked %d\n", input);
    return 0;
}