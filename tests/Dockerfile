FROM debian:10-slim
RUN apt update \
 && apt install -y lsb-release apt-transport-https ca-certificates wget \
 && wget -O /etc/apt/trusted.gpg.d/php.gpg 'https://packages.sury.org/php/apt.gpg' \
 && echo "deb https://packages.sury.org/php/ $(lsb_release -sc) main" | \
    tee /etc/apt/sources.list.d/php.list \
 && apt update \
 && apt install -y make gcc php7.4 php7.4-mbstring xvfb xterm xvkbd locales locales-all
ENV LC_ALL=en_US.UTF-8 \
    LANG=en_US.UTF-8 \
    LANGUAGE=en_US.UTF-8
COPY . /termbox
WORKDIR /termbox
RUN ./tests/run.sh
