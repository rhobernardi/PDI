
CC = pdi.cpp main.cpp
BIN = filtragem
PZIP = 8066395_Tarefa6.zip

IN = image.pgm
OUT = img_out.pgm



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

#run:
#	@./$(BIN)
	
push: clean
	git add *
	git status
	git commit -m "Tarefa 6 em andamento"
	git push