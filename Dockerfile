FROM ubuntu:22.04
ENV SRC /home/tester/src
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y && apt-get install -qy \
        locales \
        git \
        wget \
        python \
        python3 \
        python-dev \
        python3-dev \
        python3-pip \
        librocksdb-dev \
        libsnappy-dev \
        zlib1g-dev \
        libbz2-dev \
        liblz4-dev \
        && rm -rf /var/lib/apt/lists/*

#NOTE(sileht): really no utf-8 in 2017 !?
ENV LANG en_US.UTF-8
RUN update-locale
RUN locale-gen $LANG

#NOTE(sileht): Upgrade python dev tools
RUN pip3 install -U pip tox virtualenv setuptools pytest Cython

# Set username same as generic default username. Allows output build to be available to same user
ENV USER_NAME ubuntu

ARG host_uid=1001
ARG host_gid=1001
RUN groupadd -g $host_gid $USER_NAME && \
    useradd -g $host_gid -m -s /bin/bash -u $host_uid $USER_NAME

USER $USER_NAME

ENV BUILD_INPUT_DIR /home/$USER_NAME/workspace
RUN mkdir -p $BUILD_INPUT_DIR

WORKDIR $BUILD_INPUT_DIR
