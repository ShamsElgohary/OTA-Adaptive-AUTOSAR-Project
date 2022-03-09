#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/timerfd.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_TIMER_COUNT 1000

typedef enum
{
    TIMER_SINGLE_SHOT = 0, /*Periodic Timer*/
    TIMER_PERIODIC         /*Single Shot Timer*/
} t_timer;

typedef void (*time_handler)(size_t timer_id, void *user_data);

int initialize();
size_t start_timer(unsigned int interval, time_handler handler, t_timer type, void *user_data);
void stop_timer(size_t timer_id);
void finalize();


struct timer_node
{
    int fd;
    time_handler callback;
    void *user_data;
    unsigned int interval;
    t_timer type;
    struct timer_node *next;
};

static void *_timer_thread(void *data);
static pthread_t g_thread_id;
static struct timer_node *g_head = NULL;

int initialize()
{
    if (pthread_create(&g_thread_id, NULL, _timer_thread, NULL))
    {
        /*Thread creation failed*/
        return 0;
    }

    return 1;
}

size_t start_timer(unsigned int interval, time_handler handler, t_timer type, void *user_data)
{
    struct timer_node *new_node = NULL;
    struct itimerspec new_value;

    new_node = (struct timer_node *)malloc(sizeof(struct timer_node));

    if (new_node == NULL)
        return 0;

    new_node->callback = handler;
    new_node->user_data = user_data;
    new_node->interval = interval;
    new_node->type = type;

    new_node->fd = timerfd_create(CLOCK_REALTIME, 0);

    if (new_node->fd == -1)
    {
        free(new_node);
        return 0;
    }

    new_value.it_value.tv_sec = interval / 1000;
    new_value.it_value.tv_nsec = (interval % 1000) * 1000000;

    if (type == TIMER_PERIODIC)
    {
        new_value.it_interval.tv_sec = interval / 1000;
        new_value.it_interval.tv_nsec = (interval % 1000) * 1000000;
    }
    else
    {
        new_value.it_interval.tv_sec = 0;
        new_value.it_interval.tv_nsec = 0;
    }

    timerfd_settime(new_node->fd, 0, &new_value, NULL);

    /*Inserting the timer node into the list*/
    new_node->next = g_head;
    g_head = new_node;

    return (size_t)new_node;
}

void stop_timer(size_t timer_id)
{
    struct timer_node *tmp = NULL;
    struct timer_node *node = (struct timer_node *)timer_id;

    if (node == NULL)
        return;

    if (node == g_head)
    {
        g_head = g_head->next;
    }
    else
    {

        tmp = g_head;

        while (tmp && tmp->next != node)
            tmp = tmp->next;

        if (tmp)
        {
            /*tmp->next can not be NULL here*/
            tmp->next = tmp->next->next;
            close(node->fd);
            free(node);
        }
    }
}

void finalize()
{
    while (g_head)
        stop_timer((size_t)g_head);

    pthread_cancel(g_thread_id);
    pthread_join(g_thread_id, NULL);
}

struct timer_node *_get_timer_from_fd(int fd)
{
    struct timer_node *tmp = g_head;

    while (tmp)
    {
        if (tmp->fd == fd)
            return tmp;

        tmp = tmp->next;
    }
    return NULL;
}

void *_timer_thread(void *data)
{
    struct pollfd ufds[MAX_TIMER_COUNT] = {{0}};
    int iMaxCount = 0;
    struct timer_node *tmp = NULL;
    int read_fds = 0, i, s;
    uint64_t exp;

    while (1)
    {
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_testcancel();
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

        iMaxCount = 0;
        tmp = g_head;

        memset(ufds, 0, sizeof(struct pollfd) * MAX_TIMER_COUNT);
        while (tmp)
        {
            ufds[iMaxCount].fd = tmp->fd;
            ufds[iMaxCount].events = POLLIN;
            iMaxCount++;

            tmp = tmp->next;
        }
        read_fds = poll(ufds, iMaxCount, 100);

        if (read_fds <= 0)
            continue;

        for (i = 0; i < iMaxCount; i++)
        {
            if (ufds[i].revents & POLLIN)
            {
                s = read(ufds[i].fd, &exp, sizeof(uint64_t));

                if (s != sizeof(uint64_t))
                    continue;

                tmp = _get_timer_from_fd(ufds[i].fd);

                if (tmp && tmp->callback)
                    tmp->callback((size_t)tmp, tmp->user_data );
            }
        }
    }

    return NULL;
}

