import React, { useRef, useEffect } from 'react';
import { useTrainingHistory } from '../../hooks/useTrainingHistory';
import {
  LineChart, Line, AreaChart, Area, BarChart, Bar,
  XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer
} from 'recharts';

const AIBrainTab = ({ metrics }) => {
  const terminalRef = useRef(null);
  const { historyData, isLoading, rawLogs } = useTrainingHistory();

  const cliLogs = rawLogs || []; // Use real logs if provided by hook, else empty array


  useEffect(() => {
    if (terminalRef.current) {
      terminalRef.current.scrollTop = terminalRef.current.scrollHeight;
    }
  }, [cliLogs]);

  if (!metrics || !metrics.aiBrain) return <div className="p-4 text-[#848E9C]">No Data Available</div>;

  const { aiBrain } = metrics;
  
  // Custom styled Tooltip
  const CustomTooltip = ({ active, payload, label }) => {
    if (active && payload && payload.length) {
      return (
        <div className="bg-[#1E222D] border border-[#2B3139] p-3 rounded-lg shadow-xl text-[#EAECEF] text-sm font-mono z-50">
          <p className="font-bold mb-1 border-b border-[#2B3139] pb-1">Epoch / Time: {label}</p>
          {payload.map((entry, index) => (
            <div key={index} style={{ color: entry.color }} className="flex items-center gap-2">
              <span className="w-2 h-2 rounded-full" style={{ backgroundColor: entry.color }}></span>
              {entry.name}: {typeof entry.value === 'number' && !Number.isInteger(entry.value) ? entry.value.toFixed(4) : entry.value}
            </div>
          ))}
        </div>
      );
    }
    return null;
  };

  const renderMetricBox = (label, value) => (
    <div className="bg-[#1E222D] border border-[#2B3139] p-4 rounded-xl flex flex-col items-center justify-center shadow-lg">
      <span className="text-[#848E9C] text-xs uppercase font-bold tracking-wider mb-2 text-center">{label}</span>
      <span className="text-xl font-mono text-white">{value !== undefined && value !== null ? Number(value).toFixed(4) : '--'}</span>
    </div>
  );

  // Parse Action Probabilities into a flat format from historyData
  const actionData = historyData.map(d => ({
    time: d.action_probabilities?.time || d.epoch,
    epoch: d.epoch,
    buy: d.action_probabilities?.buy || 0,
    sell: d.action_probabilities?.sell || 0,
    hold: d.action_probabilities?.hold || 0
  }));

  return (
    <div className="space-y-6">
      <h2 className="text-xl font-bold text-white mb-4 border-l-4 border-concept-blue pl-3">Live Multi-Agent Reinforcement Learning</h2>
      
      {/* Chart 0: THE LIVE PRICE CHART INJECTED FROM PHASE 3 */}
      {chartData && chartData.length > 0 && (
          <div className="bg-[#1E222D] border border-[#2B3139] rounded-xl p-4 shadow-2xl h-[350px] mb-6">
              <h3 className="text-sm font-bold text-[#848E9C] uppercase tracking-wider mb-2">Live Price Action (Last 96 Candles)</h3>
              <ResponsiveContainer width="100%" height="90%">
                  <LineChart data={chartData} margin={{ top: 10, right: 30, left: -10, bottom: 0 }}>
                      <CartesianGrid stroke="#2B2B36" strokeDasharray="3 3" vertical={false} />
                      <XAxis dataKey="time" stroke="#848E9C" fontSize={10} tickLine={false} axisLine={false} tickFormatter={(val) => val ? val.split(' ')[1] : ''} />
                      <YAxis stroke="#848E9C" fontSize={10} tickLine={false} axisLine={false} domain={['auto', 'auto']} />
                      <Tooltip content={<CustomTooltip />} />
                      <Line type="monotone" dataKey="close" name="Close Price" stroke="#0ECB81" strokeWidth={2} dot={false} isAnimationActive={false} />
                  </LineChart>
              </ResponsiveContainer>
          </div>
      )}
      
      {/* Top Metrics Row */}
      <div className="grid grid-cols-2 md:grid-cols-5 lg:grid-cols-5 gap-4 mb-6">
         {renderMetricBox('Mean Reward', aiBrain.meanEpisodicReward)}
         {renderMetricBox('Policy Loss', aiBrain.policyLoss)}
         {renderMetricBox('Value Loss', aiBrain.valueLoss)}
         {renderMetricBox('Entropy', aiBrain.entropy)}
         {renderMetricBox('KL Divergence', aiBrain.klDivergence)}
         {renderMetricBox('Adv. Estimate', aiBrain.advantageEstimate)}
         {renderMetricBox('Clip Fraction', aiBrain.clipFraction)}
         {renderMetricBox('Q-Value Spread', aiBrain.qValueSpread)}
         {renderMetricBox('Prediction Acc.', aiBrain.predictionAccuracy)}
         {renderMetricBox('Expl. Ratio', aiBrain.explorationRatio)}
      </div>

      {isLoading && historyData.length === 0 ? (
        <div className="text-center p-6 text-[#848E9C] animate-pulse">Fetching Real Training History...</div>
      ) : (
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-6 pt-4">
          
          {/* Chart 1: Loss Graph */}
          <div className="bg-[#1E222D] border border-[#2B3139] rounded-xl p-4 shadow-2xl h-[350px]">
            <h3 className="text-sm font-bold text-[#848E9C] uppercase tracking-wider mb-2">Neural Network Trajectory (Losses)</h3>
            <ResponsiveContainer width="100%" height="85%">
              <LineChart data={historyData} margin={{ top: 10, right: 10, left: -20, bottom: 0 }}>
                <CartesianGrid stroke="#2B2B36" strokeDasharray="3 3" vertical={false} />
                <XAxis dataKey="epoch" stroke="#848E9C" fontSize={12} tickLine={false} axisLine={false} />
                <YAxis yAxisId="left" stroke="#848E9C" fontSize={12} tickLine={false} axisLine={false} tickFormatter={(val) => val.toFixed(3)} />
                <YAxis yAxisId="right" orientation="right" stroke="#848E9C" fontSize={12} tickLine={false} axisLine={false} tickFormatter={(val) => val.toFixed(2)} />
                <Tooltip content={<CustomTooltip />} />
                <Line yAxisId="left" type="monotone" dataKey="policy_loss" name="Policy Loss" stroke="#F6465D" strokeWidth={2} dot={false} isAnimationActive={false} />
                <Line yAxisId="right" type="monotone" dataKey="value_loss" name="Value Loss" stroke="#0ECB81" strokeWidth={2} dot={false} isAnimationActive={false} />
              </LineChart>
            </ResponsiveContainer>
          </div>

          {/* Chart 2: Learning Curve */}
          <div className="bg-[#1E222D] border border-[#2B3139] rounded-xl p-4 shadow-2xl h-[350px]">
            <h3 className="text-sm font-bold text-[#848E9C] uppercase tracking-wider mb-2">Learning Curve (Mean Reward)</h3>
            <ResponsiveContainer width="100%" height="85%">
              <LineChart data={historyData} margin={{ top: 10, right: 10, left: -20, bottom: 0 }}>
                <CartesianGrid stroke="#2B2B36" strokeDasharray="3 3" vertical={false} />
                <XAxis dataKey="epoch" stroke="#848E9C" fontSize={12} tickLine={false} axisLine={false} />
                <YAxis stroke="#848E9C" fontSize={12} tickLine={false} axisLine={false} />
                <Tooltip content={<CustomTooltip />} />
                <Line type="monotone" dataKey="mean_reward" name="Mean Reward" stroke="#8b5cf6" strokeWidth={2} dot={false} isAnimationActive={false} />
              </LineChart>
            </ResponsiveContainer>
          </div>

          {/* Chart 3: Action Probs AreaChart */}
          <div className="bg-[#1E222D] border border-[#2B3139] rounded-xl p-4 shadow-2xl h-[350px]">
            <h3 className="text-sm font-bold text-[#848E9C] uppercase tracking-wider mb-2">Action Probability Distribution</h3>
            <ResponsiveContainer width="100%" height="90%">
              <AreaChart data={actionData} margin={{ top: 10, right: 10, left: -20, bottom: 0 }}>
                <CartesianGrid stroke="#2B2B36" strokeDasharray="3 3" vertical={false} />
                <XAxis dataKey="time" stroke="#848E9C" fontSize={10} tickLine={false} axisLine={false} />
                <YAxis stroke="#848E9C" fontSize={10} tickLine={false} axisLine={false} />
                <Tooltip content={<CustomTooltip />} />
                <Area type="monotone" dataKey="buy" name="Buy" stackId="1" stroke="#0ECB81" fill="#0ECB81" fillOpacity={0.6} isAnimationActive={false} />
                <Area type="monotone" dataKey="hold" name="Hold" stackId="1" stroke="#848E9C" fill="#848E9C" fillOpacity={0.6} isAnimationActive={false} />
                <Area type="monotone" dataKey="sell" name="Sell" stackId="1" stroke="#F6465D" fill="#F6465D" fillOpacity={0.6} isAnimationActive={false} />
              </AreaChart>
            </ResponsiveContainer>
          </div>

          {/* Chart 4: Feature Importance BarChart */}
          <div className="bg-[#1E222D] border border-[#2B3139] rounded-xl p-4 shadow-2xl h-[350px] flex flex-col">
            <h3 className="text-sm font-bold text-[#848E9C] uppercase tracking-wider mb-2 flex-shrink-0">Feature Importance</h3>
            <div className="flex-1 overflow-y-auto custom-scrollbar w-full">
              <div style={{ height: Math.max(300, aiBrain.featureImportance.length * 30), width: '100%' }}>
                <ResponsiveContainer width="100%" height="100%">
                  <BarChart
                    data={aiBrain.featureImportance}
                    layout="vertical"
                    margin={{ top: 10, right: 30, left: 20, bottom: 5 }}
                  >
                    <CartesianGrid stroke="#2B2B36" strokeDasharray="3 3" horizontal={false} />
                    <XAxis type="number" stroke="#848E9C" fontSize={10} tickLine={false} axisLine={false} />
                    <YAxis dataKey="name" type="category" stroke="#D1D4DC" fontSize={11} tickLine={false} axisLine={false} width={120} />
                    <Tooltip content={<CustomTooltip />} cursor={{fill: '#2B3139', opacity: 0.4}} />
                    <Bar dataKey="value" name="Importance" fill="#F0B90B" radius={[0, 4, 4, 0]} barSize={20} isAnimationActive={false} />
                  </BarChart>
                </ResponsiveContainer>
              </div>
            </div>
          </div>

        </div>
      )}

      {/* CLI Console */}
      <div className="bg-[#0b0e11] border border-[#2B3139] rounded-xl p-4 shadow-2xl h-[250px] flex flex-col font-mono text-sm mt-6">
        <h3 className="font-bold text-[#848E9C] uppercase tracking-wider mb-2 flex items-center">
          <span className="w-2 h-2 rounded-full bg-green-500 mr-2 animate-pulse"></span>
          AlphaQuant CLI Console Pipe
        </h3>
        <div 
          ref={terminalRef}
          className="flex-1 overflow-y-auto custom-scrollbar p-2 bg-black border border-[#2B3139] rounded"
          onWheel={(e) => e.stopPropagation()}
        >
          {cliLogs.map((log, index) => (
            <div key={index} className="text-blue-300 font-mono mb-1">
              {log}
            </div>
          ))}
        </div>
      </div>

    </div>
  );
};

export default AIBrainTab;
