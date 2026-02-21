import React, { useState } from 'react';
import { MessageSquare, X, Send } from 'lucide-react';

// Khởi tạo component AlphaQuantAssistant (Trợ lý Định lượng)
const AlphaQuantAssistant = () => {
  // Trạng thái mở/đóng của khung chat
  const [isOpen, setIsOpen] = useState(false);
  
  // Trạng thái lưu trữ lịch sử tin nhắn của hệ thống
  const [messages, setMessages] = useState([
    { role: 'assistant', content: 'Kính chào Đại sư phụ. Tôi là Trí tuệ Định lượng AlphaQuant (The Singularity). Ngài cần tôi phân tích Toán học hay Truy vấn Dữ liệu Sinh tồn nào hôm nay?' } // Đã xóa chữ V2 để tránh khái niệm phiên bản
  ]);
  
  // Trạng thái lưu trữ giá trị ô nhập liệu của User
  const [input, setInput] = useState('');

  // Trạng thái hiển thị tiến trình loading
  const [isLoading, setIsLoading] = useState(false);

  // Tham chiếu (Ref) để điều khiển DOM auto-scroll
  const messagesEndRef = React.useRef(null);

  // Auto-scroll mỗi khi messages thay đổi
  React.useEffect(() => {
    messagesEndRef.current?.scrollIntoView({ behavior: "smooth" });
  }, [messages, isLoading]);

  // Hàm xử lý sự kiện gửi tin nhắn của User
  const handleSend = async () => {
    if(!input.trim()) return;
    
    const userMsg = input.trim();
    setInput('');
    
    // Cập nhật UI ngay lập tức với tin nhắn của User
    setMessages(prev => [...prev, {role: 'user', content: userMsg}]);
    setIsLoading(true);
    
    try {
        // Giao tiếp qua REST API chuẩn thay vì WebSockets
        const response = await fetch("http://localhost:8000/api/chat", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ message: userMsg, sessionId: "quant_user_1" })
        });
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        const data = await response.json();
        
        setMessages(prev => [...prev, { role: 'assistant', content: data.reply }]);
    } catch (sendError) {
        console.error("Lỗi trong lúc xử lý gửi tin nhắn:", sendError);
        setMessages(prev => [...prev, { role: 'assistant', content: "Xin lỗi, hệ thống máy chủ RAG đang gặp sự cố kết nối." }]);
    } finally {
        setIsLoading(false);
    }
  }

  return (
    <>
      {/* Floating Button */}
      {!isOpen && (
        <button 
          onClick={() => setIsOpen(true)}
          className="fixed bottom-6 right-6 w-14 h-14 bg-binance-blue text-white rounded-full flex items-center justify-center shadow-lg hover:bg-opacity-90 z-50 transition-all duration-300 transform hover:scale-105"
        >
          <MessageSquare size={28} />
        </button>
      )}

      {/* Chat Window */}
      <div className={`fixed bottom-6 right-6 w-80 h-[500px] bg-binance-panel border border-binance-border rounded-lg shadow-2xl flex flex-col z-50 transform transition-all duration-300 ${isOpen ? 'translate-y-0 opacity-100' : 'translate-y-10 opacity-0 pointer-events-none'}`}>
        
        {/* Header */}
        <div className="flex justify-between items-center p-3 border-b border-binance-border bg-[#0b0e11] rounded-t-lg">
          <div className="font-bold text-white flex items-center gap-2">
             <MessageSquare size={16} className="text-binance-blue" /> AI Quant
          </div>
          <button onClick={() => setIsOpen(false)} className="text-binance-muted hover:text-white">
            <X size={18} />
          </button>
        </div>

        {/* Messages */}
        <div className="flex-1 overflow-y-auto p-4 space-y-4">
          {messages.map((msg, i) => (
             <div key={i} className={`flex ${msg.role === 'user' ? 'justify-end' : 'justify-start'}`}>
                <div className={`max-w-[85%] rounded-lg p-3 text-sm ${msg.role === 'user' ? 'bg-binance-blue text-white rounded-br-none' : 'bg-binance-border text-binance-text rounded-bl-none'}`}>
                   {msg.content}
                </div>
             </div>
          ))}
          {isLoading && (
             <div className="flex justify-start">
                 <div className="text-xs text-binance-muted animate-pulse max-w-[85%] rounded-lg p-3 bg-binance-border text-binance-text rounded-bl-none">
                     AlphaQuant đang suy nghĩ...
                 </div>
             </div>
          )}
          <div ref={messagesEndRef} />
        </div>

        {/* Input */}
        <div className="p-3 border-t border-binance-border">
           <div className="relative">
              <input 
                 value={input}
                 onChange={(e) => setInput(e.target.value)}
                 onKeyDown={(e) => e.key === 'Enter' && handleSend()}
                 type="text" 
                 className="w-full bg-[#0b0e11] border border-binance-border text-white text-sm rounded-full pl-4 pr-10 py-2 focus:outline-none focus:border-binance-blue"
                 placeholder="Hỏi hệ thống..."
              />
              <button 
                 onClick={handleSend}
                 className="absolute right-2 top-1/2 transform -translate-y-1/2 text-binance-blue hover:text-white"
              >
                 <Send size={18} />
              </button>
           </div>
        </div>

      </div>
    </>
  );
};

export default AlphaQuantAssistant;