void time_handler1(size_t timer_id, void *user_data)
{
    printf("Single shot timer expired.(%d)\n", timer_id);
}

void time_handler2(size_t timer_id, void *user_data)
{
    printf("100 ms timer expired. (%d)\n", timer_id);
}

void time_handler3(size_t timer_id, void *user_data)
{
    printf("2000 ms timer expired. (%d)\n", timer_id);
}



#define MAX_PKT 8 /* determines packet size in bytes */
#define PORT 9002
#define MAX_SEQ 8

typedef unsigned int seq_nr; /* sequence or ack numbers */

typedef struct
{
    unsigned char data[MAX_PKT];
} packet; /* packet definition */

typedef enum
{
    data,
    ack,
    nak
} frame_kind; /* frame kind definition */

typedef struct
{                    /* frames are transported in this layer */
    frame_kind kind; /* what kind of frame is it? */
    seq_nr seq;      /* sequence number */
    seq_nr ack;      /* acknowledgement number */
    packet info;     /* the network layer packet */
} frame;
typedef enum
{
    frame_arrival,
    cksum_err,
    timeout,
    network_layer_ready
} event_type;

typedef enum
{
    notset,
    started,
    stopped,
    timerout
} timer_status;

typedef struct
{
    seq_nr timeriD;
    seq_nr frameID;
    timer_status timerCurrentStatus = notset;
} newtwork_timer;

/* Wait for an event to happen; return its type in event. */
void wait_for_event(event_type *event);

/* Fetch a packet from the network layer for transmission on the channel. */
void from_network_layer(packet *p);

/* Deliver information from an inbound frame to the network layer.*/
void to_network_layer(packet *p);

/* Go get an inbound frame from the physical layer and copy it to r.*/
void from_physical_layer(frame *r);

/* Pass the frame to the physical layer for transmission. */
void to_physical_layer(frame *s);


/* Stop the clock and disable the timeout event.*/
void stop_timer(seq_nr k);

/*Start an auxiliary timer and enable the ack timeout event.*/
void start_ack_timer(void);

/* Stop the auxiliary timer and disable the ack timeout event.*/
void stop_ack_timer(void);

/* Allow the network layer to cause a network layer ready event.*/
void enable_network_layer(void);

/* Forbid the network layer from causing a network layer ready event. */
void disable_network_layer(void);

/* Macro inc is expanded in -line : increment k circularly.*/
#define inc(k)       \
    if (k < MAX_SEQ-1) \
        k = k + 1;   \
    else             \
        k = 0



bool NetworkLayer = false;
packet NetworkLayer_Buffer[8];
seq_nr ack_expected; /* oldest frame as yet unacknowledged */ // serial to recieve
int TimeOutFlag = 0;
unsigned int client_socket;
unsigned int server_socket;

/* Ids of timers */
size_t timers[MAX_SEQ] = {0};

/* Handler of timer */
void Set_Timer_Out(size_t timer_id, void *user_data)
{
    TimeOutFlag = 1;
}

void Connect_Slave(void)
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.8");
    address.sin_port = htons(PORT);
    printf("Waiting for Connection...\n");
    while (connect(client_socket, (struct sockaddr *)&address, sizeof(address)))
        ;
    printf("Slave Connected\n");
}
void Connect_Master(void)
{
    printf("Master Waiting\n");
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.8");
    address.sin_port = htons(PORT);
    bind(server_socket, (struct sockaddr *)&address, sizeof(address));
    listen(server_socket, 1);
    client_socket = accept(server_socket, NULL, NULL);
    printf("Master connected\n");
}

/* Wait for an event to happen; return its type in event. */
void wait_for_event(event_type *event)
{
    if (NetworkLayer)
        *event = network_layer_ready;
    else if (TimeOutFlag == 1)
        *event = timeout;
    else
    {
        *event = frame_arrival;
    }
}

/* Fetch a packet from the network layer for transmission on the channel. */
void from_network_layer(packet *p)
{
    static char i = 0;
    for (char j = 0; j < 8; j++)
        p->data[j] = NetworkLayer_Buffer[i].data[j];

    i = (i + 1) % 8;
}

