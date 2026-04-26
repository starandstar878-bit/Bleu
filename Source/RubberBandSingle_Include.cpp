/*
  ==============================================================================

    Source/RubberBandSingle_Include.cpp
    Created: 23 Apr 2026
    Description: RubberBandSingle.cpp를 프로젝트 빌드에 포함시키기 위한 래퍼 파일입니다.

  ==============================================================================
*/

// RubberBand 컴파일을 위한 전역 설정
#define ALREADY_CONFIGURED 1
#define USE_BUILTIN_FFT 1
#define USE_BQRESAMPLER 1
#define NO_TIMING 1
#define NO_THREADING 1
#define NO_THREAD_CHECKS 1

#include "../Dependencies/rubberband/single/RubberBandSingle.cpp"
