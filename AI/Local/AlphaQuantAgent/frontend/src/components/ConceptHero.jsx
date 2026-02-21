import React from 'react';

const ConceptHero = () => {
    return (
        <div className="text-center py-24 relative z-10">
            <h1 className="text-5xl md:text-6xl font-light tracking-tight text-white mb-6 uppercase" style={{ fontFamily: 'system-ui, -apple-system, sans-serif' }}>
                Empower your strategy <br/> 
                <span className="font-semibold text-transparent bg-clip-text bg-gradient-to-r from-gray-100 to-gray-400">
                    with quantitative precision.
                </span>
            </h1>
            <p className="text-concept-muted text-lg font-light tracking-wide max-w-2xl mx-auto uppercase">
                Advanced charting, real-time data, and algorithmic tools for the modern trader.
            </p>
        </div>
    )
}

export default ConceptHero;
