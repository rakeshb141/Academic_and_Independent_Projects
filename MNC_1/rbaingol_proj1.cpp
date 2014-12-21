#include<iostream>
#include<string.h>
#include<sstream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<netdb.h>
#include <unistd.h>
#include <errno.h>
#include<stdio.h>
#include <vector>
#include<algorithm>
#include<fstream>
#include<map>
#include <sys/time.h>
#include<fcntl.h>
#include<sys/stat.h>

#define BUFFER_SIZE 256
#define BACKLOG 10

using namespace std;

class list {
public:
	int id;
	int socket_no;
	char ip_addr[INET_ADDRSTRLEN];
	char host_name[50];
	char port_no[10];
};
class sock_boolean_set { //For the no of connections while downloading
public:
	int is_set;
	string file_name;
	string to_write;
	int size;
	struct timeval time_val;
	int f_d;
	int rem_bytes;

};

vector<string> arr;
int no_of_connections = 0;
int accept_file = 0;
list connection_list[10];
map<int, sock_boolean_set> download_list;
map<int, sock_boolean_set>::iterator iter;

struct self_info {
	string port;
	char IP_ADDRESS[INET_ADDRSTRLEN];
	char hostname[100];
};

struct self_info self;

struct reg_params {
	vector<string> arr;
	int fdmax;
	fd_set master;
	string port;
	int no_of_connections;
	char ip_address[INET_ADDRSTRLEN];
	list connection_list[10];
	bool b_error;
};

