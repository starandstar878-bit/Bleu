# [Audio Plugin Development Master Roadmap]

## Project Overview
본 프로젝트는 기타 멀티 이펙터 및 루프 스테이션 기능이 통합된 고성능 오디오 플러그인(VST/AU/Standalone) 개발을 목표로 합니다. JUCE 프레임워크와 C++를 기반으로 하며, 사용자 경험(UX)을 극대화하기 위해 16:9 비율의 단일 패널 뷰, 부드러운 스와이프 전환, 그리고 직관적인 글래스모피즘(Glassmorphism) 및 플랫(Flat) 디자인을 채택합니다.

## Global UI/UX & Theme Specification

### 화면 비율 및 구조
- **16:9 비율 고정**: 메인 컨테이너는 16:9 비율을 유지합니다.
- **단일 패널 뷰**: 한 화면에 하나의 패널만 표시됩니다.
- **부드러운 스와이프**: 패널 간 이동 시 바운드(Bounce) 효과가 없는 선형 스와이프 애니메이션을 적용합니다.

### 그래픽 스타일
- **Glassmorphism 도입**: 배경 블러(Blur), 반투명 레이어, 세밀한 테두리(Stroke) 등을 활용하여 투명하고 입체적인 느낌을 강조합니다.
- **Modern Flat**: 스큐어모피즘을 배제한 깔끔한 UI 요소들을 배치합니다.

### 테마 시스템 (2종 지원)
- **Dark Starlight**: 매트한 남색-검정 그래디언트 배경 / 별빛 노란색 포인트 컬러.
- **Soft Snow-White**: 베이지 느낌의 파스텔 설백색 배경 / 파스텔 블루 & 옐로 포인트 컬러.

## Development Roadmap (Sequential Phases)

### Phase 1: Core Architecture & Global UI Framework
1.  **JUCE 프로젝트 초기화**: `AudioProcessor` 및 `AudioProcessorEditor`의 기본 구조 설정. 로우 레이턴시 보장을 위한 스레드 분리.
2.  **전역 테마 관리자 구현**: `ThemeManager` 클래스를 생성하여 실시간 테마 스위칭 및 색상 중앙 관리.
3.  **뷰포트 및 애니메이션**: 16:9 컨테이너 내 4개의 메인 패널(Effect, Amp, Output, Performance) 슬라이더 컴포넌트 구현. `juce::Animator` 등을 활용한 부드러운 트랜지션.

### Phase 2: EffectPanel Implementation
1.  **가로형 시그널 체인**: 최대 12개의 이펙터 블럭 일렬 배치.
2.  **드래그 앤 드롭**: 마우스 이벤트를 활용한 블럭 재배치/추가/제거.
3.  **인터랙션**: Bypass 토글(더블 클릭), 파라미터 패널 슬라이드업(좌클릭).

### Phase 3: AmpPanel Implementation
1.  **DSP 연동**: NAM(Neural Amp Modeler) 엔진 및 IR 로더 모듈 탑재.
2.  **UI 레이아웃**: 모델/IR 선택 드롭다운, 3-Band EQ 및 Gain/Level 노브 배치.
3.  **커스텀 컨트롤**: Glassmorphism 기반의 커스텀 `juce::Slider` (Linear Drag 방식).
4.  **IR 파형 렌더러**: 주파수 응답 데이터를 시각화하는 미니멀 뷰어.

### Phase 4: OutputPanel Implementation
1.  **실시간 스펙트럼 분석기 (FFT)**: 배경에 부드러운 FFT 렌더링 최적화.
2.  **대화형 EQ 오버레이**: 6개 대역 노드 조작(드래그, 휠, 우클릭 메뉴).
3.  **마스터 컨트롤**: 볼륨 페이더 및 Oversampling 설정.

### Phase 5: Performance Panel Implementation
1.  **루프 스테이션 코어**: 4개 독립 오디오 루프 버퍼(최대 60초) DSP 구현.
2.  **루프 패드 UI**: 대형 정사각형 패드 및 상태 표시(Empty, REC, PLAY, OVERDUB).
3.  **실시간 이펙트 컨트롤**: 12개 Bypass 토글 버튼 및 프리셋 호출.
4.  **Wah Pedal 슬라이더**: 세로형 페이더 위젯 및 MIDI CC 연동.

## AI Developer Guidelines (Strict Rules)

> [!IMPORTANT]
> 모든 코드를 생성하거나 수정할 때 다음 규칙을 철저히 준수해야 합니다.

1.  **최소한의 주석**: 꼭 필요한 경우를 제외하고 주석을 최소화하십시오.
2.  **명확한 위치 명시**: 수정 시 파일 경로와 함수명을 답변 최상단에 명시하십시오.
3.  **생략 없는 전문 작성**: 수정된 함수는 `...` 등의 생략 없이 **전체 코드(Full Function)**를 작성하십시오.
4.  **JUCE API 준수**: 모든 JUCE 관련 API 사용 시 @[/home/satara/data/JUCE Dev/Bleu/JUCE_API_Reference.md](file:///home/satara/data/JUCE%20Dev/Bleu/JUCE_API_Reference.md) 문서를 참고하여 **인수, 출력값, 기능**을 철저히 검증하십시오.
5.  **디자인 철학**: 글래스모피즘(Glassmorphism) 스타일을 최대한 도입하여 시각적으로 프리미엄한 느낌을 주어야 합니다.
6.  **효과 요약**: 답변의 마지막에 수정/추가된 기능과 로직 변화를 간결하게 요약하십시오.

## Verification Plan

### Automated Tests
- `make -C Builds/LinuxMakefile`을 통한 빌드 성공 여부 확인.
- 오디오 처리 루프의 유효성 검증.

### Manual Verification
- 컴파일된 Standalone 앱을 실행하여 UI 애니메이션 및 사운드 출력 확인.
- 테마 전환 및 스와이프 기능 테스트.
