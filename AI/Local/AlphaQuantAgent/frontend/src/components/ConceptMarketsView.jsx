import React, { useState, useEffect } from 'react';
import { ArrowDown, TrendingDown, TrendingUp, Minus } from 'lucide-react';
import axios from 'axios';

const ConceptMarketsView = ({ asset }) => {
    const [subTab, setSubTab] = useState('Overview');
    const [marketsData, setMarketsData] = useState([]);
    const subTabs = ['Overview', 'Performance', 'Technicals'];

    useEffect(() => {
        let isMounted = true;
        axios.get('http://127.0.0.1:8000/api/v1/market/top_movers')
            .then(res => {
                if(isMounted && res.data && res.data.status === 'success') {
                    // map the simple real static data to the extended shape required by the UI
                    const realData = res.data.data.map((item, index) => ({
                         id: index,
                         symbol: item.sym,
                         desc: 'Real Data Steam',
                         exchange: 'AlphaQuant DB',
                         price: item.price,
                         change: item.change,
                         vol24h: item.vol,
                         volChange: '-',
                         high: '-',
                         low: '-',
                         vol: item.vol,
                         techRating: item.isUp ? 'Buy' : 'Sell',
                         
                         // Fill out missing fields required by columns
                         perf1W: '-', perf1M: '-', perf3M: '-', perf6M: '-',
                         perfYTD: '-', perf1Y: '-', perf3Y: '-', perf10Y: '-',
                         perfAllTime: '-', volatility: '-',
                         
                         maRating: item.isUp ? 'Buy' : 'Sell', osRating: 'Neutral',
                         rsi: '-', mom: '-', ao: '-', cci: '-', stochK: '-', stochD: '-',
                         macdLevel: '-', macdSignal: '-'
                    }));
                    setMarketsData(realData);
                }
            })
            .catch(console.error);
        return () => { isMounted = false; };
    }, []);

    // Column Configs
    const colsOverview = [
        { label: 'Price', key: 'price' },
        { label: 'Change %', key: 'change', isPct: true },
        { label: 'Volume 24h', key: 'vol24h', isSort: true },
        { label: 'Vol Change % 24h', key: 'volChange', isPct: true },
        { label: 'High', key: 'high' },
        { label: 'Low', key: 'low' },
        { label: 'Volume', key: 'vol' },
        { label: 'Tech Rating', key: 'techRating', isRating: true },
    ];

    const colsPerformance = [
        { label: 'Change %', key: 'change', isPct: true },
        { label: 'Perf % 1W', key: 'perf1W', isPct: true },
        { label: 'Perf % 1M', key: 'perf1M', isPct: true },
        { label: 'Perf % 3M', key: 'perf3M', isPct: true },
        { label: 'Perf % 6M', key: 'perf6M', isPct: true },
        { label: 'Perf % YTD', key: 'perfYTD', isPct: true },
        { label: 'Perf % 1Y', key: 'perf1Y', isPct: true },
        { label: 'Perf % 3Y', key: 'perf3Y', isPct: true },
        { label: 'Perf % 10Y', key: 'perf10Y', isPct: true },
        { label: 'Perf % All Time', key: 'perfAllTime', isPct: true },
        { label: 'Volatility', key: 'volatility' },
    ];

    // Added specific multi-line headers for Technicals to match screenshot exactly
    const colsTechnicals = [
        { label: 'Tech Rating', key: 'techRating', isRating: true },
        { label: 'MA Rating', key: 'maRating', isRating: true },
        { label: 'Os Rating', key: 'osRating', isRating: true },
        { label: 'RSI(14)', key: 'rsi' },
        { label: 'Mom(10)', key: 'mom' },
        { label: 'AO', key: 'ao' },
        { label: 'CCI(20)', key: 'cci' },
        { label: 'Stoch(14,3,3)%K', key: 'stochK' },
        { label: 'Stoch(14,3,3)%D', key: 'stochD' },
        { label: 'MACD(12,26)Level', key: 'macdLevel' },
        { label: 'MACD(12,26)Signal', key: 'macdSignal' },
    ];

    const getActiveCols = () => {
        if (subTab === 'Overview') return colsOverview;
        if (subTab === 'Performance') return colsPerformance;
        return colsTechnicals;
    };

    const activeCols = getActiveCols();

    // Render Helpers
    const renderPct = (val) => {
        const num = parseFloat(val.replace('%', '').replace('+', ''));
        if (num > 0) return <span className="text-[#0ECB81]">{val}</span>;
        if (num < 0) return <span className="text-[#F6465D]">{val}</span>;
        return <span className="text-concept-muted">{val}</span>;
    };

    const renderRating = (val) => {
        const lower = val.toLowerCase();
        if (lower.includes('sell')) {
            return <span className="text-[#F6465D] inline-flex items-center gap-1"><TrendingDown className="w-3 h-3" /> {val}</span>;
        }
        if (lower.includes('buy')) {
            return <span className="text-[#0ECB81] inline-flex items-center gap-1"><TrendingUp className="w-3 h-3" /> {val}</span>;
        }
        return <span className="text-concept-muted inline-flex items-center gap-1"><Minus className="w-3 h-3" /> {val}</span>;
    };

    // Format multiline header if it uses () in the string mock (e.g., 'RSI(14)')
    const renderHeader = (label, isSort) => {
        const parts = label.split('(');
        if (parts.length > 1) {
             return (
                 <div className="flex flex-col items-end leading-tight text-[11px]">
                     <span>{parts[0]}</span>
                     <span>({parts[1]}</span>
                 </div>
             )
        }
        return (
            <div className="flex items-center justify-end gap-1 text-[11px]">
                {isSort && <ArrowDown className="w-3 h-3" />}
                {label}
            </div>
        );
    };

    // Exchange Logo Mock (Generates a random colored circle with the first letter)
    const ExchangeLogo = ({ name }) => {
        const char = name.charAt(0);
        // Deterministic color based on char code
        const hue = (char.charCodeAt(0) * 137) % 360; 
        return (
            <div 
                className="w-5 h-5 rounded-full flex items-center justify-center text-[10px] font-bold text-white shrink-0"
                style={{ backgroundColor: `hsl(${hue}, 70%, 50%)` }}
            >
                {char}
            </div>
        );
    };

    return (
        <div className="w-full flex-1 flex flex-col gap-6 pb-12 animate-in fade-in duration-300">
            {/* Header Text */}
            <div className="flex flex-col gap-2 max-w-4xl">
                <h2 className="text-2xl font-bold text-white tracking-wide">Exchanges</h2>
                <p className="text-sm text-concept-muted leading-relaxed">
                    Get {asset.name} rates across all available markets. Pairs in the table are sorted by trading volume, helping you spot the most active ones first. View key stats such as price, performance, and technical ratings based on various indicators.
                </p>
            </div>

            {/* Sub-Tabs (Overview, Performance, Technicals) */}
            <div className="flex gap-6 border-b border-[#2d3748] mt-2">
                {subTabs.map(tab => (
                    <button
                        key={tab}
                        onClick={() => setSubTab(tab)}
                        className={`pb-3 text-sm font-semibold transition-colors relative ${subTab === tab ? 'text-white' : 'text-concept-muted hover:text-white'}`}
                    >
                        {tab}
                        {subTab === tab && (
                            <div className="absolute bottom-0 left-0 w-full h-[2px] bg-[#2962FF]"></div>
                        )}
                    </button>
                ))}
            </div>

            {/* The Huge Table Container */}
            <div className="w-full overflow-x-auto overflow-y-hidden border border-[#2d3748] rounded-xl bg-[#0b0e11] mt-2">
                <table className="w-full text-right text-[12px] whitespace-nowrap min-w-[1200px]" style={{ borderCollapse: 'collapse' }}>
                    {/* Headers */}
                    <thead>
                        <tr className="border-b border-[#2d3748] text-concept-muted bg-[#111926]">
                            <th className="p-3 text-left sticky left-0 bg-[#111926] z-10 w-64 shadow-[1px_0_0_0_#2d3748]">
                                <div className="flex flex-col leading-tight font-normal text-[11px]">
                                    <span>Symbol</span>
                                    <span className="text-[10px]">295</span>
                                </div>
                            </th>
                            <th className="p-3 text-left sticky left-64 bg-[#111926] z-10 w-40 shadow-[1px_0_0_0_#2d3748]">
                                <span className="font-normal text-[11px]">Exchange</span>
                            </th>
                            {activeCols.map((col, idx) => (
                                <th key={col.key || idx} className="p-3 font-normal">
                                    {renderHeader(col.label, col.isSort)}
                                </th>
                            ))}
                        </tr>
                    </thead>
                    
                    {/* Body */}
                    <tbody>
                        {marketsData.map((row, i) => (
                            <tr key={i} className="border-b border-[#2d3748]/50 hover:bg-[#1E222D]/50 transition-colors group">
                                {/* Sticky Symbol Column */}
                                <td className="p-3 text-left sticky left-0 bg-[#0b0e11] group-hover:bg-[#161a25] transition-colors shadow-[1px_0_0_0_#2d3748] z-10">
                                    <div className="flex items-center gap-3">
                                        {/* Mock symbol icon matching the orange 'B' with sub-icon */}
                                        <div className="relative w-6 h-6 bg-[#F7931A] rounded-full flex items-center justify-center shrink-0">
                                            <span className="text-white font-bold text-xs italic">B</span>
                                            <div className="absolute -bottom-1 -right-1 w-3.5 h-3.5 bg-[#2B3139] border border-[#0b0e11] rounded-full flex items-center justify-center text-[7px] text-white">
                                                $
                                            </div>
                                        </div>
                                        <div className="flex flex-col">
                                            <span className="text-white font-bold tracking-wide">{row.symbol}</span>
                                            <span className="text-[#848E9C] text-[10px] truncate max-w-[150px]">{row.desc}</span>
                                        </div>
                                    </div>
                                </td>
                                
                                {/* Sticky Exchange Column */}
                                <td className="p-3 text-left sticky left-64 bg-[#0b0e11] group-hover:bg-[#161a25] transition-colors shadow-[1px_0_0_0_#2d3748] z-10">
                                    <div className="flex items-center gap-2">
                                        <ExchangeLogo name={row.exchange} />
                                        <span className="text-white font-medium">{row.exchange}</span>
                                    </div>
                                </td>

                                {/* Dynamic Columns */}
                                {activeCols.map((col, idx) => {
                                    const val = row[col.key];
                                    let renderedVal = val;
                                    
                                    if (col.isPct) {
                                        renderedVal = renderPct(String(val));
                                    } else if (col.isRating) {
                                        renderedVal = renderRating(val);
                                    }

                                    return (
                                        <td key={col.key || idx} className={`p-3 text-[#D1D4DC] ${col.isRating ? 'font-medium' : 'font-mono'}`}>
                                            {renderedVal}
                                        </td>
                                    );
                                })}
                            </tr>
                        ))}
                    </tbody>
                </table>
            </div>
        </div>
    );
};

export default ConceptMarketsView;
