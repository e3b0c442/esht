CC		:= gcc
CCFLAGS := -Wall -pedantic -ansi -g
LDFLAGS := 

OBJ		:= esht_test.o esht.o

$(OBJ): %.o: %.c
	$(CC) -c -o $@ $< $(CCFLAGS)

esht_test: esht_test.o
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean test

clean:
	rm esht.o esht_test esht_test.o

test: esht_test
	./esht_test || true
	rm esht_test esht_test.o