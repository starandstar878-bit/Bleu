/*
  ==============================================================================

    Source/LoopPad.h
    Created: 23 Apr 2026
    Description: 정사각형 네온 디자인의 루프 패드 컴포넌트.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"

class LoopPad : public juce::Component
{
public:
    enum class State { Empty, Recording, Playing, Overdubbing };

    LoopPad(int index, ThemeManager& tm) : trackIndex(index), themeManager(tm) {}

    void paint(juce::Graphics& g) override
    {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat().reduced(5.0f);
        
        juce::Colour stateColor = colors.textHeader.withAlpha(0.2f);
        juce::String stateText = "EMPTY";

        switch (currentState)
        {
            case State::Empty:       stateColor = colors.textHeader.withAlpha(0.2f); stateText = "READY"; break;
            case State::Recording:   stateColor = juce::Colours::red;               stateText = "REC";   break;
            case State::Playing:     stateColor = juce::Colours::limegreen;         stateText = "PLAY";  break;
            case State::Overdubbing: stateColor = juce::Colours::orange;            stateText = "DUB";   break;
        }

        // 1. Pad Background
        g.setColour(colors.glassBackground);
        g.fillRoundedRectangle(area, 8.0f);
        
        // 2. Glow Effect
        if (currentState != State::Empty)
        {
            g.setColour(stateColor.withAlpha(0.15f));
            g.fillRoundedRectangle(area, 8.0f);
            
            // Outer Neon Glow
            g.setColour(stateColor.withAlpha(0.3f));
            g.drawRoundedRectangle(area.expanded(1.5f), 10.0f, 3.5f);
        }

        // 3. Border
        g.setColour(stateColor.withAlpha(0.8f));
        g.drawRoundedRectangle(area, 8.0f, 2.0f);

        // 4. Text
        g.setColour(juce::Colours::white.withAlpha(0.9f));
        g.setFont(juce::Font(juce::FontOptions(18.0f).withStyle("Bold").withName("Outfit")));
        g.drawText(stateText, area.translated(0, 5), juce::Justification::centred);
        
        g.setColour(colors.textMain.withAlpha(0.5f));
        g.setFont(juce::Font(juce::FontOptions(10.0f).withName("Outfit")));
        g.drawText("TRACK " + juce::String(trackIndex + 1), area.removeFromTop(20), juce::Justification::centred);
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        if (e.mods.isRightButtonDown()) currentState = State::Empty;
        else
        {
            switch (currentState)
            {
                case State::Empty:       currentState = State::Recording;   break;
                case State::Recording:   currentState = State::Playing;     break;
                case State::Playing:     currentState = State::Overdubbing; break;
                case State::Overdubbing: currentState = State::Playing;     break;
            }
        }
        repaint();
    }

private:
    int trackIndex;
    ThemeManager& themeManager;
    State currentState = State::Empty;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoopPad)
};
