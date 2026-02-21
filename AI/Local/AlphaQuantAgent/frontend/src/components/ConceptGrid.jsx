// Import the core React library and Hooks to manage state and application lifecycle.
import React, { useEffect, useState } from 'react';
// Import dynamic vector icons from lucide-react. These act as visual signals.
import { ArrowUpRight, TrendingDown, ExternalLink } from 'lucide-react';
// Import the static Table Asset module to embed inside the screen grid.
import ConceptAssetTable from './ConceptAssetTable';
// Import the Axios HTTP engine to pierce through the Backend API and catch the Event Stream.
import axios from 'axios';
// Import the Navigation command from React Router Core to route seamlessly without Browser reloads.
import { useNavigate } from 'react-router-dom';

// Define the Concept Grid Component - The visual distribution center for the strongest asset movers
const ConceptGrid = ({ onAssetClick }) => {
    // Initialize State holding an empty Array to receive Dynamic Real Data from the Backend Server
    const [assets, setAssets] = useState([]);
    // Initialize the page space transition trigger
    const navigate = useNavigate();

    // Set up the Effect Hook to fire the Data Fetch sequence strictly once when the Grid mounts
    useEffect(() => {
        // Encase the asynchronous command inside a static function to prevent Main Thread blocking
        const fetchTopMovers = async () => {
            // Steel-clad Try Catch Truth Block - Prevent web collapse if network cuts out mid-way
            try {
                // Launch Axios network penetrating the Python Backend API port
                const response = await axios.get('http://127.0.0.1:8000/api/quant/market/top_movers');
                // Verify 2-tier Funnel Barrier: Response must contain data body and Server Status must flag 'success'
                if (response.data && response.data.status === 'success') {
                    // Open valve to Drop fresh Historical Stock Data into the React State bag (Grid auto re-renders)
                    setAssets(response.data.data);
                }
            } catch (error) {
                // Emergency Net catching any Variance due to Time-out or Backend Code collapse
                console.error("Critical: Severed Nerve Connector API Pulling Top Movers Grid", error);
            }
        };
        // Release detonator running Data Fetch function immediately
        fetchTopMovers();
    }, []); // Empty Axis Lock Array - Instructs firing useEffect exactly one single lifecycle.

    // Initialize 2 empty buffer Data variables for 2 Interface Branches 
    let cardAssets = [];
    let tableAssets = [];
    
    // Cage the Dynamic Data Array Slicing algorithm inside Try-Catch Zone to block Out of Bounds Exceptions
    try {
        // Inventory Check: Only slice if Array List Actually Holds Asset reserves (Escape Null Map Scenario)
        if (assets && assets.length > 0) {
            // Neatly slice the first 9 pieces (Top 0 to 8) to Install into 9 Left Mini Chart Component Cards
            cardAssets = assets.slice(0, 9);
            
            // Absolutely Destroy DEAD MOCKS - Render the rest fully!
            tableAssets = assets.length > 9 ? assets.slice(9) : assets;
        }
    } catch (sliceError) {
        // Error Catching Funnel If Array Length returns Torn Data Structure 
        console.error("Lethal Error while Mathematically Molding Column Form Grid Array Slice:", sliceError);
    }

    // Release HTML Grid Architecture Block Render for the Screen
    return (
        // Core Wrapper Maintaining standard Max 6XL width Centered Exactly on Monitor
        <div className="w-full max-w-6xl flex flex-col items-center">
            
            {/* The Main Dark Panel - Curved Frosted Glass Board Acting as Data Backdrop */}
            <div className="w-full bg-[#151c28]/80 backdrop-blur-2xl border border-white/5 rounded-3xl p-8 shadow-[0_30px_80px_-20px_rgba(0,0,0,0.8)] relative overflow-hidden">
                
                {/* Abstract Top Blue Glow - Plasma Light System Emitting Aurora at Top Machine Edge */}
                <div className="absolute top-0 left-1/4 right-1/4 h-[2px] bg-gradient-to-r from-transparent via-concept-blue/50 to-transparent blur-[1px]"></div>
                {/* Horizontal Cut Frame Firing Neon Sharp Piercing Led Cable */}
                <div className="absolute top-0 left-1/3 right-1/3 h-[1px] bg-concept-blue/80 shadow-[0_0_20px_rgba(59,130,246,1)]"></div>
                
                {/* Abstract Bottom Blue Glow - Hazy Glow System Covering Deep Trench Under Support Frame */}
                <div className="absolute bottom-0 left-1/4 right-1/4 h-[2px] bg-gradient-to-r from-transparent via-concept-blue/30 to-transparent blur-[1px]"></div>

                {/* Panel Header - Title Peak Laying Out Core Live Trade Block */}
                <div className="mb-6">
                    {/* Fire Ultra Wide Font Text Hailing Extreme Oscillation Asset Circuit Codes */}
                    <h2 className="text-xl font-bold text-white tracking-wide">Top Moving Assets & Capital Heat</h2>
                </div>

                {/* Two-pane Layout inside the Panel - Partition Wall Block Structure Grappling Left/Right */}
                <div className="flex flex-col lg:flex-row gap-10">
                    
                    {/* Left Pane: 3x3 Mini Chart Grid - Left Wing Zone Spraying Grid of 9 Flashing Micro-Circuit Cards */}
                    <div className="flex-1 grid grid-cols-3 gap-3">
                        {/* Array Grinding Loop Function Re-Packaging Each Object Into 1 Custom HTML Tag Component Segment */}
                        {cardAssets.map((asset, i) => (
                            <AssetCard 
                                key={i} // Poison Key Blocking React Messy Corrupted Re-render Scanning 
                                asset={asset} // Channel Flow Energy of Each Asset Straight Down to Child Component Core 
                                onClick={() => onAssetClick && onAssetClick(asset)} // Signal Transmitter Lock Blocking Unmount 
                            />
                        ))}
                    </div>

                    {/* Right Pane: Asset Table List - Right Axis Plugging List Column Near Margin Wall */}
                    <div className="w-full lg:w-[400px] shrink-0 border-l border-[#2d3748] pl-2 hidden md:block">
                        {/* Summon Support Sub-Component Responsible For Wrapping Fresh Graphic Data 6 Vertical Columns Board */}
                        <ConceptAssetTable assets={tableAssets} onRowClick={(asset) => onAssetClick && onAssetClick(asset)} />
                    </div>

                </div>
            </div>

            {/* Launch Button below the panel - Press Trigger Launch Escaping Journey To Markets */}
            <div className="mt-10">
                {/* Plant Route Transition Navigate Replace Trigger Link Route Clamp */}
                <button onClick={() => navigate('/markets')} className="px-10 py-3 rounded-full border border-concept-blue/60 text-concept-blue font-bold tracking-widest text-sm hover:bg-concept-blue hover:text-white transition-all duration-300 hover:scale-105 active:scale-95 shadow-[0_0_25px_rgba(59,130,246,0.3)] hover:shadow-[0_0_35px_rgba(59,130,246,0.6)] uppercase">
                    Initialize Full Market View
                </button>
            </div>
        </div>
    );
};

