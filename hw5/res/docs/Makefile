NAME = hw5-spec
SPEC_SRC = ../../README.md
RESOURCE_SRC = imgs

$(NAME).pdf: style.css $(SPEC_SRC)
	mkdir -p ./res/
	cp -r ../$(RESOURCE_SRC) ./res/
	pandoc --from gfm -t html5 --css style.css $(SPEC_SRC) --output $@ --metadata title=compiler-f19-$(NAME)
	rm -rf ./res

clean:
	rm -rf ./res
	rm  -f *.html $(NAME).pdf
