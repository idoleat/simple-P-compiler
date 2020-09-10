NAME = hw4-spec
SPEC_SRC = ../../README.md
RESOURCE_SRC = imgs

$(NAME).pdf: style.css $(SPEC_SRC)
	cp -r ../$(RESOURCE_SRC) .
	pandoc --from gfm -t html5 --css style.css $(SPEC_SRC) --output $@ --metadata title=compiler-s20-$(NAME)
	rm -rf $(RESOURCE_SRC)

clean:
	rm -rf $(RESOURCE_SRC)
	rm  -f *.html $(NAME).pdf
