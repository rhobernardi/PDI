
CC = pgm_floodfill.cpp
BIN = tarefa3
PZIP = 8066395_Tarefa3.zip

IN = image.pgm
OUT = img_out.pgm



all: compile


compile:
	g++ -o $(BIN) $(CC)

debug:
	valgrind --leak-check=full ./$(BIN) $(IN)


clean:
	@find -name "*~" | xargs rm -rf
	@find -name $(BIN) | xargs rm -rf
	@find -name $(OUT) | xargs rm -rf

zip: clean
	@rm -f $(PZIP)
	@zip -r $(PZIP) $(CC) Makefile README.md $(IN) #image.pgm

run:
	./$(BIN) $(IN)