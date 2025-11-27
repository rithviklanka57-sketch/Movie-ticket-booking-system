# 🎬 Movie Ticket Booking System

[![Language: C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](#license)
[![Status: Student Project](https://img.shields.io/badge/status-student--project-orange.svg)](#)

---

## 📌 Project Overview

**Movie Ticket Booking System** is a console-based C application that simulates a small movie-theatre booking workflow.  
It is implemented in three progressive parts (Part 1 → Part 3) so beginners can learn step-by-step:

- **Part 1**: Basic movie list and display (static array).  
- **Part 2**: Booking system + saving bookings to `bookings.txt`.  
- **Part 3**: Full system with file-backed movies (`movies.txt`), admin panel, and dynamic movie list.

---

## 📂 Repository Structure

Movie-Ticket-Booking/
├── part1_basic.c # Basic movie display (beginner)
├── part2_booking.c # Booking flow + save bookings
├── part3_full.c # Full system: admin + persistence
├── movies.txt # (created at runtime) movie data
├── bookings.txt # (created at runtime) booking records
├── README.md # <-- you are here
└── docs/
├── stylish_project_report_updated.pdf # formatted project report (generated)
└── Screenshot 2025-11-24 222359.png # uploaded screenshot


> **Screenshot & Report (local paths):**  
> - Screenshot: `/mnt/data/Screenshot 2025-11-24 222359.png`  
> - Stylish PDF report: `/mnt/data/stylish_project_report_updated.pdf`  
> *(If you push to GitHub, replace these local paths with repository-relative paths or upload them into `docs/` and link accordingly.)*

You can also reference the screenshot here:  
![Screenshot](/mnt/data/Screenshot 2025-11-24 222359.png)

---

## 🚀 Features

**User**
- View available movies (ID, title, price) in a neat table
- Book tickets (enter name, choose movie ID, number of seats)
- Booking summary before confirmation
- Bookings are appended to `bookings.txt`

**Admin**
- Password-protected admin access (`admin123`)
- Add new movies (appends to `movies.txt`)
- View all bookings in a formatted table

**Persistence**
- Movies are loaded from `movies.txt` at startup (created with defaults if missing)
- Bookings saved persistently to `bookings.txt`

---

## 🛠️ Technologies & Concepts Covered

- C language (structured programming)  
- `struct` usage (`Movie`, `Ticket`)  
- Arrays and dynamic arrays (`realloc`)  
- File I/O: `fopen`, `fgets`, `fprintf`, `fclose`, `sscanf`  
- Input handling with `fgets`, safe string handling (`strncpy`, `trim`)  
- Modular functions and menu-driven CLI design  
- Basic authentication (admin password check)

---

## 🧭 How to Compile & Run

**Linux / macOS / WSL**
```bash
# Compile the full program (Part 3 - recommended)
gcc part3_full.c -o ticket_system
./ticket_system


Windows (MinGW)

gcc part3_full.c -o ticket_system.exe
.\ticket_system.exe


Run Part 1 or Part 2 individually

gcc part1_basic.c -o part1 && ./part1
gcc part2_booking.c -o part2 && ./part2
