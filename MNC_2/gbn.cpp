#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>

/* ******************************************************************
 * ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose
 *
 * This code should be used for PA2, unidirectional or bidirectional
 * data transfer protocols (from A to B. Bidirectional transfer of data
 * is for extra credit and is not required).  Network properties:
 * - one way network delay averages five time units (longer if there
 * are other messages in the channel for GBN), but can be larger
 * - packets can be corrupted (either the header or the data portion)
 * or lost, according to user-defined probabilities
 * - packets will be delivered in the order in which they were sent
 * (although some can be lost).
 **********************************************************************/

#define BIDIRECTIONAL 0    /* change to 1 if you're doing extra credit */
/* and write a routine called B_output */

/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.         */
struct msg {
	char data[20];
};

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */
struct pkt {
	int seqnum;
	int acknum;
	int checksum;
	char payload[20];
};

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
#define WINDOW_SIZE 50
#define TOTAL_SEQUENCE_NOS 1200
#define BUFFER_SIZE 200//Total Buffer
#define TIMEOUT 40 //Timeout for retransmission
using namespace std;
vector<pkt> window_vector;
vector<pkt> buffer;
struct pkt prev_packet;

int seq_no = 0; //Sending Seq_no
int exp_seq_no = 0; //Expected Sequence No Receiver
int exp_ack_no = 0; //Expected Ack No Sender
float time_local = 0;

//analysis variables
int number_1 = 0;
int number_2 = 0;
int number_3 = 0;
int number_4 = 0;

void tolayer3(int AorB, struct pkt packet);
void tolayer5(int AorB, char *datasent);
void starttimer(int AorB, float increment);
void stoptimer(int AorB);

int calc_checksum(int sequence_no, int acknowledgement_no,
		char* payload_to_add) {
	int sum = 0;
	sum = sequence_no + acknowledgement_no;
	int i;
	for (i = 0; i < 20; i++) {
		sum += payload_to_add[i];
	}
	//printf("Sum = %d",sum);
	return sum;
}

void display_vector(vector<pkt> &v) {
	printf("Vector has :\n");
	if (v.size() == 0) {
		printf("Nothing (Empty)\n");
		return;
	}
	for (int i = 0; i < v.size(); i++) {
		printf("Seq No : %d Ack No : %d Payload : %s\n", v[i].seqnum,
				v[i].acknum, v[i].payload);
	}
}
//sending all packets of vector
void send_all_packets(vector<pkt> &v) {
	printf("Sending all the packets in Vector : \n");
	for (int i = 0; i < v.size(); i++) {
		printf("Sending %d %s\n", v[i].seqnum, v[i].payload);
		number_2++;
		tolayer3(0, v[i]);
	}
}
//check if a packet is corrupt
int check_if_corrupt(pkt &p) {

	//printf("In Check if corrupt\n");
	int check_for_error = 0;
	check_for_error = calc_checksum(p.seqnum, p.acknum, p.payload);
	if (check_for_error == p.checksum)
		return 0;
	else
		return 1;

}

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message) //ram's comment - students can change the return type of the function from struct to pointers if necessary
		{

	printf("in A_output\n");
	number_1++;
	struct pkt packet_to_send;
	packet_to_send.seqnum = seq_no;
	packet_to_send.acknum = seq_no;
	memcpy(packet_to_send.payload, message.data, 20);
	packet_to_send.checksum = calc_checksum(packet_to_send.seqnum,
			packet_to_send.acknum, packet_to_send.payload);
	//Window is not FULL
	if (window_vector.size() < WINDOW_SIZE) {
		printf("Sending Message : %s\n", message.data);

		printf("Seq No : %d Ack No : %d Checksum : %d\n", packet_to_send.seqnum,
				packet_to_send.acknum, packet_to_send.checksum);

		//Timer according to first packet
		if (window_vector.size() == 0) {
			printf("STARTING TIMER AT %f \n", time_local);
			printf("NEXT TIMEOUT AT %f \n",time_local+TIMEOUT);
			starttimer(0, TIMEOUT);
		}
		window_vector.push_back(packet_to_send);
		display_vector(window_vector);
		seq_no = (seq_no + 1) % TOTAL_SEQUENCE_NOS;
		number_2++;
		tolayer3(0, packet_to_send);

	}
	//Discard Packets outside window
	else {
		if (buffer.size() == BUFFER_SIZE) {
			printf("BUFFER_SIZE EXCEEDED..Discard Packet\n");
			return;
		}

		printf("WINDOW FULL..Adding data %s to Buffer\n", message.data);
		buffer.push_back(packet_to_send);
		display_vector(buffer);
		seq_no = (seq_no + 1) % TOTAL_SEQUENCE_NOS;

		return;
	}

}

