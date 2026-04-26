#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"

class NavigationBar : public juce::Component
{
public:
    NavigationBar(ThemeManager& tm) : themeManager(tm)
    {
        addItem("Effect", "layers"); // Using simple text as placeholder for icons
        addItem("Amp", "rss_feed");
        addItem("Output", "tune");
        addItem("Performance", "settings");
    }

    void addItem(const juce::String& name, const juce::String& iconName)
    {
        auto* b = buttons.add(new NavButton(name, iconName, themeManager));
        addAndMakeVisible(b);
        b->onClick = [this, index = buttons.size() - 1] {
            if (onIndexChanged) onIndexChanged(index);
            setCurrentIndex(index);
        };
    }

    void setCurrentIndex(int index)
    {
        currentIndex = index;
        for (int i = 0; i < buttons.size(); ++i)
            buttons[i]->setSelected(i == currentIndex);
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat();
        
        // Pill shaped floating bar
        g.setColour(colors.glassBackground.withAlpha(0.6f));
        g.fillRoundedRectangle(area, area.getHeight() / 2.0f);
        
        g.setColour(colors.glassOutline.withAlpha(0.2f));
        g.drawRoundedRectangle(area, area.getHeight() / 2.0f, 1.0f);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(5);
        auto w = area.getWidth() / buttons.size();
        for (auto* b : buttons)
            b->setBounds(area.removeFromLeft(w));
    }

    std::function<void(int)> onIndexChanged;

private:
    class NavButton : public juce::Button
    {
    public:
        NavButton(const juce::String& name, const juce::String& icon, ThemeManager& tm) 
            : Button(name), themeManager(tm), label(name), iconName(icon) {}

        void setSelected(bool s) { isSelected = s; repaint(); }

        void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override
        {
            auto colors = themeManager.getColors();
            auto area = getLocalBounds().toFloat().reduced(2);
            
            if (isSelected)
            {
                // Highlight for selected item (recessed/button look from image)
                g.setColour(juce::Colours::white.withAlpha(0.9f));
                g.fillRoundedRectangle(area, area.getHeight() / 2.0f);
                
                g.setColour(juce::Colours::black.withAlpha(0.1f));
                g.drawRoundedRectangle(area, area.getHeight() / 2.0f, 1.0f);
                
                g.setColour(juce::Colours::darkgrey);
            }
            else
            {
                if (isMouseOverButton)
                {
                    g.setColour(colors.primaryPoint.withAlpha(0.1f));
                    g.fillRoundedRectangle(area, area.getHeight() / 2.0f);
                }
                g.setColour(colors.textMain.withAlpha(0.6f));
            }

            g.setFont(juce::Font(juce::FontOptions(12.0f).withName("Outfit")));
            g.drawText(label, getLocalBounds(), juce::Justification::centred);
        }

    private:
        ThemeManager& themeManager;
        juce::String label, iconName;
        bool isSelected = false;
    };

    ThemeManager& themeManager;
    juce::OwnedArray<NavButton> buttons;
    int currentIndex = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NavigationBar)
};
