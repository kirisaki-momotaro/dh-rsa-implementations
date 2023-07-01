all:dh_assign_1  rsa_assign_1

dh_assign_1:dh_assign_1.c 
	gcc -o dh_assign_1  dh_assign_1.c -lgmp

rsa_assign_1:rsa_assign_1.c
	gcc -o rsa_assign_1  rsa_assign_1.c -lgmp
