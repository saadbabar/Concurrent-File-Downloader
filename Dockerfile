# Using C++ runtime
FROM gcc:latest

# setting working dir in container (where the source code will go)
WORKDIR /app

# Copy source code into container
COPY . .

# Install dependencies (libcurl)
RUN apt-get update && apt-get install -y \
    libcurl4-openssl-dev

# compile the program
RUN g++ -o downloader main.cpp downloader.cpp -lcurl

# make the entrypoint
ENTRYPOINT [ "./downloader" ]