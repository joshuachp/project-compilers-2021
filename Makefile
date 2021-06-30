BUILD_TOOL ?= Ninja
BUILD_DIR ?= build

.PHONY: all setup build clean

all: setup build

setup:
	cmake -B ${BUILD_DIR} -S . -G ${BUILD_TOOL} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

build:
	cmake --build ${BUILD_DIR}

clean:
	rm -r ${BUILD_DIR} || true
