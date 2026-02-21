import React, { useEffect, useRef, useState, useMemo } from 'react';
import { useNavigate } from 'react-router-dom';
import { createChart } from 'lightweight-charts';
import axios from 'axios';
import { X, TrendingUp, TrendingDown, Clock, Activity, Maximize2, Minimize2, Check, ExternalLink, ArrowUpRight } from 'lucide-react';

const ConceptChartModal = ({ asset, isOpen, onClose }) => {
    const navigate = useNavigate();
    const fullScreenChartRef = useRef(null);
    const [chartData, setChartData] = useState(null);
    const [isLoading, setIsLoading] = useState(true);
    const [chartMode, setChartMode] = useState('price'); // 'price' | 'marketCap'
    const [timeframe, setTimeframe] = useState('15m');
    const [orderSide, setOrderSide] = useState('buy');
    const [orderType, setOrderType] = useState('market');
    const [tradeAmount, setTradeAmount] = useState('');
    
    // NEW Phase 4 State for Related Symbols
    const [relatedSymbols, setRelatedSymbols] = useState([]);
    
    // Push state onto URL on Mount ONLY IF the user clicked from within the UI (path is different).
    useEffect(() => {
        if (!isOpen || !asset) return;
        const targetPath = `/${asset.sym}`;
        
        // Only push state if the user clicked from the dashboard (URL is still root)
        if (window.location.pathname !== targetPath) {
            window.history.pushState({}, '', targetPath);
        }
    }, [isOpen, asset]);

    // Fetch Related Symbols (Phase 4)
    useEffect(() => {
        if (!isOpen || !asset) return;
        
        const fetchRelated = async () => {
            try {
                const res = await axios.get(`http://127.0.0.1:8000/api/quant/market/correlation/${asset.sym}`);
                if (res.data && res.data.status === 'success') {
                    setRelatedSymbols(res.data.data);
                } else {
                    setRelatedSymbols([]);
                }
            } catch (err) {
                console.error("Failed to load Related Symbols", err);
                setRelatedSymbols([]);
            }
        };
        
        fetchRelated();
    }, [isOpen, asset]);

    // 2. Data Fetching Effect
    useEffect(() => {
        if (!isOpen || !asset) {
            setChartData(null);
            return;
        }
        
        setIsLoading(true);

        const fetchChartData = async () => {
             try {
                 const res = await axios.get(`http://127.0.0.1:8000/api/quant/market/history/${asset.sym}`);
                 if (res.data && res.data.status === 'success') {
                     const safelyMapped = res.data.data.map(d => {
                         let t = d.time;
                         if (typeof t === 'string') {
                             t = Math.floor(new Date(t).getTime() / 1000); // lightweight expects seconds
                         }
                         return { time: t, value: Number(d.close) };
                     })
                     .filter(item => !isNaN(item.time) && !isNaN(item.value)) // drop broken dots
                     .sort((a, b) => a.time - b.time) // must ascending
                     .filter((item, i, arr) => i === 0 || item.time !== arr[i-1].time); // must unique

                     setChartData(safelyMapped);
                 }
             } catch(err) {
                 console.error("Failed to load history for Chart", err);
             } finally {
                 setIsLoading(false);
             }
        };

        // Delay to let CSS opening animations breathe before parsing JSON
        setTimeout(fetchChartData, 300);

    }, [isOpen, asset]);

    // 2. Chart Rendering Effect (Only runs when Overview tab is active)
    // 2. Chart Rendering Effect
    useEffect(() => {
        const activeContainer = fullScreenChartRef.current;
        if (!chartData || !activeContainer) return;

        // Clear any old chart instances in this specific DOM node
        activeContainer.innerHTML = '';

        try {
            const chartInstance = createChart(activeContainer, {
                layout: {
                    background: { type: 'solid', color: 'transparent' },
                    textColor: '#9ca3af', // concept-muted
                },
                localization: {
                    priceFormatter: chartMode === 'marketCap' 
                        ? price => `${(price / 1000000000000).toFixed(2)} T`
                        : undefined,
                },
                grid: {
                    vertLines: { color: 'rgba(55, 65, 81, 0.4)' },
                    horzLines: { color: 'rgba(55, 65, 81, 0.4)' },
                },
                crosshair: {
                    mode: 0,
                    vertLine: { width: 1, color: '#3b82f6', style: 0 },
                    horzLine: { width: 1, color: '#3b82f6', style: 0 },
                },
                rightPriceScale: { borderVisible: false },
                timeScale: { borderVisible: false, timeVisible: true },
                autoSize: true, 
            });

            const lineSeries = chartInstance.addAreaSeries({
                lineColor: asset.color || '#3b82f6',
                topColor: (asset.color || '#3b82f6') + '80', // 50% opacity
                bottomColor: 'rgba(0, 0, 0, 0)',
                lineWidth: 2,
            });

            // Make sure array is not empty before fitting
            if (chartData && chartData.length > 0) {
                // If Market Cap mode, roughly multiply price to simulate trillions (Mock logic: Price * 20M supply)
                const displayData = chartMode === 'marketCap' 
                    ? chartData.map(d => ({ time: d.time, value: d.value * 19990000 }))
                    : chartData;

                lineSeries.setData(displayData);
                chartInstance.timeScale().fitContent();
            }

            // Cleanup handler
            return () => {
                chartInstance.remove();
            };

        } catch (e) {
            console.error("Critical fallback caught: Lightweight charts rendering error:", e);
        }
    }, [chartData, asset, chartMode]);

    const handleClose = () => {
        if (onClose) {
            onClose();
        } else {
            // Safe fallback if used outside App.jsx
            window.history.replaceState({}, '', '/');
            window.dispatchEvent(new PopStateEvent('popstate'));
        }
    };

    if (!isOpen || !asset) return null;

    // =========================================================================
    // THE ONLY LAYOUT: FULL SCREEN (DEDICATED ASSET WEB PAGE MODE)
    // =========================================================================
    return (
        <div className="fixed inset-0 z-[200] bg-[#0b0e11] text-concept-text flex flex-col h-screen overflow-hidden animate-in zoom-in duration-300">
            
            {/* Top Nav Breadcrumbs */}
            <div className="w-full max-w-[1600px] mx-auto px-8 py-4 flex justify-between items-center bg-[#0b0e11] sticky top-0 z-10 border-b border-[#2d3748]">
                <div className="flex items-center gap-2 text-sm text-concept-muted font-medium">
                    <button onClick={handleClose} className="hover:text-white transition-colors">Markets</button>
                    <span>/</span>
                    <span className="text-white">{asset.name || asset.sym}</span>
                </div>
                <div className="flex items-center gap-4">
                    <button onClick={handleClose} className="p-2 hover:bg-white/10 rounded-lg transition-colors group" title="Close and return to Markets">
                        <X className="text-concept-muted group-hover:text-concept-red" size={24} />
                    </button>
                </div>
            </div>

            {/* Main Content Area */}
            <div className="w-full flex-1 max-w-[1600px] mx-auto px-8 pt-8 pb-16 flex flex-col gap-6 overflow-y-auto min-h-0">
                
                {/* Header: Logo, Title, and Huge Price */}
                <div className="flex items-start justify-between shrink-0">
                    <div className="flex items-start gap-6">
                        {/* Graphic Logo */}
                        <div className="w-16 h-16 rounded-full flex items-center justify-center text-3xl font-bold shadow-[0_0_20px_rgba(255,255,255,0.1)]" style={{ backgroundColor: asset.color, color: '#fff' }}>
                            {asset.sym.charAt(0)}
                        </div>
                        
                        {/* Text Info */}
                        <div className="flex flex-col">
                            <div className="flex items-center gap-3 mb-1">
                                <h1 className="text-4xl font-bold text-white tracking-wide">{asset.name || asset.sym}</h1>
                                <span className="px-2 py-1 bg-[#1f2937] text-concept-muted rounded text-xs font-bold tracking-widest">{asset.sym}</span>
                                <span className="px-2 py-1 bg-concept-blue/20 text-concept-blue rounded text-xs font-bold tracking-widest">AlphaQuant</span>
                            </div>
                            <div className="flex items-end gap-3 mt-2">
                                <span className="text-5xl font-bold font-mono text-white tracking-tight">
                                    {typeof asset.price === 'number' ? asset.price.toLocaleString() : String(asset.price).replace('$', '')}
                                </span>
                                <span className="text-xl text-concept-muted font-bold mb-1">USD</span>
                                <span className={`text-xl font-bold mb-1 flex items-center gap-1 ${asset.isUp ? 'text-concept-green' : 'text-concept-red'}`}>
                                    {asset.change}
                                </span>
                            </div>
                            <div className="text-xs text-concept-muted mt-1">As of today at {new Date().toLocaleTimeString()}</div>
                        </div>
                    </div>
                </div>

                <div className="w-full flex-1 flex flex-col xl:flex-row gap-8 min-h-0">
                    {/* LEFT COLUMN: Chart & Stats */}
                    <div className="flex-1 flex flex-col gap-6 min-w-0">
                        {/* Chart Container Canvas */}
                        <div className="w-full flex-1 flex flex-col gap-4 min-h-[500px]">
                    <h2 className="text-2xl font-bold text-white flex items-center gap-1 cursor-pointer hover:text-concept-blue transition-colors w-max group">
                        Chart <span className="text-concept-muted group-hover:text-concept-blue">›</span>
                    </h2>
                    
                    {/* Price / Market Cap Toggles */}
                    <div className="flex gap-2">
                        <button 
                            onClick={() => setChartMode('price')}
                            className={`px-4 py-1.5 rounded-full text-sm font-bold transition-colors ${chartMode === 'price' ? 'bg-[#1E222D] text-white border border-[#2d3748]' : 'hover:bg-white/5 text-concept-muted'}`}
                        >
                            Price
                        </button>
                        <button 
                            onClick={() => setChartMode('marketCap')}
                            className={`px-4 py-1.5 rounded-full text-sm font-bold transition-colors ${chartMode === 'marketCap' ? 'bg-[#1E222D] text-white border border-[#2d3748]' : 'hover:bg-white/5 text-concept-muted'}`}
                        >
                            Market cap
                        </button>
                    </div>

                    <div className="w-full flex-1 bg-[#111926] border border-[#2d3748] rounded-xl relative shadow-lg overflow-hidden flex flex-col min-h-0">
                        
                        {isLoading && (
                            <div className="absolute inset-0 z-10 flex flex-col items-center justify-center bg-[#111926]/50 backdrop-blur-sm gap-4">
                                <Activity className="animate-pulse text-concept-blue" size={32} />
                                <span className="text-concept-muted tracking-widest text-sm uppercase">Loading Tick Data...</span>
                            </div>
                        )}

                        {/* Top Chart Toolbar */}
                        <div className="h-12 border-b border-[#2d3748] flex items-center px-4 gap-4 bg-[#0b0e11]/50 shrink-0">
                            <span className="text-xs text-concept-muted font-bold tracking-widest">TIMEFRAME:</span>
                            {['15m', '1H', '4H', '1D'].map((tf) => (
                                <button onClick={() => setTimeframe(tf)} key={tf} className={`px-2 py-1 rounded text-xs font-bold transition-all ${timeframe === tf ? 'bg-concept-blue/20 text-concept-blue' : 'text-concept-muted hover:bg-white/5 hover:text-white'}`}>{tf}</button>
                            ))}
                        </div>

                        {/* Actual Chart Area */}
                        <div className="flex-1 w-full relative min-h-0">
                            <div className="absolute inset-0" ref={fullScreenChartRef}></div>
                        </div>
                    </div>

                    {/* Timeframe Performance Banner */}
                    <div className="flex justify-between items-center w-full mt-2">
                        {[
                            { tf: '1 day', val: '+1.61%' },
                            { tf: '1 week', val: '+2.76%' },
                            { tf: '1 month', val: '-22.96%' },
                            { tf: '6 months', val: '-40.80%' },
                            { tf: 'Year to date', val: '-22.21%' },
                            { tf: '1 year', val: '-28.97%' },
                            { tf: '5 years', val: '+30.34%' },
                            { tf: '10 years', val: '+21.19K%' },
                            { tf: 'All time', val: '+23.67K%' }
                        ].map((item, idx) => (
                            <div key={item.tf} className={`flex flex-col items-center justify-center py-3 px-6 rounded-xl hover:bg-white/5 cursor-pointer transition-colors ${idx === 0 ? 'bg-white/5' : ''}`}>
                                <span className={`text-xs font-bold ${idx === 0 ? 'text-white' : 'text-concept-muted'}`}>{item.tf}</span>
                                <span className={`text-sm font-mono font-bold mt-1 ${item.val.startsWith('+') ? 'text-concept-green' : 'text-concept-red'}`}>
                                    {item.val}
                                </span>
                            </div>
                        ))}
                    </div>
                </div>
                
                {/* Key Stats Grid */}
                <div className="mt-4">
                    <h2 className="text-xl font-bold text-white mb-6">Key stats</h2>
                    <div className="grid grid-cols-4 gap-8 border-t border-[#2d3748] pt-8">
                        <div>
                            <p className="text-concept-muted text-sm mb-2 font-medium">Market capitalization</p>
                            <p className="text-white text-xl font-mono">1.36 T <span className="text-sm text-concept-muted">USD</span></p>
                        </div>
                        <div>
                            <p className="text-concept-muted text-sm mb-2 font-medium">Fully diluted market cap</p>
                            <p className="text-white text-xl font-mono">1.42 T <span className="text-sm text-concept-muted">USD</span></p>
                        </div>
                        <div>
                            <p className="text-concept-muted text-sm mb-2 font-medium">Trading volume 24h</p>
                            <p className="text-white text-xl font-mono">
                                {typeof asset.vol === 'number' ? `${(asset.vol / 1000000).toFixed(2)} M` : asset.vol} 
                                <span className="text-sm text-concept-muted ml-1">USD</span>
                            </p>
                        </div>
                        <div>
                            <p className="text-concept-muted text-sm mb-2 font-medium">Volume / Market Cap</p>
                            <p className="text-white text-xl font-mono">0.0242</p>
                        </div>
                    </div>
                </div>

                {/* Related Symbols Carousel */}
                <div className="mt-8 mb-12">
                    <div className="mb-6">
                        <h2 className="text-xl font-bold text-white mb-1">Related Symbols</h2>
                        <p className="text-sm text-concept-muted">See more assets like {asset.name || asset.sym} – sorted by Momentum and Hedging factors.</p>
                    </div>
                    
                    <div className="flex gap-4 overflow-x-auto pb-4 custom-scrollbar">
                        {relatedSymbols.length > 0 ? (
                            relatedSymbols.map((item, idx) => {
                                // Default colors based on math
                                const isPositive = item.correlation.includes("+");
                                const colorVal = isPositive ? '#0ECB81' : '#F6465D';
                                return (
                                    <div 
                                        key={idx} 
                                        onClick={() => navigate(`/${item.sym}`)}
                                        className="flex-shrink-0 w-64 bg-[#111926] border border-[#2d3748] rounded-xl p-4 hover:border-[#2962FF] transition-colors cursor-pointer group"
                                    >
                                        <div className="flex items-center gap-3 mb-4">
                                            <div className="w-8 h-8 rounded bg-white/5 border border-white/10 flex items-center justify-center font-bold text-xs" style={{ color: colorVal }}>
                                                {item.sym.charAt(0)}
                                            </div>
                                            <div>
                                                <div className="text-white font-bold text-sm">{item.sym}</div>
                                                <div className="text-concept-muted text-xs uppercase tracking-wider">{item.type}</div>
                                            </div>
                                        </div>
                                        <div className="flex items-end justify-between mt-auto">
                                            <div className="text-white font-mono text-sm">Target</div>
                                            <div className={`text-sm font-bold`} style={{color: colorVal}}>
                                                {item.correlation} Correlation
                                            </div>
                                        </div>
                                    </div>
                                );
                            })
                        ) : (
                            <div className="p-4 text-concept-muted border border-[#2d3748] rounded-xl w-full text-center">
                                No correlation data available (Computing pipeline may be running).
                            </div>
                        )}
                    </div>
                </div>
            </div> {/* END LEFT COLUMN */}

            {/* RIGHT COLUMN: Order Book & Trading Execution */}
            <div className="w-full xl:w-[400px] shrink-0 flex flex-col gap-6">
                    
                {/* Order Book Overview */}
                <div className="bg-[#111926] border border-[#2d3748] rounded-xl p-5 flex flex-col gap-2 shadow-lg">
                    <div className="text-concept-muted text-xs font-bold tracking-widest uppercase mb-2 flex justify-between">
                        <span>Price (USDT)</span>
                        <span>Amount</span>
                    </div>
                    {/* Asks (Sell) */}
                    <div className="flex flex-col gap-1 text-sm font-mono cursor-pointer">
                        <div className="flex justify-between hover:bg-white/5 px-1 rounded text-concept-red relative group"><span className="z-10">{(typeof asset.price === 'number' ? asset.price * 1.002 : 67000).toFixed(2)}</span><span className="z-10 text-white">1.45</span><div className="absolute top-0 right-0 h-full bg-concept-red/10 w-[20%] transition-all opacity-50 group-hover:opacity-100"></div></div>
                        <div className="flex justify-between hover:bg-white/5 px-1 rounded text-concept-red relative group"><span className="z-10">{(typeof asset.price === 'number' ? asset.price * 1.0015 : 66950).toFixed(2)}</span><span className="z-10 text-white">4.20</span><div className="absolute top-0 right-0 h-full bg-concept-red/10 w-[45%] transition-all opacity-50 group-hover:opacity-100"></div></div>
                        <div className="flex justify-between hover:bg-white/5 px-1 rounded text-concept-red relative group"><span className="z-10">{(typeof asset.price === 'number' ? asset.price * 1.0008 : 66800).toFixed(2)}</span><span className="z-10 text-white">12.5</span><div className="absolute top-0 right-0 h-full bg-concept-red/10 w-[80%] transition-all opacity-50 group-hover:opacity-100"></div></div>
                    </div>
                        
                    <div className="py-2 border-y border-[#2d3748] text-center my-1 flex items-center justify-center gap-2">
                        <span className={`text-xl font-bold font-mono ${asset.isUp ? 'text-concept-green' : 'text-concept-red'}`}>
                            {typeof asset.price === 'number' ? asset.price.toLocaleString() : asset.price}
                        </span>
                        {asset.isUp ? <ArrowUpRight className="text-concept-green" size={16} /> : <TrendingDown className="text-concept-red" size={16} />}
                    </div>

                    {/* Bids (Buy) */}
                    <div className="flex flex-col gap-1 text-sm font-mono cursor-pointer">
                        <div className="flex justify-between hover:bg-white/5 px-1 rounded text-concept-green relative group"><span className="z-10">{(typeof asset.price === 'number' ? asset.price * 0.9992 : 66050).toFixed(2)}</span><span className="z-10 text-white">8.4</span><div className="absolute top-0 right-0 h-full bg-concept-green/10 w-[70%] transition-all opacity-50 group-hover:opacity-100"></div></div>
                        <div className="flex justify-between hover:bg-white/5 px-1 rounded text-concept-green relative group"><span className="z-10">{(typeof asset.price === 'number' ? asset.price * 0.9985 : 65800).toFixed(2)}</span><span className="z-10 text-white">2.5</span><div className="absolute top-0 right-0 h-full bg-concept-green/10 w-[30%] transition-all opacity-50 group-hover:opacity-100"></div></div>
                        <div className="flex justify-between hover:bg-white/5 px-1 rounded text-concept-green relative group"><span className="z-10">{(typeof asset.price === 'number' ? asset.price * 0.9980 : 65700).toFixed(2)}</span><span className="z-10 text-white">0.9</span><div className="absolute top-0 right-0 h-full bg-concept-green/10 w-[10%] transition-all opacity-50 group-hover:opacity-100"></div></div>
                    </div>
                </div>

                {/* Trade Execution Panel */}
                <div className="bg-[#111926] border border-[#2d3748] rounded-xl p-5 flex flex-col gap-4 shadow-[0_0_30px_rgba(0,0,0,0.5)] relative overflow-hidden">
                    {/* Subtabs */}
                    <div className="flex border-b border-[#2d3748]">
                        <button className="flex-1 pb-2 text-white font-bold border-b-2 border-concept-blue text-sm">Spot</button>
                        <button onClick={() => alert("Giao dịch đòn bẩy ngầm định (Cross Margin). Tính năng sẽ ra mắt ở V3.")} className="flex-1 pb-2 text-concept-muted font-bold hover:text-white text-sm transition-colors">Margin</button>
                    </div>
                        
                    {/* Order Type */}
                    <div className="flex gap-2">
                        <button onClick={() => setOrderType('limit')} className={`px-3 py-1 rounded text-xs font-bold transition-all ${orderType === 'limit' ? 'bg-[#2B3139] text-white' : 'text-concept-muted hover:bg-white/5'}`}>Limit</button>
                        <button onClick={() => setOrderType('market')} className={`px-3 py-1 rounded text-xs font-bold transition-all ${orderType === 'market' ? 'bg-[#2B3139] text-white' : 'text-concept-muted hover:bg-white/5'}`}>Market</button>
                    </div>

                    {/* Buy/Sell Toggles */}
                    <div className="flex rounded bg-[#0b0e11] p-1">
                        <button 
                            onClick={() => setOrderSide('buy')} 
                            className={`flex-1 py-1.5 text-sm font-bold rounded transition-colors ${orderSide === 'buy' ? 'bg-concept-green text-white shadow-lg' : 'text-concept-muted hover:text-white'}`}
                        >BUY</button>
                        <button 
                            onClick={() => setOrderSide('sell')} 
                            className={`flex-1 py-1.5 text-sm font-bold rounded transition-colors ${orderSide === 'sell' ? 'bg-concept-red text-white shadow-lg' : 'text-concept-muted hover:text-white'}`}
                        >SELL</button>
                    </div>

                    {/* Input Area */}
                    <div className="flex flex-col gap-3 font-mono">
                        {orderType === 'limit' && (
                            <div className="flex items-center justify-between bg-[#0b0e11] border border-[#2B3139] rounded px-3 py-2 focus-within:border-concept-blue transition-colors">
                                <span className="text-concept-muted text-xs font-sans">Price</span>
                                <input type="text" className="bg-transparent text-right outline-none text-white w-full px-2" defaultValue={typeof asset.price === 'number' ? asset.price.toFixed(2) : asset.price} />
                                <span className="text-concept-muted text-xs font-sans">USDT</span>
                            </div>
                        )}

                        <div className="flex items-center justify-between bg-[#0b0e11] border border-[#2B3139] rounded px-3 py-2 focus-within:border-concept-blue transition-colors">
                            <span className="text-concept-muted text-xs font-sans">Amount</span>
                            <input type="number" placeholder="0" value={tradeAmount} onChange={(e) => setTradeAmount(e.target.value)} className="bg-transparent text-right outline-none text-white w-full px-2" />
                            <span className="text-concept-muted text-xs font-sans">{asset.sym.split('_')[0]}</span>
                        </div>

                        <div className="flex gap-1 mt-1">
                            {['25%', '50%', '75%', 'Max'].map(pct => (
                                <button key={pct} onClick={() => setTradeAmount(pct)} className="flex-1 py-1 bg-[#2B3139] text-concept-muted hover:text-white rounded text-[10px] transition-colors">{pct}</button>
                            ))}
                        </div>
                            
                        <div className="flex justify-between items-center text-xs text-concept-muted font-sans mt-2">
                            <span>Avail</span>
                            <span className="font-mono text-white text-sm">
                                {orderSide === 'buy' ? '12,500.00 USDT' : `0.00 ${asset.sym.split('_')[0]}`}
                            </span>
                        </div>
                    </div>

                    {/* Big Action Button */}
                    <button 
                        onClick={() => {
                            if (!tradeAmount) return alert("Vui lòng nhập số lượng!");
                            alert(`ĐÃ KHỚP LỆNH: ${orderSide.toUpperCase()} ${tradeAmount} ${asset.sym.split('_')[0]}\nĐẩy xuống AlphaQuant Core Wallet thành công!`);
                            setTradeAmount('');
                        }}
                        className={`w-full py-3 mt-2 rounded text-white font-bold tracking-widest uppercase transition-all duration-300 hover:scale-[1.02] active:scale-[0.98] ${orderSide === 'buy' ? 'bg-concept-green shadow-[0_0_20px_rgba(16,185,129,0.3)]' : 'bg-concept-red shadow-[0_0_20px_rgba(239,68,68,0.3)]'}`}
                    >
                        {orderSide === 'buy' ? 'Buy' : 'Sell'} {asset.sym.split('_')[0]}
                    </button>
                </div>
            </div>

        </div> {/* END SPLIT CONTAINER */}
            </div>
        </div>
    );
};

export default ConceptChartModal;
