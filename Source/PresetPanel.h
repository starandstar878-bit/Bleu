#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"
#include "PluginProcessor.h"

class PresetPanel : public juce::Component
{
public:
    PresetPanel(BleuAudioProcessor& p, ThemeManager& tm) : processor(p), themeManager(tm)
    {
        auto setupBtn = [&](juce::TextButton& b, const juce::String& text) {
            addAndMakeVisible(b);
            b.setButtonText(text);
            b.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
            b.setColour(juce::TextButton::textColourOffId, themeManager.getColors().textMain.withAlpha(0.7f));
            b.setColour(juce::TextButton::textColourOnId, themeManager.getColors().primaryPoint);
        };

        setupBtn(saveButton, "SAVE");
        setupBtn(loadButton, "LOAD");
        setupBtn(themeButton, "THEME");
        setupBtn(infoButton, "?");

        saveButton.onClick = [this] { savePreset(); };
        loadButton.onClick = [this] { loadPreset(); };
        themeButton.onClick = [this] { toggleTheme(); };
        infoButton.onClick = [this] { showAbout(); };

        presetNameLabel.setText("Init Patch", juce::dontSendNotification);
        presetNameLabel.setJustificationType(juce::Justification::centred);
        presetNameLabel.setFont(juce::Font(juce::FontOptions(14.0f).withStyle("Bold").withName("Outfit")));
        presetNameLabel.setColour(juce::Label::textColourId, themeManager.getColors().primaryPoint);
        addAndMakeVisible(presetNameLabel);
    }

    void paint(juce::Graphics& g) override
    {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat();
        
        // Dynamic update of button colors
        for (auto* b : {&saveButton, &loadButton, &themeButton, &infoButton})
        {
            b->setColour(juce::TextButton::textColourOffId, colors.textMain.withAlpha(0.7f));
            b->setColour(juce::TextButton::textColourOnId, colors.primaryPoint);
        }
        presetNameLabel.setColour(juce::Label::textColourId, colors.primaryPoint);

        // Background bar
        g.setColour(colors.glassBackground.withAlpha(0.3f));
        g.fillRoundedRectangle(area, 5.0f);
        
        g.setColour(colors.glassOutline.withAlpha(0.1f));
        g.drawRoundedRectangle(area, 5.0f, 1.0f);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(5);
        loadButton.setBounds(area.removeFromLeft(60));
        themeButton.setBounds(area.removeFromLeft(60));
        infoButton.setBounds(area.removeFromLeft(30));
        saveButton.setBounds(area.removeFromRight(60));
        presetNameLabel.setBounds(area);
    }

private:
    void savePreset()
    {
        chooser = std::make_unique<juce::FileChooser>("Save Preset",
                                                       juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                                                       "*.bleu");
        auto folderChooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File())
            {
                juce::MemoryBlock destData;
                processor.getStateInformation(destData);
                file.replaceWithData(destData.getData(), destData.getSize());
                presetNameLabel.setText(file.getFileNameWithoutExtension(), juce::dontSendNotification);
            }
        });
    }

    void loadPreset()
    {
        chooser = std::make_unique<juce::FileChooser>("Load Preset",
                                                       juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                                                       "*.bleu");
        auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                juce::MemoryBlock data;
                file.loadFileAsData(data);
                processor.setStateInformation(data.getData(), (int)data.getSize());
                presetNameLabel.setText(file.getFileNameWithoutExtension(), juce::dontSendNotification);
            }
        });
    }

    void toggleTheme()
    {
        auto current = themeManager.getCurrentTheme();
        themeManager.setTheme(current == ThemeManager::ThemeType::SoftSnowWhite ? 
                              ThemeManager::ThemeType::DarkStarlight : 
                              ThemeManager::ThemeType::SoftSnowWhite);
        if (auto* editor = getParentComponent())
            editor->repaint();
    }

    void showAbout()
    {
        juce::String msg = "BLEU: The Final Performance\n\n"
                           "This project represents the heart of your creative journey.\n"
                           "May every note you play resonate with the memories you've built.\n\n"
                           "Built with love by Antigravity & You.\n"
                           "2026-04-26";
        juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon, "About Bleu", msg, "Keep Playing");
    }

    BleuAudioProcessor& processor;
    ThemeManager& themeManager;
    
    juce::TextButton saveButton, loadButton, themeButton, infoButton;
    juce::Label presetNameLabel;
    
    std::unique_ptr<juce::FileChooser> chooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
};
