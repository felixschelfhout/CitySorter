#include "version.h"
#include <stdio.h>
#include <string.h>

void print_version(const char *program_name) {
    printf("%s %d.%d.%d\n", program_name, CITYSORTER_VERSION_MAJOR, CITYSORTER_VERSION_MINOR,
           CITYSORTER_VERSION_PATCH);
}

void print_help(const char *program_name) {
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Discover all cities of the countries of your dreams - neatly sorted in a binary search "
           "tree\n\n");
    printf("Options:\n");
    printf("  --version    Display version information\n");
    printf("  --help       Display this help message\n");
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "--version") == 0) {
            print_version(argv[0]);
            return 0;
        }
        if (strcmp(argv[1], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        }
        fprintf(stderr, "Error: Unknown option '%s'\n", argv[1]);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        return 1;
    }

    printf("%s %d.%d.%d\n", argv[0], CITYSORTER_VERSION_MAJOR, CITYSORTER_VERSION_MINOR,
           CITYSORTER_VERSION_PATCH);

    return 0;
}
