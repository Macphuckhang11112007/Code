import socket
import time

def test_socketpair_workaround():
    orig_socketpair = socket.socketpair
    
    def retry_socketpair(*args, **kwargs):
        for i in range(100):
            try:
                return orig_socketpair(*args, **kwargs)
            except ConnectionError:
                time.sleep(0.01)
                pass
        raise ConnectionError("Failed to create socketpair after 100 retries")
        
    socket.socketpair = retry_socketpair
    
    # Try it
    s1, s2 = socket.socketpair()
    print("Success! Created:", s1, s2)
    s1.close()
    s2.close()

if __name__ == "__main__":
    test_socketpair_workaround()
