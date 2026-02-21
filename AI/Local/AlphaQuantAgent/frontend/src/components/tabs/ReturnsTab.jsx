import React from 'react';
import {
  BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, Cell
} from 'recharts';

const renderMetricBox = (label, value, type = 'number') => {
  const valNum = Number(value);
  let colorClass = 'text-[#D1D4DC]'; // default text
  
  if (!isNaN(valNum) && value !== 0 && value !== undefined) {
    colorClass = valNum > 0 ? 'text-[#0ECB81]' : 'text-[#F6465D]'; // green / red
  }

  // Value formatting
  let displayValue = '--';
  if (!isNaN(valNum) && value !== undefined) {
    if (type === 'percent') {
      displayValue = `${valNum.toFixed(2)}%`;
    } else if (type === 'money') {
      const isNegative = valNum < 0;
      const absVal = Math.abs(valNum);
      const formattedNumber = absVal.toLocaleString('en-US', { minimumFractionDigits: 2, maximumFractionDigits: 2 });
      displayValue = isNegative ? `-$${formattedNumber}` : `$${formattedNumber}`;
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

const CustomTooltip = ({ active, payload, label }) => {
  if (active && payload && payload.length) {
    return (
      <div className="bg-[#1E222D] border border-[#2B3139] p-3 rounded-lg shadow-xl text-[#EAECEF] text-sm font-mono z-50">
        <p className="font-bold mb-1 border-b border-[#2B3139] pb-1">{label}</p>
        <div style={{ color: payload[0].payload.value >= 0 ? '#0ECB81' : '#F6465D' }}>
          Value: {payload[0].value.toFixed(2)}%
        </div>
      </div>
    );
  }
  return null;
};

const ReturnsTab = ({ metrics, chartData }) => {
  if (!metrics) return null;
  const returns = metrics;

  return (
    <div className="space-y-6">
      <h3 className="text-lg font-bold text-white border-l-4 border-concept-blue pl-3">Profitability & Returns</h3>
      
      {/* THE LIVE PRICE CHART INJECTED FROM PHASE 3 */}
      {chartData && chartData.length > 0 && (
          <div className="bg-[#1E222D] border border-[#2B3139] rounded-xl p-4 shadow-2xl h-[350px] mb-6">
              <h3 className="text-sm font-bold text-[#848E9C] uppercase tracking-wider mb-2">Live Price Action (Last 96 Candles)</h3>
              <ResponsiveContainer width="100%" height="90%">
                  <LineChart data={chartData} margin={{ top: 10, right: 30, left: -10, bottom: 0 }}>
                      <CartesianGrid stroke="#2B2B36" strokeDasharray="3 3" vertical={false} />
                      <XAxis dataKey="time" stroke="#848E9C" fontSize={10} tickLine={false} axisLine={false} tickFormatter={(val) => val ? val.split(' ')[1] : ''} />
                      <YAxis stroke="#848E9C" fontSize={10} tickLine={false} axisLine={false} domain={['auto', 'auto']} />
                      <Tooltip content={<CustomTooltip />} />
                      <Line type="step" dataKey="close" name="Close Price" stroke="#F0B90B" strokeWidth={2} dot={false} isAnimationActive={false} />
                  </LineChart>
              </ResponsiveContainer>
          </div>
      )}

      {/* 8 Metric Cards Grid */}
      <div className="grid grid-cols-2 md:grid-cols-4 gap-4">
        {renderMetricBox('Cumulative Return', returns.cumulative, 'percent')}
        {renderMetricBox('CAGR', returns.cagr, 'percent')}
        {renderMetricBox('YTD Return', returns.ytd, 'percent')}
        {renderMetricBox('Arithmetic Mean', returns.arithmeticMean, 'number')}
        {renderMetricBox('Geometric Mean', returns.geometricMean, 'number')}
        {renderMetricBox('Gross Profit', returns.grossProfit, 'money')}
        {renderMetricBox('Gross Loss', returns.grossLoss, 'money')}
        {renderMetricBox('Net Profit', returns.netProfit, 'money')}
      </div>

      {/* Rolling Returns Bar Chart */}
      {returns.rollingReturns && returns.rollingReturns.length > 0 && (
        <div className="bg-[#1E222D] border border-[#2B3139] rounded-xl p-4 shadow-2xl h-[350px] mt-6">
          <h3 className="text-sm font-bold text-[#848E9C] uppercase tracking-wider mb-2">Rolling Returns Trajectory (%)</h3>
          <ResponsiveContainer width="100%" height="90%">
            <BarChart
              data={returns.rollingReturns}
              margin={{ top: 10, right: 30, left: -20, bottom: 5 }}
            >
              <CartesianGrid stroke="#2B2B36" strokeDasharray="3 3" vertical={false} />
              <XAxis dataKey="period" stroke="#848E9C" fontSize={12} tickLine={false} axisLine={false} />
              <YAxis stroke="#848E9C" fontSize={12} tickLine={false} axisLine={false} tickFormatter={(val) => `${val}%`} />
              <Tooltip content={<CustomTooltip />} cursor={{fill: '#2B3139', opacity: 0.4}} />
              <Bar dataKey="value" radius={[4, 4, 0, 0]} isAnimationActive={false}>
                {returns.rollingReturns.map((entry, index) => (
                  <Cell key={`cell-${index}`} fill={entry.value >= 0 ? '#0ECB81' : '#F6465D'} />
                ))}
              </Bar>
            </BarChart>
          </ResponsiveContainer>
        </div>
      )}
    </div>
  );
};

export default ReturnsTab;
