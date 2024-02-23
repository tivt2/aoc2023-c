TARGETS := $(wildcard day*)

%.o: %.c
	gcc -g -o $@ $< util/*.c -I./util

$(TARGETS):
	@echo "Compiling and running $@..."
	gcc -g $@/main.c -o $@/main.o util/*.c -I./util
	./$@/main.o

clean:
	@echo "Cleaning up..."
	rm -f $(addsuffix .o, $(TARGETS))

.PHONY: $(TARGETS) clean
