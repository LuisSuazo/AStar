#DIRECTORIOS
dir_guard=@mkdir -p $(@D)
ODIR = obj
SDIR = src
BDIR = bin
IH= -Iheaders
Valgrind= valgrind

#REGLAS EXPLICITAS
CC = g++
OBJ = $(ODIR)/Funciones.o $(ODIR)/AStar.o $(ODIR)/Lectura.o $(ODIR)/main.o 
CFLAGS = -O2 -std=c++17 -Wall -pedantic -fopenmp -lboost_iostreams -lboost_program_options -lboost_graph -lstdc++fs

#BINARIO
TARGET=main

$(BDIR)/$(TARGET):$(OBJ)
	$(dir_guard)
	$(CC) -o $@ $^ $(IH) $(CFLAGS) 

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(dir_guard)
	$(CC) -c $(IH) -o $@ $< $(CFLAGS) 

clean:
	rm -rf $(ODIR) 
	rm -rf $(BDIR)
	rm -rf Salida
	@mkdir -p Salida
	
run:
	./$(BDIR)/$(TARGET) input/NY.co input/NY_dist.gr 31196 47883

testing = ./$(BDIR)/$(TARGET) input/NY.co input/NY_dist.gr 31196 47883

valgrind:
	$(Valgrind) --tool=memcheck -v --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes --log-file=Debug/valgrind.log $(testing) 

valkyrie:
	$(Valgrind) --leak-check=full --track-origins=yes --show-reachable=yes --xml-file=Debug/valkyrie.xml --xml=yes $(testing) 
	valkyrie --view-log Debug/valkyrie.xml 

callgrind:
	$(Valgrind) --tool=callgrind --callgrind-out-file=Debug/callgrind.out $(testing) 
	kcachegrind Debug/callgrind.out

massif:
	$(Valgrind) --tool=massif --pages-as-heap=yes --detailed-freq=1 --time-unit=B --massif-out-file=Debug/massif.out $(testing)
	massif-visualizer  Debug/massif.out