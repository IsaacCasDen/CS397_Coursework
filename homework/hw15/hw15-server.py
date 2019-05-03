import socket
import threading
import signal
import sys

class SocketConnection(object):

    def __init__(self, conn:socket.socket, addr):
        self.__connopen__=False
        self.conn = conn
        self.addr = addr
        self.thread = threading.Thread(target=self.run,args=())
        self.thread.setDaemon(True)
        self.conn
    
    def run(self):
        print('connected to:', self.addr)
        while self.__connopen__:
            data=self.conn.recv(1024)
            if not data:
                self.__connopen__=False
                break
            if self.__connopen__:
                val = data.decode()
                val= str(val).upper()
                print(self.addr, "sent:",data.decode())
                self.conn.send(val.encode())
        
        print(self.addr, 'disconnected')
    
    def open(self):
        self.__connopen__ = True
        self.thread.start()

    def close(self, forceClose = False):
        self.__connopen__ = False
        self.conn.close()
        if not forceClose:
            self.thread.join()

class PythonServer(object):

    def __init__(self, host='127.0.0.1', port=54321):
        self.running = False
        signal.signal(signal.SIGINT, self.handle_signal)
        signal.signal(signal.SIGQUIT, self.handle_signal)
        self.__host__=host
        self.__port__=port
        self.socket=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # https://stackoverflow.com/questions/6380057/python-binding-socket-address-already-in-use
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind((self.__host__,self.__port__))
        self.connections = []
        print("Created Server")
    
    def startServer(self):
        try:
            print("Server Starting","Host:",self.__host__,"Port:",self.__port__)
            self.__running__ = True
            self.socket.listen()
            while self.__running__:
                conn, addr = self.socket.accept()
                c = SocketConnection(conn,addr)
                self.connections.append(c)
                c.open()
        except Exception as err:
            print(err)
        
        self.stopServer()
        
    def stopServer(self, forceQuit=False):
        self.__running__=False
        if forceQuit:
            print("Server Shutting Down Immediately")
            self.socket.close()
            exit()
        else:
            for conn in self.connections:
                try:
                    conn.close()
                except Exception as err:
                    print(err)
            self.socket.close()
            print("Server Shutting Down")

    def handle_signal(self, signum, event):
        if signum==signal.SIGINT:
            self.stopServer()
        elif signum==signal.SIGQUIT:
            self.stopServer(True)

if __name__ == '__main__':
    argc = len(sys.argv)

    if argc==1:
        p = PythonServer()
    elif argc==2:
        p = PythonServer(port=int(sys.argv[1]))
    elif argc>2:
        p = PythonServer(host=sys.argv[1],port=int(sys.argv[2]))
    
    p.startServer()