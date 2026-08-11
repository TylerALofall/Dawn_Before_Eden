CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -Werror -pedantic -O2

.PHONY: all eyes clean

all:
	@echo "Dawn_Before_Eden ready"

eyes: eyes/eyes_map
	./eyes/eyes_map

eyes/eyes_map: eyes/eyes_map.c eyes/eyes.c eyes/eyes.h
	$(CC) $(CFLAGS) -o eyes/eyes_map eyes/eyes_map.c eyes/eyes.c

clean:
	rm -f eyes/eyes_map
	rm -rf eyes/output
