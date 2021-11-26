INCLUDE=./include

build: 
	g++ src/raylib-utils.cpp src/control.cpp src/rocket.cpp src/graphs.cpp src/GuNC.cpp main.cpp -o rocket -lraylib -lX11 -lGL -lpthread -lrt -lm -ldl -I ${INCLUDE}

run:
	make build ; ./rocket
