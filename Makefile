#!/usr/bin/make

PYTHON = python3

.PHONY: install

install:
	$(PYTHON) setup.py install --user
