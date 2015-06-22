# Makefile

CODE_DIR = src

all: source

.PHONY: all

source:
	$(MAKE) -C $(CODE_DIR)

.PHONY: source

clean: clean_main
	$(MAKE) -C $(CODE_DIR) clean
	
clean_main:
	rm launcher