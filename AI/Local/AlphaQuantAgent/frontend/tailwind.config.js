/** @type {import('tailwindcss').Config} */
export default {
  content: [
    "./index.html",
    "./src/**/*.{js,ts,jsx,tsx}",
  ],
  theme: {
    extend: {
      colors: {
        concept: {
          bg: '#111827', // Deep dark blue-gray
          panel: '#1f2937', // Lighter panel
          border: '#374151',
          text: '#f3f4f6',
          muted: '#9ca3af',
          green: '#10b981', // Neon green
          red: '#ef4444', // Neon red
          blue: '#3b82f6', // Neon blue
          accent: '#8b5cf6', // Purple accent
          glow: 'rgba(59, 130, 246, 0.15)' // Blue glow
        }
      },
      keyframes: {
        marquee: {
          '0%': { transform: 'translateX(0%)' },
          '100%': { transform: 'translateX(-50%)' },
        }
      },
      animation: {
        'marquee': 'marquee 40s linear infinite',
      }
    },
  },
  plugins: [],
}
