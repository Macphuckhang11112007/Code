import React, { useState, useEffect, useRef } from 'react';
import axios from 'axios';
import { useNavigate } from 'react-router-dom';
import { Search, X, TrendingUp, TrendingDown } from 'lucide-react';

const AssetSearchOverlay = ({ isOpen, onClose }) => {
    const navigate = useNavigate();
    const [searchTerm, setSearchTerm] = useState('');
    const [assets, setAssets] = useState([]);
    const [isLoading, setIsLoading] = useState(false);
    const inputRef = useRef(null);

    // Focus input when opened
    useEffect(() => {
        if (isOpen && inputRef.current) {
            setTimeout(() => inputRef.current.focus(), 100);
        }
        if (!isOpen) {
            setSearchTerm('');
        }
    }, [isOpen]);

    // Fetch assets database (we use top_movers as a proxy for the asset directory for now)
    useEffect(() => {
        if (!isOpen) return;
        
        // Only fetch if we haven't already
        if (assets.length > 0) return;

        const fetchAssets = async () => {
            setIsLoading(true);
            try {
                const response = await axios.get('http://127.0.0.1:8000/api/v1/market/top_movers');
                if (response.data && response.data.status === 'success') {
                    setAssets(response.data.data);
                }
            } catch (error) {
                console.error("Error fetching assets for search", error);
            } finally {
                setIsLoading(false);
            }
        };
        fetchAssets();
    }, [isOpen, assets.length]);

    // Close on escape key
    useEffect(() => {
        const handleKeyDown = (e) => {
            if (e.key === 'Escape' && isOpen) {
                onClose();
            }
        };
        window.addEventListener('keydown', handleKeyDown);
        return () => window.removeEventListener('keydown', handleKeyDown);
    }, [isOpen, onClose]);

    if (!isOpen) return null;

    // Filter logic: Smart Lookahead Regex Algorithm
    const filteredAssets = (() => {
        const trimmedTerm = searchTerm.trim().toLowerCase();
        
        // Quy tắc 1 (Empty State): Trả về toàn bộ nếu chuỗi rỗng
        if (!trimmedTerm) return assets;

        // Quy tắc 2 (Thuật toán Regex Lookahead đa từ)
        const words = trimmedTerm.split(/\s+/);
        // Xây dựng chuỗi regex bắt buộc mọi từ phải xuất hiện, không phân biệt thứ tự
        // Ví dụ: "vcb 12m" => ^(?=.*vcb)(?=.*12m).*$
        const regexStr = "^" + words.map(w => `(?=.*${w})`).join("") + ".*$";
        const regex = new RegExp(regexStr, "i"); // "i" = ignore case

        return assets.filter(a => {
            const sym = a.sym || "";
            const name = a.name || "";
            // Ghép chung sym và name để test 1 lần thay vì test rời rạc
            const combinedTarget = `${sym} ${name}`;
            return regex.test(combinedTarget);
        });
    })();

    const handleSelect = (sym) => {
        navigate(`/${sym}`);
        onClose();
    };

    return (
        <div className="fixed inset-0 z-[9999] bg-[#0b0e11]/80 backdrop-blur-sm flex justify-center items-start pt-[15vh] px-4 animate-in fade-in duration-200">
            {/* Click outside to close (Background Overlay) */}
            <div className="absolute inset-0" onClick={onClose}></div>

            {/* Modal Body */}
            <div className="relative w-full max-w-2xl bg-[#111926] border border-[#2d3748] rounded-2xl shadow-2xl flex flex-col overflow-hidden animate-in zoom-in-95 duration-200">
                
                {/* Search Header */}
                <div className="flex items-center px-4 py-4 border-b border-[#2d3748] gap-3">
                    <Search className="text-concept-muted" size={24} />
                    <input 
                        ref={inputRef}
                        type="text" 
                        placeholder="Search markets (e.g. BTC, NVDA)..."
                        className="flex-1 bg-transparent text-white text-xl outline-none placeholder:text-concept-muted"
                        value={searchTerm}
                        onChange={(e) => setSearchTerm(e.target.value)}
                    />
                    <button onClick={onClose} className="p-2 text-concept-muted hover:text-white rounded-lg hover:bg-white/5 transition-colors">
                        <X size={20} />
                    </button>
                </div>

                {/* Results List */}
                <div className="max-h-[50vh] overflow-y-auto custom-scrollbar flex flex-col p-2">
                    {isLoading ? (
                        <div className="p-8 text-center text-concept-muted">Loading markets...</div>
                    ) : filteredAssets.length > 0 ? (
                        filteredAssets.map((asset, idx) => (
                            <div 
                                key={idx} 
                                onClick={() => handleSelect(asset.sym)}
                                className="flex items-center justify-between p-3 rounded-xl hover:bg-white/5 cursor-pointer transition-colors group"
                            >
                                <div className="flex items-center gap-4">
                                    <div className="w-10 h-10 rounded-full flex items-center justify-center font-bold border border-white/5 shadow-sm" style={{backgroundColor: asset.bg || '#1E222D', color: asset.color || '#fff'}}>
                                        {asset.sym.charAt(0)}
                                    </div>
                                    <div className="flex flex-col">
                                        <div className="font-bold text-white flex items-center gap-2">
                                            {asset.sym} 
                                            <span className="text-[10px] bg-[#1E222D] text-concept-muted px-1.5 py-0.5 rounded uppercase tracking-wide">Spot</span>
                                        </div>
                                        <div className="text-xs text-concept-muted">{asset.name || `${asset.sym} Market`}</div>
                                    </div>
                                </div>
                                <div className="flex flex-col items-end text-right">
                                    <div className="text-white font-mono">{asset.price} <span className="text-xs text-concept-muted ml-0.5">USD</span></div>
                                    <div className={`text-xs font-bold flex items-center gap-1 ${asset.isUp ? 'text-concept-green' : 'text-concept-red'}`}>
                                        {asset.isUp ? <TrendingUp size={12} /> : <TrendingDown size={12} />}
                                        {asset.change}
                                    </div>
                                </div>
                            </div>
                        ))
                    ) : (
                        <div className="p-8 text-center flex flex-col items-center gap-2">
                            <span className="text-concept-muted">No markets found matching "{searchTerm}"</span>
                        </div>
                    )}
                </div>

                {/* Footer hints */}
                <div className="bg-[#0b0e11] px-4 py-3 border-t border-[#2d3748] text-xs text-concept-muted flex justify-between items-center">
                    <span>Press <kbd className="bg-[#1E222D] border border-[#2d3748] px-1.5 py-0.5 rounded font-mono text-white">ESC</kbd> to close</span>
                    <span>AlphaQuant Asset Database</span>
                </div>
            </div>
        </div>
    );
};

export default AssetSearchOverlay;
