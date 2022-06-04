FROM debian:11-slim
ARG cflags=""
RUN export DEBIAN_FRONTEND=noninteractive \
 && apt-get -y update >/dev/null \
 && apt-get -y install lsb-release apt-transport-https ca-certificates wget >/dev/null \
 && wget -qO /etc/apt/trusted.gpg.d/php.gpg 'https://packages.sury.org/php/apt.gpg' \
 && echo "deb https://packages.sury.org/php/ $(lsb_release -sc) main" | \
    tee /etc/apt/sources.list.d/php.list \
 && apt-get -y update >/dev/null \
 && apt-get -y install make gcc php8.0-cli xvfb xterm xvkbd locales locales-all >/dev/null
ENV LC_ALL=en_US.UTF-8 \
    LANG=en_US.UTF-8 \
    LANGUAGE=en_US.UTF-8
COPY . /termbox
WORKDIR /termbox
RUN CFLAGS="${cflags}" make clean test_local
