FROM ubuntu:18.04 AS riscv-builder

ENV RISCV /risc-v

RUN apt-get update

RUN apt-get install -y \
    git autoconf automake autotools-dev curl \
    libmpc-dev libmpfr-dev libgmp-dev libusb-1.0-0-dev gawk \
    build-essential  device-tree-compiler bison flex \
    texinfo gperf libtool patchutils bc zlib1g-dev \
    device-tree-compiler pkg-config libexpat-dev

RUN mkdir -p $RISCV && cd ~ && \
    git clone -b rvv-0.7.x --recursive https://github.com/riscv/riscv-gnu-toolchain && \
    cd riscv-gnu-toolchain && ./configure --prefix=$RISCV --with-arch=rv32gc --with-abi=ilp32d && \
    make -j "$(nproc)" && make clean

ENV PATH="/risc-v/bin:${PATH}"

RUN cd ~ && git clone https://github.com/riscv/riscv-pk.git && \
    cd riscv-pk && mkdir build && cd build && \
    ../configure --prefix=$RISCV --host=riscv32-unknown-elf && \
    make -j "$(nproc)" && make install -j "$(nproc)" && make clean

RUN cd ~ && git clone https://github.com/riscv/riscv-isa-sim.git && \
    cd riscv-isa-sim && mkdir build && cd build && \
    ../configure --prefix=$RISCV && make -j "$(nproc)" && make install -j "$(nproc)" && make clean


# final image
FROM ubuntu:18.04

ENV RISCVBIN /risc-v/bin
ENV RISCV /risc-v
COPY --from=riscv-builder /risc-v/bin/riscv32-unknown-elf-gcc /risc-v/bin/spike /risc-v/bin/   
COPY --from=riscv-builder /risc-v/lib/ /risc-v/lib/ 
COPY --from=riscv-builder /risc-v/libexec/ /risc-v/libexec/
COPY --from=riscv-builder /risc-v/riscv32-unknown-elf/ /risc-v/riscv32-unknown-elf/
# COPY --from=riscv-builder /risc-v/ /risc-v/

ENV PATH="/risc-v/bin:${PATH}"

RUN apt-get update && apt-get install -y \
    gosu \
    bison flex \
    device-tree-compiler \
    python3 python3-pip \
    dfu-util

RUN pip3 install platformio
ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8

# optional packages
ARG CONTAINER_USERNAME=student
ARG CONTAINER_GROUPNAME=studentg
ARG CONTAINER_HOMEDIR=/home/student

ENV DOCKER_USERNAME_PASSIN ${CONTAINER_USERNAME}
ENV DOCKER_GROUPNAME_PASSIN ${CONTAINER_GROUPNAME}
ENV DOCKER_HOMEDIR_PASSIN ${CONTAINER_HOMEDIR}

# flag to prevent student to recursively enter docker environment
ENV STATUS_DOCKER_ACTIVATED DOCKER_ACTIVATED

COPY entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh

CMD ["/bin/bash"]
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]