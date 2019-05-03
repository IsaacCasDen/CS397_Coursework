
import socket
import signal
from threading import Thread, ThreadError

def fx(signum, event):
    print('sucker')

signal.signal(signal.SIGINT, fx)
signal.pause()

def run(args):
    try:
        s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((args['host'], args['port']))
        s.listen()
        conn, addr = s.accept()

        print("connected to:",addr)
        data = conn.recv(1024)
        print(data.decode())
        conn.send('OK\n'.encode())
        conn.close()
    except Exception as err:
        print(err)


if __name__ == '__main__':
    try:
        t = Thread(target=run, args={'host':'127.0.0.1','port':54321})
        t.start()
    except Exception as err:
        print(err)