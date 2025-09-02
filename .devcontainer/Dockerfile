FROM ubuntu:22.04

RUN apt-get update && apt-get upgrade -y

RUN apt-get install -y ca-certificates gpg wget
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update && apt-get install -y kitware-archive-keyring

RUN apt-get update

RUN apt-get install -y \
    build-essential cmake git pkg-config \
    libx11-dev libxcursor-dev libxrandr-dev libxi-dev libusb-1.0-0-dev g++ \
    libgl1-mesa-dev libglu1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY .. .

CMD ["/bin/bash"]