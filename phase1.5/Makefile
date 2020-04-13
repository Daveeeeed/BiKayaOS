
default:
	@echo "Please specify a target"

umps:
	@mkdir -p build
	@$(MAKE) -f uarmmake clean
	@$(MAKE) -f umpsmake PACK=mipsel-linux-gnu

umps1:
	@mkdir -p build
	@$(MAKE) -f uarmmake clean
	@$(MAKE) -f umpsmake PACK=mipsisa64r6el-linux-gnuabi64

uarm:
	@mkdir -p build
	@$(MAKE) -f umpsmake clean
	@$(MAKE) -f uarmmake

clean:
	@rm -f build/*
	@rmdir build

.PHONY: default umps uarm clean
