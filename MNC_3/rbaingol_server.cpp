#include <string.h>
#include<stdio.h>
#include <fstream>
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<limits.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include<sstream>
#include<unistd.h>

#include<algorithm>

#define INF INT_MAX
#define MAXBUFLEN 100

using namespace std;

int no_of_servers = 0;
int no_of_neighbours = 0;
int my_server_id = 0;
string my_port = "";
string my_ip_address = "";
int neighbour_routing_table_size = 10;
vector<string> arr;
int no_of_dist_vect_packets = 0;

struct server_table_struct {
	int server_id;
	string server_ip_address;
	int port_no;
};

//This is the Struct used for Routing Table.Routing Table is an array of these structs.
struct routing_table_struct {
	int my_id;
	int node_id;
	string node_ip_address;
	string node_port;
	int cost_;
	int next_hop_server_id;
};
struct neighbour_table_struct {
	int server_id_;
	int neighbour_id;
	int cost;
	int is_disabled;
	routing_table_struct neighbour_routing_table[10];

};
int get_index_from_id(neighbour_table_struct* n, int i) {
	for (int k = 0; k < no_of_neighbours; k++) {
		if (n[k].neighbour_id == i) {
			return k;
		}
	}
	return -1;

}

int get_id_from_ip(string s, int p, server_table_struct* t) {
	for (int d = 0; d < no_of_servers; d++) {
		if (strcmp(t[d].server_ip_address.c_str(), s.c_str()) == 0
				&& t[d].port_no == p) {
			return t[d].server_id;
		}
	}

	return -1;
}

string int_to_string(int a) {
	stringstream ss;
	ss << a;
	return ss.str();
}

string short_to_string(short a) {
	stringstream ss;
	ss << a;
	return ss.str();
}

//Get port from IP
int get_port_from_ip(string s, server_table_struct* a) {
	for (int g = 0; g < no_of_servers; g++) {
		if (strcmp(s.c_str(), a[g].server_ip_address.c_str()) == 0) {
			return a[g].port_no;
		}
	}
	return -1;
}

//Calculation for algorithm
int algorithm(int source, int dest, neighbour_table_struct* n_table,
		routing_table_struct* neighbour_r) {

	if (neighbour_r[0].my_id != -1) {
		int via = neighbour_r[0].my_id;

		//Find the first parameter cost from source to via
		int first_parameter;
		int second_parameter;
		for (int d = 0; d < no_of_neighbours; d++) {

			if ((n_table[d].server_id_ == source)
					&& (n_table[d].neighbour_id == via)) {
				first_parameter = n_table[d].cost;
				break;
			}

//			if ((n_table[d].my_id == source) && (r[d].node_id) == via) {
//				first_parameter = r[d].cost_;
//				cout << "first parameter from " << r[d].my_id << " to "
//						<< r[d].node_id << " cost " << first_parameter << endl;
//				break;
//			}

		}
		//Find the second parameter cost from via to destination
		for (int k = 0; k < no_of_servers; k++) {
			if ((neighbour_r[k].my_id == via)
					&& (neighbour_r[k].node_id == dest)) {
				second_parameter = neighbour_r[k].cost_;
//				cout << "second parameter from " << neighbour_r[k].my_id
//						<< " to " << neighbour_r[k].node_id << " cost "
//						<< second_parameter << endl;
				break;

			}
		}
//		cout << "first parameter " << first_parameter << " " << second_parameter
//				<< endl;

		if (first_parameter == INT_MAX || second_parameter == INT_MAX) {
			//cout << "Returning " << INT_MAX<<endl;
			return INT_MAX;
		}
		//cout << "outside " << first_parameter << " " << second_parameter;
		int g = (first_parameter + second_parameter);
		//cout << " Returning " << g << endl;

		return g;

	} else {
		//cout << "No Routing table for this neighbour ";
		return INT_MAX;
	}

}

//find minimum time
int find_min_time(vector<int> &v) {
	if (v.empty()) {
		printf("Time Vector is empty so returning 0.0\n");
		return -1;
	}
	int min = INT_MAX;

	int min_index;
	for (int i = 0; i < v.size(); i++) {
		//printf("v[i] %f min %f\n", v[i], min);
		if (v[i] < min && v[i] != -1) {

			min = v[i];
			min_index = i;
		}

	}
	//printf("min_index %d\n", min_index);
	return min_index;

}

//For Splitting a String
vector<string> split(string str, string delim) {
	char* const_char = const_cast<char*>(str.c_str());
	char* token;
	vector<string> result;
	token = strtok(const_char, delim.c_str());
	while (token != NULL) {
		result.push_back(token);
		token = strtok(NULL, delim.c_str());
	}
	return result;
}

string get_server_ip_from_id(int id, server_table_struct* s) {
	for (int i = 0; i < no_of_servers; i++) {
		if (id == s[i].server_id) {
			return s[i].server_ip_address;

		}
	}
	return "-1";

}

void display_vector(vector<int> &v, neighbour_table_struct* n) {
	if (v.size() == 0) {
		printf("Nothing (Empty)\n");
		return;
	}
	for (unsigned int i = 0; i < v.size(); i++) {
		printf("%d Timeout %d\n", n[i].neighbour_id, v[i]);
	}
}

