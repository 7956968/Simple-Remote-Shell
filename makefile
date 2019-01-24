RemoteShell:
	g++ -o ./bin/rsclient ./src/RemoteShell_Client.cpp ./src/simpleSocket.cpp -lreadline 
	g++ -o ./bin/rsserver ./src/RemoteShell_Server.cpp ./src/simpleSocket.cpp -lreadline
clean:
	-rm -f ./bin/*