void B_output(struct msg message) /* need be completed only for extra credit */
// ram's comment - students can change the return type of this function from struct to pointers if needed
		{

}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) {
	printf("in A_input\n");
	printf("Waiting For %d and Received %d\n", exp_ack_no, packet.acknum);
	printf("ACK_received Payload %s \n", packet.payload);
	if (check_if_corrupt(packet) == 0) {

		int diff = (packet.acknum + TOTAL_SEQUENCE_NOS - exp_ack_no)
				% TOTAL_SEQUENCE_NOS;
		printf("ACK  with no corruption received...Difference is : %d\n", diff);
		fflush(stdout);
		//for checking duplicate ACKS
		if (diff >= 0 && diff < WINDOW_SIZE) {
			printf("Vector Before erasing\n");
			display_vector(window_vector);
			window_vector.erase(window_vector.begin(),
					window_vector.begin() + diff + 1);
			int no_of_elements_to_add = diff + 1;
			printf("Adding to window from buffer\n");
			printf("Buffer before\n");
			display_vector(buffer);
			//adding elements from buffer to window vector
			while (no_of_elements_to_add && !buffer.empty()) {
				window_vector.push_back(buffer.at(0));
				printf("Sending %s \n",buffer.at(0).payload);
				number_2++;
								tolayer3(0,buffer.at(0));
				buffer.erase(buffer.begin(), buffer.begin() + 1);
				no_of_elements_to_add--;
			}

			//buffer.erase(buffer.begin(),buffer.begin()+diff+1);
			printf("Buffer after erasing\n");
			display_vector(buffer);

			printf("Vector after erasing and adding from buffer\n");
			display_vector(window_vector);
			if (window_vector.size() == 0) {
				printf("STOPPING TIMER AT %f \n", time_local);;
				stoptimer(0);
			} else {
				printf("TIMER RESTARTS.\n");
				stoptimer(0);
				printf("NEXT TIMEOUT %f\n",time_local+TIMEOUT);
				starttimer(0, TIMEOUT);
			}

			exp_ack_no = (exp_ack_no + diff + 1) % TOTAL_SEQUENCE_NOS;
		} //end of diff if
		else {
			printf("Duplicate ACK No %d received..Ignore it\n", packet.acknum);
		}
	} else {
		if (check_if_corrupt(packet) == 1) {
			printf("Corrupted ACK Received.\n");
		}
		fflush(stdout);

	}

}

