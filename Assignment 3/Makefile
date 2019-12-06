all: clients

clients: client1a client1b client2a client2b

client1a: client.cpp constants.h
	g++ -O3 -o client1a client.cpp -DCTFILE=\"./ciphertext1a\" -DORACLE=\"/usr/bin/oracle1a\"

client1b: client.cpp constants.h
	g++ -O3 -o client1b client.cpp -DCTFILE=\"./ciphertext1b\" -DORACLE=\"/usr/bin/oracle1b\"

client2a: client.cpp constants.h
	g++ -O3 -o client2a client.cpp -DCTFILE=\"./ciphertext2a\" -DORACLE=\"/usr/bin/oracle2a\"

client2b: client.cpp constants.h
	g++ -O3 -o client2b client.cpp -DCTFILE=\"./ciphertext2b\" -DORACLE=\"/usr/bin/oracle2b\"
