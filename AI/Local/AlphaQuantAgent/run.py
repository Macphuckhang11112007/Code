import os
import sys
import asyncio
import socket
import uvicorn

# Monkeypatch socketpair to ignore strict peer address validation
orig_socketpair = socket.socketpair
def proxied_socketpair(family=socket.AF_INET, type=socket.SOCK_STREAM, proto=0):
    lsock = socket.socket(family, type, proto)
    try:
        lsock.bind(('127.0.0.1', 0))
        lsock.listen(1)
        csock = socket.socket(family, type, proto)
        try:
            csock.setblocking(False)
            try:
                csock.connect(lsock.getsockname())
            except (BlockingIOError, InterruptedError):
                pass
            csock.setblocking(True)
            lsock.settimeout(2.0)
            ssock, addr = lsock.accept()
            # Just accept the connection, even if it's from a local AV proxy!
            return (ssock, csock)
        except Exception:
            csock.close()
            raise
    finally:
        lsock.close()

socket.socketpair = proxied_socketpair

if sys.platform == "win32":
    asyncio.set_event_loop_policy(asyncio.WindowsProactorEventLoopPolicy())



if __name__ == "__main__":
    # Bảo toàn ngữ cảnh thư mục gốc (CWD Anchor)
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    print("=========================================================")
    print("🚀 ALPHAQUANT V2: THE SINGULARITY ENGINE (FASTAPI CORE)")
    print("=========================================================")
    print("🌐 API Server: http://localhost:8000")
    print("🔌 WebSocket Market Tick: ws://localhost:8000/ws/market/tick")
    print("🔌 WebSocket RAG Chat: ws://localhost:8000/ws/chat/rag")
    print("=========================================================")
    
    # Run the Asynchronous ASGI Server
    # reload=True is disabled to prevent asyncio socketpair() ConnectionError on Windows
    uvicorn.run("api:app", host="0.0.0.0", port=8000, reload=False, loop="none")
