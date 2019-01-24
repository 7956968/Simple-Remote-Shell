# Simple-Remote-Shell
The user of the client application can type various Unix shell commands, which execute on the server and return results back to the client.
# README
Name: Shuli He
ID: she77@ucsc.edu

# Files
bin - compiled program: wd; and outputfile
doc - report - [report link](./doc/report.pdf).
src - source file: webDownloader

#Usage
Client:
./rsclient <ipaddress> <port> 
example: ./rsclient 127.0.0.1 1234

Server:
./rsserver <port>
example: ./rsserver 1234