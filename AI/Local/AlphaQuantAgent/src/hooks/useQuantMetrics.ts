import { useState, useEffect } from 'react';

export interface FeatureImportance {
  name: string;
  value: number;
}

export interface ActionProb {
  time: string;
  buy: number;
  sell: number;
  hold: number;
}

export interface QuantMetrics {
  aiBrain: {
    meanReward: number;
    policyLoss: number;
    entropy: number;
    klDivergence: number;
    featureImportance: FeatureImportance[];
    actionProbability: ActionProb[];
  };
  riskAndVolatility: {
    historicalVolatility: number;
    downsideVolatility: number;
    upsideVolatility: number;
    parkinsonVolatility: number;
    garmanKlassVolatility: number;
    ewmaVolatility: number;
    maxDrawdown: number;
    avgDrawdown: number;
    currentDrawdown: number;
    drawdownDuration: number;
    recoveryTime: number;
    var95: number;
    cvar: number;
    skewness: number;
    kurtosis: number;
    ulcerIndex: number;
  };
  advancedRatios: {
    sharpeRatio: number;
    sortinoRatio: number;
    calmarRatio: number;
    treynorRatio: number;
    infoRatio: number;
    omegaRatio: number;
    sterlingRatio: number;
    burkeRatio: number;
    kRatio: number;
    kappaRatio: number;
  };
  execution: {
    winRate: number;
    lossRate: number;
    profitFactor: number;
    rrRatio: number;
    slippageCost: number;
    obi: number;
    vpin: number;
  };
  portfolio: {
    alpha: number;
    beta: number;
    rSquared: number;
    hurst: number;
    correlationAssets: string[];
    correlationMatrix: number[][];
  };
}

const mockMetrics: QuantMetrics = {
  aiBrain: {
    meanReward: 145.2,
    policyLoss: 0.0234,
    entropy: 1.25,
    klDivergence: 0.004,
    featureImportance: [
      { name: 'RSI_14', value: 18.5 },
      { name: 'MACD_Hist', value: 15.2 },
      { name: 'BB_Width', value: 12.0 },
      { name: 'Volume_Profile', value: 11.4 },
      { name: 'EMA_50_Dist', value: 9.8 },
      { name: 'ATR_14', value: 8.5 },
      { name: 'Stochastic_K', value: 7.2 },
      { name: 'VWAP_Dev', value: 6.5 },
      { name: 'OBV_Trend', value: 5.9 },
      { name: 'Funding_Rate', value: 5.0 }
    ],
    actionProbability: [
      { time: '09:00', buy: 45, sell: 20, hold: 35 },
      { time: '09:15', buy: 40, sell: 25, hold: 35 },
      { time: '09:30', buy: 50, sell: 15, hold: 35 },
      { time: '09:45', buy: 55, sell: 10, hold: 35 },
      { time: '10:00', buy: 60, sell: 15, hold: 25 },
      { time: '10:15', buy: 45, sell: 30, hold: 25 },
      { time: '10:30', buy: 30, sell: 45, hold: 25 },
      { time: '10:45', buy: 20, sell: 55, hold: 25 },
      { time: '11:00', buy: 15, sell: 65, hold: 20 },
      { time: '11:15', buy: 25, sell: 50, hold: 25 },
      { time: '11:30', buy: 35, sell: 40, hold: 25 },
      { time: '11:45', buy: 40, sell: 30, hold: 30 },
      { time: '12:00', buy: 42, sell: 28, hold: 30 }
    ]
  },
  riskAndVolatility: {
    historicalVolatility: 0.45,
    downsideVolatility: 0.22,
    upsideVolatility: 0.28,
    parkinsonVolatility: 0.42,
    garmanKlassVolatility: 0.43,
    ewmaVolatility: 0.46,
    maxDrawdown: -15.4,
    avgDrawdown: -4.5,
    currentDrawdown: -2.1,
    drawdownDuration: 14,
    recoveryTime: 21,
    var95: -12500.50,
    cvar: -16200.75,
    skewness: -0.45,
    kurtosis: 3.8,
    ulcerIndex: 4.2
  },
  advancedRatios: {
    sharpeRatio: 2.25,
    sortinoRatio: 3.42,
    calmarRatio: 1.85,
    treynorRatio: 0.18,
    infoRatio: 1.15,
    omegaRatio: 1.55,
    sterlingRatio: 1.62,
    burkeRatio: 1.44,
    kRatio: 0.92,
    kappaRatio: 1.25
  },
  execution: {
    winRate: 65.5,
    lossRate: 34.5,
    profitFactor: 1.54,
    rrRatio: 1.25,
    slippageCost: 1250.00,
    obi: 0.12,
    vpin: 0.24
  },
  portfolio: {
    alpha: 0.08,
    beta: 0.85,
    rSquared: 0.78,
    hurst: 0.65,
    correlationAssets: ['BTC', 'ETH', 'SOL', 'NVDA', 'AAPL', 'NDX', 'SPY', 'GOLD', 'US10Y', 'DXY'],
    correlationMatrix: [
      [ 1.00,  0.85,  0.78,  0.45,  0.35,  0.55,  0.48,  0.12, -0.22, -0.45],
      [ 0.85,  1.00,  0.88,  0.42,  0.32,  0.58,  0.46,  0.15, -0.25, -0.48],
      [ 0.78,  0.88,  1.00,  0.38,  0.28,  0.52,  0.42,  0.10, -0.28, -0.50],
      [ 0.45,  0.42,  0.38,  1.00,  0.75,  0.85,  0.72,  0.05, -0.15, -0.20],
      [ 0.35,  0.32,  0.28,  0.75,  1.00,  0.82,  0.88,  0.08, -0.18, -0.25],
      [ 0.55,  0.58,  0.52,  0.85,  0.82,  1.00,  0.92,  0.10, -0.30, -0.35],
      [ 0.48,  0.46,  0.42,  0.72,  0.88,  0.92,  1.00,  0.15, -0.35, -0.40],
      [ 0.12,  0.15,  0.10,  0.05,  0.08,  0.10,  0.15,  1.00, -0.45, -0.65],
      [-0.22, -0.25, -0.28, -0.15, -0.18, -0.30, -0.35, -0.45,  1.00,  0.55],
      [-0.45, -0.48, -0.50, -0.20, -0.25, -0.35, -0.40, -0.65,  0.55,  1.00]
    ]
  }
};

export function useQuantMetrics() {
  const [data, setData] = useState<QuantMetrics | null>(null);
  const [isLoading, setIsLoading] = useState<boolean>(true);
  const [error, setError] = useState<Error | null>(null);

  useEffect(() => {
    let isMounted = true;
    
    setIsLoading(true);
    setError(null);

    // Simulate network request loading time
    const timer = setTimeout(() => {
      if (isMounted) {
        try {
          setData(mockMetrics);
          setIsLoading(false);
        } catch (err) {
          setError(err instanceof Error ? err : new Error('Failed to load metrics'));
          setIsLoading(false);
        }
      }
    }, 1000);

    return () => {
      isMounted = false;
      clearTimeout(timer);
    };
  }, []);

  return { data, isLoading, error };
}
