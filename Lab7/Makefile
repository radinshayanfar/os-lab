all: banker

run: banker
	@./banker.out

banker:
	@rm -f banker.out
	@gcc banker.c -lpthread -o banker.out

clean:
	@rm *.out
