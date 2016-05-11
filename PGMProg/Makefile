
CC = src/main.cpp
BIN = DisplayLines
PZIP = 8066395_Tarefa6.zip

IN = images/chessboard1.png



all: compile run

compile:
	@sh compile.sh

debug: compile
	valgrind -v --leak-check=full --show-leak-kinds=all ./$(BIN) $(IN)

clean:
	@find -name "*~" | xargs rm -rf
	@find -name $(BIN) | xargs rm -rf
	@find -name $(OUT) | xargs rm -rf
	@find -name ./bin/ | xargs rm -rf
	@find -name obj_marked.pgm | xargs rm -rf

zip: clean
	@rm -f $(PZIP)
	@zip -r $(PZIP) src/ build/ Makefile README.md images/chessboard1.png

run:
	@./$(BIN) $(IN)