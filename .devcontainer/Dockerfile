FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    pkg-config \
    device-tree-compiler \
    gcc-riscv64-unknown-elf \
    verilator \
    libglfw3-dev \
    libgl1-mesa-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp
RUN git clone --depth 1 https://github.com/riscv-software-src/riscv-isa-sim.git && \
    cd riscv-isa-sim && \
    mkdir build && cd build && \
    ../configure --prefix=/usr/local && \
    make -j$(nproc) && \
    make install && \
    cd /tmp && rm -rf riscv-isa-sim

WORKDIR /workspace
