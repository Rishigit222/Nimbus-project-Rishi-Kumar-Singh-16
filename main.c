#ifndef MODELS_H
#define MODELS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME          64
#define MAX_EMAIL         64
#define MAX_DATE          16
#define MAX_VENUE         64
#define MAX_TICKET_TYPES   3
#define MAX_EVENTS        20
typedef struct {
    char  name[16];
    double price;
} TicketType;
typedef struct {
    int    bookingId;        /* 0 means empty seat */
    char   name[MAX_NAME];
    char   email[MAX_EMAIL];
    int    ticketTypeIndex;  /* index into ticketTypes[] */
    int    seatNumber;      
} Attendee;
struct WaitNode {
    Attendee           data;
    struct WaitNode   *next;
};
typedef struct WaitNode WaitNode;
typedef struct {
    char        name[MAX_NAME];
    char        date[MAX_DATE];
    char        venue[MAX_VENUE];

    int         capacity;
    Attendee   seats;                 / dynamic array size=capacity */

    int         numTicketTypes;
    TicketType  ticketTypes[MAX_TICKET_TYPES];

    int         seatsBooked;
    double      revenue;
    int         nextBookingId;

    WaitNode   *waitHead;
    WaitNode   *waitTail;
} Event;
void print_event_summary(const Event *e, int idx);
void print_attendee_list(const Event *e);
void print_waitlist(const Event *e);
void print_all_events(Event events[], int eventCount);
void init_event(Event *e, const char *name, const char *date, const char *venue, int capacity);
void add_ticket_type(Event *e, const char *name, double price);
void free_event(Event *e);
bool book_ticket(Event *e, const char *custName, const char *email,
                 int ticketTypeIndex, int *outBookingId, bool *outWaitlisted);
