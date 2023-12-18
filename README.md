# TCP/IP Match Engine

The TCP/IP Match Engine is a versatile matching system capable of sending and receiving data over the TCP/IP protocol.

## Build Instructions

Follow these steps to build the project:

1. Open a terminal and navigate to the project source directory.
2. Create a build directory:
    ```bash
    mkdir -p build
    ```
3. Generate build files using CMake:
    ```bash
    cmake -S . -B build
    ```
4. Build the project:
    ```bash
    cmake --build build
    ```

This process will generate two executables, `server` and `client`, both located in `build/bin`.

Alternatively, you can use the provided Dockerfile using the following command:

```bash
docker-compose up
```

These instructions assume you have CMake installed and configured. Adjust the commands as needed based on your project's dependencies and build system.

## Usage

### Server

Start the server by running the `server` executable:

```bash
./server
```

The server operates on port 12345. You can send orders to the server in the format `ACTION ID SYMBOL PRICE QUANTITY` using tools like `telnet` or `netcat`. Ensure that `ACTION` is specified as either `BUY` or `SELL` in all capital letters, and `QUANTITY` must be an integer.. For example:

```bash
nc localhost 12345
```

In the netcat window, type the orders you want to send.

### Client
Use the client executable to automatically generate and send one million orders.

### Calculating Average Processing Time
To determine the average processing time of the server, follow these steps:

#### 1. Redirect Server Output to a Text File
Execute the server and save the output to a `.txt` file:
```bash
./server > server_output.txt
```
#### 2. Use Command Line Utilities
Utilize command line utilities such as `grep`, `cat`, and `awk` to process the output:

```bash
cat server_output.txt \
    | grep "Time " \
    | awk '{
        sum += $6
    } 
    END {
        if (NR > 0) {
            average = sum / NR
            print "Average process time: " average " microseconds."
        }
    }'
```

#### Alternative: Compile and Use avg.cpp
If you saved the output to a file (specifically `server_output.txt`), you can use a compiled C++ program (avg.cpp). Compile it with:

```bash
gcc -g avg.cpp -o avg -O3
```

Then, run:

```bash
./avg
```

#### Docker Container (Assuming Dockerfile Usage)
If you employed a Dockerfile and want to extract data from the container logs, follow these steps:

```bash
docker logs match-engine-server-1 \
    | grep "Time " \
    | awk '{
        sum += $6
    } 
    END {
        if (NR > 0) {
            average = sum / NR
            print "Average process time: " average " microseconds."
        }
    }'
```

Ensure that you use `Ctrl-C` to interrupt the server after each order from `client` is processed. This will allow you to analyze the relevant information in the container logs.

Feel free to customize and extend these functionalities based on your specific requirements.
