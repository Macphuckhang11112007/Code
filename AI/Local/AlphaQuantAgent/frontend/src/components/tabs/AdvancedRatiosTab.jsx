import React from 'react';

const renderRatioBox = (label, value) => {
  const valNum = Number(value);
  let colorClass = 'text-[#D1D4DC]'; // default text
  
  if (!isNaN(valNum) && value !== undefined) {
    if (valNum < 1.0) {
      colorClass = 'text-[#F6465D]'; // Red
    } else if (valNum > 1.5) {
      colorClass = 'text-[#0ECB81]'; // Green
    }
  }

  let displayValue = '--';
  if (!isNaN(valNum) && value !== undefined) {
    displayValue = valNum.toFixed(2);
  }

  return (
    <div className="bg-[#1E222D] border border-[#2B3139] p-4 rounded-xl flex flex-col items-center justify-center shadow-lg">
      <span className="text-[#848E9C] text-xs uppercase font-bold tracking-wider mb-2 text-center">{label}</span>
      <span className={`text-xl font-mono ${colorClass}`}>{displayValue}</span>
    </div>
  );
};

const AdvancedRatiosTab = ({ metrics }) => {
  if (!metrics) return null;
  const ratios = metrics;

  return (
    <div className="space-y-6">
      <h3 className="text-lg font-bold text-white border-l-4 border-concept-blue pl-3">Advanced Institutional Ratios</h3>
      <div className="grid grid-cols-2 md:grid-cols-4 gap-4">
        {renderRatioBox('Sharpe Ratio', ratios.sharpe)}
        {renderRatioBox('Sortino Ratio', ratios.sortino)}
        {renderRatioBox('Calmar Ratio', ratios.calmar)}
        {renderRatioBox('Treynor Ratio', ratios.treynor)}
        {renderRatioBox('Information Ratio', ratios.infoRatio)}
        {renderRatioBox('Omega Ratio', ratios.omega)}
        {renderRatioBox('Sterling Ratio', ratios.sterling)}
        {renderRatioBox('Burke Ratio', ratios.burke)}
        {renderRatioBox('K-Ratio', ratios.kRatio)}
        {renderRatioBox('Kappa Ratio', ratios.kappa)}
      </div>
    </div>
  );
};

export default AdvancedRatiosTab;
