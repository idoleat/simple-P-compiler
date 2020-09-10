FROM ubuntu:18.04

# TODO: Combine all RUN commands

RUN apt-get update
RUN apt-get --no-install-recommends install -y \
    gosu \
    python3 python3-pip python3-setuptools python3-wheel\
    gcc \
    build-essential make \
    flex libfl-dev \
    bison \
    && rm -rf /var/lib/apt/lists/*
# RUN rm -rf /var/lib/apt/lists/*


# will be installed at /usr/local/lib/python3.6/dist-packages
RUN pip3 install colorama

# optional packages
ARG CONTAINER_USERNAME=dummy
ARG CONTAINER_GROUPNAME=dummyg
ARG CONTAINER_HOMEDIR=/home/dummy

ENV DOCKER_USERNAME_PASSIN ${CONTAINER_USERNAME}
ENV DOCKER_GROUPNAME_PASSIN ${CONTAINER_GROUPNAME}
ENV DOCKER_HOMEDIR_PASSIN ${CONTAINER_HOMEDIR}

# flag to prevent student to recursively enter docker environment
ENV STATUS_DOCKER_ACTIVATED DOCKER_ACTIVATED

COPY entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh

CMD ["/bin/bash"]
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]