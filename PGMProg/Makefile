
CC = pdi.cpp main.cpp
BIN = tarefa4
PZIP = 8066395_Tarefa3.zip

IN = image.pgm
OUT = img_out.pgm

CMD = RO +90



all: clean compile


compile:
	g++ *.cpp -o $(BIN)

debug: compile
	valgrind -v --leak-check=full --show-leak-kinds=all ./$(BIN) $(IN)

clean:
	@find -name "*~" | xargs rm -rf
	@find -name $(BIN) | xargs rm -rf
	@find -name $(OUT) | xargs rm -rf

zip: clean
	@rm -f $(PZIP)
	@zip -r $(PZIP) $(CC) Makefile README.md *.hpp $(IN)

run: compile
	./$(BIN) $(IN) $(CMD)
	
push:
	git add *
	git commit -m "Tarefa 4 em andamento"
	git push
