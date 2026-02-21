// Initialize React and lifecycle management Hooks from the core library
import React, { useState, useEffect } from 'react';
// Import the axios bridge to send HTTP network requests to FastAPI
import axios from 'axios';
// Extract Vector Icons indicating arrow states from Lucide
import { TrendingDown, TrendingUp, Minus, ArrowDown } from 'lucide-react';
// Import the TopBar component containing the horizontal asset list
import ConceptTopBar from './ConceptTopBar';
// Import the high-speed TickerTape carousel component at the bottom
import TickerTape from './TickerTape';
// Activate internal Router navigation without page reload (Single Page App)
import { useNavigate } from 'react-router-dom';

// Define the Markets Page Component - The ultimate Asset Showcase Coordinate
const MarketsPage = () => {
    // State vault storing the list of Market Data (Real-time Movers)
    const [marketsData, setMarketsData] = useState([]);
    // Switch on the Router to handle Link clicks
    const navigate = useNavigate();
    
    // Set a trap with the useEffect Hook to trigger the API shot only once at Table initialization
    useEffect(() => {
        // Plant a Lifecycle monitoring flag. If User closes tab while network is loading, forbid Rendering!
        let isMounted = true;
        
        // Steel Error Catching Grid Box (Try Catch) locking down the entire Network process
        try {
            // Fire Axios through the Quant API tunnel. Retrieve Top Movers
            axios.get('http://127.0.0.1:8000/api/quant/market/top_movers')
                .then(res => {
                    // If User disconnects network or changes Page -> Safely retreat, No memory trash dumped
                    if (!isMounted) return;
                    
                    // Double safety fence: Must possess Data package and standard Success Status Flag
                    if (res.data && res.data.status === 'success') {
                        // Pour the Fresh Raw Data directly into the State UI Reservoir
                        setMarketsData(res.data.data);
                    }
                })
                .catch(err => {
                    // Cross-Chain Error Catching When Server is Crushed or Network Fails (e.g. 404, 500)
                    console.error("Cable Snapped Loading Markets Page Top Movers Directory:", err);
                });
        } catch (fatalErr) {
            // Bottom-most Backup Net: Block All Fatal Crashes At The React JS Node Layer
            console.error("Lethal RAM Burn Error While Booting Market API Visualizer:", fatalErr);
        }
        
        // Clean up the battlefield when the Component Dies (Unmount Cleanup)
        return () => { isMounted = false; };
    }, []); // Empty Anchor Axis - Lock positioning to only detonate once upon Market Load 

    // Command Function to Fire Trade Redirect Link
    const handleAssetClick = (asset) => {
        // Catch Error If Empty Asset is Blindly Pressed
        if (asset && asset.sym) {
            // Divert Navigation to Force App Jump To Dashboard Chart, strictly using the pure backend symbol without appending _USDT
            navigate(`/${asset.sym}`);
        }
    };

    // Pour Plastic Render Base Architecture HTML Box Model Structure
    return (
        // Outermost Core Box Frame For Market Screen - Clinging to Absolute Height
        <div className="flex-1 flex flex-col relative h-screen bg-[#0b0e11] overflow-y-auto">
            {/* Command to Install TopBar Carousel And Attach Trigger Poking Trade Link Via Prop Passing */}
            <ConceptTopBar onAssetClick={handleAssetClick} />
            {/* Command to Embed Fast Sliding TickerTape Strip Cutting Link Command Wire Through */}
            <TickerTape onAssetClick={handleAssetClick} />
            
            {/* Core Configuration Containing Max 7XL Span Centering Content Compartment Depending On Screen Width */}
            <div className="max-w-7xl mx-auto w-full px-8 py-10 flex flex-col gap-6 animate-in fade-in zoom-in-95 duration-500">
                {/* Title Corner And Glaring Heading */}
                <div className="flex flex-col gap-2">
                    <h1 className="text-4xl font-bold text-white tracking-wide">Market Overview</h1>
                    <p className="text-concept-muted">Global asset performance tracking, 15-minute realtime OHLCV snapshot.</p>
                </div>
                
                {/* Black Box Background Grid Holding The Inner Analytics Table Weight System */}
                <div className="bg-[#111926] border border-[#2d3748] rounded-2xl p-6 shadow-2xl">
                    <table className="w-full text-left text-sm whitespace-nowrap">
                        {/* Table Head Arc Where The Pillar Titles Stand Firm */}
                        <thead>
                            <tr className="border-b border-[#2d3748] text-[#848E9C]">
                                <th className="pb-4 font-semibold">Asset Source Name</th>
                                <th className="pb-4 font-semibold text-right">Match Price</th>
                                <th className="pb-4 font-semibold text-right">24h Wave Margin</th>
                                <th className="pb-4 font-semibold text-right">Blood Volume Vol</th>
                                <th className="pb-4 font-semibold text-right">Pedal Command</th>
                            </tr>
                        </thead>
                        {/* Table Body Trunk Inner Ring Attached Holding The JS Array Fractures */}
                        <tbody>
                            {/* Render Funnel Function Cutting Loop Copying Array Code Rendering Returning TD Row */}
                            {marketsData.map((asset, i) => (
                                // Assign Key Lock To Avoid React Loop Collisions - Attach Hover Mouse Slide Css
                                <tr key={i} className="border-b border-[#2d3748]/50 hover:bg-white/5 transition-colors group cursor-pointer" onClick={() => handleAssetClick(asset)}>
                                    <td className="py-4">
                                        <div className="flex items-center gap-3">
                                            {/* Logo Icon Extracted From First Letters Packaged With Dynamic Instant Color Change CSS */}
                                            <div className="w-8 h-8 rounded shrink-0 flex items-center justify-center font-bold text-[10px]" style={{backgroundColor: asset.bg, color: asset.color}}>
                                                {asset.sym.substring(0,2)}
                                            </div>
                                            <div className="flex flex-col">
                                                {/* Ticker Brand Label Glows If Mouse Tip Touches It (Hover) */}
                                                <span className="text-white font-bold tracking-wide group-hover:text-concept-blue transition-colors">{asset.sym}</span>
                                            </div>
                                        </div>
                                    </td>
                                    {/* Match Price Frame Using Rigid Digital Mono Font */}
                                    <td className="py-4 text-right font-mono text-white font-semibold">
                                        {asset.price}
                                    </td>
                                    {/* Percent % Frame Alignment Switching Dynamic Back Green/Red Background */}
                                    <td className="py-4 text-right">
                                        {/* Biotech Environment CSS Pulling Aura Transformation From Returned Data Boolean IsUp Asset Color */}
                                        <span className={`px-2 py-1 rounded text-xs font-bold ${asset.isUp ? 'bg-concept-green/10 text-concept-green' : 'bg-concept-red/10 text-concept-red'}`}>
                                            {asset.change}
                                        </span>
                                    </td>
                                    {/* Twist Right-Aligned Block Displaying Faded White Text Behind Volume Hideout */}
                                    <td className="py-4 text-right font-mono text-[#D1D4DC]">
                                        {asset.vol}
                                    </td>
                                    {/* Final Action Crosshair Is The Button Press Exploding The Trade Section */}
                                    <td className="py-4 text-right">
                                        {/* Action Button Press Routing Straight Hook Catching Navigate Link To Next Pipeline */}
                                        <button 
                                            // Hot Click Function For Emergency Touch Processing Stop Propagation (Blocking Other Link Strips Passing Same Line Instantly)
                                            onClick={(e) => { e.stopPropagation(); handleAssetClick(asset); }} 
                                            className="px-4 py-1.5 rounded bg-concept-blue/20 text-concept-blue text-xs font-bold hover:bg-concept-blue hover:text-white transition-colors"
                                        >
                                            Execute (Trade)
                                        </button>
                                    </td>
                                </tr>
                            ))}
                        </tbody>
                    </table>
                    
                    {/* Status Censorship Stream If Table Empty -> Call Loading Flag Chart Shake Spinner Haunting Phantom Standing Circular Word Run */}
                    {marketsData.length === 0 && (
                        <div className="text-center py-10 text-concept-muted animate-pulse">Data Transmission Core Pulling Realtime Market Data File...</div>
                    )}
                </div>
            </div>
        </div>
    )
}

// Authenticate Main Output Gate Exit Barrier Module Entity For Parent Block
export default MarketsPage;
