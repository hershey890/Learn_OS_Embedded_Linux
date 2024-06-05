SOURCE=src
TARGET = myModule
OUTPUT=bin

SOURCES := $(wildcard $(SOURCE)/*.c)

all: $(OUTPUT) $(subst $(SOURCE),$(OUTPUT),$(wildcard $(SOURCE)/*.c)) $(OUTPUT)/Makefile
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)/$(OUTPUT) modules

# Create a symblink from src to bin
$(OUTPUT)/%: $(SOURCE)/%
	ln -s ../$< $@

# Generate a Makefile with the needed obj-m and myDriver.o's
$(OUTPUT)/Makefile: $(OUTPUT)
	eval echo "obj-m += ${TARGET}.o" > $@

# Generate the output directory
$(OUTPUT):
	mkdir -p $(OUTPUT)

clean:
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	@rm -r $(OUTPUT)