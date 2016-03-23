
CC = pdi.cpp main.cpp
BIN = tarefa4
PZIP = 8066395_Tarefa4.zip

IN = tartaruga.pgm
OUT = img_out.pgm

CMD = RO -90



all: clean compile


compile:
	g++ *.cpp -o $(BIN)

debug: compile
	valgrind -v --leak-check=full --show-leak-kinds=all ./$(BIN) $(IN)

clean:
	@find -name "*~" | xargs rm -rf
	@find -name $(BIN) | xargs rm -rf
	@find -name $(OUT) | xargs rm -rf
	@find -name obj_marked.pgm | xargs rm -rf

zip: clean
	@rm -f $(PZIP)
	@zip -r $(PZIP) $(CC) Makefile README.md *.hpp $(IN)

run: 
	./$(BIN) $(IN) $(CMD)