/* Deliver information from an inbound frame to the network layer.*/
void to_network_layer(packet *p)
{
    static int m = 0;
    for (char j = 0; j < 8; j++)
        NetworkLayer_Buffer[m].data[j] = p->data[j];
    m = (m + 1) % 8;
}
/* Go get an inbound frame from the physical_layer and copy it to r. */
void from_physical_layer(frame *r)
{
    read(client_socket, r, sizeof(frame));
    printf("From physical layer \n");
    printf("seq = %d    ack = %d\n", r->seq, r->ack);
    printf("info is : ");
    for (int i = 0; i < 8; i++)
    {
        printf("%d  ", r->info.data[i]);
    }
    printf("\n");
}
/* Pass the frame to the physical_layer for transmission. */
void to_physical_layer(frame *s)
{
    sleep(5);
    write(client_socket, s, sizeof(frame));
    printf("To physical layer \n");
    printf("seq = %d    ack = %d\n", s->seq, s->ack);
    printf("info is : ");
    for (int i = 0; i < 8; i++)
    {
        printf("%d  ", s->info.data[i]);
    }
    printf("\n");
}

/*Start an auxiliary timer and enable the ack timeout event.*/
//void start_ack_timer(void);

/* Stop the auxiliary timer and disable the ack timeout event.*/
//void stop_ack_timer(void);

/* Allow the network layer to cause a network layer ready event.*/
void enable_network_layer(void)
{
    NetworkLayer = true;
}
/* Forbid the network layer from causing a network layer ready event. */
void disable_network_layer(void)
{
    NetworkLayer = false;
}
static bool between(seq_nr a, seq_nr b, seq_nr c)
{
    /* Return true if a <=b < c circularly; false otherwise. */
    if (((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)))
        return (true);
    else
        return (false);
}
static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[])
{
    /* Construct and send a data frame. */
    frame s;                                                                      /* scratch variable */
    s.info = buffer[frame_nr];                                                    /* insert packet into frame */
    s.seq = frame_nr;                                                             /* insert sequence number into frame */
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);                           /* piggyback ack */
    to_physical_layer(&s);                                                        /* transmit the frame */
    timers[frame_nr] = start_timer(2000, Set_Timer_Out, TIMER_SINGLE_SHOT, NULL); /* start the timer running */
}
void protocol5(void)
{
    seq_nr next_frame_to_send; /* MAX SEQ > 1; used for outbound stream */ // seral to send
    seq_nr frame_expected;                                                 /* next frame expected on inbound stream */
    frame r;                                                               /* scratch variable */
    packet buffer[MAX_SEQ + 1];                                            /* buffers for the outbound stream */
    seq_nr nbuffered;                                                      /* number of output buffers currently in use */
    seq_nr i;                                                              /* used to index into the buffer array */
    event_type event;
    //enable_network_layer(); /* allow network layer ready events */
    ack_expected = 0;       /* next ack expected inbound */
    next_frame_to_send = 0; /* next frame going out */
    frame_expected = 0;     /* number of frame expected inbound */
    nbuffered = 0;          /* initially no packets are buffered */
    Connect_Slave();
    while (1)
    {
        wait_for_event(&event); /* four possibilities: see event type above */
        switch (event)
        {
        case network_layer_ready: /* the network layer has a packet to send */
            /* Accept, save, and transmit a new frame. */
            from_network_layer(&buffer[next_frame_to_send]);       /* fetch new packet */
            nbuffered = nbuffered + 1;                             /* expand the sender’s window */
            send_data(next_frame_to_send, frame_expected, buffer); /* transmit the frame */
            inc(next_frame_to_send);                               /* advance sender’s upper window edge */
            /****/ disable_network_layer();
            break;
        case frame_arrival:          /* a data or control frame has arrived */
            from_physical_layer(&r); /* get incoming frame from physical layer */
            if (r.seq == frame_expected)
            {
                /* Frames are accepted only in order. */
                to_network_layer(&r.info); /* pass packet to network layer */
                inc(frame_expected);       /* advance lower edge of receiver’s window */
            }
            /* Ack n implies n − 1, n − 2, etc. Check for this. */
            while (between(ack_expected, r.ack, next_frame_to_send))
            {
                /* Handle piggybacked ack. */
                nbuffered = nbuffered - 1; /* one frame fewer buffered */
                /****/ TimeOutFlag = 0;
                stop_timer(timers[ack_expected]); /* frame arrived intact; stop timer */
                inc(ack_expected);                /* contract sender’s window */
            }
            /****/
            if (TimeOutFlag != 1)
                enable_network_layer();
            /****/
            break;
        case cksum_err:
            break;                             /* just ignore bad frames */
        case timeout:                          /* trouble; retransmit all outstanding frames */
            next_frame_to_send = ack_expected; /* start retransmitting here */
            for (i = 1; i <= nbuffered; i++)
            {
                send_data(next_frame_to_send, frame_expected, buffer); /* resend frame */
                inc(next_frame_to_send);                               /* prepare to send the next one */
            }
        }
        /*if (nbuffered < MAX_SEQ)
            enable_network_layer();
        else
            disable_network_layer();*/
    }
}
int main()
{
    /*
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            NetworkLayer_Buffer[i].data[j] = j + 1;
    */
    initialize();
    NetworkLayer_Buffer[0] = {1, 2, 3, 4, 5, 6, 7, 8};
    NetworkLayer_Buffer[1] = {15, 25, 36, 96, 17, 26, 69, 6};
    NetworkLayer_Buffer[2] = {96, 63, 25, 45, 89, 20, 26, 78};
    NetworkLayer_Buffer[3] = {9, 6, 4, 6, 7, 12, 34, 74};
    NetworkLayer_Buffer[4] = {10, 15, 12, 13, 14, 25, 87, 71};
    NetworkLayer_Buffer[5] = {1, 6, 9, 7, 8, 15, 17, 30};
    NetworkLayer_Buffer[6] = {14, 14, 24, 56, 47, 36, 89, 31};
    NetworkLayer_Buffer[7] = {20, 21, 24, 29, 26, 74, 73, 97};
    protocol5();

    return 0;
}

