# Main Makefile

OUTPUT = output
CPP_OUTPUT = Circuit_Optimizer
BUILD = build
POST_PROC = post-proc
PYTHON = .venv/bin/python
DOCS = docs

# Define color escape codes
COLOR_RED=\033[0;31m
COLOR_GREEN=\033[0;32m
COLOR_YELLOW=\033[0;33m
COLOR_RESET=\033[0m

# Default target
all: env configure build run post

# Step 1: Configure the project using cmake
configure:
	@echo -e "$(COLOR_GREEN)=== Configuring the project ===$(COLOR_RESET)"
	mkdir -p $(BUILD)
	mkdir -p $(OUTPUT)
	mkdir -p $(DOCS)
	@if [ "$(shell uname -s)" = "Darwin" ]; then \
		cd $(BUILD) && cmake -DCMAKE_C_COMPILER=/opt/homebrew/bin/gcc-13 -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-13 .. ; \
	elif [ "$(shell uname -s)" = "Linux" ]; then \
		cd $(BUILD) && cmake -DCMAKE_C_COMPILER=/usr/bin/gcc-13 -DCMAKE_CXX_COMPILER=/usr/bin/g++-13 .. ; \
	fi

# Step 2: Build the project using the generated Makefile in the build directory
build: configure
	@echo "$(COLOR_GREEN)=== Building the project ===$(COLOR_RESET)"
	cd $(BUILD) && $(MAKE)

# Step 3: Run the compiled binary
run: configure
	@echo "$(COLOR_GREEN)=== Running the project ===$(COLOR_RESET)"
	./$(BUILD)/bin/$(CPP_OUTPUT)

post:
	@echo "$(COLOR_GREEN)=== Post-processing the results ===$(COLOR_RESET)"
	$(PYTHON) $(POST_PROC)/main.py

# Clean the build directory
clean:
	@echo "$(COLOR_RED)=== Cleaning the build directory ===$(COLOR_RESET)"
	rm -rf $(BUILD)
	@echo "$(COLOR_RED)=== Cleaning the output directory ===$(COLOR_RESET)"
	rm -rf $(OUTPUT)

# Create the environment
env:
	@echo "$(COLOR_GREEN)=== Creating python environment ===$(COLOR_RESET)"
	python3 -m venv .venv
	$(PYTHON) -m pip install -r $(POST_PROC)/requirements.txt
	@echo "$(COLOR_GREEN)=== Installing dependencies ===$(COLOR_RESET)"
	@if [ "$(shell uname -s)" = "Darwin" ]; then \
		brew install graphviz; \
		brew install gcc@13; \
		brew unlink gcc; \
		brew link --overwrite gcc@13; \
		brew install libomp; \
	elif [ "$(shell uname -s)" = "Linux" ]; then \
		. /etc/os-release; \
		if [ "$ID" = "ubuntu" ] || [ "$ID" = "debian" ]; then \
			sudo apt-get install -y graphviz; \
			sudo apt install gcc-13 g++-13 -y; \
		elif [ "$ID" = "fedora" ]; then \
			sudo dnf install -y dnf-plugins-core; \
			sudo dnf config-manager --set-enabled updates-testing; \
			sudo dnf install -y gcc gcc-c++ graphviz; \
		elif [ "$ID" = "arch" ]; then \
			sudo pacman -S --noconfirm graphviz; \
			sudo pacman -Syu gcc gcc13 gcc13-libs; \
		else \
			echo "$(COLOR_RED)Unsupported Linux distribution$(COLOR_RESET)"; \
		fi \
	fi

docs: configure build
	@echo "$(COLOR_GREEN)=== Generating documentation ===$(COLOR_RESET)"
	@command -v doxygen >/dev/null 2>&1 || { echo "$(COLOR_RED)Doxygen required to run the 'docs' command. Head over to https://www.doxygen.nl/download.html $(COLOR_RESET)" >&2; exit 1; }
	doxygen Doxyfile
	@echo "$(COLOR_GREEN)=== Documentation generated in $(DOCS) ===$(COLOR_RESET)"

.PHONY: all configure build run clean