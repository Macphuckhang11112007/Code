import React, { useState, useEffect } from 'react';
import Plot from 'react-plotly.js';
import { Camera, LineChart, Table } from 'lucide-react';
import axios from 'axios';

const ConceptSeasonalsView = ({ asset }) => {
    const [mode, setMode] = useState('Percent'); // 'Percent' | 'Regular'
    const [viewMode, setViewMode] = useState('Table'); // 'Chart' | 'Table'
    const [showAvg, setShowAvg] = useState(true);
    const [basePaths, setBasePaths] = useState(null);

    const months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
    
    useEffect(() => {
        let isMounted = true;
        axios.get('http://127.0.0.1:8000/api/v1/market/seasonals')
            .then(res => {
                if(isMounted && res.data && res.data.status === 'success') {
                    setBasePaths(res.data.data);
                }
            })
            .catch(console.error);
        return () => { isMounted = false };
    }, []);

    const colors = {
        '2026': '#2962FF', '2025': '#00E676', '2024': '#FF9800', 
        '2023': '#00BCD4', '2022': '#E91E63', '2021': '#FFC107', 
        '2020': '#9C27B0', '2019': '#795548', 'Avg': '#FFFFFF' 
    };

    // Calculate transformed node data based on Mode
    const getConvertedData = (pathArray, year) => {
        if (mode === 'Percent') return pathArray;

        // Shift data drastically to mock absolute prices (Regular mode)
        // E.g., if 2026 is at 60k, 2023 started at 16k etc.
        const basePrices = {
            '2026': 68001, '2025': 87496, '2024': 93381,
            '2023': 42258, '2022': 16528, '2021': 46214, 'Avg': 57175
        };
        const startPrice = basePrices[year] / (1 + ((pathArray[11] || pathArray[2]) / 100) || 1); 

        return pathArray.map(pct => {
            if (pct === null) return null;
            return startPrice * (1 + (pct / 100)); // Simulating regular price scaling
        });
    };

    const getColor = (year) => {
        if(colors[year]) return colors[year];
        // fallback color
        let num = parseInt(year) || 2026;
        const hue = (num * 137) % 360; 
        return `hsl(${hue}, 70%, 60%)`;
    }

    const plotData = !basePaths ? [] : Object.keys(basePaths).map(year => {
        if (year === 'Avg' && !showAvg) return null;
        
        return {
            x: months,
            y: getConvertedData(basePaths[year], year),
            type: 'scatter',
            mode: 'lines',
            name: year,
            line: {
                color: getColor(year),
                width: year === new Date().getFullYear().toString() || year === 'Avg' ? 2.5 : 1,
                dash: year === 'Avg' ? 'dot' : 'solid',
                shape: 'spline'
            },
            hoverinfo: 'name+y'
        };
    }).filter(Boolean);

    const annotations = !basePaths ? [] : Object.keys(basePaths).map(year => {
        if (year === 'Avg' && !showAvg) return null;
        
        const path = getConvertedData(basePaths[year], year);
        let lastIdx = 11;
        while (lastIdx > 0 && path[lastIdx] === null) {
            lastIdx--;
        }
        
        const finalValue = path[lastIdx] || 0;
        const displayValue = mode === 'Percent' 
            ? `${finalValue > 0 ? '+' : ''}${finalValue.toFixed(2)}%`
            : finalValue.toLocaleString('en-US', { minimumFractionDigits: 0, maximumFractionDigits: 0 });

        return {
            xref: 'paper', yref: 'y',
            x: 1.0, xanchor: 'left',
            y: finalValue, yanchor: 'middle',
            text: `<b>${year}</b>    ${displayValue}`,
            font: { family: 'Arial', size: 10, color: year === 'Avg' ? '#000' : '#FFF' },
            bgcolor: year === 'Avg' ? '#D1D4DC' : getColor(year),
            bordercolor: 'transparent',
            borderwidth: 2,
            borderpad: 3,
            showarrow: false
        };
    }).filter(Boolean);

    // === TABLE SPECIFIC LOGIC ===
    const monthsFull = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
    
    const availableYears = basePaths ? Object.keys(basePaths).filter(y => y !== 'Avg').sort().reverse() : [];
    
    const tableMatrix = !basePaths ? [] : availableYears.map(year => {
        const rawRow = basePaths[year] || new Array(12).fill(null);
        const row = getConvertedData(rawRow, year);
        let yearTotal = null;
        for (let i = 11; i >= 0; i--) {
            if (rawRow[i] !== null) {
                yearTotal = row[i];
                break;
            }
        }
        return { tag: year, data: row, isRawNulls: rawRow, total: yearTotal };
    });

    const avgRow = basePaths ? getConvertedData(basePaths['Avg'], 'Avg') : new Array(12).fill(null);

    // Calculate Rises and Falls per column
    const risesAndFalls = monthsFull.map((_, colIdx) => {
        let rises = 0; let falls = 0;
        tableMatrix.forEach(row => {
            const val = mode === 'Percent' ? row.data[colIdx] : row.isRawNulls[colIdx];
            if (val !== null && val > 0) rises++;
            if (val !== null && val < 0) falls++;
        });
        return { rises, falls };
    });

    // Helper to format table cells
    const formatCell = (val, isRawNull) => {
        if (isRawNull === null) return '—';
        if (mode === 'Percent') {
            return `${val > 0 ? '+' : ''}${val.toFixed(2)}%`;
        } else {
            return val.toFixed(2);
        }
    };

    const getCellColor = (val, isRawNull) => {
        if (isRawNull === null || val === 0) return 'transparent'; // Empty or 0
        // We use the raw pct to determine color always, even in Regular mode, because "price" being higher than start is Green
        if (isRawNull > 0) return 'bg-[#0ECB81]/20'; // Soft Green
        if (isRawNull < 0) return 'bg-[#F6465D]/20'; // Soft Red
        return 'transparent';
    };

    return (
        <div className="w-full flex-1 flex flex-col gap-6 pb-12 animate-in fade-in duration-300">
            {/* Header Text */}
            <div className="flex flex-col gap-2 max-w-4xl">
                <h2 className="text-2xl font-bold text-white tracking-wide">Historical seasonal performance</h2>
                <p className="text-sm text-concept-muted leading-relaxed">
                    Our seasonality tool reveals recurring monthly trends in an asset's performance, helping traders spot historical patterns and potential price moves. Seasonality provides useful insights, but based on past data, they shouldn't be seen as guaranteed forecasts.
                </p>
            </div>

            {/* Toolbar & Controls Layer */}
            <div className="flex justify-between items-center w-full mt-4">
                {/* Left: Icons & Timeline Label */}
                <div className="flex gap-4 items-center">
                    <div className="flex bg-[#1E222D] border border-[#2d3748] rounded self-start overflow-hidden">
                        <button 
                            onClick={() => setViewMode('Chart')}
                            className={`p-2 transition-colors border-r border-[#2d3748] ${viewMode === 'Chart' ? 'bg-white/20 text-white' : 'text-concept-muted hover:bg-white/10 hover:text-white'}`}
                        >
                            <LineChart className="w-4 h-4" />
                        </button>
                        <button 
                            onClick={() => setViewMode('Table')}
                            className={`p-2 transition-colors ${viewMode === 'Table' ? 'bg-white/20 text-white' : 'text-concept-muted hover:bg-white/10 hover:text-white'}`}
                        >
                            <Table className="w-4 h-4" />
                        </button>
                    </div>
                </div>

                {/* Center: Fake Timeline Bar (Visual Mockup for the 2011-2026 range) */}
                <div className="flex-1 max-w-3xl mx-8 relative flex items-center h-8">
                     <div className="absolute w-full h-1 bg-[#2d3748] rounded-full"></div>
                     <div className="absolute w-[30%] right-0 h-1.5 bg-[#848E9C] rounded-full"></div>
                     <div className="absolute right-[30%] w-4 h-4 bg-white border-2 border-black rounded-full shadow-lg transform translate-x-2 z-10 cursor-pointer"></div>
                     <div className="absolute right-0 w-4 h-4 bg-white border-2 border-black rounded-full shadow-lg transform translate-x-2 z-10 cursor-pointer"></div>
                     
                     <div className="absolute -top-4 w-full flex justify-between text-xs text-concept-muted">
                        <span>2011</span>
                        <span>2014</span>
                        <span>2017</span>
                        <span>2020</span>
                        <span>2023</span>
                        <span>2026</span>
                     </div>
                </div>

                {/* Right: Toggles */}
                <div className="flex gap-3 items-center">
                    <button 
                        onClick={() => setShowAvg(!showAvg)}
                        className={`text-[13px] font-bold px-3 py-1.5 rounded transition-colors flex items-center gap-2 border ${showAvg ? 'bg-white/10 text-white border-[#2d3748]' : 'bg-transparent text-concept-muted border-transparent hover:border-[#2d3748]'}`}
                    >
                        ~ Average
                    </button>
                    
                    <select 
                        className="bg-[#1E222D] text-white text-[13px] font-bold px-3 py-1.5 rounded border border-[#2d3748] outline-none cursor-pointer"
                        value={mode}
                        onChange={(e) => setMode(e.target.value)}
                    >
                        <option value="Percent">Percent</option>
                        <option value="Regular">Regular</option>
                    </select>
                </div>
            </div>

            {/* Content Logic */}
            {viewMode === 'Chart' ? (
                <div className="w-full bg-[#0b0e11] border border-[#2d3748] rounded-xl overflow-hidden shadow-lg mt-2 relative animate-in fade-in zoom-in-95 duration-200">
                    <Plot
                        data={plotData}
                        layout={{
                            paper_bgcolor: 'transparent',
                            plot_bgcolor: 'transparent',
                            font: { color: '#848E9C', family: 'Inter, sans-serif' },
                            xaxis: {
                                showgrid: true,
                                gridcolor: '#1E222D',
                                gridwidth: 1,
                                tickmode: 'array',
                                tickvals: months,
                                ticktext: months,
                                zeroline: false
                            },
                            yaxis: {
                                side: 'right', // Critical: Labels are on the right
                                showgrid: true,
                                gridcolor: '#1E222D', // Subtle grid
                                gridwidth: 1,
                                zeroline: mode === 'Percent', // Draw line at 0 for percentage mode
                                zerolinecolor: '#2B3139',
                                tickformat: mode === 'Percent' ? '.2f' : 's',
                                ticksuffix: mode === 'Percent' ? '%' : ''
                            },
                            margin: { t: 20, b: 40, l: 20, r: 80 },
                            showlegend: false, // Hide default legend, using annotations instead
                            hovermode: 'x unified', // Shows a single vertical slice on hover like TV
                            annotations: annotations,
                            height: 500
                        }}
                        config={{ displayModeBar: false, responsive: true }}
                        style={{ width: '100%', height: '500px' }}
                    />
                </div>
            ) : (
                <div className="w-full bg-[#0b0e11] text-[#D1D4DC] border border-[#2d3748] rounded-xl overflow-auto shadow-lg mt-2 animate-in fade-in zoom-in-95 duration-200">
                    <table className="w-full text-[13px] text-right table-fixed min-w-[1000px]" style={{ borderCollapse: 'collapse' }}>
                        <thead>
                            <tr className="border-b border-[#2d3748] text-concept-muted font-normal bg-[#111926]">
                                <th className="p-3 text-left w-20 sticky left-0 bg-[#111926] z-10 w-[8%]">Date</th>
                                {monthsFull.map(m => (
                                    <th key={m} className="p-3 border-l border-[#2d3748]/50 w-[7%] truncate">{m}</th>
                                ))}
                                <th className="p-3 font-bold text-white border-l border-[#2d3748] w-[8%]">Year</th>
                            </tr>
                        </thead>
                        <tbody>
                            {tableMatrix.map((row) => (
                                <tr key={row.tag} className="border-b border-[#2d3748]/50">
                                    <td className="p-3 text-left text-concept-muted sticky left-0 bg-[#0b0e11] shadow-[1px_0_0_0_#2d3748] z-10">{row.tag}</td>
                                    
                                    {row.data.map((val, i) => {
                                        const raw = row.isRawNulls[i];
                                        return (
                                            <td key={i} className={`p-3 font-mono border-l border-[#2d3748]/50 ${getCellColor(val, raw)}`}>
                                                {formatCell(val, raw)}
                                            </td>
                                        );
                                    })}
                                    
                                    {/* Year column cell */}
                                    <td className={`p-3 font-mono font-bold border-l border-[#2d3748] ${
                                        row.total === null ? '' : 
                                        mode === 'Percent' ? (row.total > 0 ? 'bg-[#0ECB81] text-black' : 'bg-[#ef5350] text-black') :
                                        (row.isRawNulls[row.isRawNulls.length-1] > 0 ? 'bg-[#0ECB81] text-black' : 'bg-[#ef5350] text-black')
                                    } ${row.total !== null ? 'text-white' : ''}`}>
                                        {formatCell(row.total, row.total === null ? null : row.total)}
                                    </td>
                                </tr>
                            ))}

                            {/* Average Row */}
                            <tr className="border-b border-[#2d3748] bg-[#1E222D]">
                                <td className="p-3 text-left font-bold text-white sticky left-0 bg-[#1E222D] shadow-[1px_0_0_0_#2d3748] z-10">Average</td>
                                {avgRow.map((val, i) => (
                                    <td key={i} className={`p-3 font-mono font-bold border-l border-[#2d3748]/50 ${basePaths['Avg'][i] > 0 ? 'text-[#0ECB81]' : 'text-[#ef5350]'}`}>
                                        {formatCell(val, basePaths['Avg'][i])}
                                    </td>
                                ))}
                                <td className={`p-3 font-mono font-bold border-l border-[#2d3748] ${basePaths['Avg'][11] > 0 ? 'text-[#0ECB81]' : 'text-[#ef5350]'}`}>
                                    {formatCell(avgRow[11], basePaths['Avg'][11])}
                                </td>
                            </tr>
                            
                            {/* Rises and falls Row */}
                            <tr className="bg-[#111926]">
                                <td className="p-3 text-left font-bold text-white sticky left-0 bg-[#111926] shadow-[1px_0_0_0_#2d3748] z-10 w-24">Rises/falls</td>
                                {risesAndFalls.map((rf, i) => (
                                    <td key={i} className="p-3 font-mono font-bold border-l border-[#2d3748]/50 text-xs text-center w-16">
                                        <div className="flex justify-center gap-2 w-full mx-auto max-w-[40px]">
                                            <span className="text-[#0ECB81] flex items-center">▲{rf.rises}</span>
                                            <span className="text-[#ef5350] flex items-center">▼{rf.falls}</span>
                                        </div>
                                    </td>
                                ))}
                                {/* Year total Rises and Falls */}
                                <td className="p-3 font-mono font-bold border-l border-[#2d3748] text-xs">
                                     <div className="flex justify-end gap-2">
                                        <span className="text-[#0ECB81] flex items-center">▲{tableMatrix.filter(r => r.isRawNulls[r.isRawNulls.length-1] > 0).length}</span>
                                        <span className="text-[#ef5350] flex items-center">▼{tableMatrix.filter(r => r.isRawNulls[r.isRawNulls.length-1] < 0).length}</span>
                                    </div>
                                </td>
                            </tr>

                        </tbody>
                    </table>
                </div>
            )}
        </div>
    );
};

export default ConceptSeasonalsView;
