# Distributed Systems Projects

Academic distributed systems project developed in C as part of the Distributed Systems course at Universidad Carlos III de Madrid.

This repository contains the progressive implementation of a distributed tuple-based key-value service. The project is organized across different branches, each one representing a different stage of the system and communication mechanism.

The goal of the project is to understand how distributed services are designed, implemented, tested and evolved using different inter-process and network communication techniques.

## Project Overview

The system implements a client-server service that allows clients to store, retrieve, modify, delete and check the existence of tuples identified by a key.

Each tuple follows this structure:

- `key`
- `value1`
- `value2[]`
- `value3`

The client uses a public API, while the communication with the server is handled internally through proxies and different communication protocols depending on the branch.

## Branches

### POSIX Message Queues

Implementation of the client-server architecture using POSIX message queues.

Main concepts:

- Client-proxy-server architecture
- POSIX message queues
- Message-based communication
- Request/response protocol
- Server-side tuple storage
- Error handling and validation

### TCP Sockets

Implementation of the same distributed service using TCP sockets.

Main concepts:

- TCP client-server communication
- Custom binary protocol
- Network byte order
- Persistent server process
- Environment variables for server IP and port
- Error handling when the server is unavailable

### RPC

Implementation of the distributed service using Remote Procedure Calls.

Main concepts:

- RPC-based communication
- Remote service definition
- Client stubs and server procedures
- Serialization of requests and responses
- Transparent remote procedure execution

## Main Features

- Tuple creation and modification
- Tuple retrieval
- Tuple deletion
- Existence checking
- Service reset
- Client-side API
- Server-side validation
- Automated testing scripts
- Progressive implementation using different distributed communication mechanisms

## Technologies

- C
- POSIX Message Queues
- TCP Sockets
- RPC
- Linux
- Makefile
- Shell scripting

## Repository Structure

The repository is organized by branches, where each branch contains a different implementation stage of the project.

Typical files include:

- `claves.c` / `claves.h`: public API and tuple service logic.
- `proxy-*.c`: client-side proxy implementation.
- `servidor-*.c`: server implementation.
- `protocol_*.h`: communication protocol definitions.
- `Makefile`: compilation rules.
- `run_tests.sh`: automated testing script.
- `PLAN_PRUEBAS.md`: testing plan and validation cases.

## What I Learned

This project helped me understand how distributed systems evolve from local APIs to real client-server architectures. I worked with different communication mechanisms such as POSIX message queues, TCP sockets and RPC, while improving my knowledge of protocol design, error handling, concurrency, testing, and low-level programming in C.