int check_in_list(int n) {

	for (int c = 0; c < no_of_connections; c++) {
		if (n == connection_list[c].id) {
			return 1;
		}
	}
	return 0;

}
int is_ip_address(string arr) {
	for (unsigned int i = 0; i < arr.length(); i++) {
		if (!isdigit(arr.at(i)) && arr.at(i) != '.') {
			return 0;
		}
	}

	return 1;
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
//n for self uploading or requested uploading
void upload(int arr1, string arr2, int n) {
	//code for reading file completely
	char buffer[BUFFER_SIZE];
	memcpy(buffer, "", sizeof buffer);
	int is_there = 0;
	if (arr1 == 1) {
		cout << "Cannot Upload to the Server\n";
		return;
	}
	for (int i = 1; i < no_of_connections; i++) {
		if (connection_list[i].id == arr1) {
			is_there = 1;
			break;
		}
	}
	if (is_there == 0) {
		cout << "Not in the Connection List..Cannot Upload\n";
		return;
	}
	int upload_fd = open((char*) arr2.c_str(), O_RDONLY);
//	if (upload_fd == -1) {
//		printf("Error opening file");
//	}
	//ifstream inputstream((char*) arr2.c_str());

	if (upload_fd == -1) {
		cout << "Cannot Find The File : " << arr2 << endl;
		if (n == 0) {
			return;
		}
		string upload_file_not_found = "u/f/a/i/l ";
		upload_file_not_found.append(arr2);
		upload_file_not_found.append(" \n");
		if (send(connection_list[arr1 - 1].socket_no,
				upload_file_not_found.c_str(), BUFFER_SIZE, 0) == -1) {
			perror("send upload file_not_found ");
			cout << "Could Not Send Upload File Not Found to the Peer\n";

		} else {
			cout
					<< "Notified the Peer.\nEnter your command or HELP for options\n>";
			cout.flush();
			return;
		}

		return;
	}
	struct timeval time;

	if (upload_fd != -1) {
		struct stat stat;
		if (fstat(upload_fd, &stat) == -1) {
			perror("Error getting File status. Error");
			return;
		}

		int size = stat.st_size;

		cout.flush();

		//Send the Peer the Filename and Size
		string desc = "/f/isi/z ";
		vector<string> sp = split(arr2, "/");

		//char filename_and_size;

		//Convert size to String
		stringstream ss;
		ss << size;
		string str = ss.str();
		char filename_and_size[2 + strlen(desc.c_str()) + strlen(arr2.c_str())
				+ strlen(str.c_str())];
		strcpy(filename_and_size, desc.c_str());
		strcat(filename_and_size, sp[sp.size() - 1].c_str());
		strcat(filename_and_size, " ");
		strcat(filename_and_size, str.c_str());
		strcat(filename_and_size, " \n");

		//cout << "Filename and Size " << filename_and_size;

		if (send(connection_list[arr1 - 1].socket_no, filename_and_size,
				BUFFER_SIZE, 0) == -1) {
			perror("send upload filename_size ");
			cout << "\nCould Not Send Filename and Size to the Peer\n";
		} else {
			//cout << "Sent filename_size " << buffer << endl;
		}
		int nbytes;
		gettimeofday(&time, NULL);
		double t1 = time.tv_sec + (time.tv_usec / 1000000.0);
		int fd = connection_list[arr1 - 1].socket_no;
		while ((nbytes = read(upload_fd, buffer, BUFFER_SIZE)) != 0) {
			//cout<<"in while"<<endl;
			if (nbytes == -1) {
				cout << "Error in reading File";
				return;
			}

			//inputstream.read(buffer, BUFFER_SIZE);
			if (send(fd, buffer, BUFFER_SIZE, 0) == -1) {
				perror("send upload");
				cout << "Could not send Buffer to Peer\n";
			}

		}

		gettimeofday(&time, NULL);
		double t2 = time.tv_sec + (time.tv_usec / 1000000.0);
		//  double rate= (size*8.0);
		double rate = (size * 8.0) / (t2 - t1);
		unsigned int rate_int = rate;
		//sending the end of file so that client knows its the end of file
		string end_of_file = "/e/o/f*&";
		if (send(connection_list[arr1 - 1].socket_no, end_of_file.c_str(),
				BUFFER_SIZE, 0) == -1) {
			perror("send upload filename_size ");
			cout << "Could Not End of file to Peer\n";
		} else {
			//cout<<"\nt2-t1 "<<t2-t1;
			double t = t2 - t1;

			cout << "\nUpload Success.\nTx (" << self.hostname << "):"
					<< self.hostname << "->"
					<< connection_list[arr1 - 1].host_name << ",File Size: "
					<< size << " Bytes,Time Taken: " << t
					<< " seconds,Tx Rate: " << rate_int << " bits/sec" << endl;
//			cout << "\nFile " << arr2 << " Uploaded.\nTX Rate : " << rate_int
//					<< " bits/sec" << endl;
			cout << "Enter your command or HELP for options\n>";
			cout.flush();
		}

	}
	close(upload_fd);
}

void creator() {
	cout
			<< "******************************\nName: RAKESH BAINGOLKAR\nUBIT Name:  rbaingol\nUB Mail:  rbaingol@buffalo.edu\n****************************** \nEnter Your command or HELP for options\n>";
	cout.flush();
}
void display_list(list* l, int n) {
	if (n == 0) {
		cout << "The List is Empty\n";
		cout.flush();
	} else {
		cout << "ID\tHostName\tIP Address\tPORT_NO\n";
		for (int i = 0; i < n; i++) {
			cout << l[i].id << "\t" << l[i].host_name << "\t" << l[i].ip_addr
					<< "\t" << l[i].port_no << "\t " << endl;
		}
	}
	return;
}
void _register(struct reg_params * obj_reg_param) {

	vector<string> arr = obj_reg_param->arr;
	int fdmax = obj_reg_param->fdmax;
	fd_set master;
	memcpy(&master, &obj_reg_param->master, sizeof obj_reg_param->master);
	string port = obj_reg_param->port;
	int no_of_connections = obj_reg_param->no_of_connections;
	char ip_address[INET_ADDRSTRLEN];
	memcpy(ip_address, obj_reg_param->ip_address, sizeof ip_address);
	list connection_list[10];
	memcpy(connection_list, obj_reg_param->connection_list,
			sizeof obj_reg_param->connection_list);

	//cout << "\nClient is not registered" << endl;
	cout << "Trying to connect.." << endl;

	//New Socket for Communication with the Server
	struct addrinfo hints_client, *servinfo_client, *p_client;
	int rv_client;
	int conn_sockfd;
	////////////
	memset(&hints_client, 0, sizeof hints_client);
	hints_client.ai_family = AF_UNSPEC;
	hints_client.ai_socktype = SOCK_STREAM;

	//cout << "AAAAAAA " << conn_sockfd << "Port No " << arr[2].c_str() << endl;

	if ((rv_client = getaddrinfo(arr[1].c_str(), arr[2].c_str(), &hints_client,
			&servinfo_client)) != 0) {
		fprintf(stderr, "register getaddrinfo: %s\n", gai_strerror(rv_client));
		obj_reg_param->b_error = true;
		return;
	}
	// loop through all the results and connect to the first we can
	for (p_client = servinfo_client; p_client != NULL;
			p_client = p_client->ai_next) {
		if ((conn_sockfd = socket(p_client->ai_family, p_client->ai_socktype,
				p_client->ai_protocol)) == -1) {
			perror("Register Client Socket Problem");
			continue;
		}

		if (connect(conn_sockfd, p_client->ai_addr, p_client->ai_addrlen)
				== -1) {
			close(conn_sockfd);
			perror("Register Client Connect Problem");
			continue;
		}

		break;
	}

	if (p_client == NULL) {
		fprintf(stderr, "Register Client Could not Connect\n");
		obj_reg_param->b_error = true;
		return;
	} else {
		FD_SET(conn_sockfd, &master);
		if (fdmax < conn_sockfd) {
			fdmax = conn_sockfd;
		}

		//cout << "Now Sending port_no to Server\n";
		const char * port_char_client = port.c_str();
		//Append this to start so the server knows its the PORT and IP
		const char * port_id = "/p/o.rt/ ";
		char ip_addr_port_no[strlen(port_id) + strlen(port_char_client)
				+ strlen(obj_reg_param->ip_address)];
		strcpy(ip_addr_port_no, port_id);
		strcat(ip_addr_port_no, port_char_client);
		strcat(ip_addr_port_no, " ");
		strcat(ip_addr_port_no, ip_address);
		strcat(ip_addr_port_no, "\n");
		//	int len = strlen(ip_addr_port_no);

		// printf("Port No + IP address  %s Length %d\n", ip_addr_port_no, len);
		fflush(stdout);
		if (send(conn_sockfd, ip_addr_port_no, BUFFER_SIZE, 0) == -1) {
			perror("send");
			cout
					<< "Could Not Send Port to Server\nUnsuccessful Registration\n";
		} else {
			//cout << "IP + Port No Sent Successfully\n";
		}
	}
	//cout << "Filling the details  in the Connection List\n";
	connection_list[no_of_connections].id = no_of_connections + 1;
	strcpy(connection_list[no_of_connections].ip_addr, arr[1].c_str());
	strcpy(connection_list[no_of_connections].port_no, arr[2].c_str());
	connection_list[no_of_connections].socket_no = conn_sockfd;
	struct hostent *he;
	struct in_addr ipv4addr;
	inet_pton(AF_INET, arr[1].c_str(), &ipv4addr);
	he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
	strcpy(connection_list[no_of_connections].host_name, he->h_name);
	no_of_connections++;

	freeaddrinfo(servinfo_client);

	obj_reg_param->arr = arr;
	obj_reg_param->fdmax = fdmax;
	memcpy(&obj_reg_param->master, &master, sizeof master);
	obj_reg_param->port = port;
	obj_reg_param->no_of_connections = no_of_connections;
	memcpy(obj_reg_param->ip_address, ip_address, sizeof ip_address);
	memcpy(obj_reg_param->connection_list, connection_list,
			sizeof connection_list);
	//cout << "Connected." << endl;

}

void display_options_server() {

	cout << "**************************************************" << endl;
	cout << "MYPORT : Display My Port" << endl;
	cout << "MYIP : Display IP Address" << endl;

	cout << "LIST : Display Server IP List" << endl;

	cout << "EXIT : Exit the Program" << endl;

	cout << "CREATOR : Display Full Name,UBIT Name and UB Email" << endl;

	cout << "**************************************************" << endl;
	cout << "Enter your command or HELP for Options \n>";
	cout.flush();

}
void display_options_client() {

	cout
			<< "**********************************************************************************************************"
			<< endl;
	cout << "MYPORT : Display My Port" << endl;
	cout << "MYIP : Display IP Address" << endl;
	cout << "REGISTER <Server IP> <Port_No> : To Register with the Server"
			<< endl;
	cout << "CONNECT <Destination IP Address> <Port_No> : To Connect to a Peer"
			<< endl;
	cout << "LIST : Display Connection List" << endl;
	cout
			<< "TERMINATE <connection id> : Terminate a Connection from Connection List"
			<< endl;
	cout << "EXIT : Close all connections and Terminate the Process" << endl;
	cout << "UPLOAD <connection id> <filename> : Upload a file to Peer" << endl;
	cout
			<< "DOWNLOAD <connection id> <filename> <connection id> <filename>: Download  files from Peers (LIMIT 3 FILES)"
			<< endl;
	cout << "CREATOR : Display Full Name,UBIT Name and UB Email" << endl;

	cout
			<< "**********************************************************************************************************"
			<< endl;
	cout << "Enter your command or HELP for Options \n>";
	cout.flush();
}

void clean_up_list(list * l) {
	for (int i = 0; i < 10; i++) {
		l[i].id = 0;
		memset(l[i].host_name, 0, strlen(l[i].host_name));
		memset(l[i].ip_addr, 0, strlen(l[i].ip_addr));
		memset(l[i].port_no, 0, strlen(l[i].port_no));
		l[i].socket_no = -1;

	}
}

void convert_table_to_string(list *serv_ip_list, const char ** table_char,
		int n) {
	string str = "";
	if (n == 0) {
		cout << "Cannot convert to String as it is Empty\n";
		*table_char = str.c_str();

		return;
	}

	for (int i = 0; i < n; i++) {
		char temp[5];
		sprintf(temp, "%d", serv_ip_list[i].id);
		str += temp;
		str += " ";
		str.append(serv_ip_list[i].host_name);
		str += " ";
		str.append(serv_ip_list[i].ip_addr);
		str += " ";
		str.append(serv_ip_list[i].port_no);
		str += " #&";
	}
	//cout << "String to send is " << str << endl;
	*table_char = str.c_str();

	cout.flush();

	return;

}

void my_port(string p) {
	cout << "The Listening PORT is " << p << endl;
	cout << "Enter your command or HELP for Options \n>";
	cout.flush();
}

void get_ip(char *ip_addr) {

	char ip_address[INET_ADDRSTRLEN];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int sock_fd;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo("8.8.8.8", "53", &hints, &servinfo)) != 0) {
		fprintf(stderr, "UDP getaddrinfo problem: %s\n", gai_strerror(rv));
		return;
	}
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("UDP Socket Problem");
			continue;
		}
		if (connect(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sock_fd);
			perror("UDP Connect Problem");
			continue;
		}

		break;
	}

	if (p == NULL) {
		cout << "UDP p==null problem" << endl;
		return;
	}

	sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	int i = getsockname(sock_fd, (sockaddr*) &addr, &addr_len);
	if (i == -1) {
		cout << "error in getsockname UDP" << endl;
	}

	inet_ntop(AF_INET, &addr.sin_addr, ip_address, sizeof ip_address);
	strcpy(ip_addr, ip_address);
	//cout << "IP Address is " << ip_address << endl<<"NEW IP "<<ip_addr<<endl;
	fflush(stdout);
	//

	close(sock_fd);
	return;
}
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

