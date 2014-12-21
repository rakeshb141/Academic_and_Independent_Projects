all: removeFile server

removeFile:
	if [ -f server ] ; then rm server; fi;

server:
	g++ rbaingol_server.cpp -o server
