// Import React and standard state/effect hooks from the core library
import React, { useEffect, useState } from 'react';
// Import Axios to fetch dynamic data from the backend matrix
import axios from 'axios';

// Define the Infinite Scrolling Ticker Tape Component for the Trading Terminal
// Intercepts the onAssetClick prop to dynamically route the user to the correct chart
const TickerTape = ({ onAssetClick }) => {
    // Initialize the Vault to hold live asset data streams (No Mock Data Allowed)
    const [assets, setAssets] = useState([]);
    
    // Attach the Lifecycle Hook to trigger data fetch immediately upon component mount
    useEffect(() => {
        // Drop an anchor to prevent memory leaks if the component unmounts prematurely
        let isMounted = true;
        
        // Deploy a massive Try-Catch net to absorb any fatal API network collisions
        try {
            // Pull the latest Realtime Top Movers string from the Quantitative Engine
            axios.get('http://127.0.0.1:8000/api/quant/market/top_movers')
                .then(res => {
                    // Safety check: Abort state update if the component is already dead
                    if (!isMounted) return;
                    
                    // Structural integrity check: Ensure the payload exists and status is 'success'
                    if (res.data && res.data.status === 'success') {
                        // Unpack the valid data matrix into the UI state
                        setAssets(res.data.data);
                    }
                })
                .catch(err => {
                    // Prevent silent failures by logging specific API routing errors to the console
                    console.error("Ticker Tape stream transmission failed:", err);
                });
        } catch (fatalErr) {
            // The absolute final defense mechanism against React threading crashes
            console.error("Critical Client-Side Error in Ticker Protocol:", fatalErr);
        }

        // Cleanup function to sever connections and destroy temporary state
        return () => { isMounted = false; };
    }, []); // Empty dependency array ensures this execution fires exactly once

    // Synthesize the HTML structure required for the sliding animation
    return (
        // Mount the thin horizontal bar frame, docking it above the layout with z-index
        <div className="h-10 border-b border-[#2B3139] flex items-center bg-[#0b0e11] overflow-hidden whitespace-nowrap z-40 relative w-full group">
            
            {/* Conditional Rendering: Only start the engines if the asset payload is populated */}
            {assets.length > 0 ? (
                // Initiate the Marquee cycle. Pause the animation strictly on hover interactions
                <div className="flex gap-10 px-4 animate-marquee group-hover:[animation-play-state:paused] shrink-0 text-sm">
                    {/* First iteration of the ticker tape elements */}
                    {assets.map((asset, idx) => (
                        <TapeItem key={`t1-${idx}`} asset={asset} onClick={() => onAssetClick && onAssetClick(asset)} />
                    ))}
                    {/* Second mirrored iteration to trick the eye into seamless infinite scrolling */}
                    {assets.map((asset, idx) => (
                        <TapeItem key={`t2-${idx}`} asset={asset} onClick={() => onAssetClick && onAssetClick(asset)} />
                    ))}
                    {/* Third iteration enforcing buffer overflow for ultra-wide monitors */}
                    {assets.map((asset, idx) => (
                        <TapeItem key={`t3-${idx}`} asset={asset} onClick={() => onAssetClick && onAssetClick(asset)} />
                    ))}
                </div>
            ) : (
                // Fallback ghost state while waiting for API resolution
                <div className="flex w-full items-center justify-center opacity-50">
                    <span className="text-concept-muted text-[11px] font-bold tracking-widest uppercase">Initializing Ticker Tape Protocol...</span>
                </div>
            )}
        </div>
    );
};

// Subcomponent architecture dedicated to rendering individual data nodes on the belt
const TapeItem = ({ asset, onClick }) => {
    // Dynamically calculate color polarity based on backend boolean directives
    const color = asset.isUp ? 'text-[#0ECB81]' : 'text-[#F6465D]';
    
    // Return block for the isolated interactive cell
    return (
        // Bind the React hover and click events. Route the click upwards to App.jsx
        <div onClick={onClick} className="flex items-center gap-2 cursor-pointer hover:bg-[#1E222D] px-2 py-1 rounded transition-colors group">
            {/* Symbol tag: Glows absolute blue when hovered. No artificial suffixes added. */}
            <span className="font-semibold text-white group-hover:text-[#3b82f6] transition-colors">{asset.sym}</span>
            {/* Neutral price anchor format */}
            <span className="text-[#D1D4DC]">
                {asset.price}
            </span>
            {/* The Delta span, shooting appropriate directional arrows and coloring automatically */}
            <span className={`${color} flex items-center`}>
                {(asset.isUp ? '▲' : '▼')} {asset.change}
            </span>
        </div>
    )
}

// Final block exporting the finished modular tape to the Dashboard infrastructure
export default TickerTape;
