import React, { useState } from 'react';
import { useNavigate, useLocation } from 'react-router-dom';
import { LayoutDashboard, LineChart, PieChart, Binary, UserCircle, Search } from 'lucide-react';
import AssetSearchOverlay from './AssetSearchOverlay';
import ProfileOverlay from './ProfileOverlay';

const ConceptSidebar = () => {
  const [isSearchOpen, setIsSearchOpen] = useState(false);
  const [isProfileOpen, setIsProfileOpen] = useState(false);
  const [tooltip, setTooltip] = useState({ visible: false, label: '', top: 0 });
  const navigate = useNavigate();
  const location = useLocation();

  return (
    <>
    <aside className="sticky top-0 w-20 lg:w-24 h-screen bg-[#0f1420] border-r border-[#1e2532] flex flex-col items-center py-6 shrink-0 z-20">
      {/* Brand Logo - The Glowing Abstract Q - Now acts as Global Search */}
      <button 
          onClick={() => setIsSearchOpen(true)}
          className="mb-10 w-16 h-16 flex items-center justify-center relative group focus:outline-none focus:ring-0" 
          title="Search Markets"
      >
        {/* Glow effect on hover */}
        <div className="absolute inset-0 bg-concept-blue/0 group-hover:bg-concept-blue/20 transition-colors rounded-full blur-xl"></div>
        <svg viewBox="0 0 100 100" className="w-full h-full drop-shadow-[0_0_10px_rgba(255,255,255,0.2)] group-hover:scale-105 transition-transform duration-300">
          {/* Top-Right Cyan Arc */}
          <path d="M 50 15 A 35 35 0 0 1 80 32" fill="none" stroke="#22d3ee" strokeWidth="3" strokeLinecap="round" style={{ filter: 'drop-shadow(0 0 8px #22d3ee)' }} />
          {/* Bottom-Left Purple/Cyan Arc */}
          <path d="M 32 80 A 35 35 0 0 1 15 50" fill="none" stroke="#a855f7" strokeWidth="3" strokeLinecap="round" style={{ filter: 'drop-shadow(0 0 8px #a855f7)' }} />
          {/* Main White Ring of the Q */}
          <circle cx="50" cy="50" r="22" fill="none" stroke="white" strokeWidth="8" />
          {/* The Leg of the Q */}
          <line x1="62" y1="62" x2="82" y2="82" stroke="white" strokeWidth="10" strokeLinecap="round" />
        </svg>
      </button>

      {/* Nav Icons */}
      <nav className="flex-1 flex flex-col items-center gap-8 w-full">
         <SidebarItem onClick={() => navigate('/')} icon={<LayoutDashboard size={22} strokeWidth={2} />} label="Dashboard" active={location.pathname === '/'} setTooltip={setTooltip} />
         <SidebarItem onClick={() => navigate('/markets')} icon={<LineChart size={22} strokeWidth={2} />} label="Markets" active={location.pathname === '/markets'} setTooltip={setTooltip} />
         <SidebarItem onClick={() => navigate('/quant-models')} icon={<PieChart size={22} strokeWidth={2} />} label="Analytics" active={location.pathname === '/quant-models'} setTooltip={setTooltip} />
      </nav>

      {/* Bottom Profile */}
      <div className="mt-auto pb-4">
         <SidebarItem onClick={() => setIsProfileOpen(true)} icon={<UserCircle size={24} strokeWidth={1.5} />} label="Profile" setTooltip={setTooltip} />
      </div>

      {/* Global Search Modal */}
      <AssetSearchOverlay isOpen={isSearchOpen} onClose={() => setIsSearchOpen(false)} />
      
      {/* Profile / Settings Modal */}
      <ProfileOverlay isOpen={isProfileOpen} onClose={() => setIsProfileOpen(false)} />
    </aside>

    {/* OUT-OF-FLOW FLOATING TOOLTIP */}
    {tooltip.visible && (
        <div 
            className="fixed z-[9999] left-[85px] lg:left-[100px] bg-[#1f2937] px-3 py-1.5 rounded-lg border border-[#374151] shadow-[0_10px_30px_rgba(0,0,0,0.8)] pointer-events-none animate-in fade-in zoom-in-95 duration-200"
            style={{ top: tooltip.top }}
        >
            <span className="text-[11px] font-bold text-white tracking-widest whitespace-nowrap">{tooltip.label}</span>
        </div>
    )}
    </>
  );
};

const SidebarItem = ({ icon, label, active = false, onClick, setTooltip }) => {
    return (
        <div 
            onClick={onClick} 
            onMouseEnter={(e) => {
                const rect = e.currentTarget.getBoundingClientRect();
                setTooltip({ visible: true, label, top: rect.top + rect.height/2 - 15 }); // Centered vertically
            }}
            onMouseLeave={() => setTooltip({ visible: false, label: '', top: 0 })}
            className={`flex flex-col items-center gap-1.5 cursor-pointer w-full group transition-transform duration-300 hover:scale-105 active:scale-95 ${active ? 'text-concept-blue' : 'text-concept-muted hover:text-white'}`}
        >
            <div className={`relative p-3 rounded-xl transition-all duration-300 ${active ? 'bg-concept-blue/20 text-concept-blue shadow-[0_0_10px_rgba(59,130,246,0.4)]' : 'group-hover:bg-white/5 text-concept-muted group-hover:text-white'}`}>
                {icon}
            </div>
            {/* The text label under the icon */}
            <span className={`text-[10px] font-medium tracking-wide mt-1 transition-colors ${active ? 'text-concept-blue' : 'text-concept-muted group-hover:text-white'}`}>
                {label.split(' ')[0]} {/* Shorthand for side bar */}
            </span>
        </div>
    )
}

export default ConceptSidebar;
