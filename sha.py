from flask import Flask, jsonify
import json
import socket
app = Flask(__name__)

def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('8.8.8.8', 1))
        ip = s.getsockname()[0]
    except Exception:
        ip = '127.0.0.1'
    finally:
        s.close()
    return ip


@app.route("/")
def stats():
    with open("monitor.json", "r") as f:
        data = json.load(f)
    return jsonify(data)

print(f"Connect to: http://{get_ip()}:8000")

if __name__ == "__main__":
    
    app.run(host='0.0.0.0', port=8000, debug=False, use_reloader=False )
    


