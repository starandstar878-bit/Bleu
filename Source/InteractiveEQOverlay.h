/*
  ==============================================================================

    Source/InteractiveEQOverlay.h
    Created: 23 Apr 2026
    Description: 대화형 EQ 오버레이. 베지어 곡선을 사용하여 부드러운 주파수 응답을 시각화합니다.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"

class InteractiveEQOverlay : public juce::Component
{
public:
    InteractiveEQOverlay(BleuAudioProcessor& p, ThemeManager& tm) 
        : processor(p), themeManager(tm)
    {
        // "Bleu Signature" Default Curve Coordinates
        struct DefaultNode { float f; float g; };
        DefaultNode defaults[] = { 
            { 100.0f, -3.0f }, { 250.0f, -1.5f }, { 850.0f, 2.0f }, 
            { 2800.0f, 1.5f }, { 6500.0f, 1.0f }, { 12000.0f, -6.0f } 
        };

        for (int i = 0; i < 6; ++i)
        {
            float x = getXForFreq(defaults[i].f);
            float y = juce::jmap(defaults[i].g, -18.0f, 18.0f, 1.0f, 0.0f);
            nodes.add(juce::Point<float>(x, y));
            updateProcessor(i);
        }
    }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat();
        juce::Array<juce::Colour> nodeColors = {
            juce::Colours::yellow, juce::Colours::orange, juce::Colours::red,
            juce::Colours::royalblue, juce::Colours::purple, juce::Colours::springgreen
        };

        // 1. Draw Actual DSP Response Curve
        juce::Path curve;
        float w = area.getWidth();
        float h = area.getHeight();
        
        bool first = true;
        for (int x = 0; x <= (int)w; x += 2)
        {
            float freq = getFreqForX((float)x / w);
            float mag = processor.getMasterEQResponse(freq);
            float db = juce::Decibels::gainToDecibels(mag);
            
            float y = juce::jmap(db, -18.0f, 18.0f, h, 0.0f);
            
            if (first) {
                curve.startNewSubPath(x, y);
                first = false;
            } else {
                curve.lineTo(x, y);
            }
        }
        
        auto colors = themeManager.getColors();
        
        // Draw Curve Glow
        g.setColour(colors.primaryPoint.withAlpha(0.15f));
        g.strokePath(curve, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        
        // Draw Main Curve
        g.setColour(colors.primaryPoint.withAlpha(0.8f));
        g.strokePath(curve, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // 2. Draw Nodes
        for (int i = 0; i < nodes.size(); ++i)
        {
            auto p = nodes[i];
            float x = p.x * area.getWidth();
            float y = p.y * area.getHeight();
            
            auto nodeColor = nodeColors[i % nodeColors.size()];
            bool isHovered = (i == hoveredIndex || i == draggedIndex);
            
            g.setColour(nodeColor.withAlpha(isHovered ? 0.4f : 0.2f));
            g.fillEllipse(x - 8, y - 8, 16, 16);
            
            g.setColour(nodeColor);
            if (i == 0 || i == 5) g.drawRect(x - 4.0f, y - 4.0f, 8.0f, 8.0f, 2.0f);
            else if (i == 1 || i == 4) {
                g.drawHorizontalLine((int)y, x - 5, x + 5);
                g.drawVerticalLine((int)x, y - 5, y + 5);
            }
            else g.fillEllipse(x - 4, y - 4, 8, 8);
            
            if (isHovered) {
                g.setColour(juce::Colours::white.withAlpha(0.5f));
                g.drawEllipse(x - 12, y - 12, 24, 24, 1.0f);
            }
        }
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        draggedIndex = findNodeAt(e.getPosition());
        repaint();
    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (draggedIndex != -1)
        {
            auto area = getLocalBounds().toFloat();
            float newX = juce::jlimit(0.0f, 1.0f, e.x / area.getWidth());
            float newY = juce::jlimit(0.0f, 1.0f, e.y / area.getHeight());
            
            nodes.set(draggedIndex, {newX, newY});
            updateProcessor(draggedIndex);
            repaint();
        }
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        draggedIndex = -1;
        repaint();
    }

    void mouseMove(const juce::MouseEvent& e) override
    {
        int newHover = findNodeAt(e.getPosition());
        if (newHover != hoveredIndex)
        {
            hoveredIndex = newHover;
            repaint();
        }
    }

private:
    float getXForFreq(float f) { return std::log10(f / 20.0f) / std::log10(20000.0f / 20.0f); }
    float getFreqForX(float x) { return 20.0f * std::pow(1000.0f, x); }
    float getGainForY(float y) { return juce::jmap(y, 1.0f, 0.0f, -18.0f, 18.0f); }

    void updateProcessor(int idx)
    {
        auto p = nodes[idx];
        float freq = getFreqForX(p.x);
        float gain = getGainForY(p.y);
        processor.updateMasterEQ(idx, freq, gain);
    }

    int findNodeAt(juce::Point<int> pos)
    {
        auto area = getLocalBounds().toFloat();
        for (int i = 0; i < nodes.size(); ++i)
        {
            auto p = nodes[i];
            juce::Point<float> actualPos(p.x * area.getWidth(), p.y * area.getHeight());
            if (actualPos.getDistanceSquaredFrom(pos.toFloat()) < 150.0f) // Slightly larger hit area
                return i;
        }
        return -1;
    }

    BleuAudioProcessor& processor;
    ThemeManager& themeManager;
    juce::Array<juce::Point<float>> nodes;
    int draggedIndex = -1;
    int hoveredIndex = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InteractiveEQOverlay)
};
