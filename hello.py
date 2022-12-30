from socket import AF_INET, SOCK_STREAM, socket
server_port = 8000;
server_ip = 'localhost'
client_socket = socket(AF_INET, SOCK_STREAM)
client_socket.connect((server_ip, server_port))
client_socket.send('GET codamx.nl versieVanHTTP'.encode())
data = client_socket.recv(1024)
print('Van server: ;', data.decode())
client_socket.close()
