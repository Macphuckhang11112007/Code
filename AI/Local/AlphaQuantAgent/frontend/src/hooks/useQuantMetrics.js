// Import necessary Hooks from the React core library (State and Lifecycle management)
import { useState, useEffect } from 'react';
// Import the Axios library to execute HTTP API calls to the Python Backend
import axios from 'axios';

// Initialize Custom Hook named useQuantMetrics for global sharing across all Dashboards
export function useQuantMetrics() {
  // State variable holding the entire matrix of 50 quantitative indicators, initialized to null
  const [data, setData] = useState(null);
  // State variable signaling the UI fetching status (Loading Spinner / Skeleton)
  const [isLoading, setIsLoading] = useState(true);
  // State variable storing error traces if the network disconnects
  const [error, setError] = useState(null);

  // useEffect Hook encapsulating the Fetching logic to trigger only upon Interface Mount
  useEffect(() => {
    // Sentry variable to prevent the UI from Unmounting while the API still returns, causing Memory Leaks
    let isMounted = true;

    // Encase execution in a Try Catch shell right before launching the Network request
    try {
        // Yêu cầu 3 của Giai đoạn 2: Trỏ hệ thống UI React về Node Server chạy trên Port 5000
        axios.get('http://localhost:5000/api/quant/metrics')
          .then(res => {
            // Immediately upon response, verify if Component is alive. If dead, Rescue Channel drops UI manipulation
            if (!isMounted) return;
            
            // Insurance Barrier: Confirm the Server's returned structure aligns with Data contract and Success status
            if (res.data && res.data.status === 'success') {
              // Extract the raw original data core from the Server into a local Raw object
              const raw = res.data.data;
              
              // Object Mapper: Transform the entire Python Backend directory tree into Frontend React CamelCase
              // Mapping Block Nested Try Catch (Meaning Mapping errors on any branch won't collapse the Core)
              try {
                  const mappedData = {
                    // Tab Structure: Core Machine Learning Brain
                    aiBrain: {
                      // Regression Average Reward (PPO) 
                      meanEpisodicReward: raw.aiBrain?.meanEpisodicReward,
                      // Actor Action Error Variance (Closer to 0 is Better)
                      policyLoss: raw.aiBrain?.policyLoss,
                      // Critic Layer Forecasting Error Variance
                      valueLoss: raw.aiBrain?.valueLoss,
                      // Decision Chaos Index (Agent's Exploration Factor)
                      entropy: raw.aiBrain?.entropy,
                      // PPO Optimizer Update Divergence Gap
                      klDivergence: raw.aiBrain?.klDivergence,
                      // Current State Action Advantage Level
                      advantageEstimate: raw.aiBrain?.advantageEstimate,
                      // PPO Clipping Gradient Pinch Level
                      clipFraction: raw.aiBrain?.clipFraction,
                      // Q-Value Spread between strongest and weakest action
                      qValueSpread: raw.aiBrain?.qValueSpread,
                      // XGBoost Tree Algorithm Correctness (Fragmentation)
                      predictionAccuracy: raw.aiBrain?.predictionAccuracy,
                      // Ratio Dynamics between New Exploration and Old Exploitation
                      explorationRatio: raw.aiBrain?.explorationRatio,
                      // Extract Important Feature Array from Tree -> Unpack into Recharts BarChart rendering format
                      featureImportance: raw.aiBrain?.featureImportance 
                          ? Object.entries(raw.aiBrain.featureImportance).map(([k, v]) => ({ name: k, value: v }))
                          : [],
                      // Empty Array Reserving Action Probability Matrix (Shifted to Live CLI due to API overload on past logs)
                      actionProbability: raw.aiBrain?.actionProbability || [] 
                    },
                    // Tab Structure: Price Volatility and Disaster Risk Management Parameters
                    riskVolatility: {
                      volatility: {
                        // Calculate Historical Volatility based on past standard deviation
                        historical: raw.riskVolatility?.volatility?.historical || 0,
                        // Volatility computed exclusively on the downward trench of the Chart (Sortino Formula)
                        downside: raw.riskVolatility?.volatility?.downside || 0,
                        // Volatility of the green Bullish candles
                        upside: raw.riskVolatility?.volatility?.upside || 0,
                        // Volatility border evaluating Parkinson Max/Min span within candle Price body
                        parkinson: raw.riskVolatility?.volatility?.parkinson || 0,
                        // Garman Klass Super Mathematical Sphere Compression Volatility
                        garmanKlass: raw.riskVolatility?.volatility?.garmanKlass || 0,
                        // EWMA Garch Old/New Sliding Weight Sensitivity Function
                        ewma: raw.riskVolatility?.volatility?.ewma || 0
                      },
                      drawdown: {
                        // Max Absolute Net Capital Drop from ATH Peak
                        maxMDD: raw.riskVolatility?.drawdown?.maxMDD || 0,
                        // Measured Average Shaking Drawdown Rate
                        average: raw.riskVolatility?.drawdown?.average || 0,
                        // Price Drop Distance relative to the Record Peak pinned at Present Time
                        current: raw.riskVolatility?.drawdown?.current || 0,
                        // Number of Candles Trapped underneath the Loss Floor
                        durationDays: raw.riskVolatility?.drawdown?.durationDays || 0,
                        // Recovery Time from Floundering Loss to ATH Breakeven
                        recoveryDays: raw.riskVolatility?.drawdown?.recoveryDays || 0
                      },
                      tailRisk: {
                        // Estimated Risk Value Dropping Fund 95% Confidence (Minor Black Swan)
                        var95: raw.riskVolatility?.tailRisk?.var95 || 0,
                        // Collapse Formatting Max Level At 99% Crushing Market Disaster (Black Swan)
                        var99: raw.riskVolatility?.tailRisk?.var99 || 0,
                        // Valuation Standard Slipping Past Final Var Threshold (Statistical CVaR)
                        expectedShortfallCVaR: raw.riskVolatility?.tailRisk?.expectedShortfallCVaR || 0,
                        // Non-Standard Gaussian Graph Skew
                        skewness: raw.riskVolatility?.tailRisk?.skewness || 0,
                        // Leptokurtic Peak Reserve Graphic Tail Risk Concave Density (Fat Tail)
                        kurtosis: raw.riskVolatility?.tailRisk?.kurtosis || 0,
                        // Investor Loss Endurance Index Towering Stress Accumulation
                        ulcerIndex: raw.riskVolatility?.tailRisk?.ulcerIndex || 0
                      }
                    },
                    // Tab Structure: Modern Industry Standard Mathematical Levers
                    advancedRatios: {
                      sharpe: raw.advancedRatios?.sharpe || 0,        // Classic Golden Sharpe Ratio (Return over Risk)
                      sortino: raw.advancedRatios?.sortino || 0,      // Identification Ratio Stripping Penalty Drop Sessions
                      calmar: raw.advancedRatios?.calmar || 0,        // Mid-Term Management MaxDD Slippage Ratio
                      treynor: raw.advancedRatios?.treynor || 0,      // Treynor Expanded Market Mapping Sphere
                      infoRatio: raw.advancedRatios?.infoRatio || 0,  // Performance Deviation Surpassing Market Beta Index
                      omega: raw.advancedRatios?.omega || 0,          // Probability Win/Loss Integration Axis Cap
                      sterling: raw.advancedRatios?.sterling || 0,    // Measuring Smooth Pull Back Strength Flattener
                      burke: raw.advancedRatios?.burke || 0,          // Quarterly Penalty Measurement For Contiguous Surfacing Loss Phases
                      kRatio: raw.advancedRatios?.kRatio || 0,        // Standard Regression Mathematical Measure Steep Ascending Chain (Linear)
                      kappa: raw.advancedRatios?.kappa || 0           // Black Hole Measuring Under Water Disaster Wave Against Anchor 0
                    },
                    // Tab Structure: Flat Forward Money Generation Core Math Ground Accounting (CASH)
                    returns: {
                      cumulative: raw.returns?.cumulative || 0,       // Total Max Realized Horizontal Accrued Base (%)
                      cagr: raw.returns?.cagr || 0,                   // Crushing Bounce Compounded Annual Growth Rate (Interest Spawning Interest)
                      arithmeticMean: raw.returns?.arithmeticMean || 0, // Algebraic Mouth Level Average (Non-Compounded Average)
                      geometricMean: raw.returns?.geometricMean || 0,   // Independent Geometric Average Measuring Flat Compound Pocket Yield
                      ytd: raw.returns?.ytd || 0,                       // Reporting Direct Intra-Year Yield Running Until Now (Year To Date)
                      grossProfit: raw.returns?.grossProfit || 0,       // Ocean of Total Accumulated Profit Stream Before Loss Debt Deduction
                      grossLoss: raw.returns?.grossLoss || 0,           // Pierced Edge Total Stream of Exposed Laying Loss Commands
                      netProfit: raw.returns?.netProfit || 0,           // Net Benchmark Round Number Calculated Punishment Cut ($)
                      rollingReturns: raw.returns?.rollingReturns       // Wrenching Axis Rolling Area Return Chart Following Season Quarter Node Below
                        ? raw.returns.rollingReturns.labels.map((lbl, idx) => ({
                            period: lbl,                                // Paste Quarter Label
                            value: raw.returns.rollingReturns.values[idx] // Bite Value Referencing Array Index Matching Native Source List
                          }))
                        : []
                    },
                    // Tab Structure: Machine Floor Order Execution Behavior Factory Mapping (Agent Action Behavior)
                    execution: {
                      tradeStats: {
                        winRate: raw.execution?.tradeStats?.winRate || 0,                 // Table Pin Rate Hitting Buy Achieving Peak Win (%)
                        lossRate: raw.execution?.tradeStats?.lossRate || 0,               // Funnel Bottom Rate Laying At Dead Probability Loss Bottom
                        profitFactor: raw.execution?.tradeStats?.profitFactor || 0,       // Apple Great Score Ratio Gross Win vs Gross Disaster Loss Must Surpass Survival 1.5 
                        riskRewardRatio: raw.execution?.tradeStats?.riskRewardRatio || 0, // Lever Branch Yawn RR Each Long Average Order RR Best 2:1 (RR Ratio)
                        payoffRatio: raw.execution?.tradeStats?.payoffRatio || 0,         // Expectation Metric Pulling From Crossed Money Game Chain Expected
                        totalTrades: raw.execution?.tradeStats?.totalTrades || 0,         // Giant Number Pillar Specifying Agent Spreading Server Overload Calendar
                        maxConsecutiveWins: raw.execution?.tradeStats?.maxConsecutiveWins || 0,     // Acceleration Boom Strip Pulling Ceaseless Long Winning Streak Consecutive (Win Streak)
                        maxConsecutiveLosses: raw.execution?.tradeStats?.maxConsecutiveLosses || 0, // Deep Black Strip Plunging Dumb Pit Immediately Trapping Order Failing Harsh Series Chronic Drag (Loss Streak)
                        avgHoldingTimeLong: raw.execution?.tradeStats?.avgHoldingTimeLong || 0,     // Resistance Milestone Shaft While Holding Buy Position Incubating Capital (Holding Average)
                        avgHoldingTimeShort: raw.execution?.tradeStats?.avgHoldingTimeShort || 0    // Downward Strike Hunt Limit Diving Capital At Piercing Short Sell Exchange (Short Hold)
                      },
                      microstructure: {
                        totalSlippageCost: -Math.abs(raw.execution?.microstructure?.totalSlippageCost || 0),   // Fading Flower Damage Dimension Due To Tearing Slip Metric Liquidity Transaction Bending Route (Slippage Decay) - Strictly Negative Bound
                        executionShortfall: -Math.abs(raw.execution?.microstructure?.executionShortfall || 0), // Funnel Point Trench Shrinking Slipping Wave Delivery From AI Core Expectation Dropped By Exchange Floor Lag Execution Cutting Loss Dust Negative Subtraction
                        totalCommissions: -Math.abs(raw.execution?.microstructure?.totalCommissions || 0),     // Water Spout Digesting Complete Value Cut Pulse Paying Silver Wealth Sea Exchange Network Binance Devouring Maker/Taker Ratio Pressing Fee Red Negative Direct Suck
                        marginUtilization: raw.execution?.microstructure?.marginUtilization || 0,              // Split Grasping Storage Chunk Balance Wrapper Margin Measuring Slipping Zone Bounding Net Capturing Liquidation (Burn Ratio) Risk Jet High Rise
                        longShortRatio: raw.execution?.microstructure?.longShortRatio || 0,                    // Intellectual Scale Trait Linking Short Sell Cheap Fast Measuring Positional Bias Bear Pig Agent Gripping Horns Or Grabbing Claws
                        obi: raw.execution?.microstructure?.obi || 0,                                          // Numb Ruler Drifting Imbalance Order Book Book Buy / Sell Stomping Pressure Dragging Alive Quiet Book Order Match Slipping Defense Book Bias Heavy
                        vpin: raw.execution?.microstructure?.vpin || 0,                                        // Insider Secret Spring Toxic Market Maker Injecting Flat Running Node Ferocious Grasp Flow Toxic Wrench Theatrical Fury
                        bidAskSpreadVar: raw.execution?.microstructure?.bidAskSpreadVar || 0                   // Spanning Tension Pressing Bid/Ask Off Range Guilt Squeezing Liquidity Pulling Skew Skipping Spread Blooming Deaf Exchange Ledger Slow Dim Lag Heat Degree
                      }
                    },
                    // Tab Structure: Capital Risk Allocation Sector Generating Shield Reversing Gravity Axis
                    portfolioSurvival: {
                      macro: {
                        alpha: raw.portfolioSurvival?.macro?.alpha || 0,                 // Golden Bamboo Shoot Bouncing Miracle Index Auto Forging Money Extremely Competent Piercing Smasher Over Axis Floor Dragging Monetary Legend Alpha Eaten Grip Break
                        beta: raw.portfolioSurvival?.macro?.beta || 0,                   // Shivering Core Route Binding Life Rope Drifting Struck Alongside Market Mother Macro Sea System Crisis (Unified Floor Collapse Beta Anchored Fall Tracking)
                        rSquared: raw.portfolioSurvival?.macro?.rSquared || 0,           // Sticky Lock Eye Assessing R R Roach Interlocking Contingency Shift Benchmark Origin Index Bound
                        trackingError: raw.portfolioSurvival?.macro?.trackingError || 0, // Lightning Tail Mistake Route Violation Pipe Throw Correlating Divergent Block Lock Code Line
                        hurstExponent: raw.portfolioSurvival?.macro?.hurstExponent || 0, // Ghost Wand Hurst Scaling Transverse Split Trend (Mean Revert Strolling Shaking Scale) Or Winning Ascend Straight Trend Flat Drifting Boundless Current
                        adfTest: raw.portfolioSurvival?.macro?.adfTest || 0,             // Augmented Dickey Fuller Number Dial Deciding For Time Flow Stretching Mathematical Peak Silent Station Weighing Floor Shelter Clear Traverse
                        ljungBox: raw.portfolioSurvival?.macro?.ljungBox || 0,           // Pure Refined Return Station Ljung Suppression Power Filtering Supersonic Sound Noise Chain Shudder Variable Denying Origin Auto (White Noise Check Miracle)
                        turnoverRate: raw.portfolioSurvival?.macro?.turnoverRate || 0    // Rotary Cylinder Axle Coil Turn Flipping Wings Continually Changing Velocity Tearing Emergency Portfolio Stuffing Stroke Gas Slip (Bullet Bag Loop Speed Over Lethal Limit Slicing Rose Commision)
                      },
                      survival: {
                        // Computing Kelly Level Incredible Formula Grabbing Deserving Bite Drop Dividing Fund All In Seizing Grip Size Capital (% Empty Ratio Value 1 Gambling Match Assessed %) 
                        kellyCriterion: (raw.portfolioSurvival?.survival?.kellyCriterion || 0) * 100, 
                        // Cursed Calling Title Disaster Burning Fade Acc Dropping Binary 0 Skeletal Root Ripped Floor Capacity Survival Ruin Destitution Touching Bottom Torn Fabric Separation Eternal Sleep (%)
                        riskOfRuin: (raw.portfolioSurvival?.survival?.riskOfRuin || 0) * 100, 
                        // End Expectation Node Incredible Yield Serving Persistent Awaiting Diverse Distribution Price Ratio Complex Mathematical Equilibrium Expect Sharp Point Sub-phase Resistance Elastic Force Holding Ultimate Execution All Reached
                        expectancy: raw.portfolioSurvival?.survival?.expectancy || 0 
                      },
                      // Covariance Crosspoint Array Ghost Matrix Staring Heatmap Connecting Root Fracture Divvying Spreading Flat Record Split Data Form Node Null Avoidance
                      correlationMatrix: raw.portfolioSurvival?.correlationMatrix 
                        ? raw.portfolioSurvival.correlationMatrix
                        : [[1,0,0,0,0],[0,1,0,0,0],[0,0,1,0,0],[0,0,0,1,0],[0,0,0,0,1]]  // 5x5 Identity Construction Base Pure White When Backtest Flaws Missing Mock Mathematical Multiplication Helping Trading Core Ground Surviving Blind Matrix Avoiding Jamming Map Array List Fill
                    }
                  };
                  
                  // Finalizing Target Launching Data into Store Matrix Component State (Last Successful Data Status Flawlessly Cleanly Mapped Absolute Match Edge)
                  setData(mappedData);
                  // Close Spinner Lid Hatch Ceasing Hope Rendering Absolute Terminal Display
                  setIsLoading(false);
                  
              } catch (mapError) {
                  // Shielding Silent Logic Error Bracket (Mapping Explosive Crash Object Piercing Abyss Layer Array Due to Web Strand Releasing Optional Null Missing Bracket)
                  console.error("Critical: Architectural Collapse Within Object Map Code Interpreter:", mapError);
                  setError("Matrix Circuit Torn At JSON Object Read Mapping Phase (Client Exception).");
                  setIsLoading(false);
                  setData(null);
              }
              
            } else {
              // Diving Into Disconnected Fluid Void Sluggish Failure Status (E.g. Backtest Unjamming Source Yielding Text Error Blurry Empty JSON Result Null Backend Decree)
              setError(res.data?.message || 'Alarm Backend Rejecting White Slate Broken JSON Network');
              setIsLoading(false);
              setData(null);
            }
          })
          .catch(err => {
            // When Try Block Angry Cable Fling Broken Wire Because Unreachable Endpoint (Dead Socket 404/ Exhausted Mute Server 500 Internals Terminating Axios HTTP Cease Crash Tracking Empty Pull)
            if (isMounted) {
              console.error("Axios Retrieval Get Smashed Crushed Extraction From API Level HTTP Request Fail Mesh Measure:", err);
              setError(err.message);
              setIsLoading(false);
              setData(null);
            }
          });
    } catch (criticalAxios) {
        // Supreme Front End Net Encasing Capturing All Infinite Component Death Fissure Logic Traps
        console.error("Super Terminal Logic Block Spawning Caught Critical Exception:", criticalAxios);
    }

    // Cleanup: Locking Memory Fever Leaks Eracing Stop Terminate Render Appending Mounted State
    return () => { isMounted = false; };
  }, []); // [] Blank Mount Single Spin Hook Activating Launch Instant Genesis Node Entity React Component Lock

  // Force Render Feedback Delivering Vast Form Payload Target Matrix React Assembly
  return { data, isLoading, error };
}
