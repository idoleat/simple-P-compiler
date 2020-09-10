all: project

.PHONY: restore project test clean

IMAGE_NAME = compiler-s20-hw2
DOCKERHUB_HOST_ACCOUNT=ianre657
IMAGE_FULLNAME = ${DOCKERHUB_HOST_ACCOUNT}/${IMAGE_NAME}:latest


# TODO: add a clean build opiton
project:
	make -C src/
project-clean:
	${MAKE} clean -C src/

test: project
	make -C test/
test-clean:
	${MAKE} clean -C test/

clean:	project-clean test-clean

docker-pull:
	docker pull ${IMAGE_FULLNAME}

autograde: clean
	make project && make test

spec:
	make -C res/docs

# Docker
# ========================================================
# ██████╗  ██████╗  ██████╗██╗  ██╗███████╗██████╗
# ██╔══██╗██╔═══██╗██╔════╝██║ ██╔╝██╔════╝██╔══██╗
# ██║  ██║██║   ██║██║     █████╔╝ █████╗  ██████╔╝
# ██║  ██║██║   ██║██║     ██╔═██╗ ██╔══╝  ██╔══██╗
# ██████╔╝╚██████╔╝╚██████╗██║  ██╗███████╗██║  ██║
# ╚═════╝  ╚═════╝  ╚═════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝
# ========================================================


.PHONY: docker-build docker-push activate

# Do not named user and group the same, this would cause error in entrypoint.sh
#	because we create the group before user exist which allowing name-crash in useradd command
CONTAINER_USERNAME = student
CONTAINER_GROUPNAME = studentg

HOST_NAME = compiler-s20
HOMEDIR = /home/$(CONTAINER_USERNAME)

# ===================== end Docker args

docker-push: docker-build
	docker login && docker tag ${IMAGE_NAME} ${IMAGE_FULLNAME} && docker push ${IMAGE_FULLNAME}

docker-build:
	${MAKE} \
		IMAGE_NAME=${IMAGE_NAME} \
		CONTAINER_USERNAME=${CONTAINER_USERNAME}\
		CONTAINER_GROUPNAME=${CONTAINER_GROUPNAME}\
		CONTAINER_HOMEDIR=${HOMEDIR}\
		HOMEDIR=${HOMEDIR} \
		-C docker

activate:
	python3 docker/activate_docker.py \
		--username ${CONTAINER_USERNAME} \
		--homedir ${HOMEDIR} \
		--imagename ${IMAGE_FULLNAME} \
		--hostname ${HOST_NAME} ${ARGS}