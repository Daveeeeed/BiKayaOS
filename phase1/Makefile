
default:
	@echo "Please specify a target"

umps:
	@mkdir -p build
	@mkdir -p build/umps
	@$(MAKE) -f umpsmake PACK=mipsel-linux-gnu

umps1:
	@mkdir -p build
	@mkdir -p build/umps
	@$(MAKE) -f umpsmake PACK=mipsisa64r6el-linux-gnuabi64

uarm:
	@mkdir -p build
	@mkdir -p build/uarm
	@$(MAKE) -f uarmmake

clean:
	@rm -r -f build

.PHONY: default umps uarm clean
