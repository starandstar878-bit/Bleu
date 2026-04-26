/*
  ==============================================================================

    Source/Utils/OversamplingHandler.h
    Created: 23 Apr 2026
    Description: HIIR 기반 2x 오버샘플링 핸들러. 
                 에일리어싱 제거를 통한 고음질 처리를 담당합니다.

  ==============================================================================
*/

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "../../Dependencies/hiir/Upsampler2xFpu.h"
#include "../../Dependencies/hiir/Downsampler2xFpu.h"

class OversamplingHandler
{
public:
    OversamplingHandler()
    {
        const double coeffs1[12] = {
            0.036681502163648017, 0.13654762463195794, 0.27463023244421021,
            0.42313861743654322, 0.56109869787254848, 0.67754004990104235,
            0.76974135381257603, 0.83989850272851217, 0.89226081800387891,
            0.93154195124318600, 0.96209454835153944, 0.98781637073289585
        };
        const double coeffs2[4] = { 0.04140024, 0.16540024, 0.36540024, 0.66540024 }; // Simple for 2nd stage
        
        for (int i = 0; i < 2; ++i)
        {
            upsamplers1[i].set_coefs(coeffs1);
            downsamplers1[i].set_coefs(coeffs1);
            upsamplers2[i].set_coefs(coeffs2);
            downsamplers2[i].set_coefs(coeffs2);
            upsamplers3[i].set_coefs(coeffs2); // Use same simple coeffs for high stages
            downsamplers3[i].set_coefs(coeffs2);
        }
    }

    void setFactor(int newFactor) { factor = newFactor; }

    void reset()
    {
        for (int i = 0; i < 2; ++i)
        {
            upsamplers1[i].clear_buffers();
            downsamplers1[i].clear_buffers();
            upsamplers2[i].clear_buffers();
            downsamplers2[i].clear_buffers();
            upsamplers3[i].clear_buffers();
            downsamplers3[i].clear_buffers();
        }
    }

    template <typename ProcessFunc>
    void process(juce::AudioBuffer<float>& buffer, ProcessFunc&& processFunc)
    {
        if (factor <= 1) { processFunc(buffer); return; }

        const int numSamples = buffer.getNumSamples();
        const int numChannels = std::min(2, buffer.getNumChannels());

        if (factor == 2)
        {
            overBuffer1.setSize(numChannels, numSamples * 2, false, false, true);
            for (int ch = 0; ch < numChannels; ++ch)
                upsamplers1[ch].process_block(overBuffer1.getWritePointer(ch), buffer.getReadPointer(ch), numSamples);
            
            processFunc(overBuffer1);

            for (int ch = 0; ch < numChannels; ++ch)
                downsamplers1[ch].process_block(buffer.getWritePointer(ch), overBuffer1.getReadPointer(ch), numSamples);
        }
        else if (factor == 4)
        {
            overBuffer1.setSize(numChannels, numSamples * 2, false, false, true);
            overBuffer2.setSize(numChannels, numSamples * 4, false, false, true);
            
            for (int ch = 0; ch < numChannels; ++ch)
            {
                upsamplers1[ch].process_block(overBuffer1.getWritePointer(ch), buffer.getReadPointer(ch), numSamples);
                upsamplers2[ch].process_block(overBuffer2.getWritePointer(ch), overBuffer1.getReadPointer(ch), numSamples * 2);
            }

            processFunc(overBuffer2);

            for (int ch = 0; ch < numChannels; ++ch)
            {
                downsamplers2[ch].process_block(overBuffer1.getWritePointer(ch), overBuffer2.getReadPointer(ch), numSamples * 2);
                downsamplers1[ch].process_block(buffer.getWritePointer(ch), overBuffer1.getReadPointer(ch), numSamples);
            }
        }
        else if (factor == 8)
        {
            overBuffer1.setSize(numChannels, numSamples * 2, false, false, true);
            overBuffer2.setSize(numChannels, numSamples * 4, false, false, true);
            overBuffer3.setSize(numChannels, numSamples * 8, false, false, true);
            
            for (int ch = 0; ch < numChannels; ++ch)
            {
                upsamplers1[ch].process_block(overBuffer1.getWritePointer(ch), buffer.getReadPointer(ch), numSamples);
                upsamplers2[ch].process_block(overBuffer2.getWritePointer(ch), overBuffer1.getReadPointer(ch), numSamples * 2);
                upsamplers3[ch].process_block(overBuffer3.getWritePointer(ch), overBuffer2.getReadPointer(ch), numSamples * 4);
            }

            processFunc(overBuffer3);

            for (int ch = 0; ch < numChannels; ++ch)
            {
                downsamplers3[ch].process_block(overBuffer2.getWritePointer(ch), overBuffer3.getReadPointer(ch), numSamples * 4);
                downsamplers2[ch].process_block(overBuffer1.getWritePointer(ch), overBuffer2.getReadPointer(ch), numSamples * 2);
                downsamplers1[ch].process_block(buffer.getWritePointer(ch), overBuffer1.getReadPointer(ch), numSamples);
            }
        }
    }

private:
    int factor = 1;
    hiir::Upsampler2xFpu<12> upsamplers1[2];
    hiir::Downsampler2xFpu<12> downsamplers1[2];
    hiir::Upsampler2xFpu<4> upsamplers2[2];
    hiir::Downsampler2xFpu<4> downsamplers2[2];
    hiir::Upsampler2xFpu<4> upsamplers3[2];
    hiir::Downsampler2xFpu<4> downsamplers3[2];
    juce::AudioBuffer<float> overBuffer1, overBuffer2, overBuffer3;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OversamplingHandler)
};
