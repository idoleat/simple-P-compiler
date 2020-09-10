CC      = gcc
LEX     = flex
YACC    = bison
CFLAGS  = -Wall -std=c11
LIBS    = -lfl -ly

SCANNER = scanner
PARSER  = parser
EXEC    = $(PARSER)
OBJS    = $(PARSER) \
	      $(SCANNER)

# Substitution reference
DEPS := $(OBJS:=.d)
OBJS := $(OBJS:=.o)

all: $(EXEC)

# Static pattern rule
$(SCANNER).c: %.c: %.l
	$(LEX) -o $@ $<

$(PARSER).c: %.c: %.y
	$(YACC) -d -o $@ -v $<

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c -MMD $<

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

clean:
	$(RM) $(DEPS) $(SCANNER:=.c) $(PARSER:=.c) $(PARSER:=.h) \
	$(PARSER:=.output) $(OBJS) $(EXEC)

-include $(DEPS)
