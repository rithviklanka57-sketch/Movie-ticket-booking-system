/* part2_booking.c*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int id;
    char name[100];
    double price;
} Movie;

typedef struct {
    char customer_name[100];
    int movie_id;
    int seats;
    double total_cost;
} Ticket;

static Movie movies[10];
static int movie_count = 0;

void init_movies(void) {
    movie_count = 3;
    movies[0].id = 1; strncpy(movies[0].name, "The Matrix", 100-1); movies[0].name[100-1] = '\0'; movies[0].price = 200.00;
    movies[1].id = 2; strncpy(movies[1].name, "Inception", 100-1); movies[1].name[100-1] = '\0'; movies[1].price = 250.00;
    movies[2].id = 3; strncpy(movies[2].name, "Interstellar", 100-1); movies[2].name[100-1] = '\0'; movies[2].price = 300.00;
}

void display_movies(void) {
    if (movie_count == 0) { printf("\nNo movies available.\n"); return; }
    printf("\nAvailable Movies:\n");
    printf("+----+-------------------------------+--------+\n");
    printf("| ID | Name                          | Price  |\n");
    printf("+----+-------------------------------+--------+\n");
    for (int i = 0; i < movie_count; ++i) {
        printf("| %-2d | %-29s | %6.2f |\n",
               movies[i].id, movies[i].name, movies[i].price);
    }
    printf("+----+-------------------------------+--------+\n");
}


int find_movie_index_by_id(int id) {
    for (int i = 0; i < movie_count; ++i) if (movies[i].id == id) return i;
    return -1;
}

int save_booking_to_file(const Ticket *t) {
    FILE *fp = fopen("bookings.txt", "a");
    if (!fp) return 0;
    int idx = find_movie_index_by_id(t->movie_id);
    const char *movie_name = (idx == -1) ? "Unknown" : movies[idx].name;
    fprintf(fp, "%s|%d|%s|%d|%.2f\n",
            t->customer_name, t->movie_id, movie_name, t->seats, t->total_cost);
    fclose(fp);
    return 1;
}

void book_ticket(void) {
    char buffer[256];
    Ticket t;

    printf("\n--- Book Ticket ---\n");
    printf("Enter your name: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0) { printf("Name cannot be empty. Booking cancelled.\n"); return; }
    strncpy(t.customer_name, buffer, 100-1); t.customer_name[100-1] = '\0';

    display_movies();
    printf("Enter Movie ID to book: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    int movie_id = atoi(buffer);
    int idx = find_movie_index_by_id(movie_id);
    if (idx == -1) { printf("Invalid Movie ID. Booking cancelled.\n"); return; }

    printf("Enter number of seats: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    int seats = atoi(buffer);
    if (seats <= 0) { printf("Invalid number of seats. Booking cancelled.\n"); return; }

    t.movie_id = movie_id; t.seats = seats; t.total_cost = seats * movies[idx].price;

    printf("\nBooking Summary:\n");
    printf("Customer: %s\n", t.customer_name);
    printf("Movie   : %s (ID %d)\n", movies[idx].name, movies[idx].id);
    printf("Seats   : %d\n", t.seats);
    printf("Price   : %.2f per seat\n", movies[idx].price);
    printf("Total   : %.2f\n", t.total_cost);

    printf("Confirm booking? (y/n): ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    if (buffer[0] != 'y' && buffer[0] != 'Y') { printf("Booking cancelled.\n"); return; }

    if (save_booking_to_file(&t)) printf("Booking saved successfully to %s.\n", "bookings.txt");
    else printf("Error saving booking.\n");
}

int main(void) {
    char buffer[256];
    init_movies();
    printf("=== Movie Ticket Booking — Part 2: Booking System ===\n");

    while (1) {
        printf("\nMain Menu:\n");
        printf("1. Display Movies\n");
        printf("2. Book Ticket\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        int choice = atoi(buffer);
        if (choice == 1) display_movies();
        else if (choice == 2) book_ticket();
        else if (choice == 3) { printf("Exiting. Goodbye!\n"); break; }
        else printf("Invalid choice. Try again.\n");
    }
    return 0;
}
