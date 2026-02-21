import React from 'react';

const renderMetricBox = (label, value, isPercent = false) => {
  const valNum = Number(value);
  let colorClass = 'text-[#D1D4DC]'; // default text
  
  if (!isNaN(valNum) && value !== undefined) {
    colorClass = valNum >= 0 ? 'text-[#0ECB81]' : 'text-[#F6465D]'; // green / red
  }

  // Value formatting
  let displayValue = '--';
  if (!isNaN(valNum) && value !== undefined) {
    if (isPercent) {
      displayValue = `${valNum.toFixed(2)}%`;
    } else {
      displayValue = valNum.toFixed(4);
    }
  }

  return (
    <div className="bg-[#1E222D] border border-[#2B3139] p-4 rounded-xl flex flex-col items-center justify-center shadow-lg">
      <span className="text-[#848E9C] text-xs uppercase font-bold tracking-wider mb-2 text-center">{label}</span>
      <span className={`text-xl font-mono ${colorClass}`}>{displayValue}</span>
    </div>
  );
};

const PortfolioSurvivalTab = ({ metrics }) => {
  if (!metrics) return null;
  const { macro, survival, correlationMatrix } = metrics;
  
  // Handle both Object (from Pandas) and fallbacks
  let assets = ['A1', 'A2', 'A3', 'A4', 'A5'];
  let matrix = correlationMatrix;

  if (correlationMatrix && !Array.isArray(correlationMatrix)) {
    assets = Object.keys(correlationMatrix);
    matrix = assets.map(rowAsset => 
      assets.map(colAsset => correlationMatrix[rowAsset][colAsset] || 0)
    );
  } else if (!matrix || !matrix.length) {
    matrix = [[1, 0, 0, 0, 0], [0, 1, 0, 0, 0], [0, 0, 1, 0, 0], [0, 0, 0, 1, 0], [0, 0, 0, 0, 1]];
  }

  const getHeatmapColor = (value) => {
    // Logic Màu Nền: Nếu = 1 (Xanh đậm), 0.5 (Xanh nhạt), 0 (Đen/Xám), -0.5 (Đỏ nhạt), -1 (Đỏ rực)
    if (value >= 0.8) return 'bg-[#0ECB81] text-black font-bold'; // Dark green
    if (value >= 0.3) return 'bg-[rgba(14,203,129,0.4)] text-[#EAECEF]'; // Light green
    if (value >= -0.2 && value < 0.3) return 'bg-[#1E222D] text-[#848E9C]'; // Dark/Gray
    if (value >= -0.8 && value < -0.2) return 'bg-[rgba(246,70,93,0.4)] text-[#EAECEF]'; // Light red
    return 'bg-[#F6465D] text-white font-bold'; // Dark red
  };

  return (
    <div className="grid grid-cols-1 xl:grid-cols-2 gap-6 w-full h-full">
      {/* Left Panel: Macro & Survival Stats */}
      <div className="space-y-6">
        <h3 className="text-lg font-bold text-white border-l-4 border-concept-blue pl-3">Macro & Portfolio Math</h3>
        <div className="grid grid-cols-2 lg:grid-cols-4 gap-4">
          {renderMetricBox('Alpha', macro?.alpha, true)}
          {renderMetricBox('Beta', macro?.beta, false)}
          {renderMetricBox('R-Squared', macro?.rSquared, false)}
          {renderMetricBox('Tracking Error', macro?.trackingError, true)}
          {renderMetricBox('Hurst Exponent', macro?.hurstExponent, false)}
          {renderMetricBox('ADF Test', macro?.adfTest, false)}
          {renderMetricBox('Ljung-Box', macro?.ljungBox, false)}
          {renderMetricBox('Turnover Rate', macro?.turnoverRate, false)}
        </div>

        <h3 className="text-lg font-bold text-white border-l-4 border-concept-blue pl-3 mt-8">Survival Diagnostics</h3>
        <div className="grid grid-cols-1 lg:grid-cols-3 gap-4">
          {renderMetricBox('Kelly Criterion', survival?.kellyCriterion, true)}
          {renderMetricBox('Risk of Ruin', survival?.riskOfRuin, true)}
          {renderMetricBox('Expectancy', survival?.expectancy, false)}
        </div>
      </div>

      {/* Right Panel: Custom Heatmap Component */}
      <div>
        <h3 className="text-lg font-bold text-white border-l-4 border-[#F0B90B] pl-3 mb-6">Cross-Asset Correlation Matrix (Dynamic)</h3>
        <div className="bg-[#0b0e11] border border-[#2B3139] rounded-xl p-4 shadow-2xl h-auto overflow-x-auto custom-scrollbar max-h-[500px] overflow-y-auto">
          <div className="min-w-max">
            {/* Headers */}
            <div className="grid gap-1 mb-1" style={{ gridTemplateColumns: `minmax(80px, 1fr) repeat(${assets.length}, minmax(60px, 1fr))` }}>
              <div className="p-2 font-bold text-[#848E9C] text-sm text-center">Asset</div>
              {assets.map((asset, i) => (
                <div key={i} className="p-2 font-bold text-[#EAECEF] text-xs text-center truncate" title={asset}>
                  {asset.split('_')[0] || asset}
                </div>
              ))}
            </div>

            {/* Matrix Rows */}
            {matrix.map((row, rowIndex) => (
              <div key={`row-${rowIndex}`} className="grid gap-1 mb-1" style={{ gridTemplateColumns: `minmax(80px, 1fr) repeat(${assets.length}, minmax(60px, 1fr))` }}>
                {/* Y-Axis Label */}
                <div className="p-2 font-bold text-[#EAECEF] text-xs text-center bg-[#1E222D] rounded flex items-center justify-center truncate" title={assets[rowIndex]}>
                  {assets[rowIndex]?.split('_')[0] || assets[rowIndex]}
                </div>
                {/* Data Cells */}
                {row.map((val, colIndex) => (
                  <div 
                    key={`cell-${rowIndex}-${colIndex}`} 
                    className={`p-2 rounded text-xs text-center flex items-center justify-center transition-all hover:opacity-80 cursor-default ${getHeatmapColor(val)}`}
                    title={`Corr(${assets[rowIndex]}, ${assets[colIndex]}) = ${val}`}
                  >
                    {Number(val).toFixed(2)}
                  </div>
                ))}
              </div>
            ))}
          </div>
        </div>
      </div>

    </div>
  );
};

export default PortfolioSurvivalTab;
