project_name = pitaya

DUNE = opam exec -- dune
opam_file = $(project_name).opam

.PHONY: help
help:
	@echo "";
	@echo "List of available make commands";
	@echo "";
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2}';
	@echo "";

.PHONY: build
build:
	$(DUNE) build @all

.PHONY: build-prod
build-prod:
	$(DUNE) build --profile=prod @all

.PHONY: build-dev
dev:
	$(DUNE) build -w @all

.PHONY: clean
clean: ## Clean artifacts
	$(DUNE) clean

.PHONY: format
format: 
	$(DUNE) build @fmt --auto-promote

.PHONY: format-check
format-check:
	$(DUNE) build @fmt

.PHONY: pin
pin:
	opam install dune.3.10.0
	opam pin add server-reason-react "https://github.com/ml-in-barcelona/server-reason-react.git#d29790ffac08e2f4f3f8d65374ca76ebde334ab3" -y

.PHONY: create-switch
create-switch:
	opam switch create . 5.1.0 --deps-only --with-test -y

.PHONY: install
install: create-switch pin

.PHONY: init
init: install

.PHONY: demo
demo: build
	yarn webpack
	$(DUNE) exec src/server/server.exe

.PHONY: server
server: build
	$(DUNE) exec src/server/server.exe