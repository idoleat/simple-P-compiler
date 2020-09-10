SCANNER = scanner
CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -g3
LEX     = flex
LIBS    = -lfl

all: $(SCANNER)

$(SCANNER): $(SCANNER).c
	$(CC) $(CCFLAGS) $< -o $(SCANNER) $(LIBS)

$(SCANNER).c: %.c: %.l
	$(LEX) -o $@ $<

.PHONY: clean

clean:
	$(RM) $(SCANNER) $(SCANNER:=.c)
