import React, { useState } from 'react';
import { ArrowUpRight, TrendingDown } from 'lucide-react';

const ConceptAssetTable = ({ assets, onRowClick }) => {
    // 1. TẠO STATE LƯU ID HOVER
    const [hoveredAssetId, setHoveredAssetId] = useState(null);

    return (
        <div className="w-full h-full flex flex-col pt-2 text-sm">
            
            {/* Table Header */}
            <div className="grid grid-cols-4 gap-4 pb-3 border-b border-[#2d3748] text-concept-muted font-medium px-4">
                <div>Asset</div>
                <div className="text-right">Price</div>
                <div className="text-right">Change</div>
                <div className="text-right">Vol</div>
            </div>

            {/* Table Rows */}
            <div className="flex-1 flex flex-col gap-2 py-2 overflow-y-auto max-h-[600px] custom-scrollbar">
                {assets.map((asset, i) => (
                    <div 
                        key={i} 
                        onClick={() => onRowClick && onRowClick(asset)}
                        onMouseEnter={() => setHoveredAssetId(asset.sym)}
                        onMouseLeave={() => setHoveredAssetId(null)}
                        className={`grid grid-cols-4 gap-4 py-2.5 px-4 rounded-lg bg-[#111926] cursor-pointer group transition-all duration-300
                            border ${hoveredAssetId === asset.sym ? 'border-concept-blue shadow-[0_0_15px_rgba(59,130,246,0.3)]' : 'border-[#2d3748]'}
                        `}
                    >
                        
                        {/* Asset Column */}
                        <div className="flex items-center gap-3">
                            <div className="w-4 h-4 rounded text-[10px] flex items-center justify-center font-bold" style={{ backgroundColor: asset.color + '40', color: asset.color }}>
                                {asset.sym.charAt(0)}
                            </div>
                            <span className="font-semibold text-white tracking-wider group-hover:text-concept-blue transition-colors">{asset.sym}</span>
                        </div>
                        
                        {/* Price Column */}
                        <div className="text-right font-medium text-concept-text">
                            {asset.price}
                        </div>
                        
                        {/* Change Column */}
                        <div className={`text-right font-bold flex items-center justify-end gap-1 ${asset.isUp ? 'text-concept-green' : 'text-concept-red'}`}>
                            {asset.isUp ? '+' : ''}{asset.change}
                        </div>
                        
                        {/* Volume Column */}
                        <div className="text-right text-concept-muted font-mono">
                            {asset.vol}
                        </div>
                    </div>
                ))}
            </div>
            
        </div>
    );
};

export default ConceptAssetTable;
