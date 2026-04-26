/*
  ==============================================================================

    Source/IRWaveformRenderer.h
    Created: 23 Apr 2026
    Description: IR 오디오 파일의 파형을 시각화하는 컴포넌트입니다.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"

class IRWaveformRenderer : public juce::Component
{
public:
    IRWaveformRenderer(ThemeManager& tm) : themeManager(tm) 
    {
        formatManager.registerBasicFormats();
    }

    void setFile(const juce::File& file)
    {
        auto* reader = formatManager.createReaderFor(file);
        if (reader != nullptr)
        {
            irThumbnail.setSource(new juce::FileInputSource(file));
            repaint();
        }
    }

    void paint(juce::Graphics& g) override
    {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat();

        // Background Glass
        ThemeManager::drawGlassPanel(g, area, colors, 10.0f);

        if (irThumbnail.getNumChannels() > 0)
        {
            auto waveformArea = getLocalBounds().reduced(5);
            
            // Draw Glow
            g.setColour(colors.primaryPoint.withAlpha(0.2f));
            irThumbnail.drawChannels(g, waveformArea.translated(0, 2), 0.0, irThumbnail.getTotalLength(), 1.1f);
            
            // Draw Main Waveform
            g.setColour(colors.primaryPoint);
            irThumbnail.drawChannels(g, waveformArea, 0.0, irThumbnail.getTotalLength(), 1.0f);
        }
        else
        {
            g.setColour(colors.primaryPoint.withAlpha(0.2f));
            g.setFont(juce::Font(juce::FontOptions(12.0f).withName("Outfit")));
            g.drawText("NO IR LOADED", getLocalBounds(), juce::Justification::centred);
        }
    }

private:
    ThemeManager& themeManager;
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache { 5 };
    juce::AudioThumbnail irThumbnail { 512, formatManager, thumbnailCache };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRWaveformRenderer)
};
