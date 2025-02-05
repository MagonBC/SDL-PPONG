RM = rm -fv
OBJECT = main.o display.o moveball.o player1_rpc_server.o player2_rpc_clnt.o player1_rpc_svc.o
LDFLAGS = -Wall -lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lnsl -ltirpc
CC = gcc

prepare:
	sudo zypper install libSDL2-devel SDL2 \
	        libSDL2_image-devel libSDL2_image-2_0-0 \
            libSDL2_ttf-devel libSDL_ttf-2_0-0 \
            libSDL2_mixer-devel libSDL2_mixer-2_0-0 \
            libtirpc-devel libnsl-devel
	sudo systemctl start rpcbind

player1: $(OBJECT)
	$(CC) -o player1 $(OBJECT) $(LDFLAGS) 

main.o: main.c display.h game.h moveball.h player1_rpc.h player2_rpc.h
	$(CC) -c main.c

moveball.o: moveball.c moveball.h game.h
	$(CC) -c moveball.c

display.o: display.c game.h display.h
	$(CC) -c display.c
	
player1_rpc_server.o: player1_rpc_server.c player1_rpc.h
	$(CC) -c player1_rpc_server.c

player2_rpc_clnt.o: player2_rpc_clnt.c player2_rpc.h
	$(CC) -c player2_rpc_clnt.c

player1_rpc_svc.o: player1_rpc_svc.c
	$(CC) -c player1_rpc_svc.c

test: test.o
	$(CC) -o test test.o $(LDFLAGS)

test.o: test.c
	$(CC) -c test.c
	
clean: 
	$(RM) $(OBJECT)
