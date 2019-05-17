
import socket
from threading import Thread, ThreadError

def run(args):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((args['host'], args['port']))
    s.send('hello socket!\n'.encode())
    data=s.recv(1024)
    print(data.decode())
    s.close()


if __name__ == '__main__':
    t = Thread(target=run, args={'host':'127.0.0.1','port':54321})
    t.start()