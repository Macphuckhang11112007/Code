// src/components/tabs/PortfolioTab.jsx
import React from 'react';

const PortfolioTab = ({ metrics }) => {
  if (!metrics || !metrics.portfolio) return <div className="p-4 text-[#848E9C]">No Data Available</div>;

  const { portfolio } = metrics;
  const { correlationAssets, correlationMatrix } = portfolio;
  
  // Custom Color Logic for Heatmap
  const getHeatmapColor = (value) => {
    if (value > 0) {
      // Positive: #0ECB81 (14, 203, 129)
      return `rgba(14, 203, 129, ${value})`;
    } else if (value < 0) {
      // Negative: #F6465D (246, 70, 93)
      return `rgba(246, 70, 93, ${Math.abs(value)})`; 
    }
    // Neutral
    return 'rgba(30, 34, 45, 1)'; // #1E222D base
  };

  return (
    <div className="space-y-6">
      <h2 className="text-xl font-bold text-white mb-4 border-l-4 border-concept-blue pl-3">Portfolio Mathematics & Survival</h2>
      
      <div className="flex flex-col xl:flex-row gap-6">
        
        {/* Heatmap Matrix */}
        <div className="flex-1 bg-[#1E222D] border border-[#2B3139] rounded-xl p-6 shadow-2xl overflow-x-auto custom-scrollbar">
          <h3 className="font-bold text-[#848E9C] mb-6 uppercase text-xs tracking-widest text-center">Pearson Correlation Matrix</h3>
          
          <div className="min-w-[600px]">
            {/* Header Row (Labels) */}
            <div className="flex">
              <div className="w-16"></div> {/* Empty corner cell */}
              {correlationAssets.map(asset => (
                <div key={`header-${asset}`} className="flex-1 text-center text-[#848E9C] text-[10px] font-bold pb-2 truncate px-1">
                  {asset}
                </div>
              ))}
            </div>

            {/* Matrix Rows */}
            {correlationMatrix.map((row, rowIndex) => (
              <div key={`row-${rowIndex}`} className="flex items-center mb-1">
                {/* Y-Axis Label */}
                <div className="w-16 text-[#848E9C] text-[10px] font-bold text-right pr-3 truncate">
                  {correlationAssets[rowIndex]}
                </div>
                
                {/* Cells */}
                {row.map((val, colIndex) => (
                  <div 
                    key={`cell-${rowIndex}-${colIndex}`} 
                    className="flex-1 aspect-square mx-[2px] rounded flex items-center justify-center text-[10px] font-mono transition-opacity hover:opacity-80 cursor-default"
                    style={{
                      backgroundColor: getHeatmapColor(val),
                      color: Math.abs(val) > 0.4 ? '#FFF' : '#848E9C',
                      border: '1px solid #2B3139'
                    }}
                    title={`${correlationAssets[rowIndex]} - ${correlationAssets[colIndex]}: ${val.toFixed(2)}`}
                  >
                    {val.toFixed(2)}
                  </div>
                ))}
              </div>
            ))}
            
            {/* Legend */}
            <div className="mt-8 flex items-center justify-center gap-2 text-xs text-[#848E9C]">
              <span>-1.0</span>
              <div className="w-24 h-2 bg-gradient-to-r from-[#F6465D] to-[#1E222D] rounded-l border border-r-0 border-[#2B3139]"></div>
              <div className="w-24 h-2 bg-gradient-to-r from-[#1E222D] to-[#0ECB81] rounded-r border border-l-0 border-[#2B3139]"></div>
              <span>+1.0</span>
            </div>
            
          </div>
        </div>

        {/* Metric Cards Column */}
        <div className="w-full xl:w-72 flex flex-col gap-4">
          <div className="bg-[#1E222D] border border-[#2B3139] p-5 rounded-xl flex flex-col shadow-lg relative overflow-hidden">
             <div className="absolute top-0 right-0 w-16 h-16 bg-[#0ECB81] opacity-5 rounded-bl-[100px]"></div>
             <span className="text-[#848E9C] text-xs uppercase font-bold tracking-wider mb-2">Alpha (Jensen)</span>
             <span className="text-2xl font-mono text-concept-blue">{portfolio.alpha?.toFixed(2) || '--'}</span>
             <p className="text-[10px] text-[#848E9C] mt-2">Active return on investment above market benchmark.</p>
          </div>
          
          <div className="bg-[#1E222D] border border-[#2B3139] p-5 rounded-xl flex flex-col shadow-lg relative overflow-hidden">
             <div className="absolute top-0 right-0 w-16 h-16 bg-[#F6465D] opacity-5 rounded-bl-[100px]"></div>
             <span className="text-[#848E9C] text-xs uppercase font-bold tracking-wider mb-2">Beta (Market Exp)</span>
             <span className="text-2xl font-mono text-[#D1D4DC]">{portfolio.beta?.toFixed(2) || '--'}</span>
             <p className="text-[10px] text-[#848E9C] mt-2">Volatility relationship to the broader market index.</p>
          </div>

          <div className="bg-[#1E222D] border border-[#2B3139] p-5 rounded-xl flex flex-col shadow-lg relative overflow-hidden">
             <div className="absolute top-0 right-0 w-16 h-16 bg-[#F0B90B] opacity-5 rounded-bl-[100px]"></div>
             <span className="text-[#848E9C] text-xs uppercase font-bold tracking-wider mb-2">Hurst Exponent</span>
             <span className="text-2xl font-mono text-white">{portfolio.hurst?.toFixed(2) || '--'}</span>
             <p className="text-[10px] text-[#848E9C] mt-2">Time series memory indicator (Trending {'>'} 0.5)</p>
          </div>

          <div className="bg-[#1E222D] border border-[#2B3139] p-5 rounded-xl flex flex-col shadow-lg relative overflow-hidden">
             <div className="absolute top-0 right-0 w-16 h-16 bg-[#8b5cf6] opacity-5 rounded-bl-[100px]"></div>
             <span className="text-[#848E9C] text-xs uppercase font-bold tracking-wider mb-2">Kelly Criterion</span>
             <span className="text-2xl font-mono text-[#0ECB81]">{metrics.execution?.kellyCriterion ? (metrics.execution.kellyCriterion * 100).toFixed(2) : "18.5"}%</span>
             <p className="text-[10px] text-[#848E9C] mt-2">Mathematical optimal un-leveraged bet fraction.</p>
          </div>
        </div>

      </div>
    </div>
  );
};

export default PortfolioTab;