void send_periodic_updates(string ip_to_send_to, int port_to_send_to_int,
		routing_table_struct* r) {

	string port_to_send = int_to_string(port_to_send_to_int);

	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(ip_to_send_to.c_str(), port_to_send.c_str(), &hints,
			&servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return;
	}

	// loop through all the results and make a socket
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return;
	}
	//port_to_send = "abcd";

	/*Make the packet to send
	 short for all the 2 bytes entities
	 For IP address I converted the Each Part of IP to an ASCII VALUE and send it
	 */
	char string_to_send[8 + (12 * no_of_servers)];

	short no_of_update_fields = no_of_servers;
	memcpy(string_to_send, &no_of_update_fields, 2);

	short sender_port = atoi(my_port.c_str());
	memcpy(string_to_send + 2, &sender_port, 2);

//convert IP to ASCII (4 Bytes)
	vector<string> c = split(my_ip_address.c_str(), ".");
	int ip_1_n = atoi(c[0].c_str());
	int ip_2_n = atoi(c[1].c_str());
	int ip_3_n = atoi(c[2].c_str());
	int ip_4_n = atoi(c[3].c_str());
	char ip_1 = ip_1_n;
	char ip_2 = ip_2_n;
	char ip_3 = ip_3_n;
	char ip_4 = ip_4_n;

	memcpy(string_to_send + 4, &ip_1, 1);
	memcpy(string_to_send + 5, &ip_2, 1);
	memcpy(string_to_send + 6, &ip_3, 1);
	memcpy(string_to_send + 7, &ip_4, 1);

	int count = 8; //position of bytes

	for (int g = 0; g < no_of_servers; g++) {

		//Send N Server IP Addresses
		c = split(r[g].node_ip_address.c_str(), ".");
		ip_1_n = atoi(c[0].c_str());
		ip_2_n = atoi(c[1].c_str());
		ip_3_n = atoi(c[2].c_str());
		ip_4_n = atoi(c[3].c_str());
		///cout << ip_1_n << "." << ip_2_n << "." << ip_3_n << "." << ip_4_n				<< " ";
		ip_1 = ip_1_n;
		ip_2 = ip_2_n;
		ip_3 = ip_3_n;
		ip_4 = ip_4_n;

		memcpy(string_to_send + count++, &ip_1, 1);
		memcpy(string_to_send + count++, &ip_2, 1);
		memcpy(string_to_send + count++, &ip_3, 1);
		memcpy(string_to_send + count++, &ip_4, 1);

		//Send the N Server Ports
		short sender_port_n = atoi(r[g].node_port.c_str());
		memcpy(string_to_send + count++, &sender_port_n, 2);
		count++;

		//For the blank space in the message
		count++;

		//Send the N Server IDs

		short server_id_n = r[g].node_id;
		memcpy(string_to_send + count++, &server_id_n, 2);
		count++;
		//cout << "  " << server_id_n << " ";

		//Send the cost of N Servers
		short server_cost_n = r[g].cost_;
		if (r[g].cost_ == INT_MAX) {
			server_cost_n = SHRT_MAX;

		}

		memcpy(string_to_send + count++, &server_cost_n, 2);
		count++;
		//cout << "  " << server_cost_n << endl;

	}
