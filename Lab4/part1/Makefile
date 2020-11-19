all: writer reader

run_writer: writer
	@./writer.out

run_reader: reader
	@./reader.out

writer:
	@rm -f writer.out
	@gcc shm_writer.c -o writer.out

reader:
	@rm -f reader.out
	@gcc shm_reader.c -o reader.out

clean:
	@rm *.out
