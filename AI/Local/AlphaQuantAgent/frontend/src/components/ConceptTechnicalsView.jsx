import React, { useState } from 'react';
import Plot from 'react-plotly.js';
import { HelpCircle } from 'lucide-react';

const createGaugeLayout = (title, mode, score) => {
    // We map a standard 0 to 100 range backward to angle radians for semi circle
    return {
        data: [
            {
                type: "indicator",
                mode: "gauge",
                value: score,
                title: { 
                    text: title, 
                    font: { size: 14, color: '#D1D4DC', family: 'Inter, sans-serif', weight: 'bold' }
                },
                gauge: {
                    axis: { range: [0, 100], visible: false },
                    bar: { color: "#1E222D", thickness: 0 }, // Hide default bar
                    bgcolor: "transparent",
                    steps: [
                        { range: [0, 20], color: "#ef5350" },       // Strong Sell
                        { range: [20, 40], color: "#ef535080" },    // Sell
                        { range: [40, 60], color: "#787b86" },      // Neutral
                        { range: [60, 80], color: "#26a69a80" },    // Buy
                        { range: [80, 100], color: "#26a69a" }      // Strong Buy
                    ],
                }
            }
        ],
        layout: {
            // Very slim layout to make the gauge compact
            paper_bgcolor: "transparent",
            plot_bgcolor: "transparent",
            margin: { t: 40, b: 0, l: 30, r: 30 },
            height: 200,
            width: 300,
            font: { color: "#D1D4DC", family: "Inter, sans-serif" }
        }
    }
};