// Subcomponent Particle Structure Rectangle Shape Sneaking Dropping/Sliding Stock Code
const AssetCard = ({ asset, onClick }) => {
    // Generate chaotic SVG paths for fake sparklines (Auto Generate Custom Embossed Path Mocking Chart Diagonal Line Enlivening UI)
    const pathUp = "M 0,30 L 20,25 L 40,35 L 60,15 L 80,20 L 100,5";  // Vector Pushing Mathematical Upward Integral Accumulation
    const pathDown = "M 0,5 L 20,15 L 40,10 L 60,30 L 80,25 L 100,35"; // Vector Tornado Vacuum Bottom Fall Hole Steep Dive

    // Guard Frame Wrapping Card Roof With Accompanying Hover Glass Drop Shadow CSS
    return (
        <div 
            onClick={onClick}
            // Aggregation of TailWind Super Classes Injecting Speed Utility For Flaring Curved Box Shadow
            className={`p-4 rounded-xl border transition-all cursor-pointer bg-[#111926] relative overflow-hidden group hover:-translate-y-1 hover:shadow-[0_0_20px_rgba(59,130,246,0.2)] border-[#2d3748] hover:border-concept-blue`}>
            
            {/* Soft inner glow on highlight - Backlight Crushing Background Twilight Flickering Mirror Stone Surface Emitting Dim Glass Light */}
            <div className="absolute inset-0 bg-concept-blue/5 pointer-events-none opacity-0 group-hover:opacity-100 transition-opacity"></div>

            {/* Symbol Pop Shape Block Displaying Hidden Link Outlet Door At Escape Wing Corner Color Dyed */}
            <ExternalLink size={14} className="absolute top-4 right-4 text-white/0 group-hover:text-white/50 transition-colors" />

            {/* Header: Name and Ticker - Prominent Block Displaying Asset Symbol Code Embedded Proudly */}
            <div className="mb-4 relative z-10 w-[80%] truncate">
                <span className={`text-[11px] font-bold tracking-widest uppercase transition-colors text-concept-muted group-hover:text-white`}>
                    {asset.sym}
                </span>
            </div>

            {/* Sparkline Visual - The Miniature Graph Container Sketching Pulse Wave Trajectories */}
            <div className="h-12 w-full mt-2 relative z-10">
                {/* Baseline Vector Grid Chart Rendering Mathematical SVG Path Scaling Fluidly */}
                <svg className="w-full h-full" viewBox="0 0 100 40" preserveAspectRatio="none">
                    <path 
                        // Toggle Filtering Math Trap: Direct Upward Path Array for Bulls / Drop Down Path for Bears
                        d={asset.isUp ? pathUp : pathDown} 
                        // Empties Core Body To Make Hollow Line Only Graph Edge Solid
                        fill="none" 
                        // Sucks Specific Monolithic Hex String Pushed Down From Backend Matrix 
                        stroke={asset.color} 
                        strokeWidth="2.5" // Force Thick Metal Line Flattening Rugged Axis Corner Stabilizing Visual Intensity 
                        strokeLinecap="round" // Blunt Bullet Strike Edge Rounding Sharp Vertex Edges
                        strokeLinejoin="round" 
                        className="opacity-90 transition-all group-hover:opacity-100" // Light Guarding Resist Blindness Effect Fading Smooth Transitions Without Tear
                        // Shadows Infusing Ground Depth Cloud Layer Giant Spreading Filtering Drop Gradient Under Tree Pure Roots Base 
                        style={{ filter: `drop-shadow(0px 6px 6px ${asset.color}60)` }} 
                    />
                </svg>
            </div>
        </div>
    )
}

// Final Axis Fastener Transmitting Total Master Grid Component Towards External React App Pipeline Receiving Core (Firing Live Export) Attached To Parent Screen Frame Dropping Capability 
export default ConceptGrid;
