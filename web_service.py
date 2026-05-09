import argparse
import json
from http.server import BaseHTTPRequestHandler, HTTPServer


class NormalizeHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        if self.path != "/normalize":
            self.send_error(404)
            return

        try:
            length = int(self.headers.get("Content-Length", "0"))
            payload = json.loads(self.rfile.read(length).decode("utf-8"))
            message = payload.get("message", "")
            normalized = " ".join(message.split())
            response = json.dumps({"message": normalized}).encode("utf-8")
        except (ValueError, json.JSONDecodeError):
            self.send_error(400)
            return

        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(response)))
        self.end_headers()
        self.wfile.write(response)

    def log_message(self, format, *args):
        return


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", type=int, default=8080, help="Web service port")
    args = parser.parse_args()

    server = HTTPServer(("127.0.0.1", args.p), NormalizeHandler)
    print("web> init normalizer 127.0.0.1:" + str(args.p), flush=True)
    server.serve_forever()


if __name__ == "__main__":
    main()
