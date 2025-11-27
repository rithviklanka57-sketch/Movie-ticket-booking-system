/* part3_full.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Movie *movies = NULL;
int movie_count = 0;

void trim_newline(char *s) {
    if (!s) return;
    size_t len = strlen(s);
    if (len == 0) return;
    if (s[len-1] == '\n') s[len-1] = '\0';
}

void load_movies(void) {
    FILE *fp = fopen("movies.txt", "r");
    if (!fp) {
        fp = fopen("movies.txt", "w");
        if (!fp) { printf("Error: cannot create %s\n", "movies.txt"); return; }
        fprintf(fp, "1|The Matrix|200.00\n");
        fprintf(fp, "2|Inception|250.00\n");
        fprintf(fp, "3|Interstellar|300.00\n");
        fclose(fp);
        fp = fopen("movies.txt", "r");
        if (!fp) return;
    }

    char line[256];
    movie_count = 0;
    free(movies);
    movies = NULL;

    while (fgets(line, sizeof(line), fp)) {
        trim_newline(line);
        if (strlen(line) == 0) continue;
        int id; char name[100]; double price;
        if (sscanf(line, "%d|%99[^|]|%lf", &id, name, &price) == 3) {
            movies = realloc(movies, (movie_count + 1) * sizeof(Movie));
            movies[movie_count].id = id;
            strncpy(movies[movie_count].name, name, 100-1);
            movies[movie_count].name[100-1] = '\0';
            movies[movie_count].price = price;
            movie_count++;
        }
    }
    fclose(fp);
}

int append_movie_to_file(const Movie *m) {
    FILE *fp = fopen("movies.txt", "a");
    if (!fp) return 0;
    fprintf(fp, "%d|%s|%.2f\n", m->id, m->name, m->price);
    fclose(fp);
    return 1;
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
    char movie_name[100] = "Unknown";
    if (idx != -1) strncpy(movie_name, movies[idx].name, 100-1);
    fprintf(fp, "%s|%d|%s|%d|%.2f\n",
            t->customer_name, t->movie_id, movie_name, t->seats, t->total_cost);
    fclose(fp);
    return 1;
}

void book_ticket(void) {
    if (movie_count == 0) { printf("No movies available to book.\n"); return; }
    char buffer[256];
    Ticket t;
    printf("\n--- Book Ticket ---\n");
    printf("Enter your name: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    trim_newline(buffer);
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

    if (save_booking_to_file(&t)) printf("Booking saved successfully.\n"); else printf("Error saving booking.\n");
}

void admin_add_movie(void) {
    char buffer[256];
    Movie m;
    printf("\n--- Add Movie (Admin) ---\n");
    printf("Enter movie name: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return; trim_newline(buffer);
    if (strlen(buffer) == 0) { printf("Movie name cannot be empty.\n"); return; }
    strncpy(m.name, buffer, 100-1); m.name[100-1] = '\0';

    printf("Enter price: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    double price = atof(buffer);
    if (price <= 0) { printf("Invalid price.\n"); return; }
    m.price = price;

    int maxid = 0; for (int i = 0; i < movie_count; ++i) if (movies[i].id > maxid) maxid = movies[i].id;
    m.id = maxid + 1;

    movies = realloc(movies, (movie_count + 1) * sizeof(Movie));
    movies[movie_count] = m; movie_count++;

    if (append_movie_to_file(&m)) printf("Movie added and saved: ID %d - %s (%.2f)\n", m.id, m.name, m.price);
    else printf("Movie added in memory but failed to persist to file.\n");
}

void admin_view_bookings(void) {
    FILE *fp = fopen("bookings.txt", "r");
    if (!fp) { printf("\nNo bookings found.\n"); return; }
    char line[256];
    printf("\n--- All Bookings ---\n");
    printf("+----------------------+----+-------------------------------+------+--------+\n");
    printf("| Customer             | ID | Movie                         | Seats| Total  |\n");
    printf("+----------------------+----+-------------------------------+------+--------+\n");
    while (fgets(line, sizeof(line), fp)) {
        trim_newline(line);
        if (strlen(line) == 0) continue;
        char customer[100], movie_name[100]; int movie_id, seats; double total;
        if (sscanf(line, "%99[^|]|%d|%99[^|]|%d|%lf", customer, &movie_id, movie_name, &seats, &total) == 5) {
            printf("| %-20s | %-2d | %-29s | %-4d | %6.2f |\n",
                   customer, movie_id, movie_name, seats, total);
        }
    }
    printf("+----------------------+----+-------------------------------+------+--------+\n");
    fclose(fp);
}

int admin_login(void) {
    char buffer[256];
    printf("Enter admin password: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
    trim_newline(buffer);
    if (strcmp(buffer, "gaurav123") == 0) return 1;
    return 0;
}


void user_menu(void) {
    char buffer[256];
    while (1) {
        printf("\n--- User Menu ---\n");
        printf("1. Display Movies\n");
        printf("2. Book Ticket\n");
        printf("3. Back to Main Menu\n");
        printf("Enter choice: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) return;
        int choice = atoi(buffer);
        if (choice == 1) display_movies();
        else if (choice == 2) book_ticket();
        else if (choice == 3) return;
        else printf("Invalid choice.\n");
    }
}

void admin_menu(void) {
    char buffer[256];
    if (!admin_login()) { printf("Incorrect password. Returning to main menu.\n"); return; }
    while (1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Movie\n");
        printf("2. View Bookings\n");
        printf("3. Back to Main Menu\n");
        printf("Enter choice: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) return;
        int choice = atoi(buffer);
        if (choice == 1) admin_add_movie();
        else if (choice == 2) admin_view_bookings();
        else if (choice == 3) return;
        else printf("Invalid choice.\n");
    }
}

int main(void) {
    load_movies();
    char buffer[256];
    printf("=== Movie Ticket Booking — Part 3: Full System ===\n");
    while (1) {
        printf("\nMain Menu:\n");
        printf("1. User (Display / Book)\n");
        printf("2. Admin (Add Movie / View Bookings)\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        int choice = atoi(buffer);
        if (choice == 1) user_menu();
        else if (choice == 2) admin_menu();
        else if (choice == 3) { printf("Exiting. Goodbye!\n"); break; }
        else printf("Invalid choice. Try again.\n");
    }
    free(movies);
    return 0;
}
