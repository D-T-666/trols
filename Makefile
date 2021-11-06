build: 
	g++ src/rocket.cpp src/graphs.cpp main.cpp -o rocket -lraylib -lX11 -lGL -lpthread -lrt -lm -ldl

run:
	make build ; ./rocket
