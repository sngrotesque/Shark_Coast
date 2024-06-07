import wtools
import socket

def listen(addr :str, port :int, path :str):
    listen_fd = socket.socket()
    listen_fd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listen_fd.bind((addr, port))
    listen_fd.listen(5)
    
    print(f'waiting client connected...')
    client_fd, client_addr = listen_fd.accept()
    
    print(f'client connected: [{client_addr[0]}:{client_addr[1]}]')
    pkt = wtools.packet()

    file_data = pkt.recv(client_fd)

    pkt.send(client_fd, b'done.')

    with open(path, 'wb') as f:
        f.write(file_data)

    client_fd.close()
    listen_fd.close()

if __name__ == '__main__':
    listen('0.0.0.0', 9981, 'test_file')
