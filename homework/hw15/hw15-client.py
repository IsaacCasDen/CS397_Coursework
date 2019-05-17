import socket
import threading
import signal
import sys

class PythonClient(object):

    def __init__(self, host='127.0.0.1', port=54321):
        self.__host__=host
        self.__port__=port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def is_connected(self):
        return False

    def connect(self):
        self.socket.connect((self.__host__,self.__port__))
    
    def disconnect(self):
        self.socket.close()
    
    def send(self,bytes):
        self.socket.send(bytes)
        self.response = self.socket.recv(1024)
        return self.response

if __name__ == '__main__':
    argc = len(sys.argv)

    if argc==1:
        p = PythonClient()
    elif argc==2:
        p = PythonClient(port=int(sys.argv[1]))
    elif argc>2:
        p = PythonClient(host=sys.argv[1],port=int(sys.argv[2]))

    p.connect()

    try:
        print("SEND>\t",end='',flush=True)
        for line in sys.stdin:
            response = p.send(line.encode())
            print("RECV>\t" + response.decode().strip())
            print("SEND>",end='\t',flush=True)
            

    except Exception as err:
        print(err)

    p.disconnect()

    # p.startServer()

    # try:
    #     s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #     s.connect(('127.0.0.1',54321))
    #     s.send('hello socket!'.encode())
    #     data = s.recv(1024)
    #     print(data.decode())