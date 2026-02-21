import React, { useEffect, useRef } from 'react';
import { createChart, CandlestickSeries, HistogramSeries } from 'lightweight-charts';
import axios from 'axios';

const TradingChart = ({ symbol }) => {
  const chartContainerRef = useRef(null);
  const chartRef = useRef(null);
  const candleSeriesRef = useRef(null);
  const volumeSeriesRef = useRef(null);

  useEffect(() => {
    // 1. Initialize Chart
    const chart = createChart(chartContainerRef.current, {
      layout: {
        background: { type: 'solid', color: '#0b0e11' },
        textColor: '#D1D4DC',
      },
      grid: {
        vertLines: { color: '#2B3139' },
        horzLines: { color: '#2B3139' },
      },
      crosshair: { mode: 0 },
      timeScale: {
        timeVisible: true,
        secondsVisible: false,
        borderVisible: false
      },
      rightPriceScale: { borderVisible: false }
    });

    const candleSeries = chart.addSeries(CandlestickSeries, {
      upColor: '#0ECB81',
      downColor: '#F6465D',
      borderVisible: false,
      wickUpColor: '#0ECB81',
      wickDownColor: '#F6465D',
    });

    const volumeSeries = chart.addSeries(HistogramSeries, {
      color: '#26a69a',
      priceFormat: { type: 'volume' },
      priceScaleId: '', // set as an overlay
      scaleMargins: { top: 0.8, bottom: 0 },
    });

    chartRef.current = chart;
    candleSeriesRef.current = candleSeries;
    volumeSeriesRef.current = volumeSeries;

    // Handle Resize
    const handleResize = () => {
      chart.applyOptions({
        width: chartContainerRef.current.clientWidth,
        height: chartContainerRef.current.clientHeight,
      });
    };
    window.addEventListener('resize', handleResize);

    // 2. Fetch Historical Data via REST
    const fetchHistory = async () => {
      try {
        const response = await axios.get(`http://localhost:8000/api/v1/market/history/${symbol}`);
        if(response.data.status === "success") {
           const ohlcv = response.data.data;
           const candles = ohlcv.map(d => ({ time: d.time, open: d.open, high: d.high, low: d.low, close: d.close }));
           const volumes = ohlcv.map(d => ({ 
               time: d.time, 
               value: d.volume, 
               color: d.close >= d.open ? 'rgba(14, 203, 129, 0.5)' : 'rgba(246, 70, 93, 0.5)' 
           }));
           
           candleSeries.setData(candles);
           volumeSeries.setData(volumes);
           chart.timeScale().fitContent();

           // 3. Mount WebSocket for Live 1-Second Ticks
           const lastCandle = candles[candles.length - 1];
           let currentCandle = { ...lastCandle };
           
           const ws = new WebSocket("ws://localhost:8000/ws/market/tick");
           ws.onmessage = (event) => {
               const tick = JSON.parse(event.data);
               // Simple 1D candle builder
               if (tick.time > currentCandle.time) {
                   // New Candle
                   currentCandle = {
                       time: tick.time,
                       open: tick.value,
                       high: tick.value,
                       low: tick.value,
                       close: tick.value
                   };
               } else {
                   // Update existing candle
                   currentCandle.high = Math.max(currentCandle.high, tick.value);
                   currentCandle.low = Math.min(currentCandle.low, tick.value);
                   currentCandle.close = tick.value;
               }
               candleSeries.update(currentCandle);
           };
        }
      } catch (e) {
        console.error("Failed to load historical data", e);
      }
    };
    
    fetchHistory();

    // Cleanup
    return () => {
      window.removeEventListener('resize', handleResize);
      chart.remove();
    };
  }, [symbol]);

  return (
    <div className="absolute top-0 left-0 right-0 bottom-0 w-full h-full" ref={chartContainerRef} />
  );
};

export default TradingChart;
