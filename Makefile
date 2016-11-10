exclude_dirs:=include bin

#commom must be compiled before any other modules
dirs:=common

subdir=$(shell find . -maxdepth 1 -type d)
dirs+=$(filter-out ./common,$(subdir))
dirs:=$(basename $(patsubst ./%,%,$(dirs)))
dirs:=$(filter-out $(exclude_dirs),$(dirs))

.PHONY: $(dirs) clean

$(dirs):
	@for dir in $(dirs); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(dirs); do \
		$(MAKE) -C $$dir clean; \
	done
