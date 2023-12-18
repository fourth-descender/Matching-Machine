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

### Getting The Average Time
To calculate the average processing time, redirect the output of the server to a `.txt` file, for example:
```bash
./server > server_output.txt
```
Then, utilize command line utilities like `grep`, `cat`, and `awk`. For instance:
```
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
Alternatively, if you saved the output to `server.txt`, you can also compile and use avg.cpp. Compile using:
```bash
gcc -g avg.cpp -o avg -O3
```
Then, run:
```bash
./avg
```
to obtain the average processing time. Feel free to customize and extend these functionalities based on your specific requirements.
