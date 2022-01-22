FROM gcc:9.4.0 AS builder

WORKDIR /app/

COPY ./src/ ./

RUN g++ main.cpp -o main -O3


FROM ubuntu:22.04

COPY  --from=builder /app/main ./

ENTRYPOINT [ "./main" ]

CMD [""]