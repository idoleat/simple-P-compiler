NAME = hw1-spec
SPEC_SRC = ../../README.md

$(NAME).pdf: style.css $(SPEC_SRC)
	pandoc --from gfm -t html5 --css style.css $(SPEC_SRC) --output $@ --metadata title=compiler-f19-$(NAME)

clean:
	rm  -f *.html $(NAME).pdf
