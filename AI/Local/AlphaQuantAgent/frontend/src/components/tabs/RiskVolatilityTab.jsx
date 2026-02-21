import React from 'react';
import {
  BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer
} from 'recharts';

const renderMetricBox = (label, value, isDrawdownOrTailRisk = false, isDays = false) => {
  const valNum = Number(value);
  let colorClass = 'text-[#D1D4DC]'; // default text
  
  if (!isNaN(valNum) && value !== undefined) {
    if (isDrawdownOrTailRisk) {
      colorClass = 'text-[#F6465D]'; // Red
    } else {
      colorClass = valNum >= 0 ? 'text-[#0ECB81]' : 'text-[#F6465D]';
    }
  }

  // Value formatting
  let displayValue = '--';
  if (!isNaN(valNum) && value !== undefined) {
    if (isDays) {
      displayValue = `${valNum} Days`;
    } else {
      displayValue = `${valNum.toFixed(2)}%`;
    }
  }

  return (
    <div className="bg-[#1E222D] border border-[#2B3139] p-4 rounded-xl flex flex-col items-center justify-center shadow-lg mb-4">
      <span className="text-[#848E9C] text-xs uppercase font-bold tracking-wider mb-2 text-center">{label}</span>
      <span className={`text-xl font-mono ${colorClass}`}>{displayValue}</span>
    </div>
  );
};

// Custom styled Tooltip for charts
const CustomTooltip = ({ active, payload, label }) => {
  if (active && payload && payload.length) {
    return (
      <div className="bg-[#1E222D] border border-[#2B3139] p-3 rounded-lg shadow-xl text-[#EAECEF] text-sm font-mono z-50">
        <p className="font-bold mb-1 border-b border-[#2B3139] pb-1">{label}</p>
        <div className="flex items-center gap-2">
          <span className="w-2 h-2 rounded-full" style={{ backgroundColor: payload[0].fill || payload[0].color }}></span>
          Volatility: {Number(payload[0].value).toFixed(2)}%
        </div>
      </div>
    );
  }
  return null;
};

const RiskVolatilityTab = ({ metrics }) => {
  if (!metrics) return null;
  const { volatility, drawdown, tailRisk } = metrics;
  
  const volData = [
    { name: 'Historical', value: volatility?.historical || 0 },
    { name: 'Downside', value: volatility?.downside || 0 },
    { name: 'Upside', value: volatility?.upside || 0 },
    { name: 'Parkinson', value: volatility?.parkinson || 0 },
    { name: 'Garman-Klass', value: volatility?.garmanKlass || 0 },
    { name: 'EWMA', value: volatility?.ewma || 0 }
  ];

  return (
    <div className="space-y-6">
      <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
        {/* Column 1: Volatility Metrics */}
        <div>
          <h3 className="text-lg font-bold text-white border-l-4 border-concept-blue pl-3 mb-6">Volatility Metrics</h3>
          <div className="grid grid-cols-2 gap-2">
            {renderMetricBox('Historical', volatility?.historical, false, false)}
            {renderMetricBox('Downside', volatility?.downside, false, false)}
            {renderMetricBox('Upside', volatility?.upside, false, false)}
            {renderMetricBox('Parkinson', volatility?.parkinson, false, false)}
            {renderMetricBox('Garman-Klass', volatility?.garmanKlass, false, false)}
            {renderMetricBox('EWMA', volatility?.ewma, false, false)}
          </div>
        </div>

        {/* Column 2: Drawdown */}
        <div>
          <h3 className="text-lg font-bold text-white border-l-4 border-[#F6465D] pl-3 mb-6">Drawdown</h3>
          {renderMetricBox('Max Drawdown (MDD)', drawdown?.maxMDD, true, false)}
          <div className="grid grid-cols-2 gap-2">
            {renderMetricBox('Average DD', drawdown?.average, true, false)}
            {renderMetricBox('Current DD', drawdown?.current, true, false)}
            {renderMetricBox('Under Water', drawdown?.durationDays, true, true)}
            {renderMetricBox('Recovery', drawdown?.recoveryDays, true, true)}
          </div>
        </div>

        {/* Column 3: Tail Risk */}
        <div>
          <h3 className="text-lg font-bold text-white border-l-4 border-[#F6465D] pl-3 mb-6">Tail Risk</h3>
          <div className="grid grid-cols-2 gap-2">
            {renderMetricBox('VaR 95%', tailRisk?.var95, true, false)}
            {renderMetricBox('VaR 99%', tailRisk?.var99, true, false)}
            {renderMetricBox('Exp Shortfall', tailRisk?.expectedShortfallCVaR, true, false)}
            {renderMetricBox('Skewness', tailRisk?.skewness, true, false)}
            {renderMetricBox('Kurtosis', tailRisk?.kurtosis, true, false)}
            {renderMetricBox('Ulcer Index', tailRisk?.ulcerIndex, true, false)}
          </div>
        </div>
      </div>

      {/* Volatility Graph Row requested by User */}
      <h3 className="text-lg font-bold text-white border-l-4 border-[#F0B90B] pl-3 mb-6 mt-8">Volatility Distribution Landscape</h3>
      <div className="bg-[#1E222D] border border-[#2B3139] rounded-xl p-4 shadow-2xl h-[400px]">
        <ResponsiveContainer width="100%" height="90%">
          <BarChart
            data={volData}
            margin={{ top: 20, right: 30, left: 0, bottom: 20 }}
          >
            <CartesianGrid stroke="#2B2B36" strokeDasharray="3 3" vertical={false} />
            <XAxis dataKey="name" stroke="#848E9C" fontSize={12} tickLine={false} axisLine={false} />
            <YAxis stroke="#848E9C" fontSize={12} tickLine={false} axisLine={false} tickFormatter={(val) => `${val}%`} />
            <Tooltip content={<CustomTooltip />} cursor={{fill: '#2B3139', opacity: 0.4}} />
            <Bar dataKey="value" name="Volatility" fill="#8b5cf6" radius={[4, 4, 0, 0]} barSize={40} isAnimationActive={false} />
          </BarChart>
        </ResponsiveContainer>
      </div>

    </div>
  );
};

export default RiskVolatilityTab;
