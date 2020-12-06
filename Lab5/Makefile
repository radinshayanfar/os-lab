all: concurrent serial

run_concurrent: concurrent
	@./concurrent.out

run_serial: serial
	@./serial.out

concurrent:
	@rm -f concurrent.out
	@gcc concurrent.c -lpthread -o concurrent.out

serial:
	@rm -f serial.out
	@gcc serial.c -lpthread -o serial.out

clean:
	@rm *.out
