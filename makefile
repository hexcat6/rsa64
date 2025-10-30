all:
	gcc -Wall -std=c23 RSA.c -o RSA

run:
	./RSA.exe -e "hello world"
	./RSA.exe -d "123 123 123 123"