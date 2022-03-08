#include <bits/stdc++.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "protocol.h"

using namespace std;
/*Global Variables*/
bool network_layer_state=true;
unsigned int client;
unsigned int server;
timer t;
packet network_packets[8];
int network_counter=0;
event_type current_event=network_layer_ready;

int main()
{
    char choice;
    int state, missed_ack;
    cout<<"Miss Acknowledge? [Y/N]"<<endl;
    cin>> choice;
    if(choice == 'Y')
    {
        state = 1;
        cout<<"Enter Frame Number You Want To Miss!"<<endl;
        cin>>missed_ack;
    }
    else if(choice == 'N')
    {
        state = 0;
    }
    else{
        /*Do Nothing*/
    }
    int next_seq_num = 0;
    frame r;
    frame s;
    Connect_Slave();
    int flag=0;
    while(true)
    {
        from_physical_layer(&r);

        if(r.seq==missed_ack && state==1)
        {
            flag++;
        }
        if(r.seq==next_seq_num && flag !=1)
        {
            cout<<"Ack For Frame ";
            cout<<next_seq_num;
            cout<<" Is Sent!"<<endl;
            s.ack = next_seq_num;
            to_physical_layer(&s);
            inc(next_seq_num);
        }
        else
        {
            cout<<"Ack For Frame ";
            cout<<next_seq_num;
            cout<<" Is Not Sent!"<<endl;
            s.ack=next_seq_num-1;
            to_physical_layer(&s);


        }


    }


    return 0;
}


static bool between(seq_nr a, seq_nr b, seq_nr c)
{
    /* Return true if a <= b < c circularly; false otherwise. */
    if (((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a)))
    return true;
    else
    return false;
}
static void send_data(seq_nr frame_nr, seq_nr frame_expected, packet buffer[ ])
{
    /* Construct and send a data frame. */
    frame s; /* scratch variable */
    s.info = buffer[frame_nr]; /* insert packet into frame */
    s.seq = frame_nr; /* insert sequence number into frame */
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1); /* piggyback ack */
    to_physical_layer(&s); /* transmit the frame */
    start_timer(frame_nr); /* start the timer running */
    cout<< "Frame ";
    cout<<frame_nr;
    cout<<" Sent To Physical Layer"<<endl;
}

void enable_network_layer(void)
{
    network_layer_state=true;
}
void disable_network_layer(void)
{
    network_layer_state=false;
}
void to_physical_layer(frame*s){
    write(client, s, sizeof(frame));
}
void from_physical_layer(frame*r){
    read(client, r, sizeof(frame));
}
void start_timer(seq_nr k)
{
    t.flag[k]=true;
}
void stop_timer(seq_nr k)
{
    t.flag[k]=false;
}
void from_network_layer(packet*p)
{
    *p=network_packets[(network_counter++)%8];
}
void to_network_layer(packet *p)
{

}
void wait_for_event(event_type *event)
{
    *event=current_event;
}
void Connect_Slave(void)
{
    client = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    cout<<"Waiting for Connection..."<<endl;
    while (connect(client, (struct sockaddr *)&address, sizeof(address)));
    cout<<"Receiver Connected"<<endl;
}

void Connect_Master(void){

    cout<<"Sender Waiting"<<endl;
    server = socket(AF_INET, SOCK_STREAM, 0);
    int opt=1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                          &opt, sizeof(opt));
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);
    bind(server, (struct sockaddr *)&address, sizeof(address));
    listen(server, 1);
    client = accept(server, NULL, NULL);
    cout<<"Receiver connected"<<endl;
}










