.PHONY: board clean

board:
	../src/compiler src/boardTest.p --save_path src/
	pio run
	pio run --target upload

clean:
	$(RM) -r src/boardTest.S .pio/
	