//Server Process
void serverprocess(string port) {
	fd_set master; //Master File Descriptor List
	fd_set read_fds; // temp file descriptor list for select()
	int fdmax; // maximum file descriptor number
	int sockfd, newfd; // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	// struct sockaddr_storage their_addr; // connector's address information
	//socklen_t sin_size;
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	char remoteIP[INET6_ADDRSTRLEN];
	//Store IP
	char ip_address[INET_ADDRSTRLEN];
	get_ip(ip_address);
	int nbytes;
	char buf[BUFFER_SIZE];
	int yes = 1;
	//char s[INET6_ADDRSTRLEN];
	int rv;
	//Server IP List
	list serv_ip_list[10];
	clean_up_list(serv_ip_list);
	//Number of Entries in Server_IP_List
	int no_of_registered_users = 0;

	FD_ZERO(&master);
	// clear the master and temp sets
	FD_ZERO(&read_fds);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	const char * port_const_char = port.c_str();

	if ((rv = getaddrinfo(NULL, port_const_char, &hints, &servinfo)) != 0) {
		fprintf(stderr, " Problem server getaddrinfo: %s\n", gai_strerror(rv));
		return;
	}

	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("Server Socket Error");
			continue;
		}
		//cout << "Server Socket ID " << sockfd << endl;

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("Set Sock Opt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("Server Bind Error: ");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr,
				"Server Failed to Bind: Choose a different port number\n");
		return;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("Server Listen Problem \n");
		exit(1);
	}
	//for input from the user
	FD_SET(0, &master);
	// add the listener to the master set
	FD_SET(sockfd, &master);
	// keep track of the biggest file descriptor
	fdmax = sockfd; // so far, it's this one

	cout << "Enter your command or HELP for Options \n>";
	cout.flush();
	string command = "";
	while (1) {
		read_fds = master; // copy it
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(4);
		}
		for (int i = 0; i <= fdmax; i++) {
			//cout << "SOCK FD " << sockfd << endl;
			if (FD_ISSET(i, &read_fds)) // we got one!!
			{ //INPUT FROM THE USER

				//cout << "Socket id " << i;
				if (i == 0) {
					{ //Command from the User
						command = "";
						arr.clear();
						getline(cin, command);
						//vector<string> arr;
						arr = split(command, " ");
						if (arr.empty()) {
							cout
									<< "Invalid Entry\nEnter your command or HELP for Options\n>";
							cout.flush();
							continue;
						}
						std::transform(arr[0].begin(), arr[0].end(),
								arr[0].begin(), ::toupper);
						if (strcmp(arr[0].c_str(), "HELP") == 0) { //HELP-Display the options
							//cout << "yes" << endl;
							display_options_server();
							continue;
							//break;
						}
						if (strcmp(arr[0].c_str(), "MYPORT") == 0) { //Display MYPORT
							my_port(port);
							continue;
						}
						if (strcmp(arr[0].c_str(), "LIST") == 0) { //Display Server Ip List
							cout << "Server IP List " << endl;
							display_list(serv_ip_list, no_of_registered_users);
							cout
									<< "Enter your command or HELP for Options \n>";
							cout.flush();
							continue;

						}
						if (strcmp(arr[0].c_str(), "CREATOR") == 0) { //Display Creator
							creator();
							continue;
						}
						if (strcmp(arr[0].c_str(), "MYIP") == 0) { //Display MYIP
							cout << "IP address is " << ip_address;
							cout
									<< "\nEnter your command or HELP for Options \n>";
							fflush(stdout);
							continue;
						}
						if (strcmp(arr[0].c_str(), "EXIT") == 0) { //EXIT the program
							if (no_of_registered_users == 0) {
								cout << "Quitting Program " << sockfd << endl;
								cout << "Closing Socket " << sockfd << endl;
								for (int k = 0; k < no_of_connections; k++) {
									close(connection_list[k].socket_no);
								}
								close(sockfd);
								cout << "Server Exited" << endl;
								exit(1);
							} else {
								cout
										<< "Server cannot exit until all the clients have EXITED\nEnter your command or HELP for options\n>";
								cout.flush();
								continue;
							}
						}

						else {
							cout
									<< "Invalid Entry.\nEnter your command or HELP for Options \n>";
							cout.flush();
						}
					}

				} //end of if i==0
				else if (sockfd == i) {
					//Listening Socket
					addrlen = sizeof remoteaddr;
					newfd = accept(sockfd, (struct sockaddr *) &remoteaddr,
							&addrlen);

					if (newfd == -1) {
						perror("Accept Problem");
					} else {
						FD_SET(newfd, &master);
						// add to master set
						if (newfd > fdmax) {
							// keep track of the max
							fdmax = newfd;
						}
						printf("\nServer: New Connection From %s on "
								"Socket %d\n",
								inet_ntop(remoteaddr.ss_family,
										get_in_addr(
												(struct sockaddr*) &remoteaddr),
										remoteIP, INET6_ADDRSTRLEN), newfd);
					}
					continue;
				} else {
					//Handling Data from Client
					if ((nbytes = recv(i, buf, BUFFER_SIZE, 0)) <= 0) {
						// got error or connection closed by client
						if (nbytes == 0) {
							// Connection Closed
							printf("\nServer: Socket %d disconnected\n", i);
							//Remove the enry from Server Ip List
							//cout << "Remove the enry from Server Ip List\n";
							cout.flush();
							int rem;
							for (rem = 0; rem < no_of_registered_users; rem++) {

								if (serv_ip_list[rem].socket_no == i) {
									serv_ip_list[rem].id = 0;
									strcpy(serv_ip_list[rem].host_name, "");
									strcpy(serv_ip_list[rem].ip_addr, "");
									strcpy(serv_ip_list[rem].port_no, "");
									serv_ip_list[rem].socket_no = -1;
//									cout << "Removed Entry of Socket No " << i
//											<< endl;
									break;
								}
							}
							//Remaining Server List Update Bring One Row Above
							for (int f = rem; f < no_of_registered_users - 1;
									f++) {
								serv_ip_list[f] = serv_ip_list[f + 1];
								serv_ip_list[f].id = serv_ip_list[f].id - 1;

							}
							no_of_registered_users--;
							//Updated Server List
							cout << "Updated Server IP List" << endl;
							if (no_of_registered_users == 0) {
								cout
										<< "No Registered Clients\nEnter Your Command or HELP for options\n>";
								cout.flush();
							} else {
								display_list(serv_ip_list,
										no_of_registered_users);
								//cout << "Sending Updated list to the Clients"<< endl;
								const char* table_char;
								convert_table_to_string(serv_ip_list,
										&table_char, no_of_registered_users);
								//cout << "Table char is " << table_char << endl;
//                            cout << "Now Sending table_char to client\n";
								const char * s_ip_start = "/S/I?p ";
								const char * s_ip_end = "/EOF/";
								char string_to_send[strlen(s_ip_start)
										+ strlen(s_ip_end) + strlen(table_char)];
								strcpy(string_to_send, s_ip_start);
								strcat(string_to_send, table_char);
								//strcat(string_to_send, " ");
								strcat(string_to_send, s_ip_end);
								strcat(string_to_send, "\n");
								//int len = strlen(string_to_send);
//                            printf("Send Server IP  %sLength %d\n",
//                                string_to_send, len);

								fflush(stdout);
								for (int j = 1; j <= fdmax; j++) {
									if (FD_ISSET(j,&master) && j != i) {
										if (j != sockfd) {
											if (send(j, string_to_send,
													BUFFER_SIZE, 0) == -1) {
												perror("send");
												cout
														<< "Could not Send the Updated Server IP list to the Clients\n";
											} else {

											}

										}

									}
								}
//								cout
//										<< "Sent the Updated Server IP List Successfully\n";
								cout
										<< "Enter your command or HELP for options\n>";
								cout.flush();
							} //End of no of occurence else

						} else {
							perror("Error in Receiving");
							cout << "Enter you command or HELP for Options\n>";
							cout.flush();
						}
						close(i); // bye!
						FD_CLR(i, &master);

						// remove from master set
					} else {
						//buf[nbytes] = '\0';
						//cout << "Buffer Size " << sizeof buf << endl;
//                        cout << "We got some data from the client\nData is "
//                                << buf << endl;
						//cout<<"display"<<buf[0]<<" "<<buf[1];
						fflush(stdout);
						//p/o.rt/
						if (buf[0] == '/' && buf[1] == 'p' && buf[2] == '/'
								&& buf[3] == 'o' && buf[4] == '.'
								&& buf[5] == 'r' && buf[6] == 't') {
							//cout << "In the Loop\n";
							char *without_slash_n = strtok(buf, "\n");
							vector<string> prt_ip;
							prt_ip = split(without_slash_n, " ");
//                            for (unsigned int i = 0; i < prt_ip.size(); i++) {
//                                printf(" Before Assigning %s\n",
//                                        prt_ip[i].c_str());
//                            }

							struct hostent *he;
							struct in_addr ipv4addr;
							//char* ip_add=(char *)prt_ip[2].c_str();
							inet_pton(AF_INET, prt_ip[2].c_str(), &ipv4addr);
							he = gethostbyaddr(&ipv4addr, sizeof(ipv4addr),
									AF_INET);
//							cout << "No of Registered users "
//									<< no_of_registered_users << endl;
							serv_ip_list[no_of_registered_users].id =
									no_of_registered_users + 1;
							strcpy(serv_ip_list[no_of_registered_users].port_no,
									prt_ip[1].c_str());
							strcpy(serv_ip_list[no_of_registered_users].ip_addr,
									prt_ip[2].c_str());
							strcpy(
									serv_ip_list[no_of_registered_users].host_name,
									he->h_name);
							serv_ip_list[no_of_registered_users].socket_no = i;

							no_of_registered_users++;

							prt_ip.erase(prt_ip.begin(), prt_ip.end());
							cout << "Updated Server IP List\n";
							display_list(serv_ip_list, no_of_registered_users);
							cout << "Enter your command or HELP for options\n>";
							cout.flush();

							const char* table_char;
							convert_table_to_string(serv_ip_list, &table_char,
									no_of_registered_users);
							//cout << "Table char is " << table_char << endl;
							//cout << "Now Sending Server Ip Table to client\n";
							const char * s_ip_start = "/S/I?p ";
							const char * s_ip_end = "/EOF/";
							char string_to_send[strlen(s_ip_start)
									+ strlen(s_ip_end) + strlen(table_char)];
							strcpy(string_to_send, s_ip_start);
							strcat(string_to_send, table_char);
							strcat(string_to_send, s_ip_end);
							strcat(string_to_send, "\n");
							//int len = strlen(string_to_send);
//                            printf("Send Server IP  %s Length %d\n",
//                                    string_to_send, len);

							fflush(stdout);

							for (int j = 1; j <= fdmax; j++) {
								if (FD_ISSET(j,&master)) {
									if (j != sockfd) {
										if (send(j, string_to_send, BUFFER_SIZE,
												0) == -1) {
											perror("send");
											cout
													<< "Could not Send the Server IP list to the Clients\n";
										} else {
//                                            cout
//                                                    << "Sent the Server IP List Successfully\n";
										}

									}

								}
							}

						} else {
							cout << "Not in the Loop\n";
						}
						fflush(stdout);

					}

					continue;
				}
			} //end of fd_set
		} //end of for

	} //end of while
	return;
}