/* called when A's timer goes off */
void A_timerinterrupt() //ram's comment - changed the return type to void.
{
	printf("TIMEOUT at %f..RETRANSMITTING ALL THE PACKETS IN VECTOR\n",time_local);
	starttimer(0, TIMEOUT);
	send_all_packets(window_vector);
	printf("NEXT TIMEOUT %f\n ",time_local+TIMEOUT);

}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init() //ram's comment - changed the return type to void.
{
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet) {
	printf("                          in B_input \n");
	number_3++;
	printf("                          Waiting For %d and Received %d\n",
			exp_seq_no, packet.seqnum);
	printf("                          Packet: Payload %s\n", packet.payload);
	if (check_if_corrupt(packet) == 0 && exp_seq_no == packet.seqnum) {
		printf(
				"                          Desired packet with no corruption...Sending ACK \n");
		char message_above[20];
		memcpy(message_above, packet.payload, 20);
		printf("                          Sending message %s above \n",
				message_above);
		number_4++;
		tolayer5(1, message_above);
		struct pkt ack_packet = packet;
		ack_packet.checksum = calc_checksum(ack_packet.seqnum,
				ack_packet.acknum, ack_packet.payload);
		prev_packet = ack_packet;
		tolayer3(1, ack_packet);
		exp_seq_no = (exp_seq_no + 1) % TOTAL_SEQUENCE_NOS;
	} else {
		if (check_if_corrupt(packet) == 1) {
			printf(
					"                          Corrupted Packet Received...Not Sending ACK\n");
			return;
		}
		if (exp_seq_no != packet.seqnum) {
			printf(
					"                          Non Desired Packet Received...Sending Previous ACK %d\n",
					prev_packet.acknum);
			//printf("prev packet %d %d %s\n",prev_packet.seqnum,prev_packet.acknum,prev_packet.payload);
			tolayer3(1, prev_packet);
			//printf("                          here\n");
			fflush(stdout);


		}

	}

}

