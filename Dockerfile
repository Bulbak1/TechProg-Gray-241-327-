FROM ubuntu:latest

RUN apt-get update \
&& apt-get install -y \
build-essential \
qtbase5-dev \
&& rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/myapp

COPY . .

WORKDIR /usr/src/myapp/server

RUN qmake echoServer.pro
RUN make

CMD ["./echoServer"]
