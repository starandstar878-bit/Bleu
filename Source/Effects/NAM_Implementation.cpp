/*
  ==============================================================================

    Source/Effects/NAM_Implementation.cpp
    Created: 23 Apr 2026
    Description: NAM 엔진의 소스 파일들을 하나의 단위로 컴파일하기 위한 구현 파일입니다.

  ==============================================================================
*/

#ifndef NAM_SAMPLE_FLOAT
#define NAM_SAMPLE_FLOAT
#endif

#ifndef EIGEN_MAX_ALIGN_BYTES
#define EIGEN_MAX_ALIGN_BYTES 0
#endif

// NAM core source files
#include "../../Dependencies/NeuralAmpModelerCore/NAM/dsp.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/get_dsp.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/lstm.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/conv1d.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/convnet.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/activations.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/ring_buffer.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/util.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/wavenet/model.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/wavenet/a2_fast.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/wavenet/slimmable.cpp"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/container.cpp"
