import React, { useState } from 'react';
import { Search, Compass, Activity, PlaySquare, Settings, UserCircle } from 'lucide-react';
import { useNavigate } from 'react-router-dom';
import ProfileOverlay from './ProfileOverlay';
import AssetSearchOverlay from './AssetSearchOverlay';

const TopNavigation = () => {
    const navigate = useNavigate();
    const [isSearchOpen, setIsSearchOpen] = useState(false);
    const [isProfileOpen, setIsProfileOpen] = useState(false);

    return (
        <nav className="h-16 px-6 flex items-center justify-between border-b border-binance-border bg-[#0b0e11] sticky top-10 z-30">
            {/* Left Nav */}
            <div className="flex items-center gap-8">
                {/* Logo */}
                <div className="text-2xl font-black text-transparent bg-clip-text bg-gradient-to-r from-binance-green to-binance-blue flex items-center gap-2">
                    AlphaQuant <span className="text-xs bg-binance-blue bg-opacity-20 text-binance-blue px-2 py-0.5 rounded uppercase tracking-widest border border-binance-blue">Pro</span>
                </div>

                {/* Primary Links */}
                <div className="hidden lg:flex items-center gap-6 font-medium text-binance-text">
                    <a href="#" onClick={(e) => { e.preventDefault(); navigate('/quant-models'); }} className="hover:text-white transition-colors">Products</a>
                    <a href="#" onClick={(e) => { e.preventDefault(); navigate('/'); }} className="hover:text-white transition-colors">Community</a>
                    <a href="#" onClick={(e) => { e.preventDefault(); navigate('/markets'); }} className="flex items-center gap-1 hover:text-white transition-colors text-white relative">
                        Markets
                        <div className="absolute -bottom-5 left-0 right-0 h-0.5 bg-binance-blue"></div>
                    </a>
                    <a href="#" onClick={(e) => { e.preventDefault(); navigate('/quant-models'); }} className="hover:text-white transition-colors">News</a>
                    <a href="#" onClick={(e) => { e.preventDefault(); } } className="hover:text-white transition-colors flex items-center gap-1"><PlaySquare size={16}/> Brokers</a>
                </div>
            </div>

            {/* Right Nav / Actions */}
            <div className="flex items-center gap-4">
                {/* Search Bar */}
                <div className="relative group hidden md:block">
                    <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 text-binance-muted group-hover:text-white transition-colors" size={18} />
                    <input 
                        type="text" 
                        onFocus={() => setIsSearchOpen(true)}
                        placeholder="Search markets, assets, users..." 
                        className="pl-10 pr-4 py-2 bg-binance-panel border border-binance-border rounded-full text-sm font-medium w-64 focus:outline-none focus:border-binance-blue hover:border-binance-muted transition-colors text-white"
                    />
                    <div className="absolute right-3 top-1/2 transform -translate-y-1/2 text-xs text-binance-muted bg-[#0b0e11] px-1.5 rounded border border-binance-border">/</div>
                </div>

                <div className="w-px h-6 bg-binance-border mx-2"></div>

                <button onClick={() => setIsProfileOpen(true)} className="text-binance-muted hover:text-white transition-colors hover:scale-110"><Settings size={20} /></button>
                <button onClick={() => setIsProfileOpen(true)} className="text-binance-muted hover:text-white transition-colors hover:scale-110"><UserCircle size={24} /></button>
                
                <button onClick={() => navigate('/markets')} className="bg-binance-blue hover:bg-opacity-80 text-white px-4 py-2 rounded-full font-bold text-sm ml-2 transition-all hover:scale-105 active:scale-95 shadow-lg relative group overflow-hidden">
                    <span className="relative z-10">Get started</span>
                    <div className="absolute inset-0 bg-white/20 translate-y-full group-hover:translate-y-0 transition-transform"></div>
                </button>
            </div>
            
            <AssetSearchOverlay isOpen={isSearchOpen} onClose={() => setIsSearchOpen(false)} />
            <ProfileOverlay isOpen={isProfileOpen} onClose={() => setIsProfileOpen(false)} />
        </nav>
    );
};

export default TopNavigation;
