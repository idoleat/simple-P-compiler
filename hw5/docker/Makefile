all: build
.PHONY: build

# Do not named user and group the same, this would cause error in entrypoint.sh
#	because we create the group before user exist which allowing name-crash in useradd command
CONTAINER_USERNAME ?= dummy
CONTAINER_GROUPNAME ?=dummyg

CONTAINER_HOMEDIR ?= /home/dummyg
IMAGE_NAME ?= dummy-image

build: Dockerfile
	docker build \
		--build-arg CONTAINER_USERNAME=${CONTAINER_USERNAME} \
		--build-arg CONTAINER_GROUPNAME=${CONTAINER_GROUPNAME} \
		--build-arg CONTAINER_HOMEDIR=${CONTAINER_HOMEDIR} \
		-t $(IMAGE_NAME) .
