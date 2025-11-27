 #include "models.h"

static void clear_line(void){ int c; while((c=getchar())!='\n' && c!=EOF){} }
static void read_str(const char *label, char *buf, int size){
    printf("%s", label); fflush(stdout);
    if (fgets(buf, size, stdin)) {
        int n = (int)strlen(buf);
        if (n>0 && buf[n-1]=='\n') buf[n-1]=0;
    } else buf[0]=0;
}
static int read_int(const char *label){
    int x; printf("%s", label); fflush(stdout);
    if (scanf("%d", &x)!=1) { x=0; } clear_line(); return x;
}
static double read_double(const char *label){
    double x; printf("%s", label); fflush(stdout);
    if (scanf("%lf", &x)!=1) { x=0; } clear_line(); return x;
}

static void seed(Event events[], int *count){
    *count = 2;

    init_event(&events[0], "Tech Fest", "2025-12-10", "Auditorium", 5);
    add_ticket_type(&events[0], "REG", 199);
    add_ticket_type(&events[0], "VIP", 499);

    init_event(&events[1], "Cultural Night", "2025-12-12", "Open Stage", 3);
    add_ticket_type(&events[1], "STU", 99);
    add_ticket_type(&events[1], "GST", 149);
}

static void create_event(Event events[], int *count){
    if (*count >= MAX_EVENTS){ printf("Event limit reached.\n"); return; }
    char name[MAX_NAME], date[MAX_DATE], venue[MAX_VENUE];
    int cap;

    read_str("Event name: ", name, sizeof(name));
    read_str("Date (YYYY-MM-DD): ", date, sizeof(date));
    read_str("Venue: ", venue, sizeof(venue));
    cap = read_int("Capacity: ");

    init_event(&events[*count], name, date, venue, cap);

    int n = read_int("How many ticket types (1-3)? ");
    if (n < 1) n = 1; if (n > MAX_TICKET_TYPES) n = MAX_TICKET_TYPES;
    for (int i=0;i<n;i++){
        char tname[24]; double p;
        printf("Ticket type %d:\n", i);
        read_str("  Name: ", tname, sizeof(tname));
        p = read_double("  Price: ");
        add_ticket_type(&events[*count], tname, p);
    }
    printf("Event created with index %d.\n", *count);
    (*count)++;
}

static void do_book(Event events[], int count){
    if (count==0){ printf("No events.\n"); return; }
    print_all_events(events, count);
    int idx = read_int("Choose event index: ");
    if (idx < 0 || idx >= count){ printf("Invalid index.\n"); return; }

    Event *e = &events[idx];
    if (e->numTicketTypes==0){ printf("No ticket types.\n"); return; }

    char name[MAX_NAME], email[MAX_EMAIL];
    read_str("Attendee name: ", name, sizeof(name));
    read_str("Attendee email: ", email, sizeof(email));

    for (int i=0;i<e->numTicketTypes;i++)
        printf("  [%d] %-8s Rs %.2f\n", i, e->ticketTypes[i].name, e->ticketTypes[i].price);
    int t = read_int("Ticket type index: ");

    int bid=0; bool waitlisted=false;
    if (book_ticket(e, name, email, t, &bid, &waitlisted)) {
        if (waitlisted) printf("Event full. Added to waitlist. Booking ID: %d\n", bid);
        else            printf("Booked! Your Booking ID: %d\n", bid);
    }
}

static void do_cancel(Event events[], int count){
    if (count==0){ printf("No events.\n"); return; }
    print_all_events(events, count);
    int idx = read_int("Choose event index: ");
    if (idx < 0 || idx >= count){ printf("Invalid index.\n"); return; }

    int bid = read_int("Enter Booking ID to cancel: ");
    if (cancel_booking(&events[idx], bid)) printf("Cancelled.\n");
}

static void do_report(Event events[], int count){
    if (count==0){ printf("No events.\n"); return; }
    print_all_events(events, count);
    int idx = read_int("Choose event index: ");
    if (idx < 0 || idx >= count){ printf("Invalid index.\n"); return; }
    print_event_summary(&events[idx], idx);
    print_attendee_list(&events[idx]);
    print_waitlist(&events[idx]);
}

static void menu(void){
    printf("\n==== College Event Ticketing (Simple) ====\n");
    printf("1) List events\n");
    printf("2) Create event\n");
    printf("3) Book ticket\n");
    printf("4) Cancel booking\n");
    printf("5) Event report\n");
    printf("0) Exit\n");
}

int main(void){
    Event events[MAX_EVENTS];
    int count=0;
    seed(events, &count);

    while (1){
        menu();
        int c = read_int("Select: ");
        if (c==0) break;
        if (c==1) print_all_events(events, count);
        else if (c==2) create_event(events, &count);
        else if (c==3) do_book(events, count);
        else if (c==4) do_cancel(events, count);
        else if (c==5) do_report(events, count);
        else printf("Invalid choice.\n");
    }

    for (int i=0;i<count;i++) free_event(&events[i]);
    printf("Goodbye!\n");
    return 0;
}
