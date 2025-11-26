 <<<<<<< HEAD
<<<<<<< HEAD
# College Event Management Ticketing System
=======
# Project 16: College Event Management & Ticketing System
>>>>>>> 1a6adba (Enhance README.md: project details, syllabus mapping, COs)

A C-based implementation of a college event ticketing system. This project models the real-world responsibilities of event organizers: registering events, allocating seats, issuing tickets, handling cancellations and waitlists, and tracking revenue.

## Overview
- Stores events (name, date, venue, capacity), attendees, and ticket types using `struct`s.
- Supports ticket booking with seat allocation on a first-come-first-served basis.
- Handles cancellations and a pointer-based waitlist (queue) for overflow.
- Computes revenue per event, remaining seats, and generates formatted attendee lists.
- Uses dynamic memory allocation for variable-size attendee lists and functions for booking/cancel/report flows.

## Key Features
- Event storage: name, date, venue, capacity
- Ticket types: regular, VIP, etc., with pricing
- Booking flow: validation, seat allocation (array-based), waitlist queuing
- Cancellation flow: free seat and move first waitlisted attendee into seat
- Reports: revenue per event, available seats, attendee lists

## Data Structures & Implementation Notes
- Uses C `struct` types for events, attendees, and ticket metadata.
- Arrays and simple queue logic (circular or linear) implement seat lists and booking order.
- Waitlist implemented as a pointer-based linked queue (dynamic nodes allocated with `malloc`).
- Functions encapsulate booking, cancellation, waitlist enqueue/dequeue, and reporting.

## Syllabus Topics Applied
- Problem solving and algorithm design (booking and waitlist mechanics)
- Control structures and decision making for validations
- Arrays and loops for listing attendees and generating reports
- Functions and modular code for booking/cancellation/reporting
- Pointers and dynamic memory (`malloc`/`free`) for attendee lists and waitlist nodes
- Basic data structures: queue for waitlist handling

## Course Outcomes (CO) Covered
- **CO1**: Design algorithms/flows for booking and waitlist mechanics — implemented via booking/cancel workflows.
- **CO2**: Apply decision making and I/O for booking validations and user input handling.
- **CO3**: Use arrays and loops to maintain lists and produce formatted reports.
- **CO4**: Implement pointers and dynamic memory for variable attendee lists and pointer-based waitlist nodes.
- **CO5**: Integrate syllabus topics into a full working system demonstrating problem solving, data structures, and memory management.

## Files
- `ticketing system.c` — main C source file implementing the system

## Build & Run
Make sure you have a C compiler installed (`gcc` from MinGW or WSL). From PowerShell in the project directory run:

```pwsh
gcc "ticketing system.c" -o ticketing_system.exe
.\ticketing_system.exe
```

Or on WSL / Linux:

```bash
gcc "ticketing system.c" -o ticketing_system
./ticketing_system
```

## Usage Example
- Run the compiled program and follow the interactive menu to add events, book tickets, cancel bookings, and generate reports.

## Future Improvements
- Persist events and bookings to a file (save/load).
# Project 16: College Event Management & Ticketing System

A C-based implementation of a college event ticketing system. This project models the real-world responsibilities of event organizers: registering events, allocating seats, issuing tickets, handling cancellations and waitlists, and tracking revenue.

## Overview
- Stores events (name, date, venue, capacity), attendees, and ticket types using `struct`s.
- Supports ticket booking with seat allocation on a first-come-first-served basis.
- Handles cancellations and a pointer-based waitlist (queue) for overflow.
- Computes revenue per event, remaining seats, and generates formatted attendee lists.
- Uses dynamic memory allocation for variable-size attendee lists and functions for booking/cancel/report flows.

## Key Features
- Event storage: name, date, venue, capacity
- Ticket types: regular, VIP, etc., with pricing
- Booking flow: validation, seat allocation (array-based), waitlist queuing
- Cancellation flow: free seat and move first waitlisted attendee into seat
- Reports: revenue per event, available seats, attendee lists

## Data Structures & Implementation Notes
- Uses C `struct` types for events, attendees, and ticket metadata.
- Arrays and simple queue logic (circular or linear) implement seat lists and booking order.
- Waitlist implemented as a pointer-based linked queue (dynamic nodes allocated with `malloc`).
- Functions encapsulate booking, cancellation, waitlist enqueue/dequeue, and reporting.

## Syllabus Topics Applied
- Problem solving and algorithm design (booking and waitlist mechanics)
- Control structures and decision making for validations
- Arrays and loops for listing attendees and generating reports
- Functions and modular code for booking/cancellation/reporting
- Pointers and dynamic memory (`malloc`/`free`) for attendee lists and waitlist nodes
- Basic data structures: queue for waitlist handling

## Course Outcomes (CO) Covered
- **CO1**: Design algorithms/flows for booking and waitlist mechanics — implemented via booking/cancel workflows.
- **CO2**: Apply decision making and I/O for booking validations and user input handling.
- **CO3**: Use arrays and loops to maintain lists and produce formatted reports.
- **CO4**: Implement pointers and dynamic memory for variable attendee lists and pointer-based waitlist nodes.
- **CO5**: Integrate syllabus topics into a full working system demonstrating problem solving, data structures, and memory management.

## Files
- `ticketing system.c` — main C source file implementing the system

## Build & Run
Make sure you have a C compiler installed (`gcc` from MinGW or WSL). From PowerShell in the project directory run:

```pwsh
gcc "ticketing system.c" -o ticketing_system.exe
.\ticketing_system.exe
```

Or on WSL / Linux:

```bash
gcc "ticketing system.c" -o ticketing_system
./ticketing_system
```

## Usage Example
- Run the compiled program and follow the interactive menu to add events, book tickets, cancel bookings, and generate reports.

## Future Improvements
- Persist events and bookings to a file (save/load).
- Add seat maps and GUI or web interface.
- Add unit tests for booking/cancellation and waitlist logic.

## Author
- Rishi

## License
Add a `LICENSE` file if you want this project to be publicly licensed. Suggested: MIT or Apache-2.0.

## Notes
- This README was expanded to reflect project requirements and the syllabus mapping. Update usage details and examples as the code evolves.
