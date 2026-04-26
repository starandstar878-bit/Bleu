# Bleu Project: Deep DSP Library Guide

이 문서는 Bleu 프로젝트의 핵심 DSP 라이브러리인 **NAM, SignalSmith, HIIR, RubberBand**의 심층 분석 결과를 바탕으로 작성되었습니다. 각 라이브러리의 상세한 API 구조와 실제 구현 시 주의해야 할 기술적 포인트들을 포함하고 있습니다.

---

## 1. NeuralAmpModelerCore (NAM)
딥러닝 모델 기반의 비선형 시스템(앰프/페달) 시뮬레이션 엔진입니다.

### 📋 핵심 API 분석
- **헤더**: `#include "NAM/get_dsp.h"`, `#include "NAM/dsp.h"`
- **모델 로드**: `std::unique_ptr<nam::DSP> model = nam::get_dsp(modelPath);`
- **오디오 처리**: `model->process(inputPtr, outputPtr, numSamples);`
    - `inputPtr`, `outputPtr`은 `float**` 또는 `double**` (채널 기반 포인터 배열)
- **초기화**: `model->ResetAndPrewarm(sampleRate, maxBufferSize);`
    - RNN 모델의 경우 이전 상태값이 소리에 영향을 주므로 재생 전 반드시 호출 필요.

### 💡 기술적 주의사항
- **Eigen 최적화**: 내부적으로 Eigen 선형 대수 라이브러리를 사용합니다. 메모리 정렬 오류를 방지하기 위해 `#define EIGEN_MAX_ALIGN_BYTES 0` 설정이 필요할 수 있습니다.
- **가변 버퍼**: 오디오 콜백의 버퍼 크기가 변경될 때 `Reset()`을 적절히 호출하여 내부 버퍼를 재할당해야 합니다.

---

## 2. Signalsmith DSP
현대적인 C++ 기법을 사용한 고성능 헤더 전용 라이브러리입니다.

### 📋 핵심 API 분석
- **딜레이 (`delay.h`)**:
    ```cpp
    signalsmith::delay::Delay<float, signalsmith::delay::InterpolatorCubic> delayLine(maxDelay);
    float output = delayLine.write(inputSample).read(delaySamples);
    ```
    - `delaySamples`에 실수 값을 넣으면 `Interpolator`가 자동으로 분수 보간을 수행합니다.
- **오버샘플링 (`rates.h`)**:
    ```cpp
    signalsmith::rates::Oversampler2xFIR<float> os(channels, maxBlock);
    os.up(inputBuffers, lowSamples);   // 2배 업샘플링
    os.down(outputBuffers, lowSamples); // 다운샘플링
    ```
- **FFT (`fft.h`)**: 고성능 FFT 엔진을 제공하며, 스펙트럼 처리에 최적화되어 있습니다.

### 💡 기술적 주의사항
- **Header-only**: 별도의 `.cpp` 컴파일 없이 헤더만 포함하면 됩니다. 컴파일러가 강력하게 인라인화하여 매우 빠릅니다.

---

## 3. HIIR
IIR 폴리페이즈 필터 기반의 초고속, 초고음질 오버샘플링 라이브러리입니다.

### 📋 핵심 API 분석
- **계수 설계**:
    ```cpp
    double coefs[8]; // 8차 필터
    hiir::PolyphaseIir2Designer::compute_coefs_spec_order_tbw(coefs, 8, 0.01);
    ```
- **처리 엔진**:
    ```cpp
    hiir::Upsampler2xFpu<8> upsampler;
    upsampler.set_coefs(coefs);
    upsampler.process_block(outPtr, inPtr, numSamples);
    ```

### 💡 기술적 주의사항
- **Latency**: IIR 기반이므로 위상 왜곡이 발생할 수 있지만, 지연 시간(Latency)은 FIR에 비해 매우 낮습니다.
- **SIMD**: `Upsampler2x8xSse` 처럼 하드웨어 가속 버전이 존재하지만 메모리 정렬(Alignment) 규칙이 매우 까다로우므로 `Fpu` 버전을 먼저 사용하는 것을 권장합니다.

---

## 4. RubberBand
가장 강력한 타임 스트레칭 및 피치 시프팅 엔진입니다.

### 📋 핵심 API 분석
- **모드 설정**:
    ```cpp
    RubberBand::RubberBandStretcher stretcher(sampleRate, channels, 
        RubberBand::RubberBandStretcher::OptionProcessRealTime | 
        RubberBand::RubberBandStretcher::OptionEngineFaster);
    ```
- **파라미터 변경**: `stretcher.setTimeRatio(ratio)`, `stretcher.setPitchScale(scale)`
- **지연 관리**: `getStartDelay()`를 통해 발생하는 레이턴시 샘플 수를 확인하고 출력에서 잘라내야 동기화가 맞습니다.

### 💡 기술적 주의사항
- **RT-Safe**: 실시간 모드에서 파라미터 변경 시 실시간 안전(Lock-free)을 보장합니다.
- **CPU 부하**: `OptionEngineFiner (R3)`는 음질은 환상적이지만 CPU 점유율이 급격히 상승하므로 성능 프로파일링이 필수입니다.

---

## 🏗️ Bleu 프로젝트 적용 전략
- **앰프 시뮬**: NAM + HIIR (Anti-Aliasing)
- **공간계**: SignalSmith Delay (Cubic Interpolation)
- **루프 스테이션**: RubberBand (Real-time Tempo Sync)
