import React, { useState, useEffect, useMemo } from 'react';
import axios from 'axios';
import { useQuantMetrics } from '../hooks/useQuantMetrics';
import AIBrainTab from './tabs/AIBrainTab';
import PortfolioSurvivalTab from './tabs/PortfolioSurvivalTab';
import RiskVolatilityTab from './tabs/RiskVolatilityTab';
import AdvancedRatiosTab from './tabs/AdvancedRatiosTab';
import ExecutionTab from './tabs/ExecutionTab';
import ReturnsTab from './tabs/ReturnsTab';

const QuantMatrixDashboard = () => {
    const [activeTab, setActiveTab] = useState('AIBrain');
    const { data: quantData, isLoading: metricsLoading } = useQuantMetrics();
    
    // NEW STATES FOR PHASE 3: 400+ SYMBOLS LAZY LOADING & SLICING
    const [symbols, setSymbols] = useState([]);
    const [activeSymbol, setActiveSymbol] = useState('');
    const [chartData, setChartData] = useState([]);
    const [isChartLoading, setIsChartLoading] = useState(false);

    const TABS = [
        { id: 'AIBrain', label: '🧠 AI Brain' },
        { id: 'Risk', label: '🌪️ Risk & Volatility' },
        { id: 'Ratios', label: '👑 Advanced Ratios' },
        { id: 'Returns', label: '📊 Returns' },
        { id: 'Execution', label: '⏱️ Execution' },
        { id: 'Portfolio', label: '🌐 Portfolio & Survival' }
    ];

    // FETCH SYMBOLS FROM DB ON MOUNT
    useEffect(() => {
        let isMounted = true;
        axios.get('http://127.0.0.1:8000/api/quant/market/symbols')
            .then(res => {
                if(isMounted && res.data && res.data.status === 'success') {
                    const fetchedSymbols = res.data.data;
                    setSymbols(fetchedSymbols);
                    if(fetchedSymbols.length > 0) {
                        setActiveSymbol(fetchedSymbols[0]); // Mặc định chọn BTC_USDT (hoặc index 0)
                    }
                }
            })
            .catch(err => console.error("Fail Fetching 400 Symbols:", err));
        return () => { isMounted = false; };
    }, []);

    // FETCH CANDLES SLICE 96 DATA WHENT ACTIVE SYMBOL CHANGES
    useEffect(() => {
        if(!activeSymbol) return;
        let isMounted = true;
        setIsChartLoading(true);
        axios.get(`http://127.0.0.1:8000/api/quant/market/data/${activeSymbol}?endDate=2025-12-31T00:00:00Z&interval=1D&timeframe=15m`)
            .then(res => {
                if(isMounted && res.data && res.data.status === 'success') {
                    setChartData(res.data.data);
                }
            })
            .catch(err => console.error(`Fail Fetching Candle Slice for ${activeSymbol}`, err))
            .finally(() => { if(isMounted) setIsChartLoading(false); });
            
        return () => { isMounted = false; };
    }, [activeSymbol]);

    return (
        <div className="flex h-screen overflow-hidden bg-[#131722] text-[#D1D4DC] font-sans">
            
            {/* SIDEBAR BẢNG GIÁ THEO YÊU CẦU: Render ĐỦ 400 mã */}
            <div className="w-64 border-r border-[#2B3139] bg-[#0b0e11] flex flex-col shrink-0">
                <div className="p-4 border-b border-[#2B3139] shadow-md z-10">
                    <h2 className="font-bold text-white tracking-wide">Market Watch</h2>
                    <p className="text-xs text-[#848E9C]">{symbols.length} Assets Pipeline</p>
                </div>
                {/* overflow-y-auto đáp ứng yêu cầu render 400 row mà không tràn trang web */}
                <div className="flex-1 overflow-y-auto custom-scrollbar p-2 space-y-1">
                    {symbols.length === 0 ? (
                        <div className="text-sm text-[#848E9C] p-2 animate-pulse">Scanning DB...</div>
                    ) : (
                        symbols.map(sym => (
                            <div 
                                key={sym}
                                onClick={() => setActiveSymbol(sym)}
                                className={`px-3 py-2 rounded-lg text-sm font-semibold cursor-pointer transition-colors ${
                                    activeSymbol === sym 
                                        ? 'bg-[#2962FF] text-white shadow-[0_0_10px_rgba(41,98,255,0.4)]' 
                                        : 'text-[#848E9C] hover:bg-[#1E222D] hover:text-[#D1D4DC]'
                                }`}
                            >
                                {sym}
                            </div>
                        ))
                    )}
                </div>
            </div>

            {/* VÙNG DASHBOARD CHÍNH BÊN PHẢI */}
            <div className="flex-1 flex flex-col h-full overflow-y-auto p-6 custom-scrollbar">
                <div className="max-w-7xl mx-auto w-full space-y-6">
                    
                    {/* Header */}
                    <div>
                        <h1 className="text-3xl font-bold text-white mb-2">AlphaQuant Matrix Dashboard</h1>
                        <p className="text-sm text-[#848E9C]">Real-time LLM agent orchestration, PPO training telemetry, and advanced portfolio risk analytics.</p>
                        <p className="text-lg text-[#0ECB81] mt-2 font-bold flex items-center gap-2">
                             Current Focus: {activeSymbol}
                             {isChartLoading && <span className="animate-spin h-4 w-4 border-2 border-[#0ECB81] border-t-transparent rounded-full inline-block"></span>}
                        </p>
                    </div>

                    {/* Tabs */}
                    <div className="flex space-x-2 border-b border-[#2B3139] pb-2 overflow-x-auto custom-scrollbar">
                        {TABS.map(tab => (
                            <button
                                key={tab.id}
                                onClick={() => setActiveTab(tab.id)}
                                className={`px-4 py-2 rounded-lg text-sm font-semibold transition-colors ${activeTab === tab.id ? 'bg-[#2B3139] text-[#EAECEF]' : 'text-[#848E9C] hover:bg-[#1E222D]'}`}
                            >
                                {tab.label}
                            </button>
                        ))}
                    </div>

                    {/* Content */}
                    <div className="bg-[#0b0e11] border border-[#2B3139] rounded-2xl p-6 min-h-[60vh]">
                        
                        {/* Tab 1: AI Brain */}
                        {activeTab === 'AIBrain' && (
                            <AIBrainTab metrics={quantData} chartData={chartData} />
                        )}

                        {/* Loading Mask state cho 5 Tab còn lại nếu QuantData json chưa fetch xong */}
                        {activeTab !== 'AIBrain' && metricsLoading && !quantData && (
                            <div className="w-full h-full min-h-[400px] flex items-center justify-center text-[#848E9C]">
                                Loading Offline Backtest Tensor Context...
                            </div>
                        )}

                        {/* Tab 2: Risk & Volatility */}
                        {activeTab === 'Risk' && quantData?.riskVolatility && (
                            <RiskVolatilityTab metrics={quantData.riskVolatility} />
                        )}

                        {/* Tab 3: Advanced Ratios */}
                        {activeTab === 'Ratios' && quantData?.advancedRatios && (
                            <AdvancedRatiosTab metrics={quantData.advancedRatios} />
                        )}

                        {/* Tab 4: Returns */}
                        {activeTab === 'Returns' && quantData?.returns && (
                            <ReturnsTab metrics={quantData.returns} chartData={chartData} />
                        )}

                        {/* Tab 5: Execution */}
                        {activeTab === 'Execution' && quantData?.execution && (
                            <ExecutionTab metrics={quantData.execution} />
                        )}

                        {/* Tab 6: Portfolio & Survival */}
                        {activeTab === 'Portfolio' && quantData?.portfolioSurvival && (
                            <PortfolioSurvivalTab metrics={quantData.portfolioSurvival} />
                        )}

                    </div>
                </div>
            </div>
        </div>
    );
};

export default QuantMatrixDashboard;
