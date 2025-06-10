#FROM gcc:latest
#FROM debian:bullseye
FROM ubuntu:latest
RUN apt-get update && \
    apt-get install -y build-essential cmake

WORKDIR /app

COPY . .
# Limpa build se existir e cria novo
RUN rm -rf build && mkdir build

# Configura build
WORKDIR /app/build
RUN cmake .. && make

EXPOSE 4000
# Define diretório de execução
CMD ["./CentralSystem"]