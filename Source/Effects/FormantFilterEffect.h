#pragma once

#include "BaseEffect.h"

class FormantFilterEffect : public BaseEffect {
public:
    FormantFilterEffect() {
        parameters.set("Vowel", 0.0f);
        parameters.set("Intensity", 0.5f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        for (int i = 0; i < 3; ++i) {
            filters[i].prepare({sr, (juce::uint32)samplesPerBlock, 2});
        }
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto vowel = getParameter("Vowel");
        auto intensity = getParameter("Intensity");
        
        // Simplified Formant Frequencies (Hz)
        // A: 700, 1200, 2500
        // E: 400, 1600, 2600
        // I: 250, 2000, 2800
        // O: 450, 800, 2300
        // U: 300, 600, 2200
        
        float f1 = 300.0f + (700.0f - 300.0f) * vowel;
        float f2 = 600.0f + (2000.0f - 600.0f) * vowel;
        float f3 = 2200.0f + (2800.0f - 2200.0f) * vowel;
        
        *filters[0].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, f1, 2.0f, intensity * 10.0f + 1.0f);
        *filters[1].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, f2, 2.0f, intensity * 10.0f + 1.0f);
        *filters[2].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, f3, 2.0f, intensity * 10.0f + 1.0f);
        
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> ctx(block);
        for (int i = 0; i < 3; ++i) filters[i].process(ctx);
    }

    juce::String getName() const override { return "Talkbox"; }

private:
    double sampleRate = 44100.0;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filters[3];
};