/* called when B's timer goes off */
void B_timerinterrupt() //ram's comment - changed the return type to void.
{
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init() //ram's comment - changed the return type to void.
{
	prev_packet.seqnum = -1;
	prev_packet.acknum = -1;
	memcpy(prev_packet.payload, "previous_packet_sent", 20);
	prev_packet.checksum = calc_checksum(prev_packet.seqnum, prev_packet.acknum,
			prev_packet.payload);

}

int TRACE = 1; /* for my debugging */
int nsim = 0; /* number of messages from 5 to 4 so far */
int nsimmax = 0; /* number of msgs to generate, then stop */

float lossprob; /* probability that a packet is dropped  */
float corruptprob; /* probability that one bit is packet is flipped */
float lambda; /* arrival rate of messages from layer 5 */
int ntolayer3; /* number sent into layer 3 */
int nlost; /* number lost in media */
int ncorrupt; /* number corrupted by media*/

/****************************************************************************/
/* jimsrand(): return a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in therange [0,mmm]        */
/****************************************************************************/
float jimsrand() {
	double mmm = 2147483647; /* largest int  - MACHINE DEPENDENT!!!!!!!!   */
	float x; /* individual students may need to change mmm */
	x = rand() / mmm; /* x should be uniform in [0,1] */
	return (x);
}

/*****************************************************************
 ***************** NETWORK EMULATION CODE IS BELOW ***********
 * The code below emulates the layer 3 and below network environment:
 * - emulates the tranmission and delivery (possibly with bit-level corruption
 * and packet loss) of packets across the layer 3/4 interface
 * - handles the starting/stopping of a timer, and generates timer
 * interrupts (resulting in calling students timer handler).
 * - generates message to be sent (passed from later 5 to 4)
 *
 * THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
 * THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
 * OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
 * the emulator, you're welcome to look at the code - but again, you should have
 * to, and you defeinitely should not have to modify
 ******************************************************************/

/* possible events: */
#define  TIMER_INTERRUPT 0
#define  FROM_LAYER5     1
#define  FROM_LAYER3     2

#define  OFF             0
#define  ON              1
#define   A    0
#define   B    1

struct event {
	float evtime; /* event time */
	int evtype; /* event type code */
	int eventity; /* entity where event occurs */
	struct pkt *pktptr; /* ptr to packet (if any) assoc w/ this event */
	struct event *prev;
	struct event *next;
};
struct event *evlist = NULL; /* the event list */

void insertevent(struct event *p) {
	struct event *q, *qold;

	if (TRACE > 2) {
		printf("            INSERTEVENT: time is %lf\n", time_local);
		printf("            INSERTEVENT: future time will be %lf\n", p->evtime);
	}
	q = evlist; /* q points to header of list in which p struct inserted */
	if (q == NULL) { /* list is empty */
		evlist = p;
		p->next = NULL;
		p->prev = NULL;
	} else {
		for (qold = q; q != NULL && p->evtime > q->evtime; q = q->next)
			qold = q;
		if (q == NULL) { /* end of list */
			qold->next = p;
			p->prev = qold;
			p->next = NULL;
		} else if (q == evlist) { /* front of list */
			p->next = evlist;
			p->prev = NULL;
			p->next->prev = p;
			evlist = p;
		} else { /* middle of list */
			p->next = q;
			p->prev = q->prev;
			q->prev->next = p;
			q->prev = p;
		}
	}
}

/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/

void generate_next_arrival() {
	double x, log(), ceil();
	struct event *evptr;
//    //char *malloc();
	float ttime;
	int tempint;

	if (TRACE > 2)
		printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");

	x = lambda * jimsrand() * 2; /* x is uniform on [0,2*lambda] */
	/* having mean of lambda        */

	evptr = (struct event *) malloc(sizeof(struct event));
	evptr->evtime = time_local + x;
	evptr->evtype = FROM_LAYER5;
	if (BIDIRECTIONAL && (jimsrand() > 0.5))
		evptr->eventity = B;
	else
		evptr->eventity = A;
	insertevent(evptr);
}

void init() /* initialize the simulator */{
	int i;
	float sum, avg;
	float jimsrand();

	printf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n\n");
	printf("Enter the number of messages to simulate: ");
	 scanf("%d", &nsimmax);
//	nsimmax = 1000;
//	lossprob = 0.6;
//	corruptprob = 0.2;
//	lambda = 50;
//	TRACE = 1;
	printf("Enter  packet loss probability [enter 0.0 for no loss]:");
	 scanf("%f", &lossprob);
	printf("Enter packet corruption probability [0.0 for no corruption]:");
	 scanf("%f", &corruptprob);
	printf(
			"Enter average time between messages from sender's layer5 [ > 0.0]:");
	 scanf("%f", &lambda);
	printf("Enter TRACE:");
	 scanf("%d", &TRACE);

	srand(9999); /* init random number generator */
	sum = 0.0; /* test random number generator for students */
	for (i = 0; i < 1000; i++)
		sum = sum + jimsrand(); /* jimsrand() should be uniform in [0,1] */
	avg = sum / 1000.0;
	if (avg < 0.25 || avg > 0.75) {
		printf("It is likely that random number generation on your machine\n");
		printf("is different from what this emulator expects.  Please take\n");
		printf(
				"a look at the routine jimsrand() in the emulator code. Sorry. \n");
		exit(0);
	}

	ntolayer3 = 0;
	nlost = 0;
	ncorrupt = 0;

	time_local = 0; /* initialize time to 0.0 */
	generate_next_arrival(); /* initialize event list */
}

//int TRACE = 1;             /* for my debugging */
//int nsim = 0;              /* number of messages from 5 to 4 so far */
//int nsimmax = 0;           /* number of msgs to generate, then stop */
//float time = 0.000;
//float lossprob;            /* probability that a packet is dropped  */
//float corruptprob;         /* probability that one bit is packet is flipped */
//float lambda;              /* arrival rate of messages from layer 5 */
//int   ntolayer3;           /* number sent into layer 3 */
//int   nlost;               /* number lost in media */
//int ncorrupt;              /* number corrupted by media*/

main() {
	struct event *eventptr;
	struct msg msg2give;
	struct pkt pkt2give;

	int i, j;
	char c;

	init();
	A_init();
	B_init();

	while (1) {
		eventptr = evlist; /* get next event to simulate */
		if (eventptr==NULL)
		goto terminate;
		evlist = evlist->next; /* remove this event from event list */
		if (evlist!=NULL)
		evlist->prev=NULL;
		if (TRACE>=2) {
			printf("\nEVENT time: %f,", eventptr->evtime);
			printf("  type: %d", eventptr->evtype);
			if (eventptr->evtype==0)
			printf(", timerinterrupt  ");
			else if (eventptr->evtype==1)
			printf(", fromlayer5 ");
			else
			printf(", fromlayer3 ");
			printf(" entity: %d\n", eventptr->eventity);
		}
		time_local = eventptr->evtime; /* update time to next event time */
		if (nsim==nsimmax)
		break; /* all done with simulation */
		if (eventptr->evtype == FROM_LAYER5 ) {
			generate_next_arrival(); /* set up future arrival */
			/* fill in msg to give with string of same letter */
			j = nsim % 26;
			for (i=0; i<20; i++)
			msg2give.data[i] = 97 + j;
			if (TRACE>2) {
				printf("          MAINLOOP: data given to student: ");
				for (i=0; i<20; i++)
				printf("%c", msg2give.data[i]);
				printf("\n");
			}
			nsim++;
			if (eventptr->eventity == A)
			A_output(msg2give);
			else
			B_output(msg2give);
		}
		else if (eventptr->evtype == FROM_LAYER3) {
			pkt2give.seqnum = eventptr->pktptr->seqnum;
			pkt2give.acknum = eventptr->pktptr->acknum;
			pkt2give.checksum = eventptr->pktptr->checksum;
			for (i=0; i<20; i++)
			pkt2give.payload[i] = eventptr->pktptr->payload[i];
			if (eventptr->eventity ==A) /* deliver packet by calling */
			A_input(pkt2give); /* appropriate entity */
			else
			B_input(pkt2give);
			free(eventptr->pktptr); /* free the memory for packet */
		}
		else if (eventptr->evtype == TIMER_INTERRUPT) {
			if (eventptr->eventity == A)
			A_timerinterrupt();
			else
			B_timerinterrupt();
		}
		else {
			printf("INTERNAL PANIC: unknown event type \n");
		}
		free(eventptr);
	}

	//FOR ANALYSIS PURPOSE

		printf("Protocol : Go-Back N\n");
		printf("%d of Packets sent from Application Layer of Sender A\n",number_1);
		printf("%d of Packets sent from Transport Layer of Sender A\n",number_2);
		printf("%d of Packets received at Transport Layer of Receiver B\n",number_3);
		printf("%d of Packets received at Application Layer of Receiver B\n",number_4);
		printf("Total Time : %f Time Units\n",time_local);
		printf("Throughput = %f Packets /time units\n",number_4/time_local);

	terminate:
	printf(" Simulator terminated at time %f\n after sending %d msgs from layer5\n", time_local, nsim);
}

/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/

/*void generate_next_arrival()
 * {
 * double x,log(),ceil();
 * struct event *evptr;
 * //char *malloc();
 * float ttime;
 * int tempint;
 *
 * if (TRACE>2)
 * printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");
 *
 * x = lambda*jimsrand()*2;  // x is uniform on [0,2*lambda]
 * // having mean of lambda
 * evptr = (struct event *)malloc(sizeof(struct event));
 * evptr->evtime =  time + x;
 * evptr->evtype =  FROM_LAYER5;
 * if (BIDIRECTIONAL && (jimsrand()>0.5) )
 * evptr->eventity = B;
 * else
 * evptr->eventity = A;
 * insertevent(evptr);
 * } */

void printevlist() {
	struct event *q;
	int i;
	printf("--------------\nEvent List Follows:\n");
	for (q = evlist; q != NULL; q = q->next) {
		printf("Event time: %f, type: %d entity: %d\n", q->evtime, q->evtype,
				q->eventity);
	}
	printf("--------------\n");
}

/********************** Student-callable ROUTINES ***********************/

/* called by students routine to cancel a previously-started timer */
void stoptimer(int AorB)
//AorB;  /* A or B is trying to stop timer */
		{
	struct event *q, *qold;

	if (TRACE > 2)
		printf("          STOP TIMER: stopping timer at %f\n", time_local);
	/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
	for (q = evlist; q != NULL; q = q->next)
		if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB)) {
			/* remove this event */
			if (q->next == NULL && q->prev == NULL)
				evlist = NULL; /* remove first and only event on list */
			else if (q->next == NULL) /* end of list - there is one in front */
				q->prev->next = NULL;
			else if (q == evlist) { /* front of list - there must be event after */
				q->next->prev = NULL;
				evlist = q->next;
			} else { /* middle of list */
				q->next->prev = q->prev;
				q->prev->next = q->next;
			}
			free(q);
			return;
		}
	printf("Warning: unable to cancel your timer. It wasn't running.\n");
}

