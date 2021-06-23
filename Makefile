OBJS = main.o 
BINARY = main
LDFLAGS = -lm
Flags = g++ -Wall -g3
ArchivosCSV = Ordenado.csv
main: crear $(OBJS)
	$(Flags) -o $(BINARY) $(OBJS) $(LDFLAGS)

main.o: main.cpp funciones.cpp
	$(Flags) -c main.cpp

crear:
	mkdir -p al
	touch al/Ordenado.csv

.PHONY: clean 
clean: 
	rm -fr $(BINARY) $(OBJS) $(ArchivosCSV) al

