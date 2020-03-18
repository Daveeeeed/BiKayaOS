
default:
	@echo "Please specify a target"

umps:
	@mkdir -p build
	@$(MAKE) -f uarmmake clean
	@$(MAKE) -f umpsmake

uarm:
	@mkdir -p build
	@$(MAKE) -f umpsmake clean
	@$(MAKE) -f uarmmake

clean:
	@rm -f build/*
	@rmdir build

.PHONY: default umps uarm clean
