import socket

# 配置服务器IP和端口
LISTEN_IP = "192.168.1.100"  # 您电脑的IP地址 (CH32发送的目标IP)
LISTEN_PORT = 2000           # CH32发送的目标端口 (必须与CH32代码中的 udp_desport 一致)

# 1. 创建UDP socket对象
#    AF_INET 表示使用 IPv4 地址族
#    SOCK_DGRAM 表示使用 UDP 协议
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# 2. 绑定IP地址和端口号
#    服务器需要绑定到一个固定的IP和端口，以便客户端知道向哪里发送数据
server_address = (LISTEN_IP, LISTEN_PORT)
print(f"UDP server starting up on {LISTEN_IP} port {LISTEN_PORT}")
try:
    sock.bind(server_address)
except socket.error as e:
    print(f"Error binding to socket: {e}")
    print("Please ensure no other application is using this IP/Port and you have permissions.")
    exit()

print("Waiting to receive messages...")

try:
    # 3. 循环接收数据
    while True:
        # recvfrom() 会阻塞程序执行，直到接收到数据
        # 它返回两个值：data (接收到的字节数据) 和 address (发送方的IP和端口)
        data, address = sock.recvfrom(4096)  # 4096是缓冲区大小，可以根据需要调整

        print(f"\nReceived {len(data)} bytes from {address}")

        # 尝试将接收到的字节数据解码为UTF-8字符串
        # CH32发送的是格式化字符串，通常是UTF-8兼容的ASCII
        try:
            message = data.decode('utf-8')
            print(f"Data: {message}")
        except UnicodeDecodeError:
            print(f"Could not decode data as UTF-8. Raw data (hex): {data.hex()}")

except KeyboardInterrupt:
    print("\nUDP server is shutting down (KeyboardInterrupt).")
except Exception as e:
    print(f"\nAn error occurred: {e}")
finally:
    # 4. 关闭socket
    print("Closing socket.")
    sock.close()