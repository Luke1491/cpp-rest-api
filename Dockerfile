# Use a lightweight base image
FROM ubuntu:latest

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libboost-all-dev \
    libsqlite3-dev \
    sqlite3 \
    && rm -rf /var/lib/apt/lists/*

# Install crow from .deb package

# Copy the source code into the container
COPY . /app
WORKDIR /app

RUN dpkg -i /app/crow-v1.0+5.deb

# Build the C++ application
RUN make

# Expose the port the application runs on
EXPOSE 8088

# Command to run the application
CMD ["./rest_api"]
