// Activate React Foundation And State Collection Steps
import React, { useEffect, useState } from 'react';
// Summon Dynamic API Door Knocker Tool Axios
import axios from 'axios';

// Define Core Component Top Screen Peak Bar (Holding Horizontal Scroll Asset List)
const ConceptTopBar = ({ isForcePaused, onAssetClick }) => {
    // Fresh Stock Code Chest Station (Comprehensive Mock Denial)
    const [assets, setAssets] = useState([]);

    // Hook Effect Catch Running Data Extract Process When Bar Just Molds DOM Base
    useEffect(() => {
        // Plant Stake Binding Component Active State
        let isMounted = true;
        
        // Corral Try Mesh Enclosure Trapping Network Server Fracture Breaks
        const fetchTopMovers = async () => {
            try {
                // Eject Axios Pull Force Destroying V1 Replacing With Quant Core Opening Top Movers Heart
                const response = await axios.get('http://127.0.0.1:8000/api/quant/market/top_movers');
                // Thread Check Funnel Cable: If Component Is Still Alive Worthy Then Pump Ammo (Prevent RAM Variable Leak)
                if (isMounted && response.data && response.data.status === 'success') {
                    // Suppress Life/Death Market Data Joints Pouring Into Assets Physical State 
                    setAssets(response.data.data);
                }
            } catch (error) {
                // Cage Short Circuit Request Spraying Console Forbidding Main Interface Slip Break
                console.error("Harsh Death Network Error Extracting Topbar Data:", error);
            }
        };
        
        // Start Engine Running Get Filter Top First Time Zero Delay
        fetchTopMovers();
        // (Silent Note Dropping Polling Because Engine Opens Separate WebSocket/Fragment Per Design)
        
        // Clean Broadcast Station When Component Block Falls Abandoning DOM
        return () => { isMounted = false; };
    }, []);

    // Link Loop Chain Coupling Array Double X2 In Generating Marquee Scroll Process Overflowing Monitor Frame Smooth Connecting Stairs Sealing Watch Belt
    const allAssets = [...assets, ...assets];

    // Export CSS Mesh Form Displaying React
    return (
        // Peak Frame Box Fastening Height 12 Navel (h-12) Tightly Covering Floating Base Frosting Black Ghost Layer Onto Screen Peak (z-50) Interweaving Drop Shadow
        <div className="h-12 w-full shrink-0 overflow-hidden border-b border-[#2d3748] bg-[#0f1420]/90 backdrop-blur-md sticky top-0 z-50 shadow-md flex items-center">
            
            {/* The Scrolling Container - Gushing Hole Roll Displaying Stock Or Waiting Error White Moon Loading */}
            {assets.length > 0 ? (
                // If Array Exists Then Bloom Flex Block Infinite Span And Assemble Dynamic Css Lock For Marquee Belt Running Text (Variable Filter When Only Horizontal Swipe Stiff Freeze/Hover Swipe Stop Freeze)
                <div className={`flex w-max animate-custom-marquee min-w-full ${isForcePaused ? '[animation-play-state:paused]' : 'hover:[animation-play-state:paused]'}`}>
                    {/* Chop Tree Releasing Every Single Assset Code Firing Attaching Into Sub Component Face Area Punching Separate Code Ascending Link Transmission */}
                    {allAssets.map((asset, idx) => (
                        // Unhook Eject Click Attack Leading Driving This Path Transmitting Quanta App Where Mandate Page URL Jump Receives
                        <AssetTicker key={idx} asset={asset} onClick={() => onAssetClick && onAssetClick(asset)} />
                    ))}
                </div>
            ) : (
                // Scenario Disconnected Network Load/API Miss Translation Not Running Or Ran Out Map Forced Rest Death Load Temporary Ghost Shadow Text Fade Blur
                <div className="flex w-full items-center justify-center opacity-50">
                    <span className="text-concept-muted text-xs tracking-widest uppercase">Alpha Signal Station Bridging Streaming Port...</span>
                </div>
            )}

        </div>
    )
}

// Personal Microchip Text Card Component Board For Each Small Item Elongating On Belt Towards End
const AssetTicker = ({asset, onClick}) => (
    // Long Bundle Crooked Section Vertical Axis Zigzag Border-r, Stuffing Reflect Hover Changing Tiny White Background Processing Light When Mouse Drags (Hover White Pure)
    <div 
        onClick={onClick}
        className="flex items-center justify-center gap-2 cursor-pointer transition-colors hover:bg-white/5 w-[10vw] min-w-[150px] shrink-0 border-r border-[#2d3748] last:border-r-0 h-10 group relative"
    >
        {/* Frost Film Glass Layer Absorbing Eye Gaze Blue Color Only When Pointing Into This Card (Group Hover Only Pulses Suppressed Blue Ray) */}
        <div className="absolute inset-0 bg-concept-blue/0 group-hover:bg-concept-blue/5 transition-colors pointer-events-none"></div>

        {/* Double Color Box Character Stroke Eye Corner Representing Coin Code Copper (Draw Icon Following Backend Base Color Pushing Blue, Purple Yellow Toss All Peak Shock Depending Coin) */}
        <div className="w-4 h-4 rounded-[4px] flex items-center justify-center text-[9px] border border-white/10" style={{backgroundColor: asset.bg, color: asset.color}}>
            {asset.sym.charAt(0)}
        </div>
        {/* Imprint Ticker Brand Name Engrave Cover Unexposed Stroke */}
        <span className="text-concept-muted text-[11px] font-bold tracking-widest uppercase truncate">{asset.sym}: <span className="text-white ml-1">{asset.price}</span></span>
        {/* Spray Leadership Bounce Degree Ascend Or Fall (IsUp Green Fresh Slant - While Dropping Then Frame Crack Red) */}
        <span className={`text-[10px] font-bold tracking-widest ${asset.isUp ? 'text-concept-green' : 'text-concept-red'}`}>({asset.change})</span>
    </div>
)

// Shut Door Axis Output Distribute Translation File Sending Out App Summon Utilize Direct Attach 
export default ConceptTopBar;