bool NetworkLayer = false;
bool TimeOutFlag = 0;
packet NetworkLayer_Buffer[8];
unsigned int client_socket;
unsigned int server_socket;
/* oldest frame as yet unacknowledged */
seq_nr ack_expected;

/* Ids of timers */
size_t timers[MAX_SEQ] = {0};

/* Handler of timer */
void Set_Timer_Out(size_t timer_id, void *user_data)
{
    printf("**Time Out**\nRetransmitting The Frames Again......\n");
    TimeOutFlag = 1;
}
void Connect_Master(void)
{
    printf("Master Waiting\n");
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.9");
    address.sin_port = htons(PORT);
    bind(server_socket, (struct sockaddr *)&address, sizeof(address));
    listen(server_socket, 1);
    client_socket = accept(server_socket, NULL, NULL);
    printf("Master connected\n");
}

/* Wait for an event to happen; return its type in event. */
void wait_for_event(event_type *event)
{
    if (NetworkLayer)
        *event = network_layer_ready;
    else if (TimeOutFlag == 1)
        *event = timeout;
    else
        *event = frame_arrival;
}

/* Fetch a packet from the network layer for transmission on the channel. */
void from_network_layer(packet *p)
{
    static char i = 0;

    for (char j = 0; j < 8; j++)
        p->data[j] = NetworkLayer_Buffer[i].data[j];

    i = (i + 1) % 8;
}
/* Deliver information from an inbound frame to the network layer.*/
void to_network_layer(packet *p)
{
    static int m = 0;

    for (char j = 0; j < 8; j++)
        NetworkLayer_Buffer[m].data[j] = p->data[j];

    m = (m + 1) % 8;
}
/* Go get an inbound frame from the physical_layer and copy it to r. */
void from_physical_layer(frame *r)
{
    read(client_socket, r, sizeof(frame));
    printf("Frame Recieved \n");
    printf("seq = %d   ack = %d\n", r->seq, r->ack);
    printf("Info is : ");
    for (int i = 0; i < 8; i++)
        printf("%d  ", r->info.data[i]);
    printf("\n");
    printf("*****************************************\n");
}
/* Pass the frame to the physical_layer for transmission. */
void to_physical_layer(frame *s)
{
    sleep(2);
    write(client_socket, s, sizeof(frame));
    printf("Frame Sent\n");
    printf("seq = %d   ack = %d\n", s->seq, s->ack);
    printf("Info is : ");
    for (int i = 0; i < 8; i++)
        printf("%d  ", s->info.data[i]);
    printf("\n");
    printf("*****************************************\n");
}
/* Allow the network layer to cause a network layer ready event.*/
void enable_network_layer(void)
{
    NetworkLayer = true;
}
/* Forbid the network layer from causing a network layer ready event. */
void disable_network_layer(void)
{
    NetworkLayer = false;
}
static bool between(seq_nr a, seq_nr b, seq_nr c)
{
    /* Return true if a <=b < c circularly abc bca cab ; false otherwise. */
    if (((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)))
        return (true);
    else
        return (false);
}
static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[])
{

    /* Construct and send a data frame. */
    static int c = 0;
    frame s;                                                                      /* scratch variable */
    s.info = buffer[frame_nr];                                                    /* insert packet into frame */
    s.seq = frame_nr;                                                             /* insert sequence number into frame */
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);                           /* piggyback ack */
    to_physical_layer(&s);                                                        /* transmit the frame */
    timers[frame_nr] = start_timer(2000, Set_Timer_Out, TIMER_SINGLE_SHOT, NULL); /* start the timer running */
}
void protocol5(void)
{
    seq_nr next_frame_to_send; /* MAX SEQ > 1; used for outbound stream */ // seral to send
    seq_nr frame_expected;                                                 /* next frame expected on inbound stream */
    frame r;                                                               /* scratch variable */
    packet buffer[MAX_SEQ + 1];                                            /* buffers for the outbound stream */
    seq_nr nbuffered;                                                      /* number of output buffers currently in use */
    seq_nr i;                                                              /* used to index into the buffer array */
    event_type event;
    enable_network_layer(); /* allow network layer ready events */
    ack_expected = 0;       /* next ack expected inbound */
    next_frame_to_send = 0; /* next frame going out */
    frame_expected = 0;     /* number of frame expected inbound */
    nbuffered = 0;          /* initially no packets are buffered */
    Connect_Master();
    while (1)
    {
        wait_for_event(&event); /* four possibilities: see event type above */
        switch (event)
        {
        case network_layer_ready: /* the network layer has a packet to send */
            /* Accept, save, and transmit a new frame. */
            from_network_layer(&buffer[next_frame_to_send]);       /* fetch new packet */
            nbuffered = nbuffered + 1;                             /* expand the sender’s window */
            send_data(next_frame_to_send, frame_expected, buffer); /* transmit the frame */
            inc(next_frame_to_send);                               /* advance sender’s upper window edge */
            disable_network_layer();
            break;
        case frame_arrival:          /* a data or control frame has arrived */
            from_physical_layer(&r); /* get incoming frame from physical layer */
            if (r.seq == frame_expected)
            {
                /* Frames are accepted only in order. */
                to_network_layer(&r.info); /* pass packet to network layer */
                inc(frame_expected);       /* advance lower edge of receiver’s window */
            }
            /* Ack n implies n − 1, n − 2, etc. Check for this. */
            while (between(ack_expected, r.ack, next_frame_to_send))
            {
                /* Handle piggybacked ack. */
                nbuffered = nbuffered - 1; /* one frame fewer buffered */
                TimeOutFlag = 0;
                stop_timer(timers[ack_expected]); /* frame arrived intact; stop timer */
                inc(ack_expected);                /* contract sender’s window */
            }
            if (TimeOutFlag != 1)
                enable_network_layer();
            break;
        case cksum_err:
            break;    /* just ignore bad frames */
        case timeout: /* trouble; retransmit all outstanding frames */
            printf("**Time Out**\nRetransmitting The Frames Again......\n");
            next_frame_to_send = ack_expected; /* start retransmitting here */
            for (i = 1; i <= nbuffered; i++)
            {
                send_data(next_frame_to_send, frame_expected, buffer); /* resend frame */
                inc(next_frame_to_send);                               /* prepare to send the next one */
            }
        }
    }
}
int main()
{
    /*
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            NetworkLayer_Buffer[i].data[j] = j + 1;
    */
    initialize();
    NetworkLayer_Buffer[0] = {1, 2, 3, 4, 5, 6, 7, 8};
    NetworkLayer_Buffer[1] = {15, 25, 36, 96, 17, 26, 69, 6};
    NetworkLayer_Buffer[2] = {96, 63, 25, 45, 89, 20, 26, 78};
    NetworkLayer_Buffer[3] = {9, 6, 4, 6, 7, 12, 34, 74};
    NetworkLayer_Buffer[4] = {10, 15, 12, 13, 14, 25, 87, 71};
    NetworkLayer_Buffer[5] = {1, 6, 9, 7, 8, 15, 17, 30};
    NetworkLayer_Buffer[6] = {14, 14, 24, 56, 47, 36, 89, 31};
    NetworkLayer_Buffer[7] = {20, 21, 24, 29, 26, 74, 73, 97};
    protocol5();

    return 0;
}

