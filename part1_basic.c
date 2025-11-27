/* part1_basic.c*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int id;                  
    char name[100];   
    double price;            
} Movie;


static Movie movies[10];
static int movie_count = 0;

void init_movies(void) {
    movie_count = 3;
    movies[0].id = 1; strncpy(movies[0].name, "The Matrix", 100-1); movies[0].name[100-1] = '\0'; movies[0].price = 200.00;
    movies[1].id = 2; strncpy(movies[1].name, "Inception", 100-1); movies[1].name[100-1] = '\0'; movies[1].price = 250.00;
    movies[2].id = 3; strncpy(movies[2].name, "Interstellar", 100-1); movies[2].name[100-1] = '\0'; movies[2].price = 300.00;
}
void display_movies(void) {
    if (movie_count == 0) {
        printf("\nNo movies available.\n");
        return;
    }
    printf("\nAvailable Movies:\n");
    printf("+----+-------------------------------+--------+\n");
    printf("| ID | Name                          | Price  |\n");
    printf("+----+-------------------------------+--------+\n");
    for (int i = 0; i < movie_count; ++i) {
        printf("| %-2d | %-29s | %6.2f |\n",
               movies[i].id,
               movies[i].name,
               movies[i].price);
    }
    printf("+----+-------------------------------+--------+\n");
}

int main(void) {
    char buffer[256];
    init_movies();
    printf("=== Movie Ticket Booking — Part 1: Basic Display ===\n");

    while (1) {
        printf("\nMain Menu:\n");
        printf("1. Display Movies\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        int choice = atoi(buffer);
        if (choice == 1) display_movies();
        else if (choice == 2) {
            printf("Exiting. Goodbye!\n");
            break;
        } else printf("Invalid choice. Try again.\n");
    }
    return 0;
}
