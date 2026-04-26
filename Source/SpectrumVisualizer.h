/*
  ==============================================================================

    Source/SpectrumVisualizer.h
    Created: 23 Apr 2026
    Description: 실시간 FFT 데이터를 시각화하는 스펙트럼 분석기 컴포넌트입니다.

  ==============================================================================
*/

#pragma once

#include <cstring>
#include "ThemeManager.h"
#include "PluginProcessor.h"

class SpectrumVisualizer : public juce::Component,
                             public juce::Timer
{
public:
    SpectrumVisualizer(BleuAudioProcessor& p, ThemeManager& tm) 
        : processor(p), themeManager(tm) 
    {
        std::memset (scopeData, 0, sizeof (scopeData));
        startTimerHz(60); // 60fps for smooth visuals
    }

    void paint(juce::Graphics& g) override
    {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat();

        // 1. Background Glass Panel
        ThemeManager::drawGlassPanel(g, area, colors, 12.0f);

        // 2. Logarithmic Grid (Hz & dB)
        drawGrid(g, area, colors);

        // 3. Smooth Spectrum Path
        drawSpectrum(g, area, colors);
    }

    void timerCallback() override
    {
        if (processor.getNextResultsReady())
        {
            float fftData[BleuAudioProcessor::fftSize / 2];
            processor.getNextFFTData (fftData);
            
            // Smoothing and Log Mapping with Linear Interpolation for FFT bins
            for (int i = 0; i < scopeSize; ++i)
            {
                float freq = getFreqForBin(i);
                float bin = getBinForFreq(freq);
                
                int binLow = (int)std::floor(bin);
                int binHigh = binLow + 1;
                float alpha = bin - (float)binLow;
                
                int maxBin = BleuAudioProcessor::fftSize / 2 - 1;
                float valLow = fftData[juce::jlimit(0, maxBin, binLow)];
                float valHigh = fftData[juce::jlimit(0, maxBin, binHigh)];
                float val = valLow + (valHigh - valLow) * alpha;
                
                val = juce::Decibels::gainToDecibels(val);
                val = juce::jlimit(-80.0f, 0.0f, val);
                val = juce::jmap(val, -80.0f, 0.0f, 0.0f, 1.0f);
                
                // Exponential decay for smoothness
                if (val > scopeData[i])
                    scopeData[i] = val;
                else
                    scopeData[i] -= (scopeData[i] - val) * 0.1f; 
            }
            
            processor.setNextResultsReady (false);
            repaint();
        }
    }

private:
    void drawGrid(juce::Graphics& g, juce::Rectangle<float> area, const ThemeManager::ThemeColors& colors)
    {
        g.setColour(colors.textMain.withAlpha(0.1f));
        
        // Frequency Lines (Logarithmic)
        float freqs[] = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };
        for (float f : freqs)
        {
            float x = getXForFreq(f) * area.getWidth();
            g.drawVerticalLine((int)x, area.getY(), area.getBottom());
            
            if (f == 100 || f == 1000 || f == 10000) {
                g.setColour(colors.textMain.withAlpha(0.3f));
                g.drawText(f >= 1000 ? juce::String(f/1000, 0) + "k" : juce::String(f, 0), 
                           x + 2, area.getBottom() - 15, 40, 15, juce::Justification::left);
                g.setColour(colors.textMain.withAlpha(0.1f));
            }
        }
        
        // dB Lines
        float dbs[] = { -12, -24, -36, -48 };
        for (float db : dbs)
        {
            float y = juce::jmap(db, -60.0f, 0.0f, area.getBottom(), area.getY());
            g.drawHorizontalLine((int)y, area.getX(), area.getRight());
        }
    }

    void drawSpectrum(juce::Graphics& g, juce::Rectangle<float> area, const ThemeManager::ThemeColors& colors)
    {
        juce::Path p;
        float w = area.getWidth();
        float h = area.getHeight();
        
        // Use a more sophisticated smoothing approach
        std::vector<juce::Point<float>> points;
        for (int i = 0; i < scopeSize; ++i) 
        {
            float x = (float)i / scopeSize * w;
            float y = area.getBottom() - (scopeData[i] * h * 0.85f) - 5.0f; // Padding
            points.push_back({x, y});
        }

        if (points.size() < 2) return;

        p.startNewSubPath(points[0]);
        for (size_t i = 1; i < points.size(); ++i)
        {
            auto p1 = points[i-1];
            auto p2 = points[i];
            auto midPoint = (p1 + p2) / 2.0f;
            p.quadraticTo(p1.x, p1.y, midPoint.x, midPoint.y);
        }
        p.lineTo(points.back());
        
        // Glow Path
        g.setColour(colors.primaryPoint.withAlpha(0.2f));
        g.strokePath(p, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        
        // Main Path (Curve)
        g.setColour(colors.primaryPoint.withAlpha(0.9f));
        g.strokePath(p, juce::PathStrokeType(2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        
        // Fill Area with Gradient
        juce::Path fillPath = p;
        fillPath.lineTo(area.getRight(), area.getBottom());
        fillPath.lineTo(area.getX(), area.getBottom());
        fillPath.closeSubPath();
        
        juce::ColourGradient fillGrad(colors.primaryPoint.withAlpha(0.15f), 0, area.getY(),
                                      colors.primaryPoint.withAlpha(0.0f), 0, area.getBottom(), false);
        g.setGradientFill(fillGrad);
        g.fillPath(fillPath);
    }

    float getXForFreq(float f) { return std::log10(f / 20.0f) / std::log10(20000.0f / 20.0f); }
    float getFreqForBin(int i) { return 20.0f * std::pow(1000.0f, (float)i / scopeSize); }
    float getBinForFreq(float f) { return f / (processor.getSampleRate() / BleuAudioProcessor::fftSize); }

    BleuAudioProcessor& processor;
    ThemeManager& themeManager;
    
    static constexpr int scopeSize = 512;
    float scopeData [scopeSize];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumVisualizer)
};
