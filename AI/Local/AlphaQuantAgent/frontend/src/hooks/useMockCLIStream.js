// Import necessary Hooks from the React core library (State and Lifecycle management)
import { useState, useEffect } from 'react';

// Custom Hook that connects to the Real-Time SSE Stream for AI Training Logs
export function useMockCLIStream() {
    // Array holding the strings of the streaming logs, initialized empty
    const [logs, setLogs] = useState([]);

    useEffect(() => {
        // Form a dedicated SSE connection link to the Backend Real-time pipe
        const eventSource = new EventSource('http://127.0.0.1:8000/api/stream/training-logs');

        // Listen and trigger callback whenever the server pushes a new event line
        eventSource.onmessage = (event) => {
            setLogs(prev => {
                // Slice to keep only the latest 99 logs + new log = 100 max, preserving DOM memory
                return [...prev.slice(-99), event.data];
            });
        };

        // Catch connection crashes or severances
        eventSource.onerror = (error) => {
            console.error("SSE CLI Stream Connection Error. Reconnecting...", error);
        };

        // Crucial Cleanup: Sever the EventSource wire if the user exits the Tab to prevent background CPU burns
        return () => {
            eventSource.close();
        };
    }, []);

    // Export the continuous Array array to feed the Terminal UI Component
    return logs;
}
