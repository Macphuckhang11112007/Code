import React, { useState } from 'react';
import { UserCircle, Shield, Key, Bell, Wallet, LogOut, Settings as SettingsIcon } from 'lucide-react';

const ProfileOverlay = ({ isOpen, onClose }) => {
    const [subTab, setSubTab] = useState('Account');
    
    if (!isOpen) return null;

    return (
        <div className="fixed inset-0 z-[1000] flex items-center justify-center bg-black/60 backdrop-blur-sm animate-in fade-in duration-200">
            <div className="relative bg-[#111926] border border-[#2B3139] w-[800px] h-[500px] rounded-2xl shadow-2xl flex overflow-hidden animate-in zoom-in-95 duration-300">
                
                {/* Left Sidebar */}
                <div className="w-64 bg-[#0b0e11] border-r border-[#2B3139] p-6 flex flex-col gap-8">
                    <div className="flex items-center gap-3">
                        <div className="bg-concept-blue/20 p-2 rounded-full text-concept-blue">
                            <UserCircle size={32} />
                        </div>
                        <div>
                            <div className="text-white font-bold tracking-wide">Quan</div>
                            <div className="text-xs text-concept-green">Verified Level 2</div>
                        </div>
                    </div>

                    <nav className="flex flex-col gap-2">
                        {[
                            { id: 'Account', icon: <UserCircle size={18}/> },
                            { id: 'Security', icon: <Shield size={18}/> },
                            { id: 'API Keys', icon: <Key size={18}/> },
                            { id: 'Settings', icon: <SettingsIcon size={18}/> },
                            { id: 'Notifications', icon: <Bell size={18}/> },
                            { id: 'Billing', icon: <Wallet size={18}/> },
                        ].map(t => (
                            <button 
                                key={t.id} 
                                onClick={() => setSubTab(t.id)}
                                className={`flex items-center gap-3 px-4 py-2.5 rounded-lg text-sm font-semibold transition-all ${subTab === t.id ? 'bg-concept-blue/20 text-concept-blue' : 'text-concept-muted hover:bg-white/5 hover:text-white'}`}
                            >
                                {t.icon} {t.id}
                            </button>
                        ))}
                    </nav>

                    <div className="mt-auto">
                        <button onClick={onClose} className="w-full flex items-center justify-center gap-2 py-2 text-concept-red hover:bg-concept-red/10 rounded-lg transition-colors text-sm font-bold">
                            <LogOut size={16} /> Logout
                        </button>
                    </div>
                </div>

                {/* Right Content */}
                <div className="flex-1 bg-[#111926] p-8 flex flex-col relative">
                    <button onClick={onClose} className="absolute top-4 right-4 text-concept-muted hover:text-white p-2">✕</button>
                    
                    <h2 className="text-2xl font-bold text-white mb-6">{subTab}</h2>
                    
                    <div className="flex-1 flex flex-col gap-6 overflow-y-auto custom-scrollbar">
                        {subTab === 'Account' && (
                            <div className="space-y-6">
                                <div className="p-4 border border-[#2B3139] rounded-xl bg-[#0b0e11]">
                                    <h3 className="text-white font-semibold mb-1">User ID</h3>
                                    <p className="text-concept-muted text-sm font-mono tracking-widest">3950183945</p>
                                </div>
                                <div className="p-4 border border-[#2B3139] rounded-xl bg-[#0b0e11]">
                                    <h3 className="text-white font-semibold mb-1">Email</h3>
                                    <div className="flex items-center justify-between">
                                        <p className="text-concept-muted text-sm">q***@alphaquant.ai</p>
                                        <span className="bg-concept-green/20 text-concept-green px-2 py-1 rounded text-[10px] font-bold">Verified</span>
                                    </div>
                                </div>
                                <div className="p-4 border border-[#2B3139] rounded-xl bg-[#0b0e11]">
                                    <h3 className="text-white font-semibold mb-1">Phone Number</h3>
                                    <div className="flex items-center justify-between">
                                        <p className="text-concept-muted text-sm">+84 *** *** 68</p>
                                        <button className="text-concept-blue text-xs font-bold hover:underline">Change</button>
                                    </div>
                                </div>
                            </div>
                        )}

                        {subTab === 'API Keys' && (
                            <div className="space-y-4">
                                <p className="text-concept-muted text-sm mb-4">Manage your Google Gemini and Binance API keys securely.</p>
                                <div className="p-4 border border-[#2B3139] rounded-xl bg-[#0b0e11]">
                                    <div className="flex items-center justify-between mb-4">
                                        <h3 className="text-white font-semibold">Gemini LLM Key</h3>
                                        <span className="w-2 h-2 rounded-full bg-concept-green shadow-[0_0_8px_#10b981]"></span>
                                    </div>
                                    <input type="password" value="AIzaSy...v1qA" readOnly className="w-full bg-[#111926] border border-[#2B3139] text-concept-muted px-3 py-2 rounded-md font-mono text-sm focus:outline-none" />
                                </div>
                                <button className="w-full py-3 bg-concept-blue/20 text-concept-blue rounded-xl font-bold hover:bg-concept-blue hover:text-white transition-colors">
                                    + Generate New Key
                                </button>
                            </div>
                        )}

                        {subTab !== 'Account' && subTab !== 'API Keys' && (
                            <div className="flex-1 flex items-center justify-center text-concept-muted border-2 border-dashed border-[#2B3139] rounded-xl">
                                {subTab} panel is currently under construction.
                            </div>
                        )}
                    </div>
                </div>

            </div>
        </div>
    );
};
export default ProfileOverlay;
