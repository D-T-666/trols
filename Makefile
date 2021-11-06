build: 
	g++ src/vectors.cpp src/rocket.cpp main.cpp -o rocket -lraylib -lX11 -lGL -lpthread -lrt -lm -ldl

run:
	make build ; ./rocket
