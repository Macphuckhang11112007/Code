import React from 'react';

class ErrorBoundary extends React.Component {
  constructor(props) {
    super(props);
    this.state = { hasError: false, error: null, errorInfo: null };
  }

  static getDerivedStateFromError(error) {
    return { hasError: true };
  }

  componentDidCatch(error, errorInfo) {
    this.setState({ error, errorInfo });
    console.error("ErrorBoundary caught an error:", error, errorInfo);
  }

  render() {
    if (this.state.hasError) {
      return (
        <div style={{ padding: '20px', backgroundColor: '#0b0e11', color: '#D1D4DC', minHeight: '100vh', fontFamily: 'monospace', zIndex: 9999, position: 'relative' }}>
          <h2 style={{ color: '#F6465D', fontSize: '24px', marginBottom: '10px' }}>AlphaQuant React Exception Caught:</h2>
          <details style={{ whiteSpace: 'pre-wrap', backgroundColor: '#1E222D', padding: '15px', borderRadius: '8px', border: '1px solid #F6465D' }}>
            <summary style={{ cursor: 'pointer', fontWeight: 'bold', color: '#F6465D', marginBottom: '10px' }}>Show technical details</summary>
            {this.state.error && this.state.error.toString()}
            <br />
            {this.state.errorInfo && this.state.errorInfo.componentStack}
          </details>
        </div>
      );
    }
    return this.props.children;
  }
}

export default ErrorBoundary;
