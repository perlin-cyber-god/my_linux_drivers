# Top-level Makefile for Linux Drivers

SUBDIRS = $(wildcard drivers/*/)

.PHONY: all clean host help

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	}

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	}

host:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir host; \
	}

help:
	@echo "Linux Drivers Repository"
	@echo "Available commands:"
	@echo "  make         - Build all drivers for the target architecture"
	@echo "  make host    - Build all drivers for the host architecture"
	@echo "  make clean   - Clean build artifacts in all subdirectories"
