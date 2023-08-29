assembler:   write.o  pass1.o pass2.o main.o metpass.o
	gcc -g -Wall write.o pass1.o pass2.o main.o metpass.o -o assembler
main.o:  main.c project.h global.h
	gcc -c  -Wall main.c -o  main.o
write.o:  write.c  project.h global.h
	gcc -c  -Wall write.c -o  write.o
pass1.o:  pass1.c  project.h global.h
	gcc -c  -Wall pass1.c -o  pass1.o
pass2.o:  pass2.c project.h global.h
	gcc -c  -Wall pass2.c -o  pass2.o
metpass.o: metpass.c project.h global.h
	gcc -c -Wall metpass.c -o metpass.o 

