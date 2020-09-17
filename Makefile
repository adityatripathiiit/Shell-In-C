all:build

build:
	@echo "Building.."
	gcc shell.c -o shell

clean:
	@echo "Cleaning"
	rm shell
