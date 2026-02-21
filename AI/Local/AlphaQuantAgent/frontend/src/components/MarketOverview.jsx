import React from 'react';
import { ArrowUpRight, TrendingUp, TrendingDown, Clock, Activity } from 'lucide-react';

const MarketOverview = () => {
    return (
        <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
            
            {/* Column 1: Crypto Indicies */}
            <div className="bg-binance-panel border border-binance-border rounded-xl  overflow-hidden flex flex-col shadow-lg">
                <div className="p-4 border-b border-binance-border flex items-center gap-2">
                    <Activity size={18} className="text-binance-blue" />
                    <h3 className="font-bold text-white">Crypto Market</h3>
                </div>
                <div className="flex-1 divide-y divide-binance-border">
                    <AssetRow symbol="BTCUSDT" name="Bitcoin" price="68,102.50" change="2.45" />
                    <AssetRow symbol="ETHUSDT" name="Ethereum" price="3,401.20" change="-0.85" />
                    <AssetRow symbol="SOLUSDT" name="Solana" price="145.60" change="5.12" />
                    <AssetRow symbol="BNBUSDT" name="BNB" price="590.25" change="1.05" />
                </div>
            </div>

            {/* Column 2: Traditional Equities */}
            <div className="bg-binance-panel border border-binance-border rounded-xl overflow-hidden flex flex-col shadow-lg">
                <div className="p-4 border-b border-binance-border flex items-center gap-2">
                    <TrendingUp size={18} className="text-binance-green" />
                    <h3 className="font-bold text-white">Top Gainers (Equities)</h3>
                </div>
                <div className="flex-1 divide-y divide-binance-border">
                    <AssetRow symbol="NVDA" name="NVIDIA Corp" price="130.55" change="5.20" />
                    <AssetRow symbol="TSLA" name="Solana" price="185.60" change="3.12" />
                    <AssetRow symbol="AAPL" name="Apple Inc" price="189.20" change="1.10" />
                    <AssetRow symbol="MSFT" name="Microsoft" price="420.25" change="0.85" />
                </div>
            </div>

            {/* Column 3: AlphaQuant Specific (Rates/Yields) */}
            <div className="bg-binance-panel border border-binance-border rounded-xl overflow-hidden flex flex-col shadow-lg relative">
                <div className="absolute top-0 right-0 w-32 h-32 bg-binance-blue opacity-5 rounded-full blur-3xl"></div>
                <div className="p-4 border-b border-binance-border flex items-center justify-between z-10">
                    <div className="flex items-center gap-2">
                         <Clock size={18} className="text-purple-500" />
                         <h3 className="font-bold text-white">Yield Curve & Bonds</h3>
                    </div>
                    <span className="text-xs bg-purple-500 bg-opacity-20 text-purple-400 px-2 py-1 rounded">Smart Contract</span>
                </div>
                <div className="flex-1 divide-y divide-binance-border z-10">
                    <AssetRow  symbol="US10Y" name="US Treasury 10Y" price="4.25" change="0.05" isYield />
                    <AssetRow  symbol="US02Y" name="US Treasury 2Y" price="4.65" change="-0.02" isYield />
                    <AssetRow  symbol="VCB_6M" name="Vietcombank Term 6M" price="4.80" change="0.00" isYield neutral />
                    <AssetRow  symbol="VCB_12M" name="Vietcombank Term 12M" price="5.10" change="0.00" isYield neutral />
                </div>
            </div>

        </div>
    );
};

const AssetRow = ({symbol, name, price, change, isYield=false, neutral=false}) => {
    const numericChange = parseFloat(change);
    const isUp = numericChange > 0;
    const isFlat = numericChange === 0 || neutral;
    
    // Aesthetic SVG Sparkline logic
    const pathData = isFlat ? "M0 10 L40 10 L80 10" 
                    : isUp ? "M0 15 L20 12 L40 14 L60 5 L80 2" 
                    : "M0 2 L20 8 L40 6 L60 12 L80 15";
    
    const strokeColor = isFlat ? "#848E9C" : isUp ? "#0ECB81" : "#F6465D";

    return (
        <div className="flex items-center justify-between p-4 hover:bg-[#2B3139] cursor-pointer transition-colors group">
            {/* Asset Identity */}
            <div className="flex items-center gap-3">
                <div className="w-8 h-8 rounded-full bg-[#0b0e11] border border-binance-border flex items-center justify-center text-xs font-bold text-white group-hover:border-binance-blue transition-colors">
                    {symbol.charAt(0)}
                </div>
                <div>
                    <h4 className="font-bold text-white text-sm group-hover:text-binance-blue transition-colors">{symbol}</h4>
                    <span className="text-xs text-binance-muted">{name}</span>
                </div>
            </div>

            {/* Sparkline Visual (Fake SVG for now) */}
            <div className="hidden sm:block">
                <svg width="80" height="20" viewBox="0 0 80 20" fill="none" xmlns="http://www.w3.org/2000/svg">
                   <path d={pathData} stroke={strokeColor} strokeWidth="2" strokeLinecap="round" strokeLinelinejoin="round"/>
                </svg>
            </div>

            {/* Price Information */}
            <div className="text-right flex flex-col items-end">
                <span className="font-bold text-white text-sm">{price}{isYield ? '%' : ''}</span>
                <span className={`text-xs font-semibold ${isFlat ? 'text-binance-muted' : isUp ? 'text-binance-green' : 'text-binance-red'} flex items-center gap-1`}>
                    {!isFlat && (isUp ? <ArrowUpRight size={12}/> : <TrendingDown size={12} />)}
                    {isUp ? '+' : ''}{change}%
                </span>
            </div>
        </div>
    )
}

export default MarketOverview;