void starttimer(int AorB, float increment)
// AorB;  /* A or B is trying to stop timer */

		{

	struct event *q;
	struct event *evptr;
	////char *malloc();

	if (TRACE > 2)
		printf("          START TIMER: starting timer at %f\n", time_local);
	/* be nice: check to see if timer is already started, if so, then  warn */
	/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
	for (q = evlist; q != NULL; q = q->next)
		if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB)) {
			printf(
					"Warning: attempt to start a timer that is already started\n");
			return;
		}

	/* create future event for when timer goes off */
	evptr = (struct event *) malloc(sizeof(struct event));
	evptr->evtime = time_local + increment;
	evptr->evtype = TIMER_INTERRUPT;
	evptr->eventity = AorB;
	insertevent(evptr);
}

/************************** TOLAYER3 ***************/
void tolayer3(int AorB, struct pkt packet) {
	struct pkt *mypktptr;
	struct event *evptr, *q;
	////char *malloc();
	float lastime, x, jimsrand();
	int i;

	ntolayer3++;

	/* simulate losses: */
	if (jimsrand() < lossprob) {
		nlost++;
		if (TRACE > 0)
			printf("          TOLAYER3: packet being lost\n");
		return;
	}

	/* make a copy of the packet student just gave me since he/she may decide */
	/* to do something with the packet after we return back to him/her */
	mypktptr = (struct pkt *) malloc(sizeof(struct pkt));
	mypktptr->seqnum = packet.seqnum;
	mypktptr->acknum = packet.acknum;
	mypktptr->checksum = packet.checksum;
	for (i = 0; i < 20; i++)
		mypktptr->payload[i] = packet.payload[i];
	if (TRACE > 2) {
		printf("          TOLAYER3: seq: %d, ack %d, check: %d ",
				mypktptr->seqnum, mypktptr->acknum, mypktptr->checksum);
		for (i = 0; i < 20; i++)
			printf("%c", mypktptr->payload[i]);
		printf("\n");
	}

	/* create future event for arrival of packet at the other side */
	evptr = (struct event *) malloc(sizeof(struct event));
	evptr->evtype = FROM_LAYER3; /* packet will pop out from layer3 */
	evptr->eventity = (AorB + 1) % 2; /* event occurs at other entity */
	evptr->pktptr = mypktptr; /* save ptr to my copy of packet */
	/* finally, compute the arrival time of packet at the other end.
	 * medium can not reorder, so make sure packet arrives between 1 and 10
	 * time units after the latest arrival time of packets
	 * currently in the medium on their way to the destination */
	lastime = time_local;
	/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next) */
	for (q = evlist; q != NULL; q = q->next)
		if ((q->evtype == FROM_LAYER3 && q->eventity == evptr->eventity))
			lastime = q->evtime;
	evptr->evtime = lastime + 1 + 9 * jimsrand();

	/* simulate corruption: */
	if (jimsrand() < corruptprob) {
		ncorrupt++;
		if ((x = jimsrand()) < .75)
			mypktptr->payload[0] = 'Z'; /* corrupt payload */
		else if (x < .875)
			mypktptr->seqnum = 999999;
		else
			mypktptr->acknum = 999999;
		if (TRACE > 0)
			printf("          TOLAYER3: packet being corrupted\n");
	}

	if (TRACE > 2)
		printf("          TOLAYER3: scheduling arrival on other side\n");
	insertevent(evptr);
}

void tolayer5(int AorB, char *datasent) {

	int i;
	if (TRACE > 2) {
		printf("          TOLAYER5: data received: ");
		for (i = 0; i < 20; i++)
			printf("%c", datasent[i]);
		printf("\n");
	}

}