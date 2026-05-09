import argparse
import json
import os
import socket
import sys
import threading
import urllib.request
from enum import Enum

class client :

    # ******************** TYPES *********************
    # *
    # * @brief Return codes for the protocol methods
    class RC(Enum) :
        OK = 0
        ERROR = 1
        USER_ERROR = 2

    # ****************** ATTRIBUTES ******************
    _server = None
    _port = -1
    _connected_user = None
    _listen_socket = None
    _listen_port = None
    _listener_thread = None
    _listener_stop = threading.Event()
    _known_users = {}
    _web_service_url = os.environ.get("WEB_SERVICE_URL", "http://127.0.0.1:8080/normalize")

    @staticmethod
    def _send_string(sock, value) :
        data = value.encode("utf-8")
        sock.sendall(data + b"\0")

    @staticmethod
    def _recv_string(sock) :
        data = bytearray()
        while True:
            chunk = sock.recv(1)
            if (not chunk):
                raise ConnectionError("Connection closed")
            if (chunk == b"\0"):
                return data.decode("utf-8")
            data.extend(chunk)

    @staticmethod
    def _recv_code(sock) :
        data = sock.recv(1)
        if (len(data) != 1):
            raise ConnectionError("Connection closed")
        return data[0]

    @staticmethod
    def _recv_exact(sock, size) :
        data = bytearray()
        while (len(data) < size):
            chunk = sock.recv(size - len(data))
            if (not chunk):
                raise ConnectionError("Connection closed")
            data.extend(chunk)
        return bytes(data)

    @staticmethod
    def _open_server_connection() :
        return socket.create_connection((client._server, client._port), timeout=5)

    @staticmethod
    def _normalize_message(message) :
        payload = json.dumps({"message": message}).encode("utf-8")
        request = urllib.request.Request(
            client._web_service_url,
            data=payload,
            headers={"Content-Type": "application/json"},
            method="POST"
        )

        with urllib.request.urlopen(request, timeout=2) as response:
            data = json.loads(response.read().decode("utf-8"))
            return data["message"]

    @staticmethod
    def _create_listener_socket() :
        listen_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        listen_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        listen_sock.bind(("", 0))
        listen_sock.listen()
        listen_sock.settimeout(1.0)
        return listen_sock, listen_sock.getsockname()[1]

    @staticmethod
    def _handle_server_message(conn) :
        operation = client._recv_string(conn)

        if (operation == "SEND MESSAGE"):
            sender = client._recv_string(conn)
            message_id = client._recv_string(conn)
            message = client._recv_string(conn)
            print("s> MESSAGE " + message_id + " FROM " + sender)
            print(message)
            print("END")
        elif (operation == "SEND MESSAGE ATTACH"):
            sender = client._recv_string(conn)
            message_id = client._recv_string(conn)
            message = client._recv_string(conn)
            filename = client._recv_string(conn)
            print("c> MESSAGE " + message_id + " FROM " + sender)
            print(message)
            print("END")
            print("FILE " + filename)
        elif (operation == "SEND MESS ACK"):
            message_id = client._recv_string(conn)
            print("c> SEND MESSAGE " + message_id + " OK")
        elif (operation == "SEND MESS ATTACH ACK"):
            message_id = client._recv_string(conn)
            filename = client._recv_string(conn)
            print("c> SENDATTACH MESSAGE " + message_id + " " + filename + " OK")
        elif (operation == "GET FILE"):
            _ = client._recv_string(conn)
            filename = client._recv_string(conn)
            try:
                size = os.path.getsize(filename)
                client._send_string(conn, "OK")
                client._send_string(conn, str(size))
                with open(filename, "rb") as source:
                    while True:
                        chunk = source.read(4096)
                        if (not chunk):
                            break
                        conn.sendall(chunk)
            except OSError:
                client._send_string(conn, "ERROR")

    @staticmethod
    def _listener_loop(listen_sock) :
        while (not client._listener_stop.is_set()):
            try:
                conn, _ = listen_sock.accept()
            except socket.timeout:
                continue
            except OSError:
                break

            with conn:
                try:
                    client._handle_server_message(conn)
                except Exception:
                    pass

    @staticmethod
    def _start_listener() :
        listen_sock, port = client._create_listener_socket()
        client._listener_stop.clear()
        client._listen_socket = listen_sock
        client._listen_port = port
        client._listener_thread = threading.Thread(
            target=client._listener_loop,
            args=(listen_sock,),
            daemon=True
        )
        client._listener_thread.start()
        return port

    @staticmethod
    def _stop_listener() :
        client._listener_stop.set()

        if (client._listen_socket is not None):
            try:
                client._listen_socket.close()
            except OSError:
                pass
            client._listen_socket = None

        if (client._listener_thread is not None):
            client._listener_thread.join(timeout=2)
            client._listener_thread = None

        client._listen_port = None

    # ******************** METHODS *******************
    # *
    # * @param user - User name to register in the system
    # * 
    # * @return OK if successful
    # * @return USER_ERROR if the user is already registered
    # * @return ERROR if another error occurred
    @staticmethod
    def  register(user) :
        try:
            with client._open_server_connection() as sock:
                client._send_string(sock, "REGISTER")
                client._send_string(sock, user)
                code = client._recv_code(sock)

            if (code == 0):
                print("c> REGISTER OK")
                return client.RC.OK
            if (code == 1):
                print("c> USERNAME IN USE")
                return client.RC.USER_ERROR
        except Exception:
            pass

        print("c> REGISTER FAIL")
        return client.RC.ERROR

    # *
    # 	 * @param user - User name to unregister from the system
    # 	 * 
    # 	 * @return OK if successful
    # 	 * @return USER_ERROR if the user does not exist
    # 	 * @return ERROR if another error occurred
    @staticmethod
    def  unregister(user) :
        try:
            with client._open_server_connection() as sock:
                client._send_string(sock, "UNREGISTER")
                client._send_string(sock, user)
                code = client._recv_code(sock)

            if (code == 0):
                if (client._connected_user == user):
                    client._stop_listener()
                    client._connected_user = None
                print("c> UNREGISTER OK")
                return client.RC.OK
            if (code == 1):
                print("c> USER DOES NOT EXIST")
                return client.RC.USER_ERROR
        except Exception:
            pass

        print("c> UNREGISTER FAIL")
        return client.RC.ERROR


    # *
    # * @param user - User name to connect to the system
    # * 
    # * @return OK if successful
    # * @return USER_ERROR if the user does not exist or if it is already connected
    # * @return ERROR if another error occurred
    @staticmethod
    def  connect(user) :
        if (client._connected_user is not None):
            print("c> USER ALREADY CONNECTED")
            return client.RC.USER_ERROR

        try:
            listen_port = client._start_listener()
            with client._open_server_connection() as sock:
                client._send_string(sock, "CONNECT")
                client._send_string(sock, user)
                client._send_string(sock, str(listen_port))
                code = client._recv_code(sock)

            if (code == 0):
                client._connected_user = user
                print("c> CONNECT OK")
                return client.RC.OK

            client._stop_listener()
            if (code == 1):
                print("c> CONNECT FAIL, USER DOES NOT EXIST")
                return client.RC.USER_ERROR
            if (code == 2):
                print("c> USER ALREADY CONNECTED")
                return client.RC.USER_ERROR
        except Exception:
            client._stop_listener()

        print("c> CONNECT FAIL")
        return client.RC.ERROR

    # *
    # * 
    # * @return OK if successful
    # * @return USER_ERROR if the user does not exist or if it is already connected
    # * @return ERROR if another error occurred
    @staticmethod
    def  users() :
        if (client._connected_user is None):
            print("c> CONNECTED USERS FAIL, USER IS NOT CONNECTED")
            return client.RC.USER_ERROR

        try:
            with client._open_server_connection() as sock:
                client._send_string(sock, "USERS")
                client._send_string(sock, client._connected_user)
                code = client._recv_code(sock)
                if (code == 0):
                    count_text = client._recv_string(sock)
                    count = int(count_text)
                    names = []
                    for _ in range(count):
                        entry = client._recv_string(sock)
                        names.append(entry)
                        parts = entry.split(" :: ")
                        if (len(parts) == 3):
                            try:
                                client._known_users[parts[0]] = (parts[1], int(parts[2]))
                            except ValueError:
                                pass

                    print("c> CONNECTED USERS (" + str(count) + " users connected) OK")
                    for name in names:
                        print(name)
                    return client.RC.OK

            if (code == 1):
                print("c> CONNECTED USERS FAIL, USER IS NOT CONNECTED")
                return client.RC.USER_ERROR
        except Exception:
            pass

        print("c> CONNECTED USERS FAIL")
        return client.RC.ERROR



    # *
    # * @param user - User name to disconnect from the system
    # * 
    # * @return OK if successful
    # * @return USER_ERROR if the user does not exist
    # * @return ERROR if another error occurred
    @staticmethod
    def  disconnect(user) :
        result = client.RC.ERROR
        printed = False

        try:
            with client._open_server_connection() as sock:
                client._send_string(sock, "DISCONNECT")
                client._send_string(sock, user)
                code = client._recv_code(sock)

            if (code == 0):
                print("c> DISCONNECT OK")
                printed = True
                result = client.RC.OK
            elif (code == 1):
                print("c> DISCONNECT FAIL, USER DOES NOT EXIST")
                printed = True
                result = client.RC.USER_ERROR
            elif (code == 2):
                print("c> DISCONNECT FAIL, USER NOT CONNECTED")
                printed = True
                result = client.RC.USER_ERROR
        except Exception:
            pass

        if (client._connected_user == user):
            client._stop_listener()
            client._connected_user = None

        if (not printed):
            print("c> DISCONNECT FAIL")

        return result

    # *
    # * @param user    - Receiver user name
    # * @param message - Message to be sent
    # * 
    # * @return OK if the server had successfully delivered the message
    # * @return USER_ERROR if the user is not connected (the message is queued for delivery)
    # * @return ERROR the user does not exist or another error occurred
    @staticmethod
    def  send(user,  message) :
        if (client._connected_user is None):
            print("c> SEND FAIL")
            return client.RC.ERROR

        try:
            message = client._normalize_message(message)
        except Exception:
            print("c> SEND FAIL")
            return client.RC.ERROR

        if (len(message.encode("utf-8")) > 255):
            print("c> SEND FAIL")
            return client.RC.ERROR

        try:
            with client._open_server_connection() as sock:
                client._send_string(sock, "SEND")
                client._send_string(sock, client._connected_user)
                client._send_string(sock, user)
                client._send_string(sock, message)
                code = client._recv_code(sock)
                if (code == 0):
                    message_id = client._recv_string(sock)
                    print("c> SEND OK - MESSAGE " + message_id)
                    return client.RC.OK

            if (code == 1):
                print("c> SEND FAIL, USER DOES NOT EXIST")
                return client.RC.USER_ERROR
        except Exception:
            pass

        print("c> SEND FAIL")
        return client.RC.ERROR

    # *
    # * @param user    - Receiver user name
    # * @param file    - file  to be sent
    # * @param message - Message to be sent
    # * 
    # * @return OK if the server had successfully delivered the message
    # * @return USER_ERROR if the user is not connected (the message is queued for delivery)
    # * @return ERROR the user does not exist or another error occurred
    @staticmethod
    def  sendAttach(user,  file,  message) :
        if (client._connected_user is None):
            print("c> SENDATTACH FAIL")
            return client.RC.ERROR

        try:
            message = client._normalize_message(message)
        except Exception:
            print("c> SENDATTACH FAIL")
            return client.RC.ERROR

        if (len(message.encode("utf-8")) > 255 or len(file.encode("utf-8")) > 255):
            print("c> SENDATTACH FAIL")
            return client.RC.ERROR

        try:
            with client._open_server_connection() as sock:
                client._send_string(sock, "SENDATTACH")
                client._send_string(sock, client._connected_user)
                client._send_string(sock, user)
                client._send_string(sock, message)
                client._send_string(sock, file)
                code = client._recv_code(sock)
                if (code == 0):
                    message_id = client._recv_string(sock)
                    print("c> SENDATTACH OK - MESSAGE " + message_id)
                    return client.RC.OK

            if (code == 1):
                print("c> SENDATTACH FAIL, USER DOES NOT EXIST")
                return client.RC.USER_ERROR
        except Exception:
            pass

        print("c> SENDATTACH FAIL")
        return client.RC.ERROR

    @staticmethod
    def getFile(user, file, localFile) :
        if (client._connected_user is None):
            print("c> FILE TRANSFER FAILED, user not connected.")
            return client.RC.ERROR

        if (user not in client._known_users):
            client.users()

        if (user not in client._known_users):
            print("c> FILE TRANSFER FAILED, user not connected.")
            return client.RC.USER_ERROR

        ip, port = client._known_users[user]

        try:
            with socket.create_connection((ip, port), timeout=5) as sock:
                client._send_string(sock, "GET FILE")
                client._send_string(sock, client._connected_user)
                client._send_string(sock, file)
                status = client._recv_string(sock)
                if (status != "OK"):
                    print("c> FILE TRANSFER FAILED")
                    return client.RC.ERROR

                size = int(client._recv_string(sock))
                data = client._recv_exact(sock, size)
                with open(localFile, "wb") as target:
                    target.write(data)

            print("c> FILE TRANSFER OK")
            return client.RC.OK
        except Exception:
            print("c> FILE TRANSFER FAILED")
            return client.RC.ERROR

    # *
    # **
    # * @brief Command interpreter for the client. It calls the protocol functions.
    @staticmethod
    def shell():

        while (True) :
            try :
                command = input("c> ")
                line = command.split(" ")
                if (len(line) > 0):

                    line[0] = line[0].upper()

                    if (line[0]=="REGISTER") :
                        if (len(line) == 2) :
                            client.register(line[1])
                        else :
                            print("Syntax error. Usage: REGISTER <userName>")

                    elif(line[0]=="UNREGISTER") :
                        if (len(line) == 2) :
                            client.unregister(line[1])
                        else :
                            print("Syntax error. Usage: UNREGISTER <userName>")

                    elif(line[0]=="CONNECT") :
                        if (len(line) == 2) :
                            client.connect(line[1])
                        else :
                            print("Syntax error. Usage: CONNECT <userName>")

                    elif(line[0]=="DISCONNECT") :
                        if (len(line) == 2) :
                            client.disconnect(line[1])
                        else :
                            print("Syntax error. Usage: DISCONNECT <userName>")

                    elif(line[0]=="USERS") :
                        if (len(line) == 1) :
                            client.users()
                        else :
                            print("Syntax error. Usage: CONNECTED_USERS <userName>")

                    elif(line[0]=="SEND") :
                        if (len(line) >= 3) :
                            #  Remove first two words
                            message = ' '.join(line[2:])
                            client.send(line[1], message)
                        else :
                            print("Syntax error. Usage: SEND <userName> <message>")

                    elif(line[0]=="SENDATTACH") :
                        if (len(line) >= 4) :
                            #  Parte 2 syntax: SENDATTACH <userName> <message> <filename>
                            message = ' '.join(line[2:-1])
                            client.sendAttach(line[1], line[-1], message)
                        else :
                            print("Syntax error. Usage: SENDATTACH <userName> <message> <filename>")

                    elif(line[0]=="GETFILE") :
                        if (len(line) == 4) :
                            client.getFile(line[1], line[2], line[3])
                        else :
                            print("Syntax error. Usage: GETFILE <userName> <fileName> <localFileName>")

                    elif(line[0]=="QUIT") :
                        if (len(line) == 1) :
                            if (client._connected_user is not None):
                                client.disconnect(client._connected_user)
                            break
                        else :
                            print("Syntax error. Use: QUIT")
                    else :
                        print("Error: command " + line[0] + " not valid.")
            except Exception as e:
                print("Exception: " + str(e))

    # *
    # * @brief Prints program usage
    @staticmethod
    def usage() :
        print("Usage: python3 client.py -s <server> -p <port>")


    # *
    # * @brief Parses program execution arguments
    @staticmethod
    def  parseArguments(argv) :
        parser = argparse.ArgumentParser()
        parser.add_argument('-s', type=str, required=True, help='Server IP')
        parser.add_argument('-p', type=int, required=True, help='Server Port')
        args = parser.parse_args()

        if (args.s is None):
            parser.error("Usage: python3 client.py -s <server> -p <port>")
            return False

        if ((args.p < 1024) or (args.p > 65535)):
            parser.error("Error: Port must be in the range 1024 <= port <= 65535");
            return False;
        
        client._server = args.s
        client._port = args.p

        return True


    # ******************** MAIN *********************
    @staticmethod
    def main(argv) :
        if (not client.parseArguments(argv)) :
            client.usage()
            return

        client.shell()
        print("+++ FINISHED +++")
    

if __name__=="__main__":
    client.main(sys.argv[1:])
