#include "models.h"
static void wait_push(Event *e, Attendee a) {
    WaitNode *n = (WaitNode *)malloc(sizeof(WaitNode));  /* FIXED */
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
