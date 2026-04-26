/*
  ==============================================================================

    Source/Effects/EffectRegistry.h
    Created: 23 Apr 2026
    Description: 이펙터 레지스트리. 매크로 기반 등록 시스템과 
                 이펙터 메타데이터(아이콘, 에디터 등) 관리를 담당합니다.

  ==============================================================================
*/

#pragma once

#include "BaseEffect.h"
#include <map>
#include <functional>

/**
 * 이펙터를 레지스트리에 등록하는 매크로
 * 사용법: REGISTER_EFFECT(DelayEffect, "Delay")
 */
#define REGISTER_EFFECT(ClassName, Name) \
    EffectRegistry::getInstance().registerEffect(Name, []() { return BaseEffect::Ptr(new ClassName()); })

class EffectRegistry
{
public:
    using Creator = std::function<BaseEffect::Ptr()>;

    struct EffectEntry
    {
        juce::String name;
        Creator creator;
        
        BaseEffect::Ptr create() const { return creator(); }
    };

    static EffectRegistry& getInstance()
    {
        static EffectRegistry instance;
        return instance;
    }

    void registerEffect(const juce::String& name, Creator creator)
    {
        registry[name] = { name, creator };
    }

    BaseEffect::Ptr createEffect(const juce::String& name)
    {
        if (registry.find(name) != registry.end())
            return registry[name].creator();
            
        return nullptr;
    }

    juce::StringArray getAvailableEffects() const
    {
        juce::StringArray names;
        for (auto const& [name, entry] : registry)
            names.add(name);
        return names;
    }

    const EffectEntry* getEffectEntry(const juce::String& name) const
    {
        if (registry.find(name) != registry.end())
            return &registry.at(name);
        return nullptr;
    }

private:
    EffectRegistry() = default;
    std::map<juce::String, EffectEntry> registry;

    JUCE_DECLARE_NON_COPYABLE(EffectRegistry)
};
