import React from 'react';

const renderMetricBox = (label, value, type = 'number', isTradeStat = false) => {
  const valNum = Number(value);
  let colorClass = 'text-[#D1D4DC]'; // default text
  
  if (!isNaN(valNum) && value !== 0 && value !== undefined) {
    if (isTradeStat && label.includes('Win Rate')) {
        colorClass = valNum > 50 ? 'text-[#0ECB81]' : 'text-[#F6465D]'; // green > 50%, red otherwise
    } else if (type !== 'money-cost') {
        colorClass = valNum > 0 ? 'text-[#0ECB81]' : 'text-[#F6465D]'; // green / red
    }
  }

  // Value formatting
  let displayValue = '--';
  if (!isNaN(valNum) && value !== undefined) {
    if (type === 'percent') {
      displayValue = `${valNum.toFixed(2)}%`;
    } else if (type === 'money-cost') {
      const isNegative = valNum < 0;
      const absVal = Math.abs(valNum);
      const formattedNumber = absVal.toLocaleString('en-US', { minimumFractionDigits: 2, maximumFractionDigits: 2 });
      displayValue = `-$${formattedNumber}`; // strict rule: must have -$
      colorClass = 'text-[#F6465D] font-bold'; // Strict rule: dark red 
    } else {
      displayValue = valNum.toFixed(2); // standard 2 decimals
    }
  }

  return (
    <div className="bg-[#1E222D] border border-[#2B3139] p-4 rounded-xl flex flex-col items-center justify-center shadow-lg">
      <span className="text-[#848E9C] text-xs uppercase font-bold tracking-wider mb-2 text-center">{label}</span>
      <span className={`text-xl font-mono ${colorClass}`}>{displayValue}</span>
    </div>
  );
};

const ExecutionTab = ({ metrics }) => {
  if (!metrics) return null;
  const { tradeStats, microstructure } = metrics;

  return (
    <div className="space-y-6">
      <h3 className="text-lg font-bold text-white border-l-4 border-concept-blue pl-3">Trade Statistics</h3>
      <div className="grid grid-cols-2 md:grid-cols-4 lg:grid-cols-5 gap-4">
        {renderMetricBox('Win Rate', tradeStats?.winRate, 'percent', true)}
        {renderMetricBox('Loss Rate', tradeStats?.lossRate, 'percent')}
        {renderMetricBox('Profit Factor', tradeStats?.profitFactor, 'number')}
        {renderMetricBox('Risk/Reward Ratio', tradeStats?.riskRewardRatio, 'number')}
        {renderMetricBox('Payoff Ratio', tradeStats?.payoffRatio, 'number')}
        {renderMetricBox('Total Trades', tradeStats?.totalTrades, 'number')}
        {renderMetricBox('Max Cons. Wins', tradeStats?.maxConsecutiveWins, 'number')}
        {renderMetricBox('Max Cons. Losses', tradeStats?.maxConsecutiveLosses, 'number')}
        {renderMetricBox('Avg Hold Long', tradeStats?.avgHoldingTimeLong, 'number')}
        {renderMetricBox('Avg Hold Short', tradeStats?.avgHoldingTimeShort, 'number')}
      </div>

      <h3 className="text-lg font-bold text-white border-l-4 border-[#F6465D] pl-3 mt-6">Microstructure & Frictions</h3>
      <div className="grid grid-cols-2 md:grid-cols-4 lg:grid-cols-5 gap-4">
        {renderMetricBox('Total Slippage', microstructure?.totalSlippageCost, 'money-cost')}
        {renderMetricBox('Execution Shortfall', microstructure?.executionShortfall, 'money-cost')}
        {renderMetricBox('Total Commissions', microstructure?.totalCommissions, 'money-cost')}
        {renderMetricBox('Margin Utilization', microstructure?.marginUtilization, 'percent')}
        {renderMetricBox('Long/Short Ratio', microstructure?.longShortRatio, 'number')}
        {renderMetricBox('Order Book Imbalance', microstructure?.obi, 'number')}
        {renderMetricBox('VPIN', microstructure?.vpin, 'number')}
        {renderMetricBox('Spread Variance', microstructure?.bidAskSpreadVar, 'number')}
      </div>
    </div>
  );
};

export default ExecutionTab;