//Client Process
void clientprocess(string port) {
	fd_set master; //Master File Descriptor List
	fd_set read_fds; // temp file descriptor list for select()
	int fdmax; // maximum file descriptor number
	int sockfd; // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	// struct sockaddr_storage their_addr; // connector's address information
	//socklen_t sin_size;
	int yes = 1;
	//char s[INET6_ADDRSTRLEN];
	//Store IP
	char ip_address[INET_ADDRSTRLEN];

	get_ip(ip_address);

	struct in_addr ipv4addr_;
	inet_pton(AF_INET, ip_address, &ipv4addr_);
	struct hostent * he_client;
	he_client = gethostbyaddr(&ipv4addr_, sizeof ipv4addr_, AF_INET);
	strcpy(self.hostname, he_client->h_name);
	int rv;
	int nbytes_client;
	char buf_client[BUFFER_SIZE];
	//Server IP List
	list s_ip_local[10];
	int s_ip_count = 0;
	clean_up_list(s_ip_local);
	//Connection List

	//Number of Connections
	no_of_connections = 0;
	clean_up_list(connection_list);

	socklen_t addrlen_client;
	struct sockaddr_storage remoteaddr_client;
	int newfd_client;
	char remoteIP_client[INET6_ADDRSTRLEN];

	//For Simultaneous Download List
	//sock_boolean_set download_set[10];

	FD_ZERO(&master);
	// clear the master and temp sets
	FD_ZERO(&read_fds);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	const char * port_const_char_client = port.c_str();

	if ((rv = getaddrinfo(NULL, port_const_char_client, &hints, &servinfo))
			!= 0) {
		fprintf(stderr, "problem client getaddrinfo: %s\n", gai_strerror(rv));
		return;
	}

	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("Client Socket Error");
			continue;
		}
		//	cout << "Client Socket ID " << sockfd << endl;

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("Problem Client Set Sock Opt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("Client Bind Error: Choose a different port number");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr,
				"Client Failed to Bind:Choose a different Port Number\n");
		return;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("Client Listen Problem");
		exit(1);
	}

	//for input from the user
	FD_SET(0, &master);
	// add the listener to the master set
	FD_SET(sockfd, &master);
	// keep track of the biggest file descriptor
	fdmax = sockfd; // so far, it's this one

	cout << "Enter your command or HELP for Options \n>";
	fflush(stdout);
	string command = "";

	while (1) {
		read_fds = master; // copy it

//        if(!FD_ISSET(0,&master))
//        {
//            cout<<"not set"<<endl;
//        }
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(4);
		}
		for (int i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) // we got one!!
			{ //INPUT FROM THE USER
			  //cout << "SOCK FD " << i;
				if (i == 0) {

					command = "";
					arr.clear();
					getline(cin, command);

					//vector<string> arr;
					arr = split(command, " ");
					if (arr.empty()) {
						cout
								<< "Invalid Entry\nEnter your command or HELP for Options\n>";
						cout.flush();
						continue;
					}
					std::transform(arr[0].begin(), arr[0].end(), arr[0].begin(),
							::toupper);
					//cout << arr[0] << endl;
					if (strcmp(arr[0].c_str(), "HELP") == 0) { //HELP-Display the options client
						//cout << "yes" << endl;
						display_options_client();
						continue;
						//break;
					}
					if (strcmp(arr[0].c_str(), "MYPORT") == 0) { //Display MYPORT
						my_port(port);
						continue;
					}
					if (strcmp(arr[0].c_str(), "LIST") == 0) { //Display Connections List
						cout << "Connection List " << endl;
						display_list(connection_list, no_of_connections);
						cout << "Enter your command or HELP for Options \n>";
						cout.flush();
						continue;
					}
					if (strcmp(arr[0].c_str(), "MYIP") == 0) { //Display MYIP

						cout << "My IP Address " << ip_address;
						cout << "\nEnter your command or HELP for Options \n>";
						fflush(stdout);
						continue;
					}
					if (strcmp(arr[0].c_str(), "UPLOAD") == 0) { //Upload a file

						// vector<string> sp=split(arr[2],"/");

						//sp[sp.size()-1]
						if (arr.size() != 3) {
							cout
									<< "Invalid Arguments\nEnter your command or HELP for options\n>";
							cout.flush();
							continue;
						}
						//	if(!isdigit(arr[1].c_str()))
						int n = atoi(arr[1].c_str());
						if (n == 1) {
							cout
									<< "Cannot Upload to the Server\nEnter your command or HELP for options\n>";
							cout.flush();
							continue;
						}
						if (n > no_of_connections) {
							cout << "Please specify a correct Connection ID\n";
							cout << "Enter your command or HELP for options\n>";
							cout.flush();
							continue;
						}
						upload(n, arr[2].c_str(), 0);
						cout << "Enter your command or HELP for Options \n>";
						fflush(stdout);
						continue;
					}
					if (strcmp(arr[0].c_str(), "DOWNLOAD") == 0) { //Upload a file

						//Check if all the connection ids exist
						//cout << "vector array size" << arr.size() << endl;

						if (arr.size() > 7) {
							cout
									<< "Download Limit upto 3 files.\nEnter your command or HELP for options\n>";
							cout.flush();
							continue;
						}
						if (arr.size() % 2 == 0) {

							cout
									<< "Invalid format of Arguments\nEnter your command or HELP for options\n>";
							cout.flush();
							continue;
						}

						int b = 0; //to check in connection list
						int a = 0; //to check if downloading from server
						for (unsigned int s = 1; s < arr.size(); s += 2) {
							int d = atoi(arr[s].c_str());
							if (d == 1) {

								a = 1;
								break;

							}
							int c = check_in_list(d);
							if (c == 0) {
								b = 1;
								break;
							}
						}
						if (a == 1) {
							cout
									<< "Cannot Download from Server\nEnter your Command or HELP for Options\n>";
							cout.flush();
							continue;
						}
						if (b == 1) {
							cout
									<< "No such Connection ID in the List.\nEnter your Command or HELP for Options\n>";
							cout.flush();
							continue;
						}
						string upload_desc = "";
						for (unsigned int s = 1; s < arr.size(); s += 2) {
							int d = atoi(arr[s].c_str()); //connection id
							upload_desc = "";
							upload_desc.append("u/p?o^r ");
							upload_desc.append(arr[s + 1].c_str());
							upload_desc.append(" ");
							upload_desc.append("\n");
							//cout << "Upload to send: " << upload_desc;
							if (send(connection_list[d - 1].socket_no,
									upload_desc.c_str(), BUFFER_SIZE, 0)
									== -1) {
								perror("send download upload_desc ");
								cout
										<< "Could Not Send Upload Request to the Peer\n";
							} else {
								//cout << "Sent Upload Request " << upload_desc;
								cout.flush();
							}
						}

//                        string upload_desc="u/p?o^r";
//                        char upload_request[2 + strlen(upload_desc.c_str()) + strlen(arr2.c_str())
//                                        + strlen(str.c_str())];
//                                strcpy(filename_and_size, desc.c_str());
//                                strcat(filename_and_size, arr2.c_str());
//                                strcat(filename_and_size, " ");
//                                strcat(filename_and_size, str.c_str());
//                                strcat(filename_and_size, "\n");
//
//                                cout << "Filename and Size " << filename_and_size;
//
//                                if (send(connection_list[arr1 - 1].socket_no, filename_and_size,
//                                        BUFFER_SIZE, 0) == -1) {
//                                    perror("send upload filename_size ");
//                                    cout << "Could Not Send Filename and Size to the Peer\n";
//                                } else {
//                                    cout << "Sent filename_size " << buffer << endl;
//                                }

						cout << "Enter your command or HELP for Options \n>";
						fflush(stdout);
						continue;
					}
					if (strcmp(arr[0].c_str(), "TERMINATE") == 0) {
						//Terminate Connection from connection list
						//cout << "in terminate" << endl;
						int n = atoi(arr[1].c_str());
						//cout << n << endl;
						if (n > no_of_connections) {
							cout
									<< "Entry does not exist.\nEnter your command or HELP for options\n>";
							cout.flush();
							continue;
						}
						int b = 0;
						if (n == 1) {
							cout
									<< "Cannot Terminate Server.Type EXIT to exit from server\nEnter your command or HELP for options\n>";
							cout.flush();
							continue;
						}
						int c;
						int soc_to_close;
						for (c = 1; c < no_of_connections; c++) {
							if (n == connection_list[c].id) {
								b = 1;
								soc_to_close = connection_list[c].socket_no;
								//cout << "Yes it is present" << endl;
								if (c == no_of_connections - 1) {

									//For the last row

									// cout << "Last row " << endl;
									connection_list[c].id = 0;

									strcpy(connection_list[c].host_name, "");
									strcpy(connection_list[c].ip_addr, "");
									strcpy(connection_list[c].port_no, "");

									no_of_connections--;
									break;
								}
								//Take remaining rows above
								for (int f = c; f < no_of_connections - 1;
										f++) {
									connection_list[f] = connection_list[f + 1];
									connection_list[f].id =
											connection_list[f].id - 1;

								}
								no_of_connections--;
								break;

							}
						}
						if (b == 0) {
							cout
									<< "Not in the Connection List\nEnter you command or HELP for options\n>";
							cout.flush();
							continue;
						}
						cout << "Updated Connection List" << endl;
						display_list(connection_list, no_of_connections);
						cout << "Enter your command or HELP for Options\n>";
						cout.flush();
						//close socket No
						close(soc_to_close);
						FD_CLR(soc_to_close, &master);

						continue;
					}
					if (strcmp(arr[0].c_str(), "CONNECT") == 0) { //Connecting with other peers
						//cout << "arr[2] " << arr[2].c_str() << endl;
						//Check for less than 3 arguments
						if (arr.size() != 3) {
							cout
									<< "Invalid Arguments : connect <IP Address> <PORT_NO>\nEnter your Command or HELP for options\n>";
							cout.flush();
							continue;
						}

						int b = 0;
						int c = 0;
						if (no_of_connections == 4) {
							cout
									<< "Maximum Limit of Peers (4) Reached\nEnter your command or HELP for options\n>";
							cout.flush();
							continue;
						}
						for (int h = 0; h < no_of_connections; h++) {
							if (strcmp(connection_list[h].ip_addr,
									arr[1].c_str()) == 0
									&& strcmp(connection_list[h].port_no,
											arr[2].c_str()) == 0) {
								cout << "Duplicate connections not allowed\n";
								cout.flush();
								c = 1;
								b = 1;
								break;
							}
						}
						if (c == 0) {
							for (int d = 0; d < s_ip_count; d++) {

								if ((strcmp(s_ip_local[d].ip_addr,
										arr[1].c_str()) == 0
										|| strcmp(s_ip_local[d].host_name,
												arr[1].c_str()) == 0)
										&& strcmp(s_ip_local[d].port_no,
												arr[2].c_str()) == 0) {
									//If Host Name is Mentioned convert to Ip Address
									if (strcmp(s_ip_local[d].host_name,
											arr[1].c_str()) == 0) {
										arr[1] = s_ip_local[d].ip_addr;
									}

									if (strcmp(arr[1].c_str(), ip_address) == 0
											&& strcmp(arr[2].c_str(),
													port.c_str()) == 0) {
										//if the client tries to connect himself
										b = 1;
										cout << "Self Connection not allowed\n";
										cout.flush();
										break;
									}

									b = 1;

									struct reg_params obj_reg_param;
									obj_reg_param.arr = arr;
									obj_reg_param.fdmax = fdmax;
									obj_reg_param.master = master;
									obj_reg_param.port = port;
									obj_reg_param.no_of_connections =
											no_of_connections;
									memcpy(obj_reg_param.ip_address, ip_address,
											sizeof ip_address);
									memcpy(obj_reg_param.connection_list,
											connection_list,
											sizeof connection_list);
									obj_reg_param.b_error = false;

									_register(&obj_reg_param);
									fdmax = obj_reg_param.fdmax;

									port = obj_reg_param.port;
									no_of_connections =
											obj_reg_param.no_of_connections;
									memcpy(ip_address, obj_reg_param.ip_address,
											sizeof ip_address);
									//fd_set temp=master;
									memcpy(connection_list,
											obj_reg_param.connection_list,
											sizeof obj_reg_param.connection_list);
									memcpy(&master, &obj_reg_param.master,
											sizeof obj_reg_param.master);
									//master=temp;
									//cout<<"Connected."<<endl;
//									cout << "Connection List " << endl;
//									display_list(connection_list,
//											no_of_connections);
									if (obj_reg_param.b_error) { //if there is an error in Register

										cout << "Error in register\n";
										cout
												<< "Enter your command or HELP for Options\n>";
										cout.flush();
										continue;
									}

									break;

								}
							}
						} //end of c==0
						if (b == 0) {

							cout
									<< "Cannot Connect.Not in Local Server IP Table\n";
							//cout << "Enter your Command or HELP for Options\n>";

						}
						cout << "Enter Your Command or Help for Options\n>";
						cout.flush();
						continue;

					}
					if (strcmp(arr[0].c_str(), "CREATOR") == 0) { //Display Creator
						creator();
						continue;
					}

					if (strcmp(arr[0].c_str(), "EXIT") == 0) { //EXIT the program
						cout << "Quitting Client Program " << sockfd << endl;
						cout << "Closing Socket " << sockfd << endl;
						close(sockfd);
						cout << "Client Exited" << endl;
						exit(1);
					}

					if (strcmp(arr[0].c_str(), "REGISTER") == 0) {
						if (arr.size() != 3) { //Wrong number of Arguments
							cout
									<< "Invalid Arguments : register  <IP Address> <PORT_NO>\nEnter your Command or HELP for options\n>";
							cout.flush();
							continue;
						}
						if (is_ip_address(arr[1]) == 0) {
							cout
									<< "Invalid Arguments : register  <IP Address> <PORT_NO>\nEnter your Command or HELP for options\n>"
									<< endl;
							cout.flush();
							continue;

						}

						if (no_of_connections > 0) {
							cout << "Client is Already Registered" << endl;
							cout
									<< "Enter your command or HELP for Options \n>";
							cout.flush();
							continue;
						}

						fflush(stdout);
						struct reg_params obj_reg_param;
						obj_reg_param.arr = arr;
						obj_reg_param.fdmax = fdmax;
						//obj_reg_param.master = master;
						memcpy(&obj_reg_param.master, &master,
								sizeof sizeof master);
						obj_reg_param.port = port;
						obj_reg_param.no_of_connections = no_of_connections;
						memcpy(obj_reg_param.ip_address, ip_address,
								sizeof ip_address);
						memcpy(obj_reg_param.connection_list, connection_list,
								sizeof connection_list);
//
						obj_reg_param.b_error = false;
//
						_register(&obj_reg_param);
						fdmax = obj_reg_param.fdmax;

						port = obj_reg_param.port;
						no_of_connections = obj_reg_param.no_of_connections;
						memcpy(ip_address, obj_reg_param.ip_address,
								sizeof ip_address);
						memcpy(connection_list, obj_reg_param.connection_list,
								sizeof obj_reg_param.connection_list);
						memcpy(&master, &obj_reg_param.master,
								sizeof obj_reg_param.master);

//                        cout << "Connection List No of Connections chaman"
//                                << no_of_connections << endl;
//                        display_list(connection_list, no_of_connections);
						if (obj_reg_param.b_error) { //if there is an error in Register
							cout << "Error in register\n";
							cout << "Enter your command or HELP for Options\n>";
							cout.flush();
							continue;
						}

						continue;
					} //End of Register
					else {
						cout
								<< "Invalid Entry.\nEnter your command or HELP for Options \n>";
						fflush(stdout);
						continue;
					}

				} else if (sockfd == i) {
					//Listening for new Connections
					addrlen_client = sizeof remoteaddr_client;
					newfd_client = accept(sockfd,
							(struct sockaddr *) &remoteaddr_client,
							&addrlen_client);

					if (newfd_client == -1) {
						perror("Accept Problem");
					} else {
						FD_SET(newfd_client, &master);
						// add to master set
						if (newfd_client > fdmax) {
							// keep track of the max
							fdmax = newfd_client;
						}
						printf("\nClient: New Connection Request From %s on "
								"Socket %d\n",
								inet_ntop(remoteaddr_client.ss_family,
										get_in_addr(
												(struct sockaddr*) &remoteaddr_client),
										remoteIP_client, INET6_ADDRSTRLEN),
								newfd_client);
					}
					continue;

				} else {
					//Handling data from Server or Peers
					if ((nbytes_client = recv(i, buf_client, BUFFER_SIZE, 0))
							<= 0) {
						// got error or connection closed by the ones connected
						if (nbytes_client == 0) {
							// Connection Closed
							printf("\nClient : Socket %d disconnected\n", i);
							iter = download_list.find(i);
							if (iter != download_list.end()) {
								//KEY exists in the map
								sock_boolean_set obj = iter->second;
								cout << "Download of File " << obj.file_name
										<< " Failed";
							}

							for (int l = 0; l < no_of_connections; l++) {
								if (connection_list[l].socket_no == i) {
									if (l == no_of_connections - 1) {

										//For the last row

										//cout << "Last row " << endl;
										connection_list[l].id = 0;

										strcpy(connection_list[l].host_name,
												"");
										strcpy(connection_list[l].ip_addr, "");
										strcpy(connection_list[l].port_no, "");

										no_of_connections--;
										break;
									}
									//Take remaining rows above
									for (int f = l; f < no_of_connections - 1;
											f++) {
										connection_list[f] = connection_list[f
												+ 1];
										connection_list[f].id =
												connection_list[f].id - 1;

									}
									no_of_connections--;
									break;

								}

							} //end of for

							cout << "Updated Connection List" << endl;
							display_list(connection_list, no_of_connections);
							cout << "Enter your command or HELP for Options\n>";
							cout.flush();
						} else {
							perror("Error in Receiving");
						}
						close(i); // bye!
						FD_CLR(i, &master);
						// remove from master set
					} else { //We have received something
							 //buf_client[nbytes_client] = '\0';
						//cout << "Data Is Received at Client " << buf_client<< endl;

						if (buf_client[0] == '/' && buf_client[1] == 'S'
								&& buf_client[2] == '/' && buf_client[3] == 'I'
								&& buf_client[4] == '?') {
							//cout << "pahua\n";

							//strcpy(buf_client, command.c_str());
							char *without_slash_buf_client = strtok(buf_client,
									"\n");
							char *token = strtok(without_slash_buf_client, " ");
							//cout << "TOKEN START " << token << endl;
							token = strtok(NULL, "/EOF/");
							//cout << "TOKEN " << token << endl;

							vector<string> table_received;
							vector<string> temp;
							table_received = split(token, "#&");
							clean_up_list(s_ip_local);
							s_ip_count = 0;
							for (unsigned int i = 0; i < table_received.size();
									i++) {
								temp = split(table_received[i], " ");
								s_ip_local[s_ip_count].id = atoi(
										temp[0].c_str());
								strcpy(s_ip_local[s_ip_count].host_name,
										temp[1].c_str());
								strcpy(s_ip_local[s_ip_count].ip_addr,
										temp[2].c_str());
								strcpy(s_ip_local[s_ip_count].port_no,
										temp[3].c_str());
								s_ip_count++;

							}
							cout << "\nUpdate Received." << endl;
							cout << "Local Server IP list " << endl;
							display_list(s_ip_local, s_ip_count);
							cout
									<< "Enter your command or HELP for Options \n>";
							cout.flush();

						}
						//p/o.rt/
						else if (buf_client[0] == '/' && buf_client[1] == 'p'
								&& buf_client[2] == '/' && buf_client[3] == 'o'
								&& buf_client[4] == '.' && buf_client[5] == 'r'
								&& buf_client[6] == 't') {

							if (no_of_connections == 4) {
								cout << "New Connection Request Rejected."
										<< endl;
								cout
										<< "Enter your command or HELP for Options\n>";
								cout.flush();
								string reject_request = "/r/jq@/e/t";
								if (send(i, reject_request.c_str(), BUFFER_SIZE,
										0) == -1) {
									perror("send reject to connect ");
									cout
											<< "\nCould Not Send Rejection to the Peer\n";
								} else {
									//cout << "Sent Rejection " << endl;
								}

							} else {
								//cout << "Connection Successful." << endl;
								string accept_req = "a/r/7/q#";
								if (send(i, accept_req.c_str(), BUFFER_SIZE, 0)
										== -1) {
									perror("send accept connection to peer ");
									cout
											<< "\nCould Not Send Acceptance to the Peer\n";
								} else {
									//cout << "Sent Acceptance " << endl;
								}

								cout << "Connection Successful\n";
								cout.flush();

								char *without_slash_n = strtok(buf_client,
										"\n");
								vector<string> prt_ip;
								prt_ip = split(without_slash_n, " ");
								struct hostent *he;
								struct in_addr ipv4addr;
								inet_pton(AF_INET, prt_ip[2].c_str(),
										&ipv4addr);
								he = gethostbyaddr(&ipv4addr, sizeof(ipv4addr),
										AF_INET);
//							cout << "No of Connected users "
//									<< no_of_connections + 1 << endl;
								connection_list[no_of_connections].id =
										no_of_connections + 1;
								strcpy(
										connection_list[no_of_connections].port_no,
										prt_ip[1].c_str());
								strcpy(
										connection_list[no_of_connections].ip_addr,
										prt_ip[2].c_str());
								strcpy(
										connection_list[no_of_connections].host_name,
										he->h_name);
								connection_list[no_of_connections].socket_no =
										i;
								no_of_connections++;

								prt_ip.erase(prt_ip.begin(), prt_ip.end());

								cout << "Updated Connection List " << endl;
								display_list(connection_list,
										no_of_connections);
								cout
										<< "Enter your command or HELP for options\n>";
								cout.flush();
							}

						} else if (buf_client[0] == '/' && buf_client[1] == 'f'
								&& buf_client[2] == '/' && buf_client[3] == 'i'
								&& buf_client[4] == 's'
								&& buf_client[5] == 'i') {
							vector<string> split_filename = split(buf_client,
									" ");
							cout << "\nStarted Downloading File : "
									<< split_filename[1] << endl;

							sock_boolean_set s;
							s.is_set = 1;
							s.file_name = split_filename[1];
							s.size = atoi(split_filename[2].c_str());
							s.rem_bytes = s.size;
							gettimeofday(&s.time_val, NULL);
							s.f_d = open(s.file_name.c_str(),
									O_WRONLY | O_CREAT, 0664);

							//ofstream out(s.file_name);
//							double t_1 = s.time_val.tv_sec + (s.time_val.tv_usec / 1000000.0);

//							struct timeval time;
//								gettimeofday(&time, NULL);
//								double t1 = time.tv_sec + (time.tv_usec / 1000000.0);

							download_list[i] = s;
						} else if (buf_client[0] == '/' && buf_client[1] == 'r'
								&& buf_client[2] == '/' && buf_client[3] == 'j'
								&& buf_client[4] == 'q' && buf_client[5] == '@'
								&& buf_client[6] == '/') {
							//Rejection for connection
							no_of_connections--;
							cout << "\nConnection Rejected\n";

							connection_list[no_of_connections].id = 0;
							strcpy(connection_list[no_of_connections].host_name,
									"");
							strcpy(connection_list[no_of_connections].ip_addr,
									"");
							strcpy(connection_list[no_of_connections].port_no,
									"");
							connection_list[no_of_connections].socket_no = -1;
							cout << "Updated Connection List\n";
							display_list(connection_list, no_of_connections);
							cout << "Enter your command or HELP for Options\n>";
							cout.flush();

						}

						else if (buf_client[0] == 'a' && buf_client[1] == '/'
								&& buf_client[2] == 'r' && buf_client[3] == '/'
								&& buf_client[4] == '7' && buf_client[5] == '/'
								&& buf_client[6] == 'q') {
							//Acceptance for connection

							cout << "\nConnection Accepted\n";

							cout << "Updated Connection List\n";
							display_list(connection_list, no_of_connections);
							cout << "Enter your command or HELP for options\n>";
							cout.flush();

						}

						else if (buf_client[0] == 'u' && buf_client[1] == '/'
								&& buf_client[2] == 'f' && buf_client[3] == '/'
								&& buf_client[4] == 'a' && buf_client[5] == '/'
								&& buf_client[6] == 'i') {

							vector<string> file_not = split(buf_client, " ");

							cout << "\nPeer Could not find the File : "
									<< file_not[1].c_str()
									<< "\nEnter your command or HELP for options\n>";
							cout.flush();

						}

						else if (buf_client[0] == 'u' && buf_client[1] == '/'
								&& buf_client[2] == 'p' && buf_client[3] == '?'
								&& buf_client[4] == 'o' && buf_client[5] == '^'
								&& buf_client[6] == 'r') {

							vector<string> upload_req = split(buf_client, " ");
							cout << "\nReceived Upload Request For File : "
									<< upload_req[1] << endl;
							for (int g = 0; g < no_of_connections; g++) {
								if (i == connection_list[g].socket_no) {
									upload(connection_list[g].id, upload_req[1],
											1);
									break;
								}

							}

						}

						else if (buf_client[0] == '/' && buf_client[1] == 'e'
								&& buf_client[2] == '/' && buf_client[3] == 'o'
								&& buf_client[4] == '/' && buf_client[5] == 'f'
								&& buf_client[6] == '*') {

//                            cout << "END of file descriptor " << buf_client
//                                    << endl;
							//Writing to file
							iter = download_list.find(i);

							if (iter != download_list.end()) {
								//KEY exists in the map
								sock_boolean_set f = iter->second;
								//ofstream out((char*) f.file_name.c_str());
//                                cout << "Size of entire string "
//                                        << strlen(f.to_write.c_str());
								double t_1 = f.time_val.tv_sec
										+ (f.time_val.tv_usec / 1000000.0);
								gettimeofday(&f.time_val, NULL);
								double t_2 = f.time_val.tv_sec
										+ (f.time_val.tv_usec / 1000000.0);
								//  double rate= (size*8.0);
								double rate_download = (f.size * 8.0)
										/ (t_2 - t_1);
								unsigned int rate_download_int = rate_download;

								//out.write(f.to_write.c_str(), f.size);

								string host = " ";
								for (int p = 0; p < no_of_connections; p++) {
									if (connection_list[p].socket_no == i) {
										host = connection_list[p].host_name;
										break;
									}
								}

								//cout<<"t_2 - t_1 " <<t_2-t_1;
								double t_taken = t_2 - t_1;
								cout << "\nDownload Success.\nFile:"
										<< f.file_name << "\nRx ("
										<< self.hostname << "):" << host << "->"
										<< self.hostname << ",File Size: "
										<< f.size << " Bytes,Time Taken: "
										<< t_taken << " seconds,Rx Rate: "
										<< rate_download_int << " bits/sec"
										<< endl;

//								cout << "\nFile Name : " << f.file_name.c_str()
//										<< " Downloaded \nRx Rate : "<<rate_download_int <<" bits/sec " << endl;
								//out.close();
								close(f.f_d);

							}

							download_list.erase(i);
							if (download_list.empty()) {
								cout
										<< "Enter your command or HELP for options\n>";
								cout.flush();
							}

						} else {

							iter = download_list.find(i);
							if (iter != download_list.end()) {
								//KEY exists in the map
								sock_boolean_set f = iter->second;
								if (f.is_set == 1) {

//                                        cout << "Buffer received "
//                                                << endl;
//                                        cout << "Appending "
//                                                << strlen(buf_client) << " "
//                                         	       << sizeof buf_client << endl;

									//out.write(f.to_write.c_str(), f.size);

//									for (int d = 0; d < BUFFER_SIZE; d++) {
//									f.to_write.push_back(buf_client[d]);
//									}
									//f.to_write+=buf_client;
									if (BUFFER_SIZE > f.rem_bytes) {
										int er = write(f.f_d, buf_client,
												f.rem_bytes);

									} else {
										int er = write(f.f_d, buf_client,
												BUFFER_SIZE);
										f.rem_bytes = f.rem_bytes - BUFFER_SIZE;
									}
									//cout<<"After write"<<endl;
									//int er=write(f.f_d,buf_client,)

									//cout<<" Socket No "<<i;
//                                        cout << "f to write size "
//                                                << sizeof f.to_write << endl;

									//f.to_write.append(buf_client);
									//download_list.insert(std::pair<int, sock_boolean_set>(i, f));

								}
								download_list[i] = f;

							}

						}

						cout.flush();
					}
					continue;

				}
			}

		} //end of for
	} //end of while

	return;
}

int main(int argc, char* args[]) {
//Check for valid arguments
	if (argc != 3) {
		cout << "Enter Valid Arguments : c/s <PORT_NO>" << endl;
		return 1;
	}
//Converting port_no to integer
	string port_no_string = args[2];
	int port_no;

	//do stuff

	istringstream(port_no_string) >> port_no;
//Check the first argument
	if (*args[1] == 's') {

		serverprocess(args[2]);

		return 1;
	}
//
	if (*args[1] == 'c') {
		clientprocess(args[2]);
		return 1;

	}

}
