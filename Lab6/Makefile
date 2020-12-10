all: reader_writer dining_philosophers

run_reader_writer: reader_writer
	@./reader_writer.out

run_dining_philosophers: dining_philosophers
	@./dining_philosophers.out

reader_writer:
	@rm -f reader_writer.out
	@gcc reader_writer.c -lpthread -o reader_writer.out

dining_philosophers:
	@rm -f dining_philosophers.out
	@gcc dining_philosophers.c -lpthread -o dining_philosophers.out

clean:
	@rm *.out
