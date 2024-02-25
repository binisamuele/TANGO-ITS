import socket
import threading
import time

def send_message(udp_socket, broadcast_address, port):
    while True:
        message = "Hello, broadcast!"
        udp_socket.sendto(message.encode(), (broadcast_address, port))
        print(f"Sent message: '{message}'")
        time.sleep(2)

def receive_messages(udp_socket):
    while True:
        try:
            data, address = udp_socket.recvfrom(1024)
            print(f"Received from {address}: {data.decode('utf-8')}")
        except Exception as e:
            print(f"Error receiving data: {e}")

def main():
    # Create a UDP socket
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Set the broadcast address and port
    broadcast_address = '192.168.1.255'
    port = 8989

    # Enable broadcasting on the socket
    udp_socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

    # Bind the socket to a specific address and port to receive messages
    udp_socket.bind(("0.0.0.0", port))

    # Create and start the sending thread
    send_thread = threading.Thread(target=send_message, args=(udp_socket, broadcast_address, port), daemon=True)
    send_thread.start()

    # Start receiving messages in the main thread
    print("Listening for UDP datagrams on broadcast address and port 8787...")
    receive_messages(udp_socket)

    # Close the socket when done (this will not be reached in this example)
    udp_socket.close()

if __name__ == "__main__":
    main()
