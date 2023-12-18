FROM alpine:latest

RUN apk update && \
    apk add --no-cache g++ cmake make

ENV DOCKER_ENV=1
WORKDIR /app

COPY . /app

RUN rm -rf build
RUN mkdir -p build && cd build && cmake .. && make

CMD ["sh"]