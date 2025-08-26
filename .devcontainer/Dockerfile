FROM debian:bookworm-slim

RUN apt-get update && apt-get upgrade

RUN apt-get install -y \
    build-essential cmake git pkg-config \
    libx11-dev libxcursor-dev libxrandr-dev libxi-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY .. .

CMD ["/bin/bash"]