//cout<<"sending "<<ip_to_send_to<<" : "<<port_to_send_to_int<<endl;
	if ((numbytes = sendto(sockfd, string_to_send, sizeof(string_to_send), 0,
			p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}
	freeaddrinfo(servinfo);
	close(sockfd);

	return;

}

void sort_routing_table(routing_table_struct* r) {
	for (int i = 0; i < no_of_servers; i++) {
		for (int j = 0; j < no_of_servers; j++) {
			if (r[i].node_id < r[j].node_id) {
				routing_table_struct temp = r[i];
				r[i] = r[j];
				r[j] = temp;
			}
		}

	}

}

string get_port_from_id(server_table_struct* p, int m) {
	for (int i = 0; i < no_of_servers; i++) {
		if (p[i].server_id == m) {
			stringstream ss;
			ss << p[i].port_no;
			return ss.str();

		}
	}
	return "-1";
}

// get sockaddr, IPv4 or IPv6:

void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

//Display Server Table
void display_table(server_table_struct* p) {
	printf(".......Server_Table.....\n");
	printf("Server_ID     \t   Server_IP  \t    Port_No \n");

	for (int i = 0; i < no_of_servers; i++) {
		printf("%d    \t    %s    \t  %d \n", p[i].server_id,
				p[i].server_ip_address.c_str(), p[i].port_no);
	}
}
//Display Neighbour Table
void display_ntable(neighbour_table_struct* p) {
	printf(".......Neighbour_Table.....\n");
	printf("Server_ID \t   Neighbour_ID\t    Cost \n");

	for (int i = 0; i < no_of_neighbours; i++) {
		printf("%d    \t    %d    \t  %d \n", p[i].server_id_,
				p[i].neighbour_id, p[i].cost);
	}
}
//Display Routing Table
void display_rtable(routing_table_struct* p) {
	printf(".......Routing_Table.....\n");
	printf(
			"Server_ID  Node_ID   Node_IP_Address   Node_Port  Cost  Next_Hop_ID\n");

	for (int i = 0; i < no_of_servers; i++) {
		printf("%d    \t    %d \t %s  \t %s \t", p[i].my_id, p[i].node_id,
				p[i].node_ip_address.c_str(), p[i].node_port.c_str());
		//string cost;
		if (p[i].cost_ == INT_MAX) {
			printf("%s\t", "INF");
		} else {
			printf("%d\t", p[i].cost_);
		}
		printf("%d\n", p[i].next_hop_server_id);

	}
}
void display_test_rtable(routing_table_struct* p) {
	printf(".......Routing_Table.....\n");
	printf("destination-server-ID   next-hop-server-ID  cost-of-path\n");

	for (int i = 0; i < no_of_servers; i++) {
		printf("%d \t %d  \t", p[i].node_id, p[i].next_hop_server_id);
		if (p[i].cost_ == INT_MAX) {
			printf("%s\n", "INF");
		} else {
			printf("%d\n", p[i].cost_);
		}

	}
}
//Display neighbour routing table
void display_neigh_rtable(routing_table_struct* p) {
	printf(".......Routing_Table.....\n");
	printf("Server_ID  Node_ID   Node_IP_Address   Cost  \n");

	for (int i = 0; i < no_of_servers; i++) {
		printf("%d    \t    %d \t %s  \t\t", p[i].my_id, p[i].node_id,
				p[i].node_ip_address.c_str());
		//string cost;
		if (p[i].cost_ == INT_MAX) {
			printf("%s\t", "INF");
		} else {
			printf("%d", p[i].cost_);
		}
		printf("\n");

	}
}
void routing_update(routing_table_struct* routing_table,
		neighbour_table_struct* neighbour_table,
		server_table_struct* all_server_table) {

//			Calculating the Routing Table again
	int f = 0;
	for (f = 0; f < no_of_neighbours; f++) {
		routing_table[f].my_id = my_server_id;
		routing_table[f].node_id = neighbour_table[f].neighbour_id;
		routing_table[f].node_ip_address = get_server_ip_from_id(
				routing_table[f].node_id, all_server_table);
		routing_table[f].node_port = get_port_from_id(all_server_table,
				routing_table[f].node_id);
		routing_table[f].cost_ = neighbour_table[f].cost;
		if (neighbour_table[f].cost == INT_MAX) {
			routing_table[f].next_hop_server_id = -1;
		} else {
			routing_table[f].next_hop_server_id =
					neighbour_table[f].neighbour_id;
		}

	}
	//Self Loop
	routing_table[f].my_id = my_server_id;
	routing_table[f].next_hop_server_id = my_server_id;
	routing_table[f].node_id = my_server_id;
	routing_table[f].node_ip_address = my_ip_address;
	routing_table[f].node_port = my_port;
	routing_table[f].cost_ = 0;

	//For NON-Neighbours enter Infinity
	int d = 0;
	for (int g = 0; g < no_of_servers; g++) {
		for (int n = 0; n < no_of_neighbours; n++) {
			if (neighbour_table[n].neighbour_id == all_server_table[g].server_id
					|| my_server_id == all_server_table[g].server_id) {
				d = 1;
			}

		}

		if (d == 0) {
//					cout << "ID not present " << all_server_table[g].server_id
//							<< endl;
			f++;
			//cout << "f is " << f << "sie " << no_of_servers << endl;
			routing_table[f].my_id = my_server_id;
			routing_table[f].node_id = all_server_table[g].server_id;
			routing_table[f].node_ip_address = get_server_ip_from_id(
					routing_table[f].node_id, all_server_table);
			//cout << "here " << endl;
			routing_table[f].node_port = get_port_from_id(all_server_table,
					routing_table[f].node_id);
			routing_table[f].cost_ = INT_MAX;
			routing_table[f].next_hop_server_id = -1;

		}
		d = 0;

	}
	sort_routing_table(routing_table);
//			cout<<"ROuting Table new"<<endl;
//			display_rtable(routing_table);

	//Implement DV again
	//Implement DV Algorithm
	for (int f = 0; f < no_of_servers; f++) {
		//For every server except self
		if (routing_table[f].node_id != my_server_id) {
			int source = my_server_id;
			int destination = routing_table[f].node_id;
			//Search every neighbours routing table for minimum cost
			int min_cost = INT_MAX;
			int min_cost_hop_id = -1;
			int cost;
			for (int k = 0; k < no_of_neighbours; k++) {
//				cout << "Source " << source << " Destination " << destination
//						<< " via " << neighbour_table[k].neighbour_id << endl;
				//via = neighbour_table[k].neighbour_id;
				cost = algorithm(source, destination, neighbour_table,
						neighbour_table[k].neighbour_routing_table);
				//cout << "cost " << cost;
				if (cost < min_cost) {
					min_cost_hop_id = neighbour_table[k].neighbour_id;
					min_cost = cost;
				}
				//cout << " min_cost " << min_cost << endl;

			}

			//Updating our own value to the minimum cost and Changing the next hop field
			if (min_cost != INT_MAX && routing_table[f].cost_ > min_cost
					&& min_cost_hop_id != -1) {
//				cout << "changing cost to " << min_cost << "and next hop to "
//						<< min_cost_hop_id << endl;
				routing_table[f].cost_ = min_cost;
				routing_table[f].next_hop_server_id = min_cost_hop_id;
			}

		}

	}

	//scout << "UPDATED NEW ROUTING TABLE " << endl;
	//display_rtable(routing_table);
}

int main(int argc, char* args[]) {

	//printf("No of arguments : %d\n", argc);
	if (argc != 5) {
		cout << "ERROR IN THE ARGUMENTS\n" << endl;
		return 1;
	}
	char* file_name;
	int timeout_interval;
	if (strcmp(args[1], "-t") == 0 && strcmp(args[3], "-i") == 0) {
		file_name = args[2];
		timeout_interval = atoi(args[4]);

	} else if (strcmp(args[3], "-t") == 0 && strcmp(args[1], "-i") == 0) {
		file_name = args[4];
		timeout_interval = atoi(args[2]);
	} else {
		cout << "ERROR IN THE ARGUMENTS\n" << endl;
		return 1;
	}

	string line;
	ifstream input_file;
	//file_name = args[1];

	//Open the File
	input_file.open(file_name);

	// To get information line by line.
	getline(input_file, line);
	no_of_servers = atoi(line.c_str());
	//printf("No of Servers : %d\n", no_of_servers);
	getline(input_file, line);
	no_of_neighbours = atoi(line.c_str());
	//printf("No of Neighbours : %d\n", no_of_neighbours);
	//int l=INT_MAX+INT_MAX;
	//cout<<"int max "<<INT_MAX<<"int max+int_max "<<l<<endl;

	//Server Table
	server_table_struct all_server_table[no_of_servers];
	int n = 0;
	while (n < no_of_servers) {
		getline(input_file, line);
		//cout << line << endl;
		vector<string> a = split(line, " ");
		//cout << a[0] << a[1] << a[2] << endl;
		all_server_table[n].server_id = atoi(a[0].c_str());
		all_server_table[n].server_ip_address = a[1];
		all_server_table[n].port_no = atoi(a[2].c_str());
		n++;

	}

	//Initialize the Routing Table
	routing_table_struct routing_table[no_of_servers];

	//Neighbour Table + one entry for self loop
	neighbour_table_struct neighbour_table[10];
	n = 0;

	while (n < no_of_neighbours) {
		getline(input_file, line);
		vector<string> b = split(line, " ");
		neighbour_table[n].server_id_ = atoi(b[0].c_str());
		routing_table[n].my_id = neighbour_table[n].server_id_;
		my_server_id = neighbour_table[n].server_id_;

		neighbour_table[n].neighbour_id = atoi(b[1].c_str());
		routing_table[n].node_id = neighbour_table[n].neighbour_id;
		routing_table[n].node_ip_address = get_server_ip_from_id(
				routing_table[n].node_id, all_server_table);

		routing_table[n].node_port = get_port_from_id(all_server_table,
				routing_table[n].node_id);

		neighbour_table[n].cost = atoi(b[2].c_str());
		routing_table[n].cost_ = neighbour_table[n].cost;

		routing_table[n].next_hop_server_id = neighbour_table[n].neighbour_id;

		//Initial all the neighbouring routing tables my_id to -1
		neighbour_table[n].neighbour_routing_table[0].my_id = -1;
		neighbour_table[n].is_disabled = 0;
//		for(int c=0;c<neighbour_routing_table_size;c++)
//		{
//			neighbour_table[n].neighbour_routing_table[c].cost_ = INT_MAX;
//		}

		//display_rtable(routing_table);

		n++;

	}
	//Self Loop
	routing_table[n].my_id = my_server_id;
	routing_table[n].node_id = my_server_id;
	routing_table[n].node_ip_address = get_server_ip_from_id(my_server_id,
			all_server_table);

	routing_table[n].node_port = get_port_from_id(all_server_table,
			routing_table[n].node_id);
	routing_table[n].cost_ = 0;
	routing_table[n].next_hop_server_id = my_server_id;

//	display_rtable(routing_table);

//Get my own port
	my_port = get_port_from_id(all_server_table, my_server_id);

	my_ip_address = get_server_ip_from_id(my_server_id, all_server_table);

//Close File
	input_file.close();

	//Initialise timer vector(all neighbours time+ last entry for own timer)
	vector<int> timer_vector;
	//For all the Neighbours
	for (int t = 0; t < no_of_neighbours; t++) {
		timer_vector.push_back(3 * timeout_interval);
	}
	timer_vector.push_back(timeout_interval);
//	cout << "Timer Vector Initially" << endl;
//	display_vector(timer_vector);

//For NON-Neighbours enter Infinity
	int d = 0;
	for (int i = 0; i < no_of_servers; i++) {
		for (int j = 0; j < no_of_neighbours + 1; j++) {
			//printf("%d  %d \n",all_server_table[i].server_id,neighbour_table[j].neighbour_id);
			if (all_server_table[i].server_id == routing_table[j].node_id) {
				d = 1;
			}

		}
		//printf(" Value of d %d\n",d);
		//Not Present
		if (d == 0) {
			n++;
			routing_table[n].my_id = my_server_id;
			routing_table[n].node_id = all_server_table[i].server_id;
			routing_table[n].node_ip_address = get_server_ip_from_id(
					routing_table[n].node_id, all_server_table);
			routing_table[n].node_port = get_port_from_id(all_server_table,
					routing_table[n].node_id);
			routing_table[n].cost_ = INT_MAX;
			routing_table[n].next_hop_server_id = -1;
		}
		d = 0;
	}

//	cout<<"Before Sorting"<<endl;
//			display_rtable(routing_table);

	//Sort the Routing Table
	sort_routing_table(routing_table);

//Displaying Tables
//	display_table(all_server_table);
//	display_ntable(neighbour_table);
//	display_rtable(routing_table);
	//cout<<"Size of int "<<sizeof(int)<<" short int "<<sizeof(short)<<" long int "<<sizeof(long)<<endl;
	//........................................................................................//

	//Select statement
	int next_timeout = timeout_interval;
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];
	string command = "";

	fd_set master; // master file descriptor list
	fd_set read_fds;
	int fdmax; // maximum file descriptor number
	int i;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	// clear the master and temp sets
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	//timeinterval
	timeval t;
	t.tv_sec = timeout_interval;
	t.tv_usec = 0;

	if ((rv = getaddrinfo(NULL, my_port.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	//printf("Listening Socket is %d\n", sockfd);

	// add the listener to the master set
	FD_SET(sockfd, &master);
	FD_SET(0, &master);
	// keep track of the biggest file descriptor
	fdmax = sockfd;

	// main loop
	for (;;) {
		int timeout_set = 0;
		cout << "BEFORE SELECT TIMEOUT " << next_timeout << endl;
		t.tv_sec = next_timeout;
		t.tv_usec = 0;
		FD_SET(sockfd, &master);
		FD_SET(0, &master);
		read_fds = master; // copy it
		if (select(fdmax + 1, &master, NULL, NULL, &t) == -1) {
			perror("select");
			exit(4);
		}
		// run through the existing connections looking for data to read
		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &master)) { // we got one!!
				//cout << "The set descriptor is " << i << endl;
				if (i == 0) {
					//cout << "Standard Input " << endl;
					command = "";
					arr.clear();
					getline(cin, command);
					//printf("Entered String is %s\n", command.c_str());
					vector<string> arr = split(command, " ");
					if (arr.empty()) {
						cout << "Invalid Entry\nEnter a proper command\n>";
						cout.flush();
						continue;
					}
					std::transform(arr[0].begin(), arr[0].end(), arr[0].begin(),
							::toupper);
					if (strcmp(arr[0].c_str(), "UPDATE") == 0) {

						if (arr.size() != 4) {
							cout << "UPDATE Invalid Number of Arguments.."
									<< endl;
							continue;
						}

						int my_id_1;
						int serv_id_2;
						if (atoi(arr[1].c_str()) == my_server_id) {
							my_id_1 = my_server_id;
							serv_id_2 = atoi(arr[2].c_str());
						} else {
							cout << "UPDATE Invalid Server IDs.." << endl;
							continue;
						}
						if (my_id_1 == serv_id_2) {
							cout << "UPDATE Invalid IDs" << endl;
							continue;
						}
						//check if such id exists
						int exists = 0;
						for (int b = 0; b < no_of_servers; b++) {
							if (serv_id_2 == all_server_table[b].server_id) {
								exists = 1;
								break;
							}
						}
						if (exists == 0) {
							cout << "UPDATE ID Does not exist" << endl;
							continue;
						}
//						cout << my_server_id << " " << my_id_1 << " "
//								<< serv_id_2 << endl;
						std::transform(arr[3].begin(), arr[3].end(),
								arr[3].begin(), ::toupper);
						//cout << arr[3] << endl;
						int found = 0;
						int is_disabled_ = 0;
						;
						for (int d = 0; d < no_of_neighbours; d++) {
							if (neighbour_table[d].neighbour_id == serv_id_2) {
								if (neighbour_table[d].is_disabled == 1) {
									is_disabled_ = 1;
								}

								if (strcmp(arr[3].c_str(), "INF") == 0) {
									neighbour_table[d].cost = INT_MAX;
								} else {
									neighbour_table[d].cost = atoi(
											arr[3].c_str());
								}
								found = 1;
								break;

							}
						}
						if (found == 0) {
							cout << "UPDATE Not a Neighbour so cannot update "
									<< endl;
							continue;
						}
						if (is_disabled_ == 1) {
							cout
									<< "UPDATE Link is Disabled so cannot be updated "
									<< endl;
							continue;
						}

//						//not present then make a new entry
//						if (found == 0) {
//							neighbour_table[no_of_neighbours].server_id_ =
//									my_server_id;
//							neighbour_table[no_of_neighbours].neighbour_id =
//									serv_id_2;
//							if (strcmp(arr[3].c_str(), "INF") == 0) {
//
//								neighbour_table[no_of_neighbours].cost = INT_MAX;
//								cout << neighbour_table[no_of_neighbours].cost
//										<< endl;
//							} else {
//								neighbour_table[no_of_neighbours].cost = atoi(
//										arr[3].c_str());
//							}
//							neighbour_table[no_of_neighbours].neighbour_routing_table[0].my_id =
//									-1;
//							neighbour_table[no_of_neighbours].is_disabled = 0;
//
//							no_of_neighbours++;
//
//						}
						//cout << "After Update " << endl;
						//display_ntable(neighbour_table);
						routing_update(routing_table, neighbour_table,
								all_server_table);
//						cout << "After Update " << endl;
						//display_rtable(routing_table);
						cout << "UPDATE SUCCESS" << endl;
						continue;

					}
					if (strcmp(arr[0].c_str(), "STEP") == 0) {

						if (arr.size() != 1) {
							cout << "STEP Invalid Number of Arguments.."
									<< endl;
							continue;
						}

						for (int k = 0; k < no_of_neighbours; k++) {
							for (int j = 0; j < no_of_servers; j++) {
								if (neighbour_table[k].neighbour_id
										== all_server_table[j].server_id
										&& neighbour_table[k].is_disabled
												== 0) {
									printf(
											"SENDING UPDATE TO SERVER %d   \t PORT %d\n",
											all_server_table[j].server_id,
											all_server_table[j].port_no);
									send_periodic_updates(
											all_server_table[j].server_ip_address,
											all_server_table[j].port_no,
											routing_table);
								}
							}
						}
						cout << "STEP SUCCESS" << endl;
						continue;

					}
					if (strcmp(arr[0].c_str(), "PACKETS") == 0) {

						if (arr.size() != 1) {
							cout << "PACKETS Invalid Number of Arguments.."
									<< endl;
							continue;
						}
						cout
								<< "DISTANCE VECTOR PACKETS SINCE LAST INVOCATION : "
								<< no_of_dist_vect_packets << endl;
						no_of_dist_vect_packets = 0;
						cout << "PACKETS SUCCESS" << endl;
						continue;

					}
					if (strcmp(arr[0].c_str(), "DISPLAY") == 0) {

						if (arr.size() != 1) {
							cout << "DISPLAY Invalid Number of Arguments.."
									<< endl;
							continue;
						}
						display_test_rtable(routing_table);
						cout << "DISPLAY SUCCESS" << endl;
						continue;

					}
					if (strcmp(arr[0].c_str(), "DISABLE") == 0) {

//						if (arr.size() != 2) {
//							cout << "DISABLE Invalid Number of Arguments.."
//									<< endl;
//							continue;
//						}
						int server_to_disable = atoi(arr[1].c_str());
						int serv_found = 0;
						for (int p = 0; p < no_of_neighbours; p++) {
							if (server_to_disable
									== neighbour_table[p].neighbour_id) {
								neighbour_table[p].cost = INT_MAX;
								neighbour_table[p].is_disabled = 1;
								serv_found = 1;
								break;
							}

						}
						if (serv_found == 0) {
							cout
									<< "DISABLE Not a Neighbour so cannot disable.."
									<< endl;
							continue;
						}

						routing_update(routing_table, neighbour_table,
								all_server_table);
//						cout << "After Disabling " << server_to_disable << endl;
//						display_rtable(routing_table);
//						display_ntable(neighbour_table);
						cout << "DISABLE SUCCESS" << endl;

						continue;

					}
					if (strcmp(arr[0].c_str(), "CRASH") == 0) {
						if (arr.size() != 1) {
							cout << "CRASH Invalid Number of Arguments.."
									<< endl;
							continue;
						}
						cout << "CRASH SUCCESS " << endl;
						while (1) {

						}

						continue;

					} else {
						cout << "Invalid Entry.\n";
						cout.flush();
					}

					break;
				} else if (i == sockfd) {
					// handle new connections
					//cout << "Listener " << endl;
					addr_len = sizeof their_addr;
					if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
							(struct sockaddr *) &their_addr, &addr_len))
							== -1) {
						perror("recvfrom");
						exit(1);
					}

					string ip_address_sender = inet_ntop(their_addr.ss_family,
							get_in_addr((struct sockaddr *) &their_addr), s,
							sizeof s);

					short recv_no_of_servers;
					memcpy(&recv_no_of_servers, buf, 2);

					short recv_sender_port;
					memcpy(&recv_sender_port, buf + 2, 2);
					//cout << recv_sender_port << endl;
//					printf("listener: got packet from %s port %d\n",
//							ip_address_sender.c_str(), recv_sender_port);

					//Sender IP
					char recv_ip_c_1;
					char recv_ip_c_2;
					char recv_ip_c_3;
					char recv_ip_c_4;
					memcpy(&recv_ip_c_1, buf + 4, 1);
					memcpy(&recv_ip_c_2, buf + 5, 1);
					memcpy(&recv_ip_c_3, buf + 6, 1);
					memcpy(&recv_ip_c_4, buf + 7, 1);
					int recv_ip_1 = (recv_ip_c_1 + 256) % 256;
					int recv_ip_2 = (recv_ip_c_2 + 256) % 256;
					int recv_ip_3 = (recv_ip_c_3 + 256) % 256;
					int recv_ip_4 = (recv_ip_c_4 + 256) % 256;

					string recv_ip_address = int_to_string(recv_ip_1) + "."
							+ int_to_string(recv_ip_2) + "."
							+ int_to_string(recv_ip_3) + "."
							+ int_to_string(recv_ip_4);

					int recv_port = recv_sender_port;
					int check_for_disable = get_id_from_ip(recv_ip_address,
							recv_port, all_server_table);
					int to_discard = 0;
					//cout<<"Received Port "<<recv_port<<endl;

					for (int h = 0; h < no_of_neighbours; h++) {
						//cout<<"index and disabled "<< h<< " " <<neighbour_table[h].is_disabled<<endl;
						if (check_for_disable == neighbour_table[h].neighbour_id
								&& neighbour_table[h].is_disabled == 1) {
							to_discard = 1;
							break;
						}
					}
					//cout << "neig " << endl;
					//display_ntable(neighbour_table);
					//cout << "MESSAGE FROM SERVER " << check_for_disable << endl;
					cout << "RECEIVED A MESSAGE FROM SERVER "
							<< check_for_disable << endl;

					if (to_discard == 0) {

						cout << "Time Elapsed " << (next_timeout - t.tv_sec)
								<< endl;
						int time_elapsed = next_timeout - t.tv_sec;
						for (int j = 0; j < timer_vector.size(); j++) {
							if (timer_vector[j] != -1) {
//								cout << "Time vector " << timer_vector[j]
//										<< endl;
								timer_vector[j] = timer_vector[j]
										- time_elapsed;
							}
						}

						no_of_dist_vect_packets++;

						//Initialize the neighbour routing table
						int recv_sender_port_int = recv_sender_port;
						int neighbour_server_id = get_id_from_ip(
								recv_ip_address, recv_sender_port_int,
								all_server_table);
						//cout<<"here"<<endl;
						//cout << "Neighbour server id " << neighbour_server_id<< endl;
						int neighbour_index = get_index_from_id(neighbour_table,
								neighbour_server_id);
						//display_ntable(neighbour_table);
						//cout << "Neighbour index " << neighbour_index << endl;
						//		int recv_no_of_servers_int = recv_no_of_servers;

//					neighbour_table[neighbour_index].neighbour_routing_table_size =
//							recv_no_of_servers_int;
						//cout << "recv_no_of_servers_int " << recv_no_of_servers_int<< endl;

						int recv_count = 8;

						for (int f = 0; f < recv_no_of_servers; f++) {
							//Received Server IP Addresses

							memcpy(&recv_ip_c_1, buf + recv_count++, 1);
							memcpy(&recv_ip_c_2, buf + recv_count++, 1);
							memcpy(&recv_ip_c_3, buf + recv_count++, 1);
							memcpy(&recv_ip_c_4, buf + recv_count++, 1);
							recv_ip_1 = recv_ip_c_1;
							recv_ip_2 = recv_ip_c_2;
							recv_ip_3 = recv_ip_c_3;
							recv_ip_4 = recv_ip_c_4;

							string recv_ip_address = int_to_string(recv_ip_1)
									+ "." + int_to_string(recv_ip_2) + "."
									+ int_to_string(recv_ip_3) + "."
									+ int_to_string(recv_ip_4);
							//cout << recv_ip_address << " ";

							//Received Server Ports
							short recv_sender_port_n;
							memcpy(&recv_sender_port_n, buf + recv_count++, 2);
							//cout << recv_sender_port_n << " ";
							recv_count++;

							//For the Blank Space in the Message
							recv_count++;

							//Receive N Server Ids
							short recv_server_id_n;
							memcpy(&recv_server_id_n, buf + recv_count++, 2);
							//cout << recv_server_id_n << " ";
							recv_count++;

							//Receive N server Costs
							short recv_server_cost_n;
							memcpy(&recv_server_cost_n, buf + recv_count++, 2);
							//cout <<"Receiver server cost "<< recv_server_cost_n << endl;
							recv_count++;

							//Putting values in neighbouring routing table
							neighbour_table[neighbour_index].neighbour_routing_table[f].my_id =
									neighbour_server_id;
							int recv_server_id_int = recv_server_id_n;

							neighbour_table[neighbour_index].neighbour_routing_table[f].node_id =
									recv_server_id_int;
							//cout << "f is " << f << endl;

							neighbour_table[neighbour_index].neighbour_routing_table[f].node_ip_address =
									recv_ip_address;
							if (recv_server_cost_n != SHRT_MAX) {

								neighbour_table[neighbour_index].neighbour_routing_table[f].cost_ =
										recv_server_cost_n;
							} else {
								neighbour_table[neighbour_index].neighbour_routing_table[f].cost_ =
										INT_MAX;
							}

						} //End of for
//					cout << "Neighbour Routing Table " << endl;
//					display_neigh_r		table(
//							neighbour_table[neighbour_index].neighbour_routing_table);

						timer_vector[neighbour_index] = 3 * timeout_interval;
						int min_in = find_min_time(timer_vector);
						next_timeout = timer_vector[min_in];

						//display_vector(timer_vector, neighbour_table);

						//Implement DV Algorithm
						for (int f = 0; f < no_of_servers; f++) {
							//For every server except self
							if (routing_table[f].node_id != my_server_id) {
								int source = my_server_id;
								int destination = routing_table[f].node_id;
								//Search every neighbours routing table for minimum cost
								int min_cost = INT_MAX;
								int min_cost_hop_id = -1;
								int cost;
								for (int k = 0; k < no_of_neighbours; k++) {
//								cout << "Source " << source << " Destination "
//										<< destination << " via "
//										<< neighbour_table[k].neighbour_id
//										<< endl;
									//via = neighbour_table[k].neighbour_id;
									cost =
											algorithm(source, destination,
													neighbour_table,
													neighbour_table[k].neighbour_routing_table);
									//	cout << "cost " << cost;
									if (cost < min_cost) {
										min_cost_hop_id =
												neighbour_table[k].neighbour_id;
										min_cost = cost;
									}
									//cout << " min_cost " << min_cost << endl;

								}

								//Updating our own value to the minimum cost and Changing the next hop field
								if (min_cost != INT_MAX
										&& routing_table[f].cost_ > min_cost
										&& min_cost_hop_id != -1) {
//								cout << "changing cost to " << min_cost
//										<< "and next hop to " << min_cost_hop_id
//										<< endl;
									routing_table[f].cost_ = min_cost;
									routing_table[f].next_hop_server_id =
											min_cost_hop_id;
								}

							}

						}
//						cout << "Updated Routing Table " << endl;
//						display_rtable(routing_table);
					} else {

						cout << "MESSAGE DISCARDED" << endl;
					}

				} // END got new incoming connection
			} //end of isset
			else {
				//Not set and no timeout
				timeout_set++;

			}
		} // END looping through file descriptors

		// Timeout
		if (timeout_set > fdmax) {

			//subtract interval from every other entry
//			for (unsigned int h = 0; h < timer_vector.size() - 1; h++) {
//				timer_vector[h] = timer_vector[h] - timeout_interval;
//				if (timer_vector[h] == 0) {
//					//Now for all the entries which are 0 we have not gotten updates for 3 intervals
//					cout << "NO UPDATES FOR 3 INTERVALS FROM SERVER "
//							<< neighbour_table[h].neighbour_id << endl;
//					timer_vector[h] = -1;
//					//setting the cost to INF in neighbour and calculating the routing table again.
//					neighbour_table[h].cost = INT_MAX;
//					neighbour_table[h].neighbour_routing_table[0].my_id = -1; //Since the node is not present
//					//neighbour_table[h].is_disabled = 1;
//
//				}
//				if (timer_vector[h] < 0) {
//
//					timer_vector[h] = -1;
//
//				}
//			}
//			cout << "Updated Neighbour Table after no Update received" << endl;
//			display_ntable(neighbour_table);

			for (int d = 0; d < timer_vector.size() ; d++) {

				if (timer_vector[d] != -1) {

					timer_vector[d] = timer_vector[d] - next_timeout;
				}
				if (timer_vector[d] == 0) {
					if (d == timer_vector.size() - 1) { //mine the periodic updates
						timer_vector[timer_vector.size() - 1] =
								timeout_interval;
						cout << "TIMEOUT SEND MESSAGES TO ALL NEIGHBOURS"
								<< endl;
						for (int k = 0; k < no_of_neighbours; k++) {
							for (int j = 0; j < no_of_servers; j++) {
								if (neighbour_table[k].neighbour_id
										== all_server_table[j].server_id
										&& neighbour_table[k].is_disabled
												== 0) {
									//						printf("Sending update to Server %d at port %d\n",
									//								all_server_table[j].server_id,
									//								all_server_table[j].port_no);
									send_periodic_updates(
											all_server_table[j].server_ip_address,
											all_server_table[j].port_no,
											routing_table);
								}
							}
						} //end of for
					} else { //if not mine

						//Now for all the entries which are 0 we have not gotten updates for 3 intervals
						cout << "NO UPDATES FOR 3 INTERVALS FROM SERVER "
								<< neighbour_table[d].neighbour_id << endl;
						timer_vector[d] = -1;
						//setting the cost to INF in neighbour and calculating the routing table again.
						neighbour_table[d].cost = INT_MAX;
						neighbour_table[d].neighbour_routing_table[0].my_id =
								-1; //Since the node is not present
						//neighbour_table[h].is_disabled = 1;

					}
				}
			}
			int min_ind_ex = find_min_time(timer_vector);
			int prev_min = timer_vector[min_ind_ex];

			t.tv_sec = timer_vector[min_ind_ex];
			next_timeout = t.tv_sec;
			cout << "NEXT TIMEOUT " << next_timeout << endl;
			display_vector(timer_vector, neighbour_table);

			routing_update(routing_table, neighbour_table, all_server_table);

			///cout << "After Update " << endl;

			//display_vector(timer_vector, neighbour_table);

			//t.tv_sec = timeout_interval;
		}

	}

}
