INSTALL     = sudo $(shell which install)
CC          = $(shell which gcc)
OS 	   	   := $(shell uname)


install:	passgen.c
	@echo "Building valgrind compatible binary..."
	$(CC) passgen.c -o passgen
	sudo mv passgen /usr/local/bin 
	clear
	@echo "enter your password to continue..."
	clear
	passgen
	@echo ""

uninstall:
	@read -p "Do you want to uninstall passgen? (y/n) " confirm; \
	if [ $$confirm == "y" ]; then \
  		echo "Uninstalling passgen from system..."; \
  		echo "Enter your password to continue..."; \
  		sudo rm /usr/local/bin/passgen; \
  		clear; \
  		echo "passgen uninstalled successfully"; \
 		else \
  		echo "Uninstall cancelled"; \
 	fi
