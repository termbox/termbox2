FROM debian:trixie-slim
ARG cflags=""
RUN export DEBIAN_FRONTEND=noninteractive \
 && apt-get -y update >/dev/null \
 && apt-get -y install lsb-release apt-transport-https ca-certificates wget >/dev/null \
 && wget -qO /usr/share/keyrings/sury-php.gpg 'https://packages.sury.org/php/apt.gpg' \
 && echo "deb [signed-by=/usr/share/keyrings/sury-php.gpg] https://packages.sury.org/php/ bookworm main" | \
    tee /etc/apt/sources.list.d/php.list \
 && apt-get -y update >/dev/null \
 && apt-get -y install make gcc php8.4-cli xvfb xterm xvkbd locales locales-all >/dev/null
ENV LC_ALL=en_US.UTF-8 \
    LANG=en_US.UTF-8 \
    LANGUAGE=en_US.UTF-8
COPY . /termbox
WORKDIR /termbox
RUN CFLAGS="${cflags}" make clean test_local
