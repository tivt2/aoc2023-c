TARGETS := $(wildcard day*)

%.o: %.c
	gcc -g -o $@ $<

$(TARGETS):
	@echo "Compiling and running $@..."
	gcc -g $@/main.c -o $@/main.o
	./$@/main.o

clean:
	@echo "Cleaning up..."
	rm -f $(addsuffix .o, $(TARGETS))

.PHONY: $(TARGETS) clean
