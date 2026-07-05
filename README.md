# Distributed Systems Projects

Academic distributed systems project developed in C as part of the Distributed Systems course at Universidad Carlos III de Madrid.

This repository contains the progressive implementation of a distributed tuple-based key-value service. The project is organized across different branches, each one representing a different stage of the system and a different communication mechanism.

The goal of the project is to understand how distributed services are designed, implemented, tested and evolved using different inter-process and network communication techniques.

## Project Overview

The system implements a client-server service that allows clients to store, retrieve, modify, delete and check the existence of tuples identified by a key.

Each tuple follows this structure:

- `key`
- `value1`
- `value2[]`
- `value3`

The client interacts with the system through a public API, while the communication with the server is handled internally through proxies and different communication protocols depending on the branch.

## Branches

This repository is organized by branches. Each branch represents a different implementation stage of the same distributed service.

### `main` – POSIX Message Queues

Implementation of the client-server architecture using POSIX message queues.

Main concepts:

- Client-proxy-server architecture
- POSIX message queues
- Message-based communication
- Request/response protocol
- Server-side tuple storage
- Error handling and validation

### `ejercicio-2` – Intermediate Implementation Stage

Intermediate stage of the distributed service implementation.

Main concepts:

- Evolution of the client-server design
- Improved communication structure
- API validation
- Error handling
- Testing and debugging of distributed operations

### `ejercicio-3` – TCP Sockets

Implementation of the same distributed service using TCP sockets.

Main concepts:

- TCP client-server communication
- Custom communication protocol
- Network byte order
- Server IP and port configuration
- Persistent server process
- Error handling when the server is unavailable

### `practica-final` – RPC / Final Version

Final implementation of the distributed service, integrating the main concepts developed throughout the project and introducing Remote Procedure Calls.

Main concepts:

- Complete distributed key-value tuple service
- RPC-based communication
- Client-server architecture
- Remote service procedures
- Data serialization
- Server-side validation
- Automated testing
- Robust error management

## Main Features

- Tuple creation
- Tuple modification
- Tuple retrieval
- Tuple deletion
- Existence checking
- Service reset
- Client-side public API
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

The repository structure may vary depending on the selected branch, since each branch contains a different stage of the project.

Typical files include:

- `claves.c` / `claves.h`: public API and tuple service logic.
- `proxy-*.c`: client-side proxy implementation.
- `servidor-*.c`: server implementation.
- `protocol_*.h`: communication protocol definitions.
- `Makefile`: compilation rules.
- `run_tests.sh`: automated testing script.
- `PLAN_PRUEBAS.md`: testing plan and validation cases.

## How to Explore the Project

To review the different implementations, switch between branches:

```bash
git checkout main
git checkout ejercicio-2
git checkout ejercicio-3
git checkout practica-final
```

Each branch contains a different version of the distributed service and shows the progression from basic inter-process communication to more advanced distributed communication mechanisms.

What I Learned

This project helped me understand how distributed systems evolve from a local API to real client-server architectures.

I worked with different communication mechanisms such as POSIX message queues, TCP sockets and RPC, while improving my knowledge of protocol design, error handling, concurrency, automated testing, Linux development and low-level programming in C.

Academic Context

This project was developed as part of the Distributed Systems course at Universidad Carlos III de Madrid.

The main objective was to implement and progressively improve a distributed service while applying theoretical and practical concepts related to communication, synchronization, client-server architectures and distributed programming.
