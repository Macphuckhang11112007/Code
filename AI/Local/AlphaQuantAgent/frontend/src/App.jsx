// ==========================================
// CORE APP ROUTER & OVERLAY MANAGER
// ==========================================
// This file initializes the Single Page Application routing,
// mounts the permanent UI elements (Sidebar, TopBar, Ticker),
// and controls the global Chart Modal.

import React from 'react';
import { useLocation, useNavigate, Routes, Route } from 'react-router-dom';
import AlphaQuantAssistant from './components/AlphaQuantAssistant';
import ConceptSidebar from './components/ConceptSidebar';
import ConceptHero from './components/ConceptHero';
import ConceptGrid from './components/ConceptGrid';
import ConceptTopBar from './components/ConceptTopBar';
import ConceptChartModal from './components/ConceptChartModal';
import QuantMatrixDashboard from './components/QuantMatrixDashboard';
import TickerTape from './components/TickerTape';
import MarketsPage from './components/MarketsPage';

// The Dashboard acts as the primary layout view for the HOME route
function Dashboard() {
  const navigate = useNavigate();
  return (
    <div className="flex-1 flex flex-col relative h-screen bg-glow-gradient overflow-y-auto">
      {/* Top Asset Bar - Navigate directly using the exact symbol from backend */}
      <ConceptTopBar onAssetClick={(asset) => navigate(`/${asset.sym}`)} />
      {/* Ticker Tape - Navigate directly using the exact symbol from backend */}
      <TickerTape onAssetClick={(asset) => navigate(`/${asset.sym}`)} />

      {/* Hero Text Introduction */}
      <ConceptHero />

      {/* The Glowing Grid showing top market movers */}
      <div className="flex justify-center px-8 pb-16">
        <ConceptGrid onAssetClick={(asset) => navigate(`/${asset.sym}`)} />
      </div>
    </div>
  );
}

// Global App State and Routing Core
function App() {
  const location = useLocation();
  const navigate = useNavigate();

  // Deduce the current modal state natively from the URL pathname
  // E.g., "/BTC_USDT" -> "BTC_USDT" or "/VCB.VN" -> "VCB.VN"
  const currentPath = location.pathname.replace('/', '').toUpperCase();
  
  // Create a blacklist of base routes so they don't trigger the asset modal
  const baseRoutes = ['', 'MARKETS', 'QUANT-MODELS'];
  const isAssetRoute = currentPath.length > 0 && !baseRoutes.includes(currentPath);
  
  // If we are on an asset route, build a strict payload for the Modal to consume
  const selectedAsset = isAssetRoute 
    ? { sym: currentPath, name: currentPath, price: "--", change: "--", isUp: true, vol: "--", color: "#3b82f6" } 
    : null;

  return (
    <div className="min-h-screen bg-concept-bg text-concept-text flex font-sans overflow-hidden">
      {/* 1. Left Sidebar Navigation */}
      <ConceptSidebar />

      {/* 2. Primary Layout Routes */}
      <Routes>
        <Route path="/" element={<Dashboard />} />
        <Route path="/quant-models" element={<QuantMatrixDashboard />} />
        <Route path="/markets" element={<MarketsPage />} />
        <Route path="/*" element={<Dashboard />} />
      </Routes>

      {/* Floating Assistant (RAG Chat) - Summoned on demand */}
      <AlphaQuantAssistant />

      {/* Global Interactive Chart Modal - Automatically overlays if URL is an Asset Symbol */}
      <ConceptChartModal 
          isOpen={!!selectedAsset} 
          asset={selectedAsset} 
          onClose={() => navigate('/')} 
      />
    </div>
  );
}

export default App;