bool cancel_booking(Event *e, int bookingId);
#endif 
void print_event_summary(const Event *e, int idx) {
    printf("\n--------------------------------------------------\n");
    printf("Event #%d: %s\n", idx, e->name);
    printf("Date: %s | Venue: %s\n", e->date, e->venue);
    printf("Capacity: %d | Booked: %d | Available: %d\n",
           e->capacity, e->seatsBooked, e->capacity - e->seatsBooked);

    printf("Ticket Types:\n");
    for (int i = 0; i < e->numTicketTypes; ++i) {
        printf("  [%d] %-10s : Rs %.2f\n", i, e->ticketTypes[i].name, e->ticketTypes[i].price);
    }
    printf("Revenue: Rs %.2f\n", e->revenue);
}
void print_attendee_list(const Event *e) {
    printf("\nConfirmed Attendees:\n");
    printf("  %-5s %-24s %-28s %-8s %-5s\n", "Seat", "Name", "Email", "Type", "BID");
    int shown = 0;
    for (int i = 0; i < e->capacity; ++i) {
        if (e->seats[i].bookingId != 0) {
            const Attendee *a = &e->seats[i];
            const char *typeName = (a->ticketTypeIndex >= 0 && a->ticketTypeIndex < e->numTicketTypes)
                ? e->ticketTypes[a->ticketTypeIndex].name : "?";
            printf("  %-5d %-24s %-28s %-8s %-5d\n",
                   a->seatNumber, a->name, a->email, typeName, a->bookingId);
            shown++;
        }
    }
    if (shown == 0) printf("  (none)\n");
}
void print_waitlist(const Event *e) {
    printf("\nWaitlist:\n");
    if (!e->waitHead) { printf("  (empty)\n"); return; }
    printf("  %-5s %-24s %-28s %-8s %-5s\n", "Pos", "Name", "Email", "Type", "BID");
    int pos = 1;
    for (const WaitNode *n = e->waitHead; n; n = n->next) {
        const Attendee *a = &n->data;
        const char *typeName = (a->ticketTypeIndex >= 0 && a->ticketTypeIndex < e->numTicketTypes)
            ? e->ticketTypes[a->ticketTypeIndex].name : "?";
        printf("  %-5d %-24s %-28s %-8s %-5d\n",
               pos++, a->name, a->email, typeName, a->bookingId);
    }
}
void print_all_events(Event events[], int eventCount) {
    printf("\n================= EVENTS =================\n");
    for (int i = 0; i < eventCount; ++i) {
        printf("[%d] %s (on %s at %s)  Seats: %d/%d  Revenue: Rs %.2f\n",
               i, events[i].name, events[i].date, events[i].venue,
               events[i].seatsBooked, events[i].capacity, events[i].revenue);
    }
    if (eventCount == 0) printf("(no events yet)\n");
    printf("==========================================\n");
}
static void wait_push(Event *e, Attendee a) {
    WaitNode *n = (WaitNode *)malloc(sizeof(WaitNode));  
    if (!n) { fprintf(stderr, "Out of memory\n"); exit(1); }
    a.seatNumber = -1;
    n->data = a;
    n->next = NULL;
    if (!e->waitTail) e->waitHead = e->waitTail = n;
    else { e->waitTail->next = n; e->waitTail = n; }
}
static bool wait_pop(Event *e, Attendee *out) {
    if (!e->waitHead) return false;
    WaitNode *n = e->waitHead;
    *out = n->data;
    e->waitHead = n->next;
    if (!e->waitHead) e->waitTail = NULL;
    free(n);
    return true;
}
static double price_of(const Event *e, int t) {
    if (t < 0 || t >= e->numTicketTypes) return 0.0;
    return e->ticketTypes[t].price;
}
void init_event(Event *e, const char *name, const char *date, const char *venue, int capacity) {
    strncpy(e->name, name, MAX_NAME - 1); e->name[MAX_NAME - 1] = 0;
    strncpy(e->date, date, MAX_DATE - 1); e->date[MAX_DATE - 1] = 0;
    strncpy(e->venue, venue, MAX_VENUE - 1); e->venue[MAX_VENUE - 1] = 0;

    e->capacity = capacity;
    e->seats = (Attendee*)calloc(capacity, sizeof(Attendee));
    if (!e->seats) { fprintf(stderr, "Out of memory\n"); exit(1); }

    e->numTicketTypes = 0;
    e->seatsBooked = 0;
    e->revenue = 0.0;
    e->nextBookingId = 1;
    e->waitHead = e->waitTail = NULL;
}
void add_ticket_type(Event *e, const char *name, double price) {
    if (e->numTicketTypes >= MAX_TICKET_TYPES) {
        printf("Cannot add more ticket types.\n");
        return;
    }
    strncpy(e->ticketTypes[e->numTicketTypes].name, name,
            sizeof(e->ticketTypes[0].name) - 1);
    e->ticketTypes[e->numTicketTypes].name[sizeof(e->ticketTypes[0].name) - 1] = 0;
    e->ticketTypes[e->numTicketTypes].price = price;
    e->numTicketTypes++;
}
void free_event(Event *e) {
    free(e->seats);
    e->seats = NULL;
    WaitNode *cur = e->waitHead;
    while (cur) { WaitNode *nx = cur->next; free(cur); cur = nx; }
    e->waitHead = e->waitTail = NULL;
}
bool book_ticket(Event *e, const char *custName, const char *email,
                 int ticketTypeIndex, int *outBookingId, bool *outWaitlisted) {
    if (ticketTypeIndex < 0 || ticketTypeIndex >= e->numTicketTypes) {
        printf("Invalid ticket type.\n");
        return false;
    }

    Attendee a;
    a.bookingId = e->nextBookingId++;
    strncpy(a.name, custName, MAX_NAME - 1); a.name[MAX_NAME - 1] = 0;
    strncpy(a.email, email, MAX_EMAIL - 1);  a.email[MAX_EMAIL - 1] = 0;
    a.ticketTypeIndex = ticketTypeIndex;
    a.seatNumber = -1;

    if (e->seatsBooked < e->capacity) {
        for (int i = 0; i < e->capacity; ++i) {
            if (e->seats[i].bookingId == 0) {
                a.seatNumber = i + 1;
                e->seats[i] = a;
                e->seatsBooked++;
                e->revenue += price_of(e, ticketTypeIndex);
                if (outBookingId) *outBookingId = a.bookingId;
                if (outWaitlisted) *outWaitlisted = false;
                return true;
            }
        }
    }

    wait_push(e, a);
    if (outBookingId) *outBookingId = a.bookingId;
    if (outWaitlisted) *outWaitlisted = true;
    return true;
}
bool cancel_booking(Event *e, int bookingId) {
    int pos = -1;
    for (int i = 0; i < e->capacity; ++i) {
        if (e->seats[i].bookingId == bookingId) { pos = i; break; }
    }
    if (pos == -1) {
        printf("Booking ID %d not found.\n", bookingId);
        return false;
    }

    e->seats[pos].bookingId = 0;
    e->seatsBooked--;

    Attendee next;
    if (wait_pop(e, &next)) {
        next.seatNumber = pos + 1;
        e->seats[pos] = next;
        e->seatsBooked++;
        e->revenue += price_of(e, next.ticketTypeIndex);
        printf("Promoted from waitlist: %s (BID %d)\n", next.name, next.bookingId);
    }
    return true;
}
static void clear_line(void) { int c; while ((c = getchar()) != '\n' && c != EOF) {} }
static void read_str(const char *label, char *buf, int size) {
    printf("%s", label); fflush(stdout);
    if (fgets(buf, size, stdin)) {
        int n = (int)strlen(buf);
        if (n > 0 && buf[n - 1] == '\n') buf[n - 1] = 0;
    } else buf[0] = 0;
}
static int read_int(const char *label) {
    int x; printf("%s", label); fflush(stdout);
    if (scanf("%d", &x) != 1) { x = 0; }
    clear_line(); return x;
}
static double read_double(const char *label) {
    double x; printf("%s", label); fflush(stdout);
    if (scanf("%lf", &x) != 1) { x = 0; }
    clear_line(); return x;
}
static void seed(Event events[], int *count) {
    *count = 2;
    init_event(&events[0], "Tech Fest", "2025-12-10", "Auditorium", 5);
    add_ticket_type(&events[0], "REG", 199);
    add_ticket_type(&events[0], "VIP", 499);

    init_event(&events[1], "Cultural Night", "2025-12-12", "Open Stage", 3);
    add_ticket_type(&events[1], "STU", 99);
    add_ticket_type(&events[1], "GST", 149);
}
static void create_event(Event events[], int *count) {
    if (*count >= MAX_EVENTS) { printf("Event limit reached.\n"); return; }
    char name[MAX_NAME], date[MAX_DATE], venue[MAX_VENUE];
    int cap;

    read_str("Event name: ", name, sizeof(name));
    read_str("Date (YYYY-MM-DD): ", date, sizeof(date));
    read_str("Venue: ", venue, sizeof(venue));
    cap = read_int("Capacity: ");

    init_event(&events[*count], name, date, venue, cap);

    int n = read_int("How many ticket types (1-3)? ");
    if (n < 1) n = 1;
    if (n > MAX_TICKET_TYPES) n = MAX_TICKET_TYPES;

    for (int i = 0; i < n; i++) {
        char tname[24]; double p;
        printf("Ticket type %d:\n", i);
        read_str("  Name: ", tname, sizeof(tname));
        p = read_double("  Price: ");
        add_ticket_type(&events[*count], tname, p);
    }
    printf("Event created with index %d.\n", *count);
    (*count)++;
}
static void do_book(Event events[], int count) {
    if (count == 0) { printf("No events.\n"); return; }
    print_all_events(events, count);
    int idx = read_int("Choose event index: ");
    if (idx < 0 || idx >= count) { printf("Invalid index.\n"); return; }

    Event *e = &events[idx];
    if (e->numTicketTypes == 0) { printf("No ticket types.\n"); return; }

    char name[MAX_NAME], email[MAX_EMAIL];
    read_str("Attendee name: ", name, sizeof(name));
    read_str("Attendee email: ", email, sizeof(email));

    for (int i = 0; i < e->numTicketTypes; i++)
        printf("  [%d] %-8s Rs %.2f\n", i, e->ticketTypes[i].name, e->ticketTypes[i].price);
    int t = read_int("Ticket type index: ");

    int bid = 0; bool waitlisted = false;
    if (book_ticket(e, name, email, t, &bid, &waitlisted)) {
        if (waitlisted) printf("Event full. Added to waitlist. Booking ID: %d\n", bid);
        else printf("Booked! Your Booking ID: %d\n", bid);
    }
}
static void do_cancel(Event events[], int count) {
    if (count == 0) { printf("No events.\n"); return; }
    print_all_events(events, count);
    int idx = read_int("Choose event index: ");
    if (idx < 0 || idx >= count) { printf("Invalid index.\n"); return; }

    int bid = read_int("Enter Booking ID to cancel: ");
    if (cancel_booking(&events[idx], bid)) printf("Cancelled.\n");
}
static void do_report(Event events[], int count) {
    if (count == 0) { printf("No events.\n"); return; }
    print_all_events(events, count);
    int idx = read_int("Choose event index: ");
    if (idx < 0 || idx >= count) { printf("Invalid index.\n"); return; }
    print_event_summary(&events[idx], idx);
    print_attendee_list(&events[idx]);
    print_waitlist(&events[idx]);
}
static void menu(void) {
    printf("\n==== College Event Ticketing (Simple) ====\n");
    printf("1) List events\n");
    printf("2) Create event\n");
    printf("3) Book ticket\n");
    printf("4) Cancel booking\n");
    printf("5) Event report\n");
    printf("0) Exit\n");
}
int main(void) {
    Event events[MAX_EVENTS];
    int count = 0;
    seed(events, &count);

    while (1) {
        menu();
        int c = read_int("Select: ");
        if (c == 0) break;
        if (c == 1) print_all_events(events, count);
        else if (c == 2) create_event(events, &count);
        else if (c == 3) do_book(events, count);
        else if (c == 4) do_cancel(events, count);
        else if (c == 5) do_report(events, count);
        else printf("Invalid choice.\n");
    }

    for (int i = 0; i < count; i++) free_event(&events[i]);
    printf("Goodbye!\n");
    return 0;
}