const ConceptTechnicalsView = ({ asset }) => {
    const timeframes = ['1 minute', '5 minutes', '15 minutes', '30 minutes', '1 hour', '2 hours', '4 hours', '1 day', '1 week', '1 month'];
    const [selectedTF, setSelectedTF] = useState('1 day');

    // Mocks for tables
    const oscillators = [
        { name: "Relative Strength Index (14)", val: 38, action: "Neutral", color: "text-concept-muted" },
        { name: "Stochastic %K (14, 3, 3)", val: 50, action: "Neutral", color: "text-concept-muted" },
        { name: "Commodity Channel Index (20)", val: -55, action: "Neutral", color: "text-concept-muted" },
        { name: "Average Directional Index (14)", val: 58, action: "Neutral", color: "text-concept-muted" },
        { name: "Awesome Oscillator", val: -9926, action: "Neutral", color: "text-concept-muted" },
        { name: "Momentum (10)", val: -740, action: "Buy", color: "text-[#2962FF]" },
        { name: "MACD Level (12, 26)", val: -4256, action: "Buy", color: "text-[#2962FF]" },
        { name: "Stochastic RSI Fast (3, 3, 14, 14)", val: 80, action: "Neutral", color: "text-concept-muted" },
        { name: "Williams Percent Range (14)", val: -58, action: "Neutral", color: "text-concept-muted" },
        { name: "Bull Bear Power", val: -3493, action: "Neutral", color: "text-concept-muted" },
        { name: "Ultimate Oscillator (7, 14, 28)", val: 48, action: "Neutral", color: "text-concept-muted" }
    ];

    const movingAverages = [
        { name: "Exponential Moving Average (10)", val: "68,365", action: "Sell", color: "text-concept-red" },
        { name: "Simple Moving Average (10)", val: "67,834", action: "Buy", color: "text-[#2962FF]" },
        { name: "Exponential Moving Average (20)", val: "71,263", action: "Sell", color: "text-concept-red" },
        { name: "Simple Moving Average (20)", val: "69,715", action: "Sell", color: "text-concept-red" },
        { name: "Exponential Moving Average (30)", val: "74,218", action: "Sell", color: "text-concept-red" },
        { name: "Simple Moving Average (30)", val: "75,425", action: "Sell", color: "text-concept-red" },
        { name: "Exponential Moving Average (50)", val: "78,663", action: "Sell", color: "text-concept-red" },
        { name: "Simple Moving Average (50)", val: "82,201", action: "Sell", color: "text-concept-red" },
        { name: "Exponential Moving Average (100)", val: "85,883", action: "Sell", color: "text-concept-red" },
        { name: "Simple Moving Average (100)", val: "85,882", action: "Sell", color: "text-concept-red" },
        { name: "Exponential Moving Average (200)", val: "92,813", action: "Sell", color: "text-concept-red" },
        { name: "Simple Moving Average (200)", val: "99,397", action: "Sell", color: "text-concept-red" },
        { name: "Ichimoku Base Line (9, 26, 52, 26)", val: "75,184", action: "Neutral", color: "text-concept-muted" },
        { name: "Volume Weighted Moving Average (20)", val: "69,598", action: "Sell", color: "text-concept-red" },
        { name: "Hull Moving Average (9)", val: "66,760", action: "Buy", color: "text-[#2962FF]" }
    ];

    const pivots = [
        { pivot: "R3", classic: "128,811", fibonacci: "106,427", camarilla: "84,791", woodie: "112,220", dm: "—" },
        { pivot: "R2", classic: "106,427", fibonacci: "97,876", camarilla: "82,739", woodie: "105,080", dm: "—" },
        { pivot: "R1", classic: "92,531", fibonacci: "92,594", camarilla: "80,687", woodie: "89,836", dm: "88,287" },
        { pivot: "P", classic: "84,043", fibonacci: "84,043", camarilla: "84,043", woodie: "82,696", dm: "81,921" },
        { pivot: "S1", classic: "70,147", fibonacci: "75,492", camarilla: "76,583", woodie: "67,452", dm: "65,903" },
        { pivot: "S2", classic: "61,659", fibonacci: "70,210", camarilla: "74,531", woodie: "60,312", dm: "—" },
        { pivot: "S3", classic: "39,275", fibonacci: "61,659", camarilla: "72,479", woodie: "45,068", dm: "—" }
    ];

    return (
        <div className="w-full flex-1 flex flex-col gap-8 pb-12 animate-in fade-in duration-300">
            {/* Header Description */}
            <div className="flex flex-col gap-2 max-w-4xl">
                <h2 className="text-2xl font-bold text-white tracking-wide">Indicators' summary</h2>
                <p className="text-sm text-concept-muted leading-relaxed">
                    See technical analysis overview for the selected timeframe. It includes key data from moving averages, oscillators, and pivots — all summed up in the Summary gauge, where you can instantly see whether indicators suggest a buy, sell, or neutral signal. Learn more about how these signals are formed with <span className="text-[#2962FF] hover:underline cursor-pointer">Technical Ratings</span>.
                </p>
            </div>

            {/* Timeframe Selection Row */}
            <div className="flex bg-[#0b0e11] border border-[#2d3748] rounded self-start p-1 mt-2">
                {timeframes.map(tf => (
                    <button 
                        key={tf}
                        onClick={() => setSelectedTF(tf)}
                        className={`text-[13px] font-bold px-4 py-1.5 rounded transition-colors ${selectedTF === tf ? 'bg-white/10 text-white' : 'text-concept-muted hover:text-white'}`}
                    >
                        {tf}
                    </button>
                ))}
            </div>

            {/* Top Row: 3 Gauges */}
            <div className="grid grid-cols-3 gap-8 mt-4 items-end">
                {/* Gauge 1: Oscillators */}
                <div className="flex flex-col items-center">
                    <Plot 
                        data={createGaugeLayout('Oscillators', 'Oscillator', 48).data}
                        layout={createGaugeLayout('Oscillators', 'Oscillator', 48).layout}
                        config={{displayModeBar: false}}
                    />
                    <div className="text-2xl font-bold text-[#2962FF] -mt-10 mb-4 z-10">Buy</div>
                    <div className="flex gap-6 text-sm text-center">
                        <div className="flex flex-col"><span className="text-concept-muted font-bold mb-1">Sell</span><span className="text-white text-lg">0</span></div>
                        <div className="flex flex-col"><span className="text-concept-muted font-bold mb-1">Neutral</span><span className="text-white text-lg">9</span></div>
                        <div className="flex flex-col"><span className="text-concept-muted font-bold mb-1">Buy</span><span className="text-white text-lg">2</span></div>
                    </div>
                </div>

                {/* Gauge 2: Summary (Bigger) */}
                <div className="flex flex-col items-center">
                    <Plot 
                        data={createGaugeLayout('Summary', 'Summary', 25).data}
                        layout={createGaugeLayout('Summary', 'Summary', 25).layout}
                        config={{displayModeBar: false}}
                        style={{transform: 'scale(1.2)'}}
                    />
                    <div className="text-3xl font-bold text-concept-red z-10 -mt-2 mb-8">Sell</div>
                    <div className="flex gap-8 text-sm text-center">
                        <div className="flex flex-col"><span className="text-concept-muted font-bold mb-1">Sell</span><span className="text-white text-xl">12</span></div>
                        <div className="flex flex-col"><span className="text-concept-muted font-bold mb-1">Neutral</span><span className="text-white text-xl">10</span></div>
                        <div className="flex flex-col"><span className="text-concept-muted font-bold mb-1">Buy</span><span className="text-white text-xl">4</span></div>
                    </div>
                </div>

                {/* Gauge 3: Moving Averages */}
                <div className="flex flex-col items-center">
                     <Plot 
                        data={createGaugeLayout('Moving Averages', 'MA', 15).data}
                        layout={createGaugeLayout('Moving Averages', 'MA', 15).layout}
                        config={{displayModeBar: false}}
                    />
                    <div className="text-2xl font-bold text-concept-red -mt-10 mb-4 z-10">Strong sell</div>
                    <div className="flex gap-6 text-sm text-center">
                        <div className="flex flex-col"><span className="text-concept-muted font-bold mb-1">Sell</span><span className="text-white text-lg">12</span></div>
                        <div className="flex flex-col"><span className="text-concept-muted font-bold mb-1">Neutral</span><span className="text-white text-lg">1</span></div>
                        <div className="flex flex-col"><span className="text-concept-muted font-bold mb-1">Buy</span><span className="text-white text-lg">2</span></div>
                    </div>
                </div>
            </div>

            {/* Bottom Row: Data Tables */}
            <div className="grid grid-cols-2 gap-16 mt-12">
                
                {/* Table: Oscillators */}
                <div className="flex flex-col">
                    <h3 className="text-lg font-bold text-white mb-6 border-b border-[#2d3748] pb-4 flex items-center justify-between">
                        <span>Oscillators</span>
                    </h3>
                    
                    <div className="flex justify-between text-concept-muted mb-4 px-2 text-[13px] font-medium pr-6">
                        <span>Name</span>
                        <div className="flex gap-16 w-1/3 justify-end pr-4">
                            <span>Value</span>
                            <span>Action</span>
                        </div>
                    </div>

                    <div className="flex flex-col text-[13px] font-medium border-b border-[#2d3748] max-h-[600px] overflow-y-auto custom-scrollbar pr-2 pb-4">
                        {oscillators.map((row, i) => (
                            <div key={i} className="flex justify-between items-center py-3 px-2 border-t border-[#2d3748]/50 hover:bg-white/5 transition-colors">
                                <span className="text-[#D1D4DC]">{row.name}</span>
                                <div className="flex gap-16 w-1/3 justify-end items-center pr-4 font-mono text-sm">
                                    <span className="text-[#D1D4DC]">{row.val}</span>
                                    <span className={`${row.color} w-12 text-right tracking-wider`}>{row.action}</span>
                                </div>
                            </div>
                        ))}
                    </div>
                </div>

                {/* Table: Moving Averages */}
                <div className="flex flex-col">
                    <h3 className="text-lg font-bold text-white mb-6 border-b border-[#2d3748] pb-4 flex items-center justify-between">
                        <span>Moving Averages</span>
                    </h3>
                    
                    <div className="flex justify-between text-concept-muted mb-4 px-2 text-[13px] font-medium pr-6">
                        <span>Name</span>
                        <div className="flex gap-16 w-1/3 justify-end pr-4">
                            <span>Value</span>
                            <span>Action</span>
                        </div>
                    </div>

                    <div className="flex flex-col text-[13px] font-medium border-b border-[#2d3748] max-h-[600px] overflow-y-auto custom-scrollbar pr-2 pb-4">
                        {movingAverages.map((row, i) => (
                            <div key={i} className="flex justify-between items-center py-3 px-2 border-t border-[#2d3748]/50 hover:bg-white/5 transition-colors">
                                <span className="text-[#D1D4DC]">{row.name}</span>
                                <div className="flex gap-16 w-1/3 justify-end items-center pr-4 font-mono text-sm">
                                    <span className="text-[#D1D4DC]">{row.val}</span>
                                    <span className={`${row.color} w-12 text-right tracking-wider`}>{row.action}</span>
                                </div>
                            </div>
                        ))}
                    </div>
                </div>

            </div>

            {/* Bottom Row 2: Pivots Table */}
            <div className="flex flex-col mt-4">
                <h3 className="text-lg font-bold text-white mb-6 border-b border-[#2d3748] pb-4">
                    <span>Pivots</span>
                </h3>
                
                <div className="flex flex-col text-[13px] font-medium w-full">
                    {/* Header Row */}
                    <div className="flex justify-between text-concept-muted mb-4 px-2 tracking-wider">
                        <span className="flex-1 max-w-[100px]">Pivot</span>
                        <span className="flex-1 text-right">Classic</span>
                        <span className="flex-1 text-right">Fibonacci</span>
                        <span className="flex-1 text-right">Camarilla</span>
                        <span className="flex-1 text-right">Woodie</span>
                        <span className="flex-1 text-right">DM</span>
                    </div>

                    {/* Data Rows */}
                    <div className="flex flex-col border-b border-[#2d3748]">
                        {pivots.map((row, i) => (
                            <div key={i} className="flex justify-between items-center py-4 px-2 border-t border-[#2d3748]/50 hover:bg-white/5 transition-colors">
                                <span className="flex-1 max-w-[100px] text-white font-bold">{row.pivot}</span>
                                <span className="flex-1 text-right font-mono text-[#D1D4DC]">{row.classic}</span>
                                <span className="flex-1 text-right font-mono text-[#D1D4DC]">{row.fibonacci}</span>
                                <span className="flex-1 text-right font-mono text-[#D1D4DC]">{row.camarilla}</span>
                                <span className="flex-1 text-right font-mono text-[#D1D4DC]">{row.woodie}</span>
                                <span className="flex-1 text-right font-mono text-concept-muted">{row.dm}</span>
                            </div>
                        ))}
                    </div>
                </div>
            </div>

        </div>
    );
}

export default ConceptTechnicalsView;
