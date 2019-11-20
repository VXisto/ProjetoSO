all: SOqueromeformar
SOqueromeformar: 
	gcc projeto.c -lpthread -o SOqueromeformar
clean:
	rm -rf *.o
mrproper: clean
	rm -rf SOqueromeformar
