build: 
	g++ src/raylib-utils.cpp src/rocket.cpp src/graphs.cpp main.cpp -o rocket -lraylib -lX11 -lGL -lpthread -lrt -lm -ldl

run:
	make build ; ./rocket
