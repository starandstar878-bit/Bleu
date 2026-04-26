# JUCE API 레퍼런스 문서

> **소스**: https://docs.juce.com/master/index.html  
> **버전**: JUCE Master  
> **작성 기준**: 공식 HTML 문서 기반 / Deprecated API 제외  
> ⭐ = 핵심(Core) API

---

## 목차

1. [juce_core — 핵심 기반 클래스](#1-juce_core--핵심-기반-클래스)
2. [juce_audio_basics — 오디오 기초](#2-juce_audio_basics--오디오-기초)
3. [juce_audio_devices — 오디오·MIDI 장치](#3-juce_audio_devices--오디오-midi-장치)
4. [juce_audio_formats — 오디오 파일 포맷](#4-juce_audio_formats--오디오-파일-포맷)
5. [juce_audio_processors — 오디오 프로세서·플러그인](#5-juce_audio_processors--오디오-프로세서-플러그인)
6. [juce_events — 이벤트·타이머](#6-juce_events--이벤트-타이머)
7. [juce_graphics — 2D 그래픽](#7-juce_graphics--2d-그래픽)
8. [juce_gui_basics — GUI 기본 컴포넌트](#8-juce_gui_basics--gui-기본-컴포넌트)
9. [juce_dsp — DSP 처리](#9-juce_dsp--dsp-처리)
10. [juce_data_structures — 데이터 구조](#10-juce_data_structures--데이터-구조)
11. [juce_animation — 애니메이션](#11-juce_animation--애니메이션)
12. [juce_cryptography — 암호화](#12-juce_cryptography--암호화)

---

## 1. juce_core — 핵심 기반 클래스

### ⭐ `juce::String`

**헤더**: `juce_core/text/juce_String.h`  
**설명**: 레퍼런스 카운팅 내부 표현을 사용하는 고성능 문자열 클래스. 내부 인코딩은 UTF-8(`CharPointer_UTF8`).

#### 생성자

| 시그니처 | 설명 |
|---|---|
| `String() noexcept` | 빈 문자열 생성 |
| `String(const char* text)` | ASCII C-문자열로부터 생성 |
| `String(const char* text, size_t maxChars)` | 최대 길이 지정 생성 |
| `String(const wchar_t* text)` | wchar_t 문자열로부터 생성 |
| `String(const std::string&)` | std::string(UTF-8)으로부터 생성 |
| `String(CharPointer_UTF8 text)` | UTF-8 CharPointer로부터 생성 |
| `String(CharPointer_UTF16 text)` | UTF-16 CharPointer로부터 생성 |
| `String(CharPointer_UTF32 text)` | UTF-32 CharPointer로부터 생성 |
| `String(const String&) noexcept` | 복사 생성자 |
| `String(String&&) noexcept` | 이동 생성자 |

#### 주요 메서드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `int` | `length()` | — | 문자 수 반환 |
| `bool` | `isEmpty()` | — | 빈 문자열 여부 |
| `bool` | `isNotEmpty()` | — | 비어있지 않은지 여부 |
| `String` | `toUpperCase()` | — | 대문자 변환 |
| `String` | `toLowerCase()` | — | 소문자 변환 |
| `String` | `trim()` | — | 앞뒤 공백 제거 |
| `String` | `trimStart()` | — | 앞 공백 제거 |
| `String` | `trimEnd()` | — | 뒤 공백 제거 |
| `bool` | `startsWith(StringRef)` | `text` | 특정 문자열로 시작하는지 확인 |
| `bool` | `endsWith(StringRef)` | `text` | 특정 문자열로 끝나는지 확인 |
| `bool` | `contains(StringRef)` | `text` | 포함 여부 확인 |
| `bool` | `containsIgnoreCase(StringRef)` | `text` | 대소문자 무시 포함 여부 |
| `int` | `indexOf(StringRef)` | `text` | 첫 번째 발견 위치 반환(-1 = 없음) |
| `int` | `lastIndexOf(StringRef)` | `text` | 마지막 발견 위치 반환 |
| `String` | `substring(int start)` | `startIndex` | 부분 문자열 반환 |
| `String` | `substring(int start, int end)` | `startIndex, endIndex` | 범위 부분 문자열 |
| `String` | `replace(StringRef, StringRef)` | `target, replacement` | 문자열 치환 |
| `String` | `replaceFirstOccurrenceOf(StringRef, StringRef)` | `target, replacement` | 첫 번째 발견 치환 |
| `StringArray` | `split(StringRef)` | `separator` | 구분자로 분리 |
| `int` | `getIntValue()` | — | 정수 변환 |
| `double` | `getDoubleValue()` | — | double 변환 |
| `float` | `getFloatValue()` | — | float 변환 |
| `int64` | `getLargeIntValue()` | — | int64 변환 |
| `const char*` | `toRawUTF8()` | — | UTF-8 C-문자열 포인터 반환 |
| `std::string` | `toStdString()` | — | std::string 변환 |
| `int` | `hashCode()` | — | 32비트 해시 반환 |
| `int64` | `hashCode64()` | — | 64비트 해시 반환 |
| `bool` | `equalsIgnoreCase(StringRef)` | `other` | 대소문자 무시 비교 |
| `String& ` | `operator+=(const String&)` | `stringToAppend` | 문자열 이어붙이기 |
| `void` | `append(const String&, size_t)` | `textToAppend, maxCharsToTake` | 최대 길이 제한 이어붙이기 |

#### 정적 메서드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `String` | `String::formatted(const char* format, ...)` | `formatStr, ...` | printf 스타일 포맷 문자열 |
| `String` | `String::charToString(juce_wchar)` | `character` | 단일 문자로부터 생성 |
| `String` | `String::createStringFromData(const void*, int)` | `data, size` | 바이트 배열로부터 생성 |
| `String` | `String::toHexString(int)` | `number` | 16진수 문자열 변환 |
| `String` | `String::toHexString(const void*, int, int)` | `data, size, groupSize` | 바이트 배열을 16진수 문자열로 |

---

### `juce::StringArray`

**설명**: 문자열 목록을 담는 특수 배열.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `int` | `size()` | — | 원소 수 |
| `void` | `add(const String&)` | `text` | 문자열 추가 |
| `void` | `insert(int, const String&)` | `index, text` | 특정 인덱스에 삽입 |
| `void` | `remove(int)` | `index` | 인덱스 원소 제거 |
| `void` | `clear()` | — | 전체 제거 |
| `void` | `sort(bool ignoreCase)` | `ignoreCase` | 정렬 |
| `void` | `removeDuplicates(bool ignoreCase)` | `ignoreCase` | 중복 제거 |
| `bool` | `contains(StringRef)` | `text` | 포함 여부 |
| `int` | `indexOf(StringRef)` | `text` | 위치 반환 |
| `String` | `joinIntoString(StringRef, int, int)` | `separator, start, end` | 하나의 문자열로 결합 |
| `void` | `addTokens(StringRef, StringRef, StringRef)` | `str, breakChars, quoteChars` | 토큰화하여 추가 |

---

### ⭐ `juce::File`

**헤더**: `juce_core/files/juce_File.h`  
**설명**: 파일/디렉토리를 나타내는 클래스.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `exists()` | — | 파일 존재 여부 |
| `bool` | `isDirectory()` | — | 디렉토리 여부 |
| `bool` | `isReadable()` | — | 읽기 가능 여부 |
| `bool` | `isWritable()` | — | 쓰기 가능 여부 |
| `bool` | `createDirectory()` | — | 디렉토리 생성 |
| `bool` | `deleteFile()` | — | 파일 삭제 |
| `bool` | `deleteRecursively()` | — | 디렉토리 재귀 삭제 |
| `bool` | `copyFileTo(const File&)` | `destFile` | 파일 복사 |
| `bool` | `moveFileTo(const File&)` | `destFile` | 파일 이동 |
| `String` | `getFileName()` | — | 파일명만 반환 |
| `String` | `getFileNameWithoutExtension()` | — | 확장자 제외 파일명 |
| `String` | `getFileExtension()` | — | 확장자 반환 |
| `String` | `getFullPathName()` | — | 전체 경로 반환 |
| `File` | `getParentDirectory()` | — | 상위 디렉토리 반환 |
| `File` | `getChildFile(StringRef)` | `subPath` | 하위 경로 파일 반환 |
| `int64` | `getSize()` | — | 파일 크기 (바이트) |
| `String` | `loadFileAsString()` | — | 파일 내용 전체 문자열로 로드 |
| `bool` | `replaceWithText(const String&)` | `text` | 텍스트로 파일 전체 교체 |
| `bool` | `appendText(const String&)` | `text` | 텍스트 파일에 추가 |
| `Array<File>` | `findChildFiles(int, bool, const String&)` | `whatToLookFor, searchRecursively, wildCardPattern` | 조건에 맞는 파일 목록 |

#### 정적 메서드

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `File` | `File::getSpecialLocation(SpecialLocationType)` | 특수 위치(홈, 데스크탑 등) 반환 |
| `File` | `File::createTempFile(StringRef)` | 임시 파일 생성 |
| `File` | `File::getCurrentWorkingDirectory()` | 현재 작업 디렉토리 반환 |

---

### ⭐ `juce::ValueTree`

**헤더**: `juce_data_structures/values/juce_ValueTree.h`  
**설명**: XML 유사 트리 데이터 구조. 상태 저장/복원, Undo/Redo에 활용.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `isValid()` | — | 유효 여부 확인 |
| `Identifier` | `getType()` | — | 노드 타입 반환 |
| `var` | `getProperty(const Identifier&)` | `name` | 속성 값 반환 |
| `var` | `getProperty(const Identifier&, const var&)` | `name, defaultReturnValue` | 기본값 포함 속성 반환 |
| `void` | `setProperty(const Identifier&, const var&, UndoManager*)` | `name, value, undoManager` | 속성 설정 |
| `bool` | `hasProperty(const Identifier&)` | `name` | 속성 존재 여부 |
| `void` | `removeProperty(const Identifier&, UndoManager*)` | `name, undoManager` | 속성 제거 |
| `int` | `getNumProperties()` | — | 속성 수 반환 |
| `int` | `getNumChildren()` | — | 자식 노드 수 |
| `ValueTree` | `getChild(int)` | `index` | 인덱스로 자식 반환 |
| `ValueTree` | `getChildWithName(const Identifier&)` | `type` | 타입명으로 자식 반환 |
| `void` | `addChild(ValueTree, int, UndoManager*)` | `child, insertIndex, undoManager` | 자식 노드 추가 |
| `void` | `removeChild(const ValueTree&, UndoManager*)` | `child, undoManager` | 자식 노드 제거 |
| `void` | `removeAllChildren(UndoManager*)` | `undoManager` | 모든 자식 제거 |
| `ValueTree` | `getParent()` | — | 부모 노드 반환 |
| `ValueTree` | `createCopy()` | — | 깊은 복사 |
| `std::unique_ptr<XmlElement>` | `createXml()` | — | XML 변환 |
| `void` | `addListener(Listener*)` | `listener` | 변경 리스너 등록 |
| `void` | `removeListener(Listener*)` | `listener` | 리스너 해제 |

#### 정적 메서드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `ValueTree` | `ValueTree::fromXml(const XmlElement&)` | `xml` | XML로부터 ValueTree 생성 |

---

### `juce::var`

**설명**: 모든 타입을 담을 수 있는 Variant 타입.

| 타입 | 설명 |
|---|---|
| `int`, `int64`, `double`, `bool` | 기본 숫자/불리언 타입 |
| `String` | 문자열 |
| `Array<var>` | 배열 |
| `DynamicObject*` | 객체 |

---

### ⭐ `juce::Thread`

**헤더**: `juce_core/threads/juce_Thread.h`  
**설명**: 플랫폼 독립 스레드 기반 클래스.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `startThread()` | — | 스레드 시작 |
| `void` | `startThread(Priority)` | `priority` | 우선순위 지정 시작 |
| `void` | `stopThread(int)` | `timeOutMilliseconds` | 스레드 정지 요청 및 대기 |
| `bool` | `isThreadRunning()` | — | 실행 중 여부 확인 |
| `void` | `signalThreadShouldExit()` | — | 종료 신호 전달 |
| `bool` | `threadShouldExit()` | — | 종료 신호 확인 (오버라이드 run() 내부에서 사용) |
| `bool` | `waitForThreadToExit(int)` | `timeOutMs` | 스레드 종료 대기 |
| `virtual void` | `run()` | — | **[오버라이드 필수]** 스레드 실행 로직 구현 |
| `ThreadID` | `getThreadId()` | — | 스레드 ID 반환 |
| `String` | `getThreadName()` | — | 스레드 이름 반환 |

#### 정적 메서드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `Thread::sleep(int)` | `milliseconds` | 현재 스레드 슬립 |
| `void` | `Thread::yield()` | — | 스레드 양보 |
| `ThreadID` | `Thread::getCurrentThreadId()` | — | 현재 스레드 ID 반환 |
| `bool` | `Thread::isMainThread()` | — | 메인 스레드 여부 |

---

### `juce::CriticalSection`

**설명**: 뮤텍스/임계 구역. 멀티스레드 동기화에 사용.

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `void` | `enter()` | 락 획득 |
| `void` | `exit()` | 락 해제 |
| `bool` | `tryEnter()` | 락 획득 시도 (즉시 반환) |

> `ScopedLock lock(criticalSection);` 로 RAII 방식 사용 권장.

---

### `juce::OwnedArray<T>`

**설명**: 소유권을 가지는 포인터 배열. 삭제 시 원소 자동 소멸.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `int` | `size()` | — | 원소 수 |
| `T*` | `add(T*)` | `newObject` | 원소 추가 (소유권 이전) |
| `T*` | `insert(int, T*)` | `index, newObject` | 특정 위치에 삽입 |
| `void` | `remove(int, bool)` | `index, deleteObject` | 원소 제거 |
| `void` | `clear(bool)` | `deleteObjects` | 전체 제거 |
| `T*` | `operator[](int)` | `index` | 인덱스 접근 |
| `void` | `sort(Comparator&)` | `comparator` | 정렬 |

---

### `juce::HashMap<KeyType, ValueType>`

**설명**: 해시 기반 키-값 맵.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `set(KeyTypeParameter, ValueTypeParameter)` | `key, value` | 키-값 설정 |
| `ValueType` | `operator[](KeyTypeParameter)` | `key` | 값 접근 |
| `bool` | `contains(KeyTypeParameter)` | `key` | 키 존재 여부 |
| `void` | `remove(KeyTypeParameter)` | `key` | 키 제거 |
| `void` | `clear()` | — | 전체 제거 |
| `int` | `size()` | — | 원소 수 |

---

## 2. juce_audio_basics — 오디오 기초

### ⭐ `juce::AudioBuffer<Type>`

**헤더**: `juce_audio_basics/buffers/juce_AudioSampleBuffer.h`  
**설명**: 다채널 부동소수점 오디오 샘플 버퍼. 타입 파라미터는 주로 `float` 또는 `double`.

#### 생성자

| 시그니처 | 설명 |
|---|---|
| `AudioBuffer() noexcept` | 빈 버퍼 (0채널, 0샘플) |
| `AudioBuffer(int numChannels, int numSamples)` | 지정 채널/샘플 수로 할당 |
| `AudioBuffer(Type* const* data, int numChannels, int numSamples)` | 외부 메모리 참조 버퍼 |
| `AudioBuffer(Type* const* data, int numChannels, int startSample, int numSamples)` | 오프셋 포함 외부 메모리 참조 |
| `AudioBuffer(const AudioBuffer&)` | 복사 생성자 |
| `AudioBuffer(AudioBuffer&&) noexcept` | 이동 생성자 |

#### 크기/접근

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `int` | `getNumChannels()` | — | 채널 수 반환 |
| `int` | `getNumSamples()` | — | 샘플 수 반환 |
| `const Type*` | `getReadPointer(int channel)` | `channelNumber` | 읽기 전용 채널 포인터 |
| `const Type*` | `getReadPointer(int channel, int sample)` | `channelNumber, sampleIndex` | 샘플 오프셋 포함 읽기 포인터 |
| `Type*` | `getWritePointer(int channel)` | `channelNumber` | 쓰기용 채널 포인터 |
| `Type*` | `getWritePointer(int channel, int sample)` | `channelNumber, sampleIndex` | 오프셋 포함 쓰기 포인터 |
| `const Type* const*` | `getArrayOfReadPointers()` | — | 전체 채널 읽기 포인터 배열 |
| `Type* const*` | `getArrayOfWritePointers()` | — | 전체 채널 쓰기 포인터 배열 |
| `void` | `setSize(int, int, bool, bool, bool)` | `newNumChannels, newNumSamples, keepExistingContent, clearExtraSpace, avoidReallocating` | 버퍼 크기 변경 |
| `void` | `setDataToReferTo(Type* const*, int, int)` | `dataToReferTo, newNumChannels, newNumSamples` | 외부 메모리 참조로 전환 |

#### 데이터 조작

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `clear()` | — | 모든 샘플 0으로 초기화 |
| `void` | `clear(int start, int num)` | `startSample, numSamples` | 특정 범위 초기화 |
| `void` | `clear(int channel, int start, int num)` | `channel, startSample, numSamples` | 특정 채널의 범위 초기화 |
| `bool` | `hasBeenCleared()` | — | 클리어 상태 여부 |
| `Type` | `getSample(int channel, int index)` | `channel, sampleIndex` | 단일 샘플 값 반환 |
| `void` | `setSample(int channel, int index, Type value)` | `destChannel, destSample, newValue` | 단일 샘플 값 설정 |
| `void` | `addSample(int channel, int index, Type value)` | `destChannel, destSample, valueToAdd` | 단일 샘플에 값 누적 |
| `void` | `applyGain(int, int, int, Type)` | `channel, startSample, numSamples, gain` | 채널 특정 범위에 게인 적용 |
| `void` | `applyGain(int, int, Type)` | `startSample, numSamples, gain` | 모든 채널 범위에 게인 적용 |
| `void` | `applyGain(Type)` | `gain` | 전체 데이터에 게인 적용 |
| `void` | `applyGainRamp(int, int, int, Type, Type)` | `channel, startSample, numSamples, startGain, endGain` | 게인 램프 적용 |
| `void` | `addFrom(int, int, const AudioBuffer&, int, int, int, Type)` | `destChannel, destStart, source, srcChannel, srcStart, numSamples, gain` | 다른 버퍼에서 샘플 누적 |
| `void` | `copyFrom(int, int, const AudioBuffer&, int, int, int)` | `destChannel, destStart, source, srcChannel, srcStart, numSamples` | 다른 버퍼에서 샘플 복사 |
| `void` | `copyFromWithRamp(int, int, const Type*, int, Type, Type)` | `destChannel, destStart, source, numSamples, startGain, endGain` | 게인 램프를 적용하며 복사 |
| `Range<Type>` | `findMinMax(int, int, int)` | `channel, startSample, numSamples` | 범위 내 최소/최대 반환 |
| `Type` | `getMagnitude(int, int, int)` | `channel, startSample, numSamples` | 채널 범위의 최대 절대값 |
| `Type` | `getMagnitude(int, int)` | `startSample, numSamples` | 모든 채널의 최대 절대값 |
| `Type` | `getRMSLevel(int, int, int)` | `channel, startSample, numSamples` | 채널의 RMS 레벨 |
| `void` | `reverse(int, int, int)` | `channel, startSample, numSamples` | 채널 범위 반전 |

---

### ⭐ `juce::MidiMessage`

**헤더**: `juce_audio_basics/midi/juce_MidiMessage.h`  
**설명**: MIDI 메시지 하나를 캡슐화하는 클래스.

#### 생성자

| 시그니처 | 설명 |
|---|---|
| `MidiMessage(int byte1, int byte2, int byte3, double timeStamp=0)` | 3바이트 짧은 메시지 |
| `MidiMessage(int byte1, int byte2, double timeStamp=0)` | 2바이트 메시지 |
| `MidiMessage(int byte1, double timeStamp=0)` | 1바이트 메시지 |
| `MidiMessage(const void* data, int numBytes, double timeStamp=0)` | 바이트 블록으로부터 생성 |
| `MidiMessage() noexcept` | 빈 SysEx 메시지 |

#### 메타데이터

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `const uint8*` | `getRawData()` | — | 원시 MIDI 데이터 포인터 |
| `int` | `getRawDataSize()` | — | 데이터 바이트 수 |
| `String` | `getDescription()` | — | 사람이 읽기 쉬운 설명 문자열 |
| `double` | `getTimeStamp()` | — | 타임스탬프 반환 |
| `void` | `setTimeStamp(double)` | `newTimestamp` | 타임스탬프 변경 |
| `void` | `addToTimeStamp(double)` | `delta` | 타임스탬프 증가 |
| `MidiMessage` | `withTimeStamp(double)` | `newTimestamp` | 새 타임스탬프로 복사본 반환 |
| `int` | `getChannel()` | — | MIDI 채널 반환 (1~16) |
| `void` | `setChannel(int)` | `newChannelNumber` | MIDI 채널 변경 |
| `bool` | `isForChannel(int)` | `channelNumber` | 특정 채널에 해당하는지 |

#### 노트 이벤트

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `isNoteOn(bool)` | `returnTrueForVelocity0=false` | Note On 여부 |
| `bool` | `isNoteOff(bool)` | `returnTrueForNoteOnVelocity0=true` | Note Off 여부 |
| `bool` | `isNoteOnOrOff()` | — | Note On 또는 Off 여부 |
| `int` | `getNoteNumber()` | — | 노트 번호 (0~127) |
| `void` | `setNoteNumber(int)` | `newNoteNumber` | 노트 번호 변경 |
| `uint8` | `getVelocity()` | — | Velocity 반환 (0~127) |
| `float` | `getFloatVelocity()` | — | Velocity 0~1.0으로 반환 |
| `void` | `setVelocity(float)` | `newVelocity` | Velocity 변경 |
| `void` | `multiplyVelocity(float)` | `scaleFactor` | Velocity 배율 적용 |

#### 컨트롤러/피치

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `isController()` | — | CC 메시지 여부 |
| `int` | `getControllerNumber()` | — | CC 번호 반환 |
| `int` | `getControllerValue()` | — | CC 값 반환 (0~127) |
| `bool` | `isPitchWheel()` | — | 피치휠 메시지 여부 |
| `int` | `getPitchWheelValue()` | — | 피치휠 값 반환 (0~16383) |
| `bool` | `isAftertouch()` | — | 애프터터치 여부 |
| `int` | `getAfterTouchValue()` | — | 애프터터치 값 반환 |
| `bool` | `isProgramChange()` | — | 프로그램 체인지 여부 |
| `int` | `getProgramChangeNumber()` | — | 프로그램 번호 반환 |

#### 페달/SysEx

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `bool` | `isSustainPedalOn()` | 서스테인 페달 ON 여부 |
| `bool` | `isSustainPedalOff()` | 서스테인 페달 OFF 여부 |
| `bool` | `isSysEx()` | SysEx 메시지 여부 |
| `const uint8*` | `getSysExData()` | SysEx 데이터 포인터 |
| `int` | `getSysExDataSize()` | SysEx 데이터 크기 |

#### 정적 팩토리 메서드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `MidiMessage` | `noteOn(int, int, uint8)` | `channel, noteNumber, velocity` | Note On 메시지 생성 |
| `MidiMessage` | `noteOff(int, int, uint8)` | `channel, noteNumber, velocity` | Note Off 메시지 생성 |
| `MidiMessage` | `controllerEvent(int, int, int)` | `channel, controllerType, value` | CC 메시지 생성 |
| `MidiMessage` | `pitchWheel(int, int)` | `channel, position` | 피치휠 메시지 생성 |
| `MidiMessage` | `programChange(int, int)` | `channel, programNumber` | 프로그램 체인지 생성 |
| `MidiMessage` | `allNotesOff(int)` | `channel` | All Notes Off 생성 |
| `MidiMessage` | `allSoundOff(int)` | `channel` | All Sound Off 생성 |
| `MidiMessage` | `createSysExMessage(const void*, int)` | `sysexData, dataSize` | SysEx 메시지 생성 |
| `MidiMessage` | `midiClock()` | — | MIDI Clock 메시지 |
| `MidiMessage` | `midiStart()` | — | MIDI Start 메시지 |
| `MidiMessage` | `midiStop()` | — | MIDI Stop 메시지 |
| `MidiMessage` | `midiContinue()` | — | MIDI Continue 메시지 |
| `String` | `getMidiNoteName(int, bool, bool, int)` | `noteNumber, useSharps, includeOctaveNumber, octaveNumForMiddleC` | MIDI 노트명 반환 |
| `int` | `getMidiNoteInHertz(int, double)` | `noteNumber, frequencyOfA` | 노트 번호를 주파수로 변환 |

---

### ⭐ `juce::MidiBuffer`

**설명**: 타임스탬프를 가진 MIDI 메시지들의 컬렉션.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `clear()` | — | 버퍼 비우기 |
| `void` | `clear(int, int)` | `start, numSamples` | 범위 내 메시지 제거 |
| `void` | `addEvent(const MidiMessage&, int)` | `message, sampleNumber` | 이벤트 추가 |
| `void` | `addEvents(const MidiBuffer&, int, int, int)` | `otherBuffer, startSample, numSamples, sampleDeltaToAdd` | 다른 버퍼 이벤트 추가 |
| `bool` | `isEmpty()` | — | 비어있는지 여부 |
| `int` | `getNumEvents()` | — | 이벤트 수 반환 |
| `MidiBufferIterator` | `begin()` | — | 이터레이터 시작 |
| `MidiBufferIterator` | `end()` | — | 이터레이터 끝 |

---

### `juce::ADSR`

**설명**: 가장 기본적인 Attack-Decay-Sustain-Release 엔벨로프 제너레이터.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setSampleRate(double)` | `newSampleRate` | 샘플 레이트 설정 |
| `void` | `setParameters(const Params&)` | `parameters` | ADSR 파라미터 설정 |
| `void` | `noteOn()` | — | 노트 시작 (Attack 트리거) |
| `void` | `noteOff()` | — | 노트 릴리스 (Release 트리거) |
| `void` | `reset()` | — | 엔벨로프 초기화 |
| `bool` | `isActive()` | — | 엔벨로프 활성 여부 |
| `float` | `getNextSample()` | — | 다음 엔벨로프 샘플 반환 |
| `void` | `applyEnvelopeToBuffer(AudioBuffer<float>&, int, int)` | `buffer, startSample, numSamples` | 버퍼에 엔벨로프 적용 |

```cpp
// ADSR::Params 구조체
struct Params {
    float attack  = 0.1f;  // 초
    float decay   = 0.1f;  // 초
    float sustain = 1.0f;  // 0~1
    float release = 0.1f;  // 초
};
```

---

### ⭐ `juce::Synthesiser`

**설명**: 다성 신디사이저 엔진 기반 클래스.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `addVoice(SynthesiserVoice*)` | `newVoice` | 보이스 추가 |
| `void` | `clearVoices()` | — | 모든 보이스 제거 |
| `int` | `getNumVoices()` | — | 보이스 수 반환 |
| `SynthesiserVoice*` | `getVoice(int)` | `index` | 보이스 반환 |
| `void` | `addSound(const SynthesiserSound::Ptr&)` | `newSound` | 사운드 추가 |
| `void` | `clearSounds()` | — | 모든 사운드 제거 |
| `int` | `getNumSounds()` | — | 사운드 수 반환 |
| `void` | `setCurrentPlaybackSampleRate(double)` | `newRate` | 샘플 레이트 설정 |
| `void` | `renderNextBlock(AudioBuffer<float>&, const MidiBuffer&, int, int)` | `outputAudio, inputMidi, startSample, numSamples` | 다음 오디오 블록 렌더링 |
| `void` | `noteOn(int, int, float)` | `midiChannel, midiNoteNumber, velocity` | 노트 시작 |
| `void` | `noteOff(int, int, float, bool)` | `midiChannel, midiNoteNumber, velocity, allowTailOff` | 노트 오프 |
| `void` | `allNotesOff(int, bool)` | `midiChannel, allowTailOff` | 모든 노트 오프 |
| `void` | `setNoteStealingEnabled(bool)` | `shouldSteal` | 노트 스틸링 활성화 |

---

### `juce::SmoothedValue<FloatType, SmoothingType>`

**설명**: 파라미터 값을 부드럽게 보간하여 팝핑 방지.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `reset(double, double)` | `sampleRate, rampLengthInSeconds` | 샘플레이트 기반 초기화 |
| `void` | `reset(int)` | `numSteps` | 스텝 수 기반 초기화 |
| `void` | `setCurrentAndTargetValue(FloatType)` | `newValue` | 현재·목표값 동시 설정 |
| `void` | `setTargetValue(FloatType)` | `newValue` | 목표값 설정 |
| `FloatType` | `getNextValue()` | — | 보간된 다음 값 반환 |
| `FloatType` | `getCurrentValue()` | — | 현재 값 반환 |
| `FloatType` | `getTargetValue()` | — | 목표값 반환 |
| `bool` | `isSmoothing()` | — | 보간 진행 중 여부 |
| `void` | `skip(int)` | `numSamples` | N개 샘플 건너뛰기 |
| `void` | `applyGain(AudioBuffer<FloatType>&, int)` | `buffer, numSamples` | 버퍼에 게인 보간 적용 |

---

### `juce::FloatVectorOperations`

**설명**: SIMD 최적화 부동소수점 벡터 연산 유틸리티.

| 반환 타입 | 정적 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `clear(float*, int)` | `dest, numValues` | 0으로 초기화 |
| `void` | `fill(float*, float, int)` | `dest, valueToFill, numValues` | 단일 값으로 채우기 |
| `void` | `copy(float*, const float*, int)` | `dest, src, numValues` | 복사 |
| `void` | `add(float*, float, int)` | `dest, amountToAdd, numValues` | 스칼라 덧셈 |
| `void` | `add(float*, const float*, int)` | `dest, src, numValues` | 벡터 덧셈 |
| `void` | `multiply(float*, float, int)` | `dest, multiplier, numValues` | 스칼라 곱셈 |
| `void` | `multiply(float*, const float*, int)` | `dest, src, numValues` | 벡터 곱셈 |
| `void` | `negate(float*, int)` | `dest, numValues` | 부호 반전 |
| `void` | `abs(float*, int)` | `dest, numValues` | 절대값 |
| `float` | `findMinimum(const float*, int)` | `src, numValues` | 최소값 반환 |
| `float` | `findMaximum(const float*, int)` | `src, numValues` | 최대값 반환 |
| `void` | `clip(float*, const float*, float, float, int)` | `dest, src, low, high, numValues` | 클리핑 |
| `void` | `convertFixedToFloat(float*, const int*, float, int)` | `dest, src, multiplier, numValues` | 고정소수점 → 부동소수점 |

---

## 3. juce_audio_devices — 오디오·MIDI 장치

### ⭐ `juce::AudioDeviceManager`

**헤더**: `juce_audio_devices/audio_io/juce_AudioDeviceManager.h`  
**설명**: 오디오·MIDI 입출력 장치 상태 관리. `ChangeBroadcaster` 상속.

#### 초기화

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `String` | `initialise(int, int, const XmlElement*, bool, const String&, const AudioDeviceSetup*)` | `maxInputChs, maxOutputChs, savedState, selectDefaultOnFailure, preferredDeviceName, preferredSetupOptions` | 오디오 장치 초기화 |
| `String` | `initialiseWithDefaultDevices(int, int)` | `numInputChs, numOutputChs` | 기본 장치로 초기화 |

#### 장치 설정

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `AudioDeviceSetup` | `getAudioDeviceSetup()` | — | 현재 장치 설정 반환 |
| `String` | `setAudioDeviceSetup(const AudioDeviceSetup&, bool)` | `newSetup, treatAsChosenDevice` | 장치 설정 변경 |
| `AudioIODevice*` | `getCurrentAudioDevice()` | — | 현재 오디오 장치 반환 |
| `String` | `getCurrentAudioDeviceType()` | — | 현재 장치 타입 문자열 반환 |
| `void` | `setCurrentAudioDeviceType(const String&, bool)` | `type, treatAsChosenDevice` | 장치 타입 변경 |
| `void` | `closeAudioDevice()` | — | 현재 장치 닫기 |
| `void` | `restartLastAudioDevice()` | — | 마지막 장치 재시작 |

#### 콜백 관리

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `addAudioCallback(AudioIODeviceCallback*)` | `newCallback` | 오디오 콜백 등록 |
| `void` | `removeAudioCallback(AudioIODeviceCallback*)` | `callback` | 오디오 콜백 제거 |

#### MIDI

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setMidiInputDeviceEnabled(const String&, bool)` | `deviceIdentifier, enabled` | MIDI 입력 장치 활성화/비활성화 |
| `bool` | `isMidiInputDeviceEnabled(const String&)` | `deviceIdentifier` | MIDI 입력 활성화 여부 |
| `void` | `addMidiInputDeviceCallback(const String&, MidiInputCallback*)` | `deviceIdentifier, callback` | MIDI 입력 콜백 등록 |
| `void` | `removeMidiInputDeviceCallback(const String&, MidiInputCallback*)` | `deviceIdentifier, callback` | MIDI 입력 콜백 제거 |
| `void` | `setDefaultMidiOutputDevice(const String&)` | `deviceIdentifier` | 기본 MIDI 출력 장치 설정 |
| `String` | `getDefaultMidiOutputDeviceIdentifier()` | — | 기본 MIDI 출력 식별자 반환 |
| `MidiOutput*` | `getDefaultMidiOutput()` | — | 기본 MIDI 출력 장치 반환 |

#### 상태

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `double` | `getCpuUsage()` | 오디오 콜백의 CPU 사용률 반환 (0.0~1.0) |
| `std::unique_ptr<XmlElement>` | `createStateXml()` | 현재 상태 XML 직렬화 |

---

### `juce::AudioIODeviceCallback`

**설명**: 오디오 입출력 콜백 인터페이스. `AudioDeviceManager::addAudioCallback()`에 등록.

| 반환 타입 | 순수 가상 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `audioDeviceIOCallbackWithContext(const float* const*, int, float* const*, int, int, const AudioIODeviceCallbackContext&)` | `inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples, context` | **[오버라이드 필수]** 오디오 처리 콜백 |
| `void` | `audioDeviceAboutToStart(AudioIODevice*)` | `device` | **[오버라이드 필수]** 장치 시작 전 호출 |
| `void` | `audioDeviceStopped()` | — | **[오버라이드 필수]** 장치 정지 후 호출 |
| `void` | `audioDeviceError(const String&)` | `errorMessage` | 오류 발생 시 호출 |

---

### `juce::MidiInput`

**설명**: 구형 바이트스트림 포맷의 MIDI 입력 장치.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `String` | `getName()` | — | 장치 이름 반환 |
| `String` | `getIdentifier()` | — | 장치 식별자 반환 |
| `void` | `start()` | — | MIDI 입력 수신 시작 |
| `void` | `stop()` | — | MIDI 입력 수신 정지 |

#### 정적 메서드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `Array<MidiDeviceInfo>` | `MidiInput::getAvailableDevices()` | — | 사용 가능한 MIDI 입력 장치 목록 |
| `MidiDeviceInfo` | `MidiInput::getDefaultDevice()` | — | 기본 MIDI 입력 장치 정보 |
| `std::unique_ptr<MidiInput>` | `MidiInput::openDevice(const String&, MidiInputCallback*)` | `deviceIdentifier, callback` | 장치 열기 |

---

### `juce::MidiOutput`

**설명**: 구형 바이트스트림 포맷의 MIDI 출력 장치.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `String` | `getName()` | — | 장치 이름 반환 |
| `void` | `sendMessageNow(const MidiMessage&)` | `message` | 즉시 MIDI 메시지 전송 |
| `void` | `sendBlockOfMessages(const MidiBuffer&, double, double)` | `buffer, millisecondCounterToStartAt, samplesPerSecond` | 블록 메시지 전송 |
| `void` | `clearAllPendingMessages()` | — | 대기 중 메시지 삭제 |
| `void` | `startBackgroundThread()` | — | 백그라운드 스레드 시작 |
| `void` | `stopBackgroundThread()` | — | 백그라운드 스레드 정지 |

#### 정적 메서드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `Array<MidiDeviceInfo>` | `MidiOutput::getAvailableDevices()` | — | 사용 가능한 출력 장치 목록 |
| `std::unique_ptr<MidiOutput>` | `MidiOutput::openDevice(const String&)` | `deviceIdentifier` | 장치 열기 |

---

## 4. juce_audio_formats — 오디오 파일 포맷

### ⭐ `juce::AudioFormatManager`

**설명**: 다양한 오디오 포맷 리더/라이터 등록 및 관리.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `registerBasicFormats()` | — | 기본 포맷(WAV, AIFF 등) 일괄 등록 |
| `void` | `registerFormat(AudioFormat*, bool)` | `newFormat, makeThisTheDefaultFormat` | 포맷 등록 |
| `int` | `getNumKnownFormats()` | — | 등록된 포맷 수 |
| `AudioFormat*` | `getKnownFormat(int)` | `index` | 인덱스로 포맷 반환 |
| `AudioFormat*` | `getDefaultFormat()` | — | 기본 포맷 반환 |
| `AudioFormatReader*` | `createReaderFor(const File&)` | `audioFile` | 파일에서 리더 생성 |
| `AudioFormatReader*` | `createReaderFor(std::unique_ptr<InputStream>)` | `audioFileStream` | 스트림에서 리더 생성 |
| `String` | `getWildcardForAllFormats()` | — | 파일 선택 필터 와일드카드 반환 |

---

### ⭐ `juce::AudioFormatReader`

**설명**: 오디오 파일에서 샘플 데이터를 읽는 기반 클래스.

| 멤버 | 타입 | 설명 |
|---|---|---|
| `sampleRate` | `double` | 파일의 샘플 레이트 |
| `numChannels` | `unsigned int` | 채널 수 |
| `lengthInSamples` | `int64` | 전체 샘플 수 |
| `bitsPerSample` | `unsigned int` | 비트 깊이 |
| `usesFloatingPointData` | `bool` | 부동소수점 데이터 여부 |

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `read(int* const*, int, int64, int, bool)` | `destChannels, numDestChannels, startSampleInSource, numSamplesToRead, fillLeftoverChannelsWithCopies` | 정수 형태로 샘플 읽기 |
| `bool` | `read(AudioBuffer<float>*, int, int, int64, bool, bool)` | `buffer, startSampleInDestBuffer, numSamples, readerStartSample, useReaderLeftChan, useReaderRightChan` | AudioBuffer에 샘플 읽기 |
| `void` | `readMaxLevels(int64, int64, float&, float&, float&, float&)` | `startSample, numSamples, lowestLeft, highestLeft, lowestRight, highestRight` | 최대 레벨 분석 |
| `AudioChannelSet` | `getChannelLayout()` | — | 채널 레이아웃 반환 |

---

### `juce::AudioFormatWriter`

**설명**: 오디오 파일에 데이터를 쓰는 기반 클래스.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `writeFromAudioReader(AudioFormatReader&, int64, int64)` | `reader, startSample, numSamplesToRead` | 리더로부터 직접 쓰기 |
| `bool` | `writeFromAudioSource(AudioSource&, int, int)` | `source, numSamplesToRead, samplesPerBlock` | AudioSource에서 쓰기 |
| `bool` | `writeFromAudioSampleBuffer(const AudioBuffer<float>&, int, int)` | `source, startSample, numSamples` | AudioBuffer에서 쓰기 |
| `double` | `getSampleRate()` | — | 샘플 레이트 반환 |
| `int` | `getNumChannels()` | — | 채널 수 반환 |
| `int` | `getBitsPerSample()` | — | 비트 깊이 반환 |
| `bool` | `isFloatingPoint()` | — | 부동소수점 여부 |

---

## 5. juce_audio_processors — 오디오 프로세서·플러그인

### ⭐ `juce::AudioProcessor`

**헤더**: `juce_audio_processors_headless/processors/juce_AudioProcessor.h`  
**설명**: VST/AU/AAX/LV2 등 오디오 플러그인의 기반 추상 클래스.

#### 반드시 오버라이드할 순수 가상 메서드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `const String` | `getName()` | — | 프로세서 이름 반환 |
| `void` | `prepareToPlay(double, int)` | `sampleRate, maximumExpectedSamplesPerBlock` | 재생 시작 전 준비 (버퍼 할당 등) |
| `void` | `releaseResources()` | — | 재생 종료 후 리소스 해제 |
| `void` | `processBlock(AudioBuffer<float>&, MidiBuffer&)` | `buffer, midiMessages` | **핵심 처리 루프** — float 블록 처리 |
| `double` | `getTailLengthSeconds()` | — | 테일 길이(초) 반환 |
| `bool` | `acceptsMidi()` | — | MIDI 입력 수락 여부 |
| `bool` | `producesMidi()` | — | MIDI 출력 생산 여부 |
| `AudioProcessorEditor*` | `createEditor()` | — | 에디터(GUI) 생성 |
| `bool` | `hasEditor()` | — | 에디터 보유 여부 |
| `int` | `getNumPrograms()` | — | 프리셋 수 반환 |
| `int` | `getCurrentProgram()` | — | 현재 프리셋 인덱스 |
| `void` | `setCurrentProgram(int)` | `index` | 프리셋 변경 |
| `const String` | `getProgramName(int)` | `index` | 프리셋 이름 반환 |
| `void` | `changeProgramName(int, const String&)` | `index, newName` | 프리셋 이름 변경 |
| `void` | `getStateInformation(MemoryBlock&)` | `destData` | 상태 직렬화 (저장) |
| `void` | `setStateInformation(const void*, int)` | `data, sizeInBytes` | 상태 역직렬화 (불러오기) |

#### 선택적 오버라이드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `processBlock(AudioBuffer<double>&, MidiBuffer&)` | `buffer, midiMessages` | double 정밀도 처리 (옵션) |
| `void` | `processBlockBypassed(AudioBuffer<float>&, MidiBuffer&)` | `buffer, midiMessages` | 바이패스 상태 처리 |
| `bool` | `supportsDoublePrecisionProcessing()` | — | 더블 정밀도 지원 여부 |
| `bool` | `isMidiEffect()` | — | MIDI 이펙트 여부 |
| `bool` | `supportsMPE()` | — | MPE 지원 여부 |
| `void` | `memoryWarningReceived()` | — | 메모리 경고 수신 |

#### 버스 관련

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `int` | `getBusCount(bool)` | `isInput` | 입/출력 버스 수 반환 |
| `Bus*` | `getBus(bool, int)` | `isInput, busIndex` | 버스 반환 |
| `bool` | `addBus(bool)` | `isInput` | 버스 동적 추가 |
| `bool` | `removeBus(bool)` | `isInput` | 버스 동적 제거 |
| `BusesLayout` | `getBusesLayout()` | — | 현재 버스 레이아웃 반환 |
| `bool` | `setBusesLayout(const BusesLayout&)` | `layouts` | 버스 레이아웃 설정 |
| `AudioChannelSet` | `getChannelLayoutOfBus(bool, int)` | `isInput, busIndex` | 특정 버스 채널 레이아웃 반환 |
| `int` | `getMainBusNumInputChannels()` | — | 메인 입력 버스 채널 수 |
| `int` | `getMainBusNumOutputChannels()` | — | 메인 출력 버스 채널 수 |
| `int` | `getTotalNumInputChannels()` | — | 총 입력 채널 수 |
| `int` | `getTotalNumOutputChannels()` | — | 총 출력 채널 수 |
| `template<typename T> AudioBuffer<T>` | `getBusBuffer(AudioBuffer<T>&, bool, int)` | `processBlockBuffer, isInput, busIndex` | 특정 버스 채널 포인터 AudioBuffer 반환 |

#### 상태 및 설정

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `double` | `getSampleRate()` | 현재 샘플 레이트 반환 |
| `int` | `getBlockSize()` | 현재 블록 크기 반환 |
| `int` | `getLatencySamples()` | 레이턴시 샘플 수 반환 |
| `void` | `setLatencySamples(int)` | 레이턴시 설정 |
| `ProcessingPrecision` | `getProcessingPrecision()` | 처리 정밀도 반환 |
| `void` | `setProcessingPrecision(ProcessingPrecision)` | 처리 정밀도 설정 |
| `AudioPlayHead*` | `getPlayHead()` | 현재 재생 헤드 반환 |
| `const CriticalSection&` | `getCallbackLock()` | 콜백 락 반환 |
| `void` | `suspendProcessing(bool)` | 처리 일시정지/재개 |
| `bool` | `isSuspended()` | 일시정지 상태 여부 |
| `void` | `updateHostDisplay(const ChangeDetails&)` | 호스트 디스플레이 업데이트 요청 |

---

### ⭐ `juce::AudioProcessorValueTreeState`

**설명**: AudioProcessor의 파라미터를 ValueTree에 연동. 호스트 자동화 지원.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `RangedAudioParameter*` | `addParameterGroup(...)` | — | 파라미터 그룹 추가 |
| `RangedAudioParameter*` | `getParameter(const String&)` | `parameterID` | ID로 파라미터 반환 |
| `float` | `getParameterRange(const String&)` | `parameterID` | 파라미터 범위 반환 |
| `std::atomic<float>*` | `getRawParameterValue(StringRef)` | `parameterID` | 원시 파라미터 값 포인터 반환 (오디오 스레드 안전) |
| `void` | `state` | — | ValueTree `state` 멤버 (저장/복원에 사용) |
| `void` | `addParameterListener(const String&, Listener*)` | `parameterID, listener` | 파라미터 변경 리스너 등록 |
| `void` | `removeParameterListener(const String&, Listener*)` | `parameterID, listener` | 리스너 제거 |
| `UndoManager*` | `getUndoManager()` | — | 언두매니저 반환 |

---

## 6. juce_events — 이벤트·타이머

### ⭐ `juce::Timer`

**헤더**: `juce_events/timers/juce_Timer.h`  
**설명**: 메시지 스레드에서 주기적 콜백을 제공하는 추상 기반 클래스.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `virtual void` | `timerCallback()` | — | **[오버라이드 필수]** 주기적으로 호출되는 콜백 |
| `void` | `startTimer(int)` | `intervalInMilliseconds` | 타이머 시작 (밀리초 간격) |
| `void` | `startTimerHz(int)` | `timerFrequencyHz` | Hz 단위로 타이머 시작 |
| `void` | `stopTimer()` | — | 타이머 정지 |
| `bool` | `isTimerRunning()` | — | 타이머 실행 여부 |
| `int` | `getTimerInterval()` | — | 현재 타이머 간격 반환 (ms) |

#### 정적 메서드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `static void` | `Timer::callAfterDelay(int, std::function<void()>)` | `milliseconds, functionToCall` | N ms 후 람다 호출 |

---

### `juce::MessageManager`

**설명**: 어플리케이션 메시지 루프 관리. 메인 스레드 이벤트 처리.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `isThisTheMessageThread()` | — | 현재 스레드가 메시지 스레드인지 |
| `void` | `callFunctionOnMessageThread(MessageCallbackFunction*, void*)` | `callback, userData` | 메시지 스레드에서 함수 호출 |

#### 정적 메서드

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `MessageManager*` | `MessageManager::getInstance()` | 싱글톤 인스턴스 반환 |
| `void` | `MessageManager::callAsync(std::function<void()>)` | 메시지 스레드에서 비동기 람다 호출 |

---

### `juce::ChangeListener` / `juce::ChangeBroadcaster`

**설명**: 옵저버 패턴 기반 변경 알림 시스템.

**ChangeBroadcaster**:

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `addChangeListener(ChangeListener*)` | `listener` | 리스너 등록 |
| `void` | `removeChangeListener(ChangeListener*)` | `listener` | 리스너 제거 |
| `void` | `sendChangeMessage()` | — | 변경 알림 전송 |
| `void` | `sendSynchronousChangeMessage()` | — | 동기 변경 알림 전송 |

**ChangeListener** (오버라이드):

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `virtual void` | `changeListenerCallback(ChangeBroadcaster*)` | `source` | **[오버라이드 필수]** 변경 알림 수신 |

---

## 7. juce_graphics — 2D 그래픽

### ⭐ `juce::Graphics`

**헤더**: `juce_graphics/contexts/juce_GraphicsContext.h`  
**설명**: 컴포넌트/이미지에 그림을 그리기 위한 그래픽 컨텍스트. `Component::paint()`의 인자로 전달됨.

#### 색상·채우기 설정

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setColour(Colour)` | `newColour` | 그리기 색상 설정 |
| `void` | `setOpacity(float)` | `newOpacity` | 현재 색상의 투명도 설정 |
| `void` | `setGradientFill(const ColourGradient&)` | `gradient` | 그라디언트 채우기 설정 |
| `void` | `setTiledImageFill(const Image&, int, int, float)` | `imageToUse, anchorX, anchorY, opacity` | 타일 이미지 채우기 설정 |
| `void` | `setFillType(const FillType&)` | `newFill` | 채우기 타입 설정 |

#### 폰트/텍스트

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setFont(const Font&)` | `newFont` | 폰트 설정 |
| `void` | `setFont(float)` | `newFontHeight` | 폰트 크기만 변경 |
| `Font` | `getCurrentFont()` | — | 현재 폰트 반환 |
| `void` | `drawSingleLineText(const String&, int, int, Justification)` | `text, startX, baselineY, justification` | 한 줄 텍스트 그리기 |
| `void` | `drawMultiLineText(const String&, int, int, int, Justification, float)` | `text, startX, baselineY, maxLineWidth, justification, leading` | 여러 줄 텍스트 그리기 |
| `void` | `drawText(const String&, int, int, int, int, Justification, bool)` | `text, x, y, width, height, justificationType, useEllipsesIfTooBig` | 사각형 내 텍스트 그리기 |
| `void` | `drawText(const String&, Rectangle<int>, Justification, bool)` | `text, area, justificationType, useEllipsesIfTooBig` | Rectangle 내 텍스트 그리기 |
| `void` | `drawFittedText(const String&, int, int, int, int, Justification, int, float)` | `text, x, y, width, height, justificationFlags, maxLines, minHorizontalScale` | 지정 공간에 맞게 텍스트 그리기 |

#### 도형 그리기

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `fillAll()` | — | 전체 클립 영역 채우기 |
| `void` | `fillAll(Colour)` | `colourToUse` | 지정 색상으로 전체 채우기 |
| `void` | `fillRect(Rectangle<int>)` | `rectangle` | 사각형 채우기 |
| `void` | `fillRect(int, int, int, int)` | `x, y, width, height` | 좌표로 사각형 채우기 |
| `void` | `fillRoundedRectangle(float, float, float, float, float)` | `x, y, width, height, cornerSize` | 라운드 사각형 채우기 |
| `void` | `fillRoundedRectangle(Rectangle<float>, float)` | `rectangle, cornerSize` | 라운드 사각형 채우기 |
| `void` | `fillEllipse(float, float, float, float)` | `x, y, width, height` | 타원 채우기 |
| `void` | `fillEllipse(Rectangle<float>)` | `area` | Rectangle으로 타원 채우기 |
| `void` | `fillPath(const Path&, const AffineTransform&)` | `path, transform` | 패스 채우기 |
| `void` | `drawRect(Rectangle<int>, int)` | `rectangle, lineThickness` | 사각형 외곽선 그리기 |
| `void` | `drawRect(int, int, int, int, int)` | `x, y, width, height, lineThickness` | 좌표로 사각형 외곽선 |
| `void` | `drawRoundedRectangle(float, float, float, float, float, float)` | `x, y, width, height, cornerSize, lineThickness` | 라운드 사각형 외곽선 |
| `void` | `drawEllipse(float, float, float, float, float)` | `x, y, width, height, lineThickness` | 타원 외곽선 |
| `void` | `drawLine(float, float, float, float)` | `startX, startY, endX, endY` | 선 그리기 |
| `void` | `drawLine(float, float, float, float, float)` | `startX, startY, endX, endY, lineThickness` | 두께 지정 선 그리기 |
| `void` | `drawLine(const Line<float>&)` | `line` | Line 객체로 선 그리기 |
| `void` | `drawLine(const Line<float>&, float)` | `line, lineThickness` | 두께 지정 Line 그리기 |
| `void` | `drawArrow(const Line<float>&, float, float, float)` | `line, lineThickness, arrowheadWidth, arrowheadLength` | 화살표 그리기 |
| `void` | `strokePath(const Path&, const PathStrokeType&, const AffineTransform&)` | `path, strokeType, transform` | 패스 외곽선 그리기 |

#### 이미지

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `drawImage(const Image&, int, int, int, int, int, int, int, int, bool)` | `imageToDraw, dx, dy, dw, dh, sx, sy, sw, sh, fillAlphaChannelWithCurrentColour` | 이미지 그리기 |
| `void` | `drawImage(const Image&, Rectangle<float>, RectanglePlacement, bool)` | `imageToDraw, targetArea, placementWithinTarget, fillAlphaChannelWithCurrentColour` | 사각형 내 이미지 그리기 |
| `void` | `drawImageAt(const Image&, int, int, bool)` | `image, topLeftX, topLeftY, fillAlphaChannelWithCurrentColour` | 좌표에 이미지 그리기 |
| `void` | `drawImageTransformed(const Image&, const AffineTransform&, bool)` | `image, transform, fillAlphaChannelWithCurrentColour` | 변환 적용 이미지 그리기 |

#### 클리핑/변환

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `reduceClipRegion(int, int, int, int)` | `x, y, width, height` | 클립 영역 축소 |
| `bool` | `reduceClipRegion(const Path&, const AffineTransform&)` | `path, transform` | 패스로 클립 영역 설정 |
| `Rectangle<int>` | `getClipBounds()` | — | 현재 클립 영역 반환 |
| `bool` | `isClipEmpty()` | — | 클립 영역 비어있는지 여부 |
| `void` | `addTransform(const AffineTransform&)` | `transform` | 변환 누적 적용 |
| `void` | `saveState()` | — | 현재 상태 저장 |
| `void` | `restoreState()` | — | 저장된 상태 복원 |
| `Graphics::ScopedSaveState` | (RAII) | — | 스코프 기반 상태 저장/복원 |

---

### ⭐ `juce::Colour`

**설명**: ARGB 색상을 나타내는 클래스.

| 반환 타입 | 메서드/팩토리 | 인자 | 설명 |
|---|---|---|---|
| `Colour` | `Colour(uint8 r, uint8 g, uint8 b)` | `red, green, blue` | RGB 생성 |
| `Colour` | `Colour(uint8 r, uint8 g, uint8 b, uint8 a)` | `red, green, blue, alpha` | RGBA 생성 |
| `Colour` | `Colour(uint32 argb)` | `argb` | 32비트 ARGB 정수로 생성 |
| `Colour` | `Colour::fromRGB(uint8, uint8, uint8)` | `red, green, blue` | 정적 팩토리 |
| `Colour` | `Colour::fromRGBA(uint8, uint8, uint8, uint8)` | `red, green, blue, alpha` | 정적 팩토리 |
| `Colour` | `Colour::fromHSV(float, float, float, float)` | `hue, saturation, brightness, alpha` | HSV로부터 생성 |
| `Colour` | `Colour::fromHSL(float, float, float, float)` | `hue, saturation, lightness, alpha` | HSL로부터 생성 |
| `Colour` | `withAlpha(uint8)` | `newAlpha` | 알파 변경 복사 |
| `Colour` | `withAlpha(float)` | `newAlpha` | 알파 변경 복사 (0~1) |
| `Colour` | `withBrightness(float)` | `brightness` | 밝기 변경 복사 |
| `Colour` | `withSaturation(float)` | `saturation` | 채도 변경 복사 |
| `Colour` | `brighter(float)` | `amount=0.4f` | 밝게 |
| `Colour` | `darker(float)` | `amount=0.4f` | 어둡게 |
| `Colour` | `contrasting(float)` | `amount=1.0f` | 대비 색상 반환 |
| `uint8` | `getRed()` | — | R 채널 반환 |
| `uint8` | `getGreen()` | — | G 채널 반환 |
| `uint8` | `getBlue()` | — | B 채널 반환 |
| `uint8` | `getAlpha()` | — | A 채널 반환 |
| `float` | `getFloatRed()` | — | R (0~1) |
| `float` | `getFloatGreen()` | — | G (0~1) |
| `float` | `getFloatBlue()` | — | B (0~1) |
| `float` | `getFloatAlpha()` | — | A (0~1) |
| `bool` | `isTransparent()` | — | 완전 투명 여부 |
| `bool` | `isOpaque()` | — | 완전 불투명 여부 |
| `String` | `toDisplayString(bool)` | `includeAlphaValue` | CSS 스타일 문자열 변환 |
| `uint32` | `getARGB()` | — | ARGB 32비트 정수 반환 |

---

### `juce::Path`

**설명**: 벡터 그래픽 경로 정의. 도형, 곡선, 복합 경로 구성.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `startNewSubPath(float, float)` | `x, y` | 새 서브패스 시작 |
| `void` | `closeSubPath()` | — | 서브패스 닫기 |
| `void` | `lineTo(float, float)` | `x, y` | 직선 추가 |
| `void` | `quadraticTo(float, float, float, float)` | `controlX, controlY, endX, endY` | 2차 베지어 곡선 |
| `void` | `cubicTo(float, float, float, float, float, float)` | `cp1x, cp1y, cp2x, cp2y, endX, endY` | 3차 베지어 곡선 |
| `void` | `addRectangle(float, float, float, float)` | `x, y, width, height` | 사각형 추가 |
| `void` | `addRoundedRectangle(float, float, float, float, float)` | `x, y, width, height, cornerSize` | 라운드 사각형 추가 |
| `void` | `addEllipse(float, float, float, float)` | `x, y, width, height` | 타원 추가 |
| `void` | `addArc(float, float, float, float, float, float, bool)` | `x, y, width, height, fromRadians, toRadians, startAsNewSubPath` | 호 추가 |
| `void` | `addArrow(const Line<float>&, float, float, float)` | `line, lineThickness, arrowheadWidth, arrowheadLength` | 화살표 추가 |
| `void` | `applyTransform(const AffineTransform&)` | `transform` | 변환 적용 |
| `bool` | `contains(float, float)` | `x, y` | 좌표가 패스 내부인지 |
| `bool` | `isEmpty()` | — | 패스가 비어있는지 |
| `Rectangle<float>` | `getBounds()` | — | 바운딩 박스 반환 |
| `void` | `clear()` | — | 패스 초기화 |

---

## 8. juce_gui_basics — GUI 기본 컴포넌트

### ⭐ `juce::Component`

**헤더**: `juce_gui_basics/components/juce_Component.h`  
**설명**: JUCE UI 객체의 기반 클래스.

#### 생성/소멸

| 시그니처 | 설명 |
|---|---|
| `Component() noexcept` | 기본 생성 |
| `Component(const String& componentName) noexcept` | 이름 지정 생성 |
| `virtual ~Component()` | 소멸자 |

#### 반드시 오버라이드할 주요 가상 메서드

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `virtual void` | `paint(Graphics&)` | `g` | 컴포넌트 그리기 |
| `virtual void` | `resized()` | — | 크기 변경 시 레이아웃 처리 |
| `virtual void` | `mouseDown(const MouseEvent&)` | `event` | 마우스 버튼 누름 |
| `virtual void` | `mouseUp(const MouseEvent&)` | `event` | 마우스 버튼 뗌 |
| `virtual void` | `mouseDrag(const MouseEvent&)` | `event` | 마우스 드래그 |
| `virtual void` | `mouseMove(const MouseEvent&)` | `event` | 마우스 이동 |
| `virtual void` | `mouseEnter(const MouseEvent&)` | `event` | 마우스 진입 |
| `virtual void` | `mouseExit(const MouseEvent&)` | `event` | 마우스 이탈 |
| `virtual void` | `mouseDoubleClick(const MouseEvent&)` | `event` | 마우스 더블클릭 |
| `virtual void` | `mouseWheelMove(const MouseEvent&, const MouseWheelDetails&)` | `event, wheel` | 마우스 휠 |
| `virtual bool` | `keyPressed(const KeyPress&)` | `key` | 키 눌림 (소비 시 true 반환) |
| `virtual void` | `focusGained(FocusChangeType)` | `cause` | 포커스 획득 |
| `virtual void` | `focusLost(FocusChangeType)` | `cause` | 포커스 잃음 |
| `virtual void` | `visibilityChanged()` | — | 가시성 변경 |
| `virtual void` | `parentHierarchyChanged()` | — | 부모 계층 변경 |
| `virtual void` | `childrenChanged()` | — | 자식 변경 |

#### 위치/크기

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setBounds(int, int, int, int)` | `x, y, width, height` | 위치·크기 설정 |
| `void` | `setBounds(Rectangle<int>)` | `newBounds` | Rectangle로 위치·크기 설정 |
| `void` | `setSize(int, int)` | `newWidth, newHeight` | 크기만 설정 |
| `void` | `setTopLeftPosition(int, int)` | `x, y` | 좌상단 위치 설정 |
| `void` | `setTopLeftPosition(Point<int>)` | `pos` | Point로 위치 설정 |
| `void` | `setCentrePosition(int, int)` | `x, y` | 중심점으로 위치 설정 |
| `void` | `centreWithSize(int, int)` | `width, height` | 부모 기준 중앙 배치 |
| `int` | `getX()` | — | X 좌표 반환 |
| `int` | `getY()` | — | Y 좌표 반환 |
| `int` | `getWidth()` | — | 너비 반환 |
| `int` | `getHeight()` | — | 높이 반환 |
| `int` | `getRight()` | — | 오른쪽 끝 X 좌표 |
| `int` | `getBottom()` | — | 아래쪽 끝 Y 좌표 |
| `Rectangle<int>` | `getBounds()` | — | 경계 Rectangle 반환 |
| `Rectangle<int>` | `getLocalBounds()` | — | 로컬 좌표 경계 반환 |
| `int` | `getScreenX()` | — | 화면 기준 X 좌표 |
| `int` | `getScreenY()` | — | 화면 기준 Y 좌표 |
| `Rectangle<int>` | `getScreenBounds()` | — | 화면 기준 경계 반환 |
| `Point<int>` | `getLocalPoint(const Component*, Point<int>)` | `sourceComponent, point` | 다른 컴포넌트 좌표 → 로컬 좌표 변환 |

#### 가시성/Z순서

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setVisible(bool)` | `shouldBeVisible` | 가시성 설정 |
| `bool` | `isVisible()` | — | 가시 여부 반환 |
| `bool` | `isShowing()` | — | 자신과 모든 부모 가시 여부 |
| `void` | `toFront(bool)` | `shouldAlsoGainKeyboardFocus` | 앞으로 이동 |
| `void` | `toBack()` | — | 뒤로 이동 |
| `void` | `setAlwaysOnTop(bool)` | `shouldStayOnTop` | 항상 앞에 유지 |

#### 자식 컴포넌트 관리

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `addAndMakeVisible(Component*, int)` | `child, zOrder=-1` | 자식 추가 및 가시화 |
| `void` | `addChildComponent(Component*, int)` | `child, zOrder=-1` | 자식 추가 (숨김 상태) |
| `void` | `removeChildComponent(Component*)` | `child` | 자식 제거 |
| `void` | `removeAllChildren()` | — | 모든 자식 제거 |
| `int` | `getNumChildComponents()` | — | 자식 수 반환 |
| `Component*` | `getChildComponent(int)` | `index` | 인덱스로 자식 반환 |
| `Component*` | `getParentComponent()` | — | 부모 컴포넌트 반환 |

#### 기타

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `repaint()` | — | 전체 리페인트 요청 |
| `void` | `repaint(int, int, int, int)` | `x, y, width, height` | 특정 영역 리페인트 요청 |
| `void` | `setOpaque(bool)` | `shouldBeOpaque` | 불투명 여부 설정 |
| `void` | `setBufferedToImage(bool)` | `shouldBeBuffered` | 이미지 버퍼링 설정 |
| `void` | `setInterceptsMouseClicks(bool, bool)` | `allowClicksOnThisComponent, allowClicksOnChildComponents` | 마우스 클릭 수신 여부 설정 |
| `void` | `setEnabled(bool)` | `shouldBeEnabled` | 활성화 여부 설정 |
| `bool` | `isEnabled()` | — | 활성화 여부 반환 |
| `void` | `grabKeyboardFocus()` | — | 키보드 포커스 획득 |
| `bool` | `hasKeyboardFocus(bool)` | `trueIfChildIsFocused` | 키보드 포커스 여부 |
| `void` | `addToDesktop(int, void*)` | `windowStyleFlags, nativeWindowToAttachTo` | 데스크탑 창으로 추가 |
| `void` | `removeFromDesktop()` | — | 데스크탑에서 제거 |
| `String` | `getName()` | — | 컴포넌트 이름 반환 |
| `void` | `setName(const String&)` | `newName` | 이름 설정 |
| `void` | `setComponentID(const String&)` | `newID` | ID 설정 |
| `String` | `getComponentID()` | — | ID 반환 |

---

### `juce::Button`

**설명**: 모든 버튼 컴포넌트의 기반 추상 클래스.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setButtonText(const String&)` | `newText` | 버튼 텍스트 설정 |
| `String` | `getButtonText()` | — | 버튼 텍스트 반환 |
| `void` | `setToggleState(bool, NotificationType)` | `shouldBeOn, notification` | 토글 상태 설정 |
| `bool` | `getToggleState()` | — | 토글 상태 반환 |
| `void` | `setClickingTogglesState(bool)` | `shouldToggle` | 클릭 시 토글 여부 설정 |
| `void` | `setRadioGroupId(int, NotificationType)` | `newGroupId, notification` | 라디오 그룹 ID 설정 |
| `void` | `triggerClick()` | — | 프로그래밍적 클릭 유발 |
| `void` | `addListener(Listener*)` | `newListener` | 클릭 이벤트 리스너 등록 |
| `void` | `removeListener(Listener*)` | `listener` | 리스너 제거 |
| `virtual void` | `clicked(const ModifierKeys&)` | `modifiers` | 클릭 콜백 (오버라이드 가능) |
| `virtual void` | `paintButton(Graphics&, bool, bool)` | `g, isMouseOver, isButtonDown` | **[오버라이드 필수 for 커스텀]** 버튼 그리기 |

#### 주요 구체 서브클래스

| 클래스 | 설명 |
|---|---|
| `TextButton` | 텍스트 레이블 버튼 |
| `ToggleButton` | 체크박스 스타일 토글 버튼 |
| `ImageButton` | 이미지 기반 버튼 |
| `DrawableButton` | Drawable 객체 기반 버튼 |
| `ArrowButton` | 화살표 버튼 |

---

### `juce::Slider`

**설명**: 슬라이더/노브 UI 컴포넌트.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setRange(double, double, double)` | `newMinimum, newMaximum, newInterval` | 범위 및 스텝 설정 |
| `void` | `setValue(double, NotificationType)` | `newValue, notification` | 값 설정 |
| `double` | `getValue()` | — | 현재 값 반환 |
| `double` | `getMinimum()` | — | 최솟값 반환 |
| `double` | `getMaximum()` | — | 최댓값 반환 |
| `void` | `setSliderStyle(SliderStyle)` | `newStyle` | 슬라이더 스타일 설정 |
| `SliderStyle` | `getSliderStyle()` | — | 현재 스타일 반환 |
| `void` | `setTextBoxStyle(TextEntryBoxPosition, bool, int, int)` | `where, isReadOnly, textEntryBoxWidth, textEntryBoxHeight` | 텍스트박스 스타일 설정 |
| `void` | `setSkewFactor(double, bool)` | `factor, symmetricSkew` | 스큐(로그 스케일 등) 설정 |
| `void` | `setSkewFactorFromMidPoint(double)` | `sliderValueToShowAtMidPoint` | 중간점 기준 스큐 설정 |
| `void` | `addListener(Listener*)` | `listener` | 값 변경 리스너 등록 |
| `void` | `removeListener(Listener*)` | `listener` | 리스너 제거 |

---

### `juce::Label`

**설명**: 텍스트 레이블 컴포넌트.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setText(const String&, NotificationType)` | `newText, notification` | 텍스트 설정 |
| `String` | `getText(bool)` | `returnActiveEditorContents` | 텍스트 반환 |
| `void` | `setFont(const Font&)` | `newFont` | 폰트 설정 |
| `Font` | `getFont()` | — | 현재 폰트 반환 |
| `void` | `setJustificationType(Justification)` | `newJustification` | 정렬 설정 |
| `void` | `setEditable(bool, bool, bool)` | `editOnSingleClick, editOnDoubleClick, lossOfFocusDiscards` | 편집 가능 여부 설정 |
| `void` | `setColour(int, Colour)` | `colourId, colour` | 색상 설정 |
| `void` | `addListener(Listener*)` | `listener` | 텍스트 변경 리스너 등록 |
| `Component*` | `getCurrentTextEditor()` | — | 현재 텍스트 에디터 반환 |

---

### `juce::ComboBox`

**설명**: 드롭다운 콤보박스 컴포넌트.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `addItem(const String&, int)` | `newItemText, newItemId` | 항목 추가 |
| `void` | `addSeparator()` | — | 구분선 추가 |
| `void` | `addSectionHeading(const String&)` | `headingName` | 섹션 헤더 추가 |
| `void` | `clear(NotificationType)` | `notification` | 모든 항목 제거 |
| `void` | `setSelectedId(int, NotificationType)` | `newItemId, notification` | 선택 ID 설정 |
| `int` | `getSelectedId()` | — | 선택된 항목 ID 반환 |
| `void` | `setSelectedItemIndex(int, NotificationType)` | `newItemIndex, notification` | 인덱스로 선택 설정 |
| `int` | `getSelectedItemIndex()` | — | 선택된 인덱스 반환 |
| `String` | `getText()` | — | 현재 선택 텍스트 반환 |
| `int` | `getNumItems()` | — | 항목 수 반환 |
| `String` | `getItemText(int)` | `index` | 인덱스로 항목 텍스트 반환 |
| `void` | `addListener(Listener*)` | `listener` | 변경 리스너 등록 |

---

### `juce::LookAndFeel`

**설명**: 전체 UI 스타일/테마 커스터마이징 기반 클래스.

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `static void` | `LookAndFeel::setDefaultLookAndFeel(LookAndFeel*)` | 전역 기본 LookAndFeel 설정 |
| `static LookAndFeel&` | `LookAndFeel::getDefaultLookAndFeel()` | 기본 LookAndFeel 반환 |
| `void` | `setColour(int, Colour)` | 색상 등록 |
| `Colour` | `findColour(int)` | 등록된 색상 반환 |
| `virtual void` | `drawButtonBackground(Graphics&, Button&, const Colour&, bool, bool)` | 버튼 배경 그리기 (오버라이드) |
| `virtual void` | `drawButtonText(Graphics&, TextButton&, bool, bool)` | 버튼 텍스트 그리기 (오버라이드) |
| `virtual void` | `drawLinearSlider(...)` | 리니어 슬라이더 그리기 (오버라이드) |
| `virtual void` | `drawRotarySlider(...)` | 로타리 슬라이더 그리기 (오버라이드) |
| `virtual Font` | `getLabelFont(Label&)` | 레이블 폰트 반환 (오버라이드) |

---

## 9. juce_dsp — DSP 처리

### `juce::dsp::ProcessSpec`

**설명**: DSP 프로세서에 전달하는 처리 사양 구조체.

```cpp
struct ProcessSpec {
    double sampleRate;          // 샘플 레이트
    uint32 maximumBlockSize;    // 최대 블록 크기
    uint32 numChannels;         // 채널 수
};
```

---

### `juce::dsp::ProcessContextReplacing<T>`

**설명**: 입출력이 같은 버퍼를 사용하는 처리 컨텍스트.

| 멤버 | 타입 | 설명 |
|---|---|---|
| `getInputBlock()` | `const AudioBlock<const T>&` | 입력 블록 반환 |
| `getOutputBlock()` | `AudioBlock<T>&` | 출력 블록 반환 |
| `isBypassed` | `bool` | 바이패스 여부 |

---

### `juce::dsp::AudioBlock<T>`

**설명**: 오디오 데이터의 경량 래퍼 뷰 (복사 없음).

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `size_t` | `getNumChannels()` | — | 채널 수 |
| `size_t` | `getNumSamples()` | — | 샘플 수 |
| `T*` | `getChannelPointer(size_t)` | `channel` | 채널 포인터 반환 |
| `AudioBlock<T>` | `getSingleChannelBlock(size_t)` | `channel` | 단일 채널 블록 반환 |
| `AudioBlock<T>` | `getSubBlock(size_t, size_t)` | `startSample, numSamples` | 서브블록 반환 |
| `void` | `clear()` | — | 0으로 초기화 |
| `void` | `copyFrom(const AudioBuffer<T>&)` | `buffer` | AudioBuffer에서 복사 |
| `void` | `copyTo(AudioBuffer<T>&)` | `buffer` | AudioBuffer로 복사 |
| `AudioBlock&` | `add(T)` | `scalar` | 스칼라 덧셈 |
| `AudioBlock&` | `multiplyBy(T)` | `scalar` | 스칼라 곱셈 |

---

### `juce::dsp::IIR::Filter<T>` / `juce::dsp::IIR::Coefficients<T>`

**설명**: IIR 필터 및 계수 클래스. 저주파/고주파/밴드패스 등 구현.

**Coefficients 팩토리 메서드** (모두 정적):

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `Ptr` | `Coefficients::makeLowPass(double, T, T)` | `sampleRate, frequency, Q` | 로우패스 필터 계수 |
| `Ptr` | `Coefficients::makeHighPass(double, T, T)` | `sampleRate, frequency, Q` | 하이패스 필터 계수 |
| `Ptr` | `Coefficients::makeBandPass(double, T, T)` | `sampleRate, frequency, Q` | 밴드패스 필터 계수 |
| `Ptr` | `Coefficients::makeNotch(double, T, T)` | `sampleRate, frequency, Q` | 노치 필터 계수 |
| `Ptr` | `Coefficients::makeAllPass(double, T, T)` | `sampleRate, frequency, Q` | 올패스 필터 계수 |
| `Ptr` | `Coefficients::makeLowShelf(double, T, T, T)` | `sampleRate, cutOffFrequency, Q, gainFactor` | 로우쉘프 EQ 계수 |
| `Ptr` | `Coefficients::makeHighShelf(double, T, T, T)` | `sampleRate, cutOffFrequency, Q, gainFactor` | 하이쉘프 EQ 계수 |
| `Ptr` | `Coefficients::makePeakFilter(double, T, T, T)` | `sampleRate, frequency, Q, gainFactor` | 피크 EQ 계수 |

**Filter 메서드**:

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `prepare(const ProcessSpec&)` | `spec` | 필터 초기화 |
| `void` | `reset()` | — | 상태 초기화 |
| `void` | `process(const ProcessContext&)` | `context` | 블록 처리 |
| `T` | `processSample(T)` | `sample` | 단일 샘플 처리 |

---

### `juce::dsp::Gain<T>`

**설명**: DSP 게인 프로세서. SmoothedValue 기반 부드러운 게인 변경.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `prepare(const ProcessSpec&)` | `spec` | 초기화 |
| `void` | `reset()` | — | 상태 초기화 |
| `void` | `process(const ProcessContext&)` | `context` | 블록 처리 |
| `void` | `setGainLinear(T)` | `newGain` | 리니어 게인 설정 |
| `void` | `setGainDecibels(T)` | `newGainDecibels` | dB 게인 설정 |
| `T` | `getGainLinear()` | — | 현재 리니어 게인 반환 |
| `T` | `getGainDecibels()` | — | 현재 dB 게인 반환 |
| `SmoothedValue<T>&` | `getGainLinearSmoothed()` | — | SmoothedValue 참조 반환 |

---

### `juce::dsp::Reverb`

**설명**: Schroeder/Moorer 리버브 알고리즘 구현.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `prepare(const ProcessSpec&)` | `spec` | 초기화 |
| `void` | `reset()` | — | 상태 초기화 |
| `void` | `process(const ProcessContextReplacing<T>&)` | `context` | 블록 처리 |
| `void` | `setParameters(const Reverb::Parameters&)` | `params` | 파라미터 설정 |

```cpp
struct Reverb::Parameters {
    float roomSize = 0.5f;    // 룸 크기 (0~1)
    float damping  = 0.5f;    // 감쇠 (0~1)
    float wetLevel = 0.33f;   // 웻 레벨 (0~1)
    float dryLevel = 0.4f;    // 드라이 레벨 (0~1)
    float width    = 1.0f;    // 스테레오 폭 (0~1)
    float freezeMode = 0.0f;  // 프리즈 모드 (0~1)
};
```

---

## 10. juce_data_structures — 데이터 구조

### `juce::UndoManager`

**설명**: 실행 취소/재실행 스택 관리.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `perform(UndoableAction*)` | `action` | 액션 실행 및 스택에 추가 |
| `bool` | `undo()` | — | 실행 취소 |
| `bool` | `redo()` | — | 재실행 |
| `bool` | `canUndo()` | — | 취소 가능 여부 |
| `bool` | `canRedo()` | — | 재실행 가능 여부 |
| `String` | `getUndoDescription()` | — | 취소 액션 설명 반환 |
| `String` | `getRedoDescription()` | — | 재실행 액션 설명 반환 |
| `void` | `beginNewTransaction()` | — | 새 트랜잭션 시작 |
| `void` | `beginNewTransaction(const String&)` | `actionName` | 이름 있는 트랜잭션 시작 |
| `void` | `clearUndoHistory()` | — | 전체 히스토리 삭제 |
| `int` | `getNumActionsInCurrentTransaction()` | — | 현재 트랜잭션 액션 수 |

---

### `juce::ApplicationProperties`

**설명**: 애플리케이션 설정(설정 파일, 사용자 설정) 관리.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setStorageParameters(const PropertiesFile::Options&)` | `options` | 스토리지 옵션 설정 |
| `PropertiesFile*` | `getUserSettings()` | — | 사용자 설정 파일 반환 |
| `PropertiesFile*` | `getCommonSettings(bool)` | `returnUserPropsIfOptionNotSet` | 공통 설정 파일 반환 |
| `void` | `saveIfNeeded()` | — | 필요시 저장 |
| `void` | `closeFiles()` | — | 설정 파일 닫기 |

---

## 11. juce_animation — 애니메이션

### `juce::Animator`

**설명**: 단일 애니메이션 핸들. `AnimatorSetBuilder`를 통해 생성.

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `bool` | `isAnimating()` | 애니메이션 진행 중 여부 |
| `void` | `cancel(bool)` | 애니메이션 취소 (즉시 완료 여부) |
| `void` | `complete()` | 애니메이션 즉시 완료 |

---

### `juce::VBlankAnimatorUpdater`

**설명**: VBlank 콜백에 동기화된 애니메이터 업데이터.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `addAnimator(Animator&)` | `animator` | 애니메이터 등록 |
| `void` | `removeAnimator(Animator&)` | `animator` | 애니메이터 제거 |

---

### `juce::Easings`

**설명**: 다양한 이징 함수 제공.

| 정적 메서드 | 설명 |
|---|---|
| `Easings::linear()` | 리니어 이징 |
| `Easings::easeIn()` | 이즈인 |
| `Easings::easeOut()` | 이즈아웃 |
| `Easings::easeInOut()` | 이즈인아웃 |
| `Easings::bounceOut()` | 바운스 아웃 |
| `Easings::elastic()` | 스프링 이징 |
| `Easings::spring(SpringEasingOptions)` | 스프링 옵션 기반 이징 |

---

## 12. juce_cryptography — 암호화

### `juce::RSAKey`

**설명**: RSA 공개키/비밀키 암호화.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `RSAKey::createKeyPair(RSAKey&, RSAKey&, int, const int*, int)` | `publicKey, privateKey, numBits, randomSeeds, numRandomSeeds` | 키 쌍 생성 (정적) |
| `bool` | `applyToValue(BigInteger&)` | `value` | 값에 키 적용 |
| `String` | `toString()` | — | 키를 문자열로 직렬화 |
| `bool` | `isValid()` | — | 유효 여부 |

---

### `juce::MD5`

**설명**: MD5 해시 계산.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| | `MD5(const MemoryBlock&)` | `data` | 메모리 블록으로부터 MD5 계산 |
| | `MD5(const File&)` | `file` | 파일로부터 MD5 계산 |
| | `MD5(InputStream&, int64)` | `stream, numBytesToRead` | 스트림으로부터 MD5 계산 |
| | `MD5(const String&)` | `text` | 문자열로부터 MD5 계산 |
| `String` | `toHexString()` | — | MD5를 16진수 문자열로 반환 |
| `bool` | `operator==(const MD5&)` | `other` | 동일 여부 비교 |

---

### `juce::SHA256`

**설명**: SHA-256 해시 계산.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| | `SHA256(const MemoryBlock&)` | `data` | 메모리 블록으로부터 계산 |
| | `SHA256(const File&)` | `file` | 파일로부터 계산 |
| `String` | `toHexString()` | — | SHA256을 16진수 문자열로 반환 |
| `MemoryBlock` | `getRawData()` | — | 원시 바이트 반환 |

---

## 13. juce_gui_extra — 고급 GUI 컴포넌트

### ⭐ `juce::TextEditor`

**헤더**: `juce_gui_extra/misc/juce_TextEditor.h`  
**설명**: 멀티라인 텍스트 편집기 컴포넌트. 신택스 강조, 언두/리두 지원.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setText(const String&, bool)` | `newText, sendChangeMessage` | 전체 텍스트 설정 |
| `String` | `getText()` | — | 전체 텍스트 반환 |
| `void` | `setReadOnly(bool)` | `shouldBeReadOnly` | 읽기 전용 여부 설정 |
| `bool` | `isReadOnly()` | — | 읽기 전용 여부 반환 |
| `void` | `setMultiLine(bool, bool)` | `shouldBeMultiLine, shouldWordWrap` | 멀티라인/워드랩 설정 |
| `void` | `selectAll()` | — | 전체 선택 |
| `void` | `selectRegion(int, int)` | `startIndex, numCharsToSelect` | 범위 선택 |
| `String` | `getSelectedText()` | — | 선택 텍스트 반환 |
| `int` | `getCaretPosition()` | — | 캐럿 위치 반환 |
| `void` | `setCaretPosition(int)` | `newPosition` | 캐럿 위치 설정 |
| `int` | `getTotalNumChars()` | — | 전체 문자 수 |
| `void` | `clear()` | — | 텍스트 초기화 |
| `void` | `insertTextAtCaret(const String&)` | `textToInsert` | 캐럿 위치에 삽입 |
| `void` | `scrollDown()` | — | 아래로 스크롤 |
| `void` | `scrollToLine(int)` | `lineNumber` | 특정 줄로 스크롤 |
| `int` | `getNumLines()` | — | 총 줄 수 |
| `int` | `getLineLength(int)` | `lineNumber` | 줄 길이 반환 |
| `String` | `getLineText(int)` | `lineNumber` | 특정 줄 텍스트 반환 |
| `void` | `copyToClipboard()` | — | 클립보드 복사 |
| `void` | `pasteFromClipboard()` | — | 클립보드 붙여넣기 |
| `void` | `cut()` | — | 잘라내기 |
| `void` | `copy()` | — | 복사 |
| `void` | `paste()` | — | 붙여넣기 |
| `void` | `addListener(Listener*)` | `newListener` | 변경 리스너 등록 |
| `void` | `removeListener(Listener*)` | `listener` | 리스너 제거 |
| `void` | `setFont(const Font&)` | `newFont` | 폰트 설정 |
| `void` | `setColour(int, Colour)` | `colourId, newColour` | 색상 설정 |
| `void` | `setScrollbarsShown(bool)` | `show` | 스크롤바 표시 여부 |
| `bool` | `hasKeyboardFocus(bool)` | `trueIfChildIsFocused` | 키보드 포커스 여부 |
| `void` | `grabKeyboardFocus()` | — | 키보드 포커스 획득 |

---

### `juce::Toolbar`

**설명**: 도구 모음 컴포넌트. 아이콘/버튼 배치 및 커스터마이징.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `addDefaultItems(ToolbarItemFactory&)` | `factory` | 기본 아이템 추가 |
| `void` | `clear()` | — | 모든 아이템 제거 |
| `int` | `getNumItems()` | — | 아이템 수 반환 |
| `void` | `setVertical(bool)` | `shouldBeVertical` | 수직/수평 방향 설정 |
| `bool` | `isVertical()` | — | 수직 여부 |
| `void` | `setStyle(ToolbarItemComponent::ToolbarItemStyle)` | `newStyle` | 스타일 설정 |

---

### `juce::TreeView`

**설명**: 트리 구조 표시 컴포넌트. 폴더/계층 구조 표현.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setRootItem(TreeViewItem*)` | `newRootItem` | 루트 아이템 설정 |
| `TreeViewItem*` | `getRootItem()` | — | 루트 아이템 반환 |
| `void` | `deleteRootItem()` | — | 루트 아이템 삭제 |
| `void` | `setOpenness(TreeViewItem*, bool, bool)` | `item, isOpen, updateTree` | 아이템 열기/닫기 |
| `void` | `setDefaultOpenness(bool)` | `isOpen` | 기본 열림 상태 설정 |
| `int` | `getNumSelectedItems()` | — | 선택된 아이템 수 |
| `void` | `clearSelection()` | — | 선택 제거 |
| `void` | `selectItem(TreeViewItem*, bool)` | `item, shouldBeSelected` | 아이템 선택 |
| `void` | `scrollToKeepItemVisible(TreeViewItem*)` | `item` | 아이템이 보이도록 스크롤 |
| `void` | `addListener(Listener*)` | `newListener` | 리스너 등록 |
| `void` | `removeListener(Listener*)` | `listener` | 리스너 제거 |

---

### `juce::TabbedComponent`

**설명**: 탭 기반 인터페이스. 여러 패널을 탭으로 전환.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `addTab(const String&, Colour, Component*, bool)` | `tabName, tabBackgroundColour, contentComponent, deleteComponentWhenNotNeeded` | 탭 추가 |
| `void` | `setCurrentTabIndex(int, bool)` | `newTabIndex, sendChangeMessage` | 현재 탭 변경 |
| `int` | `getCurrentTabIndex()` | — | 현재 탭 인덱스 반환 |
| `Component*` | `getCurrentContentComponent()` | — | 현재 탭 컨텐트 반환 |
| `void` | `removeTab(int)` | `tabIndex` | 탭 제거 |
| `void` | `clearTabs()` | — | 모든 탭 제거 |
| `int` | `getNumTabs()` | — | 탭 수 반환 |
| `void` | `addChangeListener(ChangeListener*)` | `listener` | 탭 변경 리스너 등록 |

---

### `juce::ListBox`

**설명**: 스크롤 가능한 리스트 컴포넌트. 행 기반 렌더링.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setModel(ListBoxModel*)` | `newModel` | 모델 설정 |
| `ListBoxModel*` | `getModel()` | — | 모델 반환 |
| `void` | `repaintRow(int)` | `rowNumber` | 특정 행 다시 그리기 |
| `void` | `selectRow(int, bool, bool)` | `rowNumber, dontScrollToShowThisRow, deselectOthersFirst` | 행 선택 |
| `void` | `selectRangeOfRows(int, int, bool)` | `startIndex, endIndex, shouldBSelected` | 범위 행 선택 |
| `void` | `deselectRow(int)` | `rowNumber` | 행 선택 해제 |
| `void` | `selectColumnOfRows(int, bool)` | `rowsToSelect, shouldBeSelected` | 행 그룹 선택 |
| `int` | `getSelectedRow(int)` | `index` | N번째 선택된 행 반환 |
| `int` | `getNumSelectedRows()` | — | 선택된 행 수 |
| `void` | `clearSelection()` | — | 모든 선택 해제 |
| `int` | `getTopRowWithFullHeight()` | — | 맨 위에 보이는 행 반환 |
| `void` | `scrollToEnsureRowIsVisible(int)` | `rowNumber` | 행이 보이도록 스크롤 |
| `void` | `updateContent()` | — | 모델 데이터 변경 후 업데이트 |

---

### ⭐ `juce::FileChooser`

**설명**: 파일/디렉토리 선택 대화창.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `browseForFileToOpen(FileChooser::Callback*)` | `callback` | 파일 열기 대화 (비동기) |
| `void` | `browseForFileToSave(bool, FileChooser::Callback*)` | `warnAboutOverwriting, callback` | 파일 저장 대화 |
| `void` | `browseForDirectory(FileChooser::Callback*)` | `callback` | 디렉토리 선택 대화 |
| `void` | `browseForMultipleFilesToOpen(FileChooser::Callback*)` | `callback` | 다중 파일 선택 대화 |
| `File` | `getResult()` | — | 선택된 파일 반환 (동기 호출 후) |
| `Array<File>` | `getResults()` | — | 선택된 파일 목록 반환 |

---

### `juce::AsyncUpdater`

**설명**: 메시지 스레드에서 비동기 업데이트 큐. 멀티스레드 코드 안전성.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `triggerAsyncUpdate()` | — | 업데이트 요청 (메시지 스레드에서 handleAsyncUpdate 호출) |
| `virtual void` | `handleAsyncUpdate()` | — | **[오버라이드 필수]** 메시지 스레드에서 호출되는 핸들러 |
| `void` | `cancelPendingUpdate()` | — | 대기 중인 업데이트 취소 |
| `bool` | `isUpdatePending()` | — | 업데이트 대기 중 여부 |

---

### `juce::AlertWindow`

**설명**: 알림/경고/질문 대화창.

| 반환 타입 | 정적 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `AlertWindow::showMessageBox(AlertIconType, const String&, const String&, const String&, Component*, ModalComponentManager::Callback*)` | `iconType, title, message, buttonText, associatedComponent, callback` | 메시지 박스 표시 |
| `void` | `AlertWindow::showOkCancelBox(AlertIconType, const String&, const String&, Component*, ModalComponentManager::Callback*)` | `iconType, title, message, component, callback` | OK/Cancel 대화 |
| `void` | `AlertWindow::showYesNoCancelBox(AlertIconType, const String&, const String&, Component*, ModalComponentManager::Callback*)` | `iconType, title, message, component, callback` | Yes/No/Cancel 대화 |
| `int` | `AlertWindow::showNativeDialogBox(const String&, const String&, bool)` | `title, bodyText, isQuestion` | 네이티브 대화 (0=Yes, 1=No) |

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `addButton(const String&, int, const KeyPress&, const KeyPress&)` | `buttonText, returnValue, shortcut1, shortcut2` | 버튼 추가 |
| `void` | `addTextEditor(const String&, const String&, const String&, bool)` | `name, initialValue, onScreenLabel, isPasswordField` | 텍스트 입력 필드 추가 |
| `void` | `addComboBox(const String&, const StringArray&, const String&)` | `name, items, onScreenLabel` | 콤보박스 추가 |
| `void` | `addCheckBox(const String&, bool)` | `name, shouldBeChecked` | 체크박스 추가 |
| `String` | `getTextEditorContents(const String&)` | `name` | 텍스트 입력값 반환 |
| `bool` | `getCheckBoxState(const String&)` | `name` | 체크박스 상태 반환 |
| `void` | `setMessage(const String&)` | `message` | 메시지 변경 |
| `void` | `showAsync(Component*, ModalComponentManager::Callback*, bool)` | `component, callback, isModal` | 비동기 표시 |

---

### `juce::Tooltip`

**설명**: 마우스 호버 시 표시되는 팁 텍스트.

| 반환 타입 | 정적 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `Tooltip::setMillisecondsBeforeTipAppears(int)` | `millisecondsOfMouseHover` | 팁 표시 지연 시간 설정 |
| `String` | `Tooltip::getTipFor(Component*)` | `component` | 컴포넌트 팁 문자열 반환 |

**Component 메서드**:

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setTooltip(const String&)` | `newTooltip` | 컴포넌트 팁 설정 |

---

## 14. juce_audio_plugin_client — 플러그인 개발

### `juce::AudioProcessorEditor`

**헤더**: `juce_audio_plugin_client/utility/juce_AudioProcessorEditor.h`  
**설명**: 플러그인 GUI 편집기의 기반 클래스. 모든 플러그인 에디터가 상속.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `AudioProcessor&` | `getAudioProcessor()` | — | 연결된 AudioProcessor 반환 |
| `void` | `audioProcessorChanged()` | — | 프로세서 변경 시 호출 (오버라이드 옵션) |
| `void` | `synchroniseUIWithParameterValues()` | — | 호스트에서 파라미터 변경 시 UI 업데이트 |
| `virtual void` | `timerCallback()` | — | Timer 상속 시 주기 업데이트 (옵션) |

---

### `juce::RangedAudioParameter` / `juce::AudioParameterFloat`

**설명**: 플러그인 파라미터 정의. AudioProcessorValueTreeState와 자동 연동.

**AudioParameterFloat** 생성:

```cpp
// 정규화된 범위 [0, 1]로 자동 매핑
new AudioParameterFloat("id", "name", NormalisableRange<float>(0.0f, 100.0f), 50.0f, 
                        "Unit", 
                        AudioProcessorParameter::genericParameter, 
                        valueToTextFunc, 
                        textToValueFunc)
```

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `float` | `get()` | — | 비정규화 값 반환 |
| `void` | `setValueNotifyingHost(float)` | `newValue` | 호스트 통보 후 값 변경 |
| `String` | `getCurrentValueAsText()` | — | 텍스트 표현 반환 |
| `String` | `getLabel()` | — | 파라미터 단위/레이블 반환 |
| `int` | `getNumSteps()` | — | 스텝 수 반환 |
| `bool` | `isBoolean()` | — | 불리언 파라미터 여부 |
| `float` | `getDefaultValue()` | — | 기본값 반환 |
| `float` | `getValueForText(const String&)` | `text` | 텍스트 → 파라미터 값 변환 |

---

### `juce::AudioParameterChoice`

**설명**: 제한된 선택지를 가진 파라미터 (정수 인덱스).

```cpp
StringArray choices { "Off", "On", "Auto" };
new AudioParameterChoice("id", "name", choices, 0)  // 기본값 인덱스 0
```

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `int` | `getIndex()` | — | 현재 선택 인덱스 반환 |
| `String` | `getCurrentChoiceName()` | — | 현재 선택 텍스트 반환 |
| `int` | `getNumChoices()` | — | 선택지 개수 |

---

## 15. juce_audio_utils — 오디오 유틸리티

### ⭐ `juce::AudioTransportSource`

**헤더**: `juce_audio_devices/sources/juce_AudioTransportSource.h`  
**설명**: 재생/일시정지/정지 제어 기능이 있는 오디오 소스 래퍼.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setSource(AudioFormatReader*, int, TimeSliceThread*, double, int)` | `newAudioReader, readAheadBufferSize, readAheadThread, sourceSampleRateToCorrectTo, maxNumChannels` | 오디오 소스 설정 |
| `void` | `play()` | — | 재생 시작 |
| `void` | `stop()` | — | 재생 정지 |
| `void` | `pause()` | — | 일시정지 |
| `bool` | `isPlaying()` | — | 재생 중 여부 |
| `void` | `setPosition(double)` | `newPosition` | 재생 위치 설정 (초) |
| `double` | `getCurrentPosition()` | — | 현재 재생 위치 반환 (초) |
| `double` | `getLengthInSeconds()` | — | 전체 길이 반환 (초) |
| `void` | `setGain(float)` | `newGain` | 재생 게인 설정 |

---

### `juce::MidiKeyboardComponent`

**설명**: 시각적 MIDI 키보드 UI. 마우스/터치로 노트 입력.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setMidiChannel(int)` | `channel` | MIDI 채널 설정 (1~16) |
| `int` | `getMidiChannel()` | — | 현재 MIDI 채널 반환 |
| `void` | `setMidiChannelsToDisplay(int)` | `mask` | 표시할 채널 마스크 설정 |
| `void` | `setKeyWidth(float)` | `width` | 키 너비 설정 (픽셀) |
| `float` | `getKeyWidth()` | — | 키 너비 반환 |
| `void` | `setOrientation(Orientation)` | `orientation` | 방향 설정 (가로/세로) |
| `void` | `setOctaveForMiddleC(int)` | `octave` | 중간 C 옥타브 설정 |
| `void` | `dragKeyboardMouseMove(const MouseEvent&)` | `event` | 마우스 드래그 (외부 호출용) |
| `void` | `dragKeyboardMouseUp(const MouseEvent&)` | `event` | 마우스 버튼 뗌 |

---

## 16. juce_opengl — OpenGL 렌더링

### `juce::OpenGLContext`

**설명**: OpenGL 렌더링 컨텍스트. Component와 통합.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `attachTo(Component&)` | `component` | 컴포넌트에 OpenGL 컨텍스트 연결 |
| `void` | `detach()` | — | OpenGL 컨텍스트 분리 |
| `bool` | `isActive()` | — | 활성 여부 |
| `void` | `setRenderer(OpenGLRenderer*)` | `renderer` | 렌더러 설정 |
| `void` | `swapBuffers()` | — | 버퍼 스왑 |
| `double` | `getFramesPerSecond()` | — | 현재 FPS 반환 |
| `void` | `setContinuousRepainting(bool)` | `shouldRepaint` | 연속 리페인트 설정 |

---

### `juce::OpenGLRenderer`

**설명**: OpenGL 렌더링 콜백 인터페이스.

| 반환 타입 | 순수 가상 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `newOpenGLContextCreated()` | — | **[오버라이드]** OpenGL 컨텍스트 생성 시 호출 |
| `void` | `renderOpenGL()` | — | **[오버라이드]** 매 프레임마다 OpenGL 그리기 |
| `void` | `openGLContextClosing()` | — | **[오버라이드]** OpenGL 컨텍스트 소멸 전 호출 |

---

## 17. juce_osc — Open Sound Control

### `juce::osc::OSCMessage`

**설명**: OSC 메시지 포맷 정의 및 전송.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `OSCMessage(const String&)` | (생성자) | `addressPattern` | 주소 패턴으로 메시지 생성 |
| `void` | `addInt32(int32)` | `value` | 32비트 정수 인자 추가 |
| `void` | `addFloat32(float)` | `value` | 32비트 부동소수점 인자 추가 |
| `void` | `addString(const String&)` | `value` | 문자열 인자 추가 |
| `void` | `addBlob(const MemoryBlock&)` | `data` | 바이너리 데이터(blob) 추가 |
| `void` | `addTimeTag(uint64)` | `timeTag` | 시간 태그 추가 |
| `String` | `getAddressPattern()` | — | 주소 패턴 반환 |
| `int` | `size()` | — | 인자 개수 |
| `OSCType` | `getType(int)` | `index` | N번째 인자 타입 |
| `int32` | `getInt32(int)` | `index` | N번째 인자 정수값 |
| `float` | `getFloat32(int)` | `index` | N번째 인자 부동소수점값 |
| `String` | `getString(int)` | `index` | N번째 인자 문자열 |
| `MemoryBlock` | `getBlob(int)` | `index` | N번째 인자 blob 데이터 |

---

### `juce::osc::OSCReceiver`

**설명**: OSC 메시지 수신.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `connect(int)` | `portNumber` | 포트 바인드 후 수신 시작 |
| `void` | `disconnect()` | — | 수신 정지 |
| `bool` | `isConnected()` | — | 연결 상태 여부 |
| `void` | `addListener(Listener*, const String&)` | `listener, oscAddressPattern` | 메시지 리스너 등록 |
| `void` | `removeListener(Listener*)` | `listener` | 리스너 제거 |

---

### `juce::osc::OSCSender`

**설명**: OSC 메시지 전송.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `connect(const String&, int)` | `hostName, portNumber` | 원격 호스트 연결 |
| `void` | `disconnect()` | — | 연결 해제 |
| `bool` | `isConnected()` | — | 연결 상태 여부 |
| `bool` | `send(const OSCMessage&)` | `message` | 메시지 전송 |
| `bool` | `send(const String&, int32)` | `addressPattern, value` | 주소 패턴 + 정수값 전송 |
| `bool` | `send(const String&, float)` | `addressPattern, value` | 주소 패턴 + 부동소수점값 전송 |
| `bool` | `send(const String&, const String&)` | `addressPattern, value` | 주소 패턴 + 문자열 전송 |

---

## 18. juce_video — 비디오 재생/캡처

### `juce::VideoComponent`

**설명**: 비디오 파일 재생 컴포넌트 (플랫폼 기반).

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `load(const File&)` | `fileToLoad` | 비디오 파일 로드 |
| `bool` | `load(const String&)` | `urlToLoad` | URL에서 로드 |
| `void` | `closeVideo()` | — | 비디오 닫기 |
| `void` | `play()` | — | 재생 시작 |
| `void` | `stop()` | — | 재생 정지 |
| `bool` | `isPlaying()` | — | 재생 중 여부 |
| `void` | `setPlaySpeed(double)` | `newSpeed` | 재생 속도 설정 (1.0 = 정상) |
| `void` | `setPosition(double)` | `seconds` | 재생 위치 설정 |
| `double` | `getPosition()` | — | 현재 재생 위치 반환 |
| `double` | `getDuration()` | — | 전체 길이 반환 |
| `void` | `setLooping(bool)` | `shouldLoop` | 반복 재생 설정 |
| `int` | `getVideoWidth()` | — | 비디오 너비 |
| `int` | `getVideoHeight()` | — | 비디오 높이 |

---

### `juce::CameraDevice`

**설명**: 웹캠/카메라 장치 접근 및 캡처.

| 반환 타입 | 정적 메서드 | 인자 | 설명 |
|---|---|---|---|
| `StringArray` | `CameraDevice::getAvailableDevices()` | — | 사용 가능한 카메라 목록 반환 |
| `CameraDevice*` | `CameraDevice::openDevice(int, int, int, int, CameraDevice::Listener*)` | `deviceIndex, width, height, frameRateInHz, listener` | 카메라 장치 열기 |

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `startRecording()` | — | 캡처 시작 |
| `void` | `stopRecording()` | — | 캡처 정지 |
| `String` | `getDeviceName()` | — | 장치명 반환 |

---

## 19. juce_midi_ci — MIDI Capability Inquiry

### `juce::midi_ci::DeviceInfo`

**설명**: MIDI-CI 장치 정보 정의.

| 메서드 | 반환 타입 | 설명 |
|---|---|---|
| `getDeviceManufacturer()` | `uint24` | 제조사 ID |
| `getDeviceFamily()` | `uint16` | 장치 제품군 |
| `getDeviceModelNumber()` | `uint16` | 모델 번호 |
| `getSoftwareRevisionLevel()` | `std::array<uint8, 4>` | 펌웨어 버전 |
| `getUMPVersionMajor()` | `uint8` | UMP 버전 메이저 |
| `getUMPVersionMinor()` | `uint8` | UMP 버전 마이너 |

---

## 20. juce_javascript — JavaScript 엔진

### `juce::JavascriptEngine`

**설명**: JavaScript 코드 실행 및 호스트 코드 통합.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `var` | `execute(const String&)` | `javascriptCode` | JavaScript 코드 실행 및 반환값 |
| `void` | `registerNativeObject(const String&, DynamicObject*)` | `objectName, object` | 네이티브 객체 등록 |
| `var` | `callFunction(const String&, const var*, int)` | `functionName, parameters, numParameters` | JavaScript 함수 호출 |
| `var` | `getVariableObject(const String&, bool)` | `variableName, createIfDoesntExist` | JavaScript 변수 접근 |
| `void` | `setVariableObject(const String&, DynamicObject*)` | `variableName, object` | JavaScript 변수 설정 |

---

## 21. juce_box2d — 2D 물리 엔진

### `juce::box2d::World`

**설명**: Box2D 물리 시뮬레이션 월드.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `body*` | `createBody(const BodyDef&)` | `bodyDef` | 물리 바디 생성 |
| `void` | `destroyBody(body*)` | `body` | 바디 제거 |
| `void` | `step(float32, int32, int32)` | `timeStep, velocityIterations, positionIterations` | 시뮬레이션 스텝 수행 |
| `void` | `setGravity(const Vec2&)` | `gravity` | 중력 설정 |
| `Vec2` | `getGravity()` | — | 현재 중력 반환 |
| `void` | `clearForces()` | — | 모든 바디의 힘 초기화 |
| `bool` | `isLocked()` | — | 월드 락 상태 여부 |

---

## 22. juce_product_unlocking — 라이선스/DRM

### `juce::OnlineUnlockStatus`

**설명**: 온라인 라이선싱/잠금 해제 상태 관리.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `addListener(Listener*)` | `listener` | 상태 변경 리스너 등록 |
| `void` | `removeListener(Listener*)` | `listener` | 리스너 제거 |
| `bool` | `isUnlocked()` | — | 잠금 해제 여부 |
| `String` | `getAccountEmail()` | — | 계정 이메일 반환 |
| `String` | `getLicenseKey()` | — | 라이선스 키 반환 |
| `void` | `setLicenseKey(const String&)` | `licenseKey` | 라이선스 키 설정 |
| `void` | `performPostRequest(const String&, const String&, Listener*)` | `url, dataToSend, listener` | 서버 요청 수행 (비동기) |

---

## 23. juce_analytics — 분석/텔레메트리

### `juce::Analytics`

**설명**: 사용 통계 수집 및 전송.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `setEventProperties(const StringPairArray&)` | `properties` | 이벤트 기본 속성 설정 |
| `void` | `logEvent(const String&, const StringPairArray&, bool)` | `eventName, eventProperties, forceSynchronous` | 이벤트 기록 |
| `void` | `addDestination(std::unique_ptr<AnalyticsDestination>)` | `newDestination` | 분석 대상 추가 (서버 URL 등) |
| `void` | `setUserProperties(const StringPairArray&)` | `properties` | 사용자 속성 설정 |
| `void` | `resetUserProperties()` | — | 사용자 속성 초기화 |

---

## 부록: 자주 사용되는 열거형 및 상수

### `juce::Justification`

| 상수 | 설명 |
|---|---|
| `Justification::left` | 왼쪽 정렬 |
| `Justification::right` | 오른쪽 정렬 |
| `Justification::centred` | 수평 중앙 정렬 |
| `Justification::centredTop` | 상단 중앙 정렬 |
| `Justification::centredBottom` | 하단 중앙 정렬 |
| `Justification::topLeft` | 좌상단 정렬 |
| `Justification::bottomRight` | 우하단 정렬 |

### `juce::Slider::SliderStyle`

| 상수 | 설명 |
|---|---|
| `LinearHorizontal` | 수평 리니어 슬라이더 |
| `LinearVertical` | 수직 리니어 슬라이더 |
| `Rotary` | 로타리 노브 |
| `RotaryHorizontalDrag` | 수평 드래그 로타리 |
| `RotaryVerticalDrag` | 수직 드래그 로타리 |
| `TwoValueHorizontal` | 듀얼값 수평 슬라이더 |
| `ThreeValueHorizontal` | 삼중값 수평 슬라이더 |

### `juce::AudioProcessor::WrapperType`

| 상수 | 설명 |
|---|---|
| `wrapperType_VST` | VST2 플러그인 |
| `wrapperType_VST3` | VST3 플러그인 |
| `wrapperType_AudioUnit` | AU 플러그인 |
| `wrapperType_AudioUnitv3` | AUv3 플러그인 |
| `wrapperType_AAX` | AAX 플러그인 |
| `wrapperType_LV2` | LV2 플러그인 |
| `wrapperType_Standalone` | 독립 실행 앱 |

---

## 부록: 추가 고급 클래스

### ⭐ `juce::FileInputStream` / `juce::FileOutputStream`

**설명**: 파일 읽기/쓰기 스트림.

**FileInputStream**:

```cpp
FileInputStream input(file);
if (input.openedOk()) {
    int value = input.readInt();
}
```

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `openedOk()` | — | 파일 열기 성공 여부 |
| `int64` | `getTotalLength()` | — | 파일 크기 반환 |
| `int` | `read(void*, int)` | `destBuffer, numBytesToRead` | 바이트 읽기 |
| `File` | `getFile()` | — | 파일 객체 반환 |

**FileOutputStream**:

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `bool` | `openedOk()` | — | 파일 열기 성공 여부 |
| `bool` | `write(const void*, int)` | `data, numBytesToWrite` | 바이트 쓰기 |
| `void` | `flush()` | — | 버퍼 플러시 |
| `void` | `truncate()` | — | 파일 크기 축소 |
| `File` | `getFile()` | — | 파일 객체 반환 |

---

### `juce::MemoryInputStream` / `juce::MemoryOutputStream`

**설명**: 메모리 기반 스트림.

```cpp
MemoryOutputStream output;
output.writeInt(42);
output.writeString("Hello");
MemoryBlock data = output.getMemoryBlock();
```

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `MemoryBlock` | `getMemoryBlock()` | 메모리 블록 반환 (OutputStream) |
| `size_t` | `getDataSize()` | 쓰여진 바이트 수 반환 |

---

### ⭐ `juce::Identifier`

**설명**: ValueTree의 속성명으로 사용하는 빠른 문자열 비교 클래스. 인턴된 문자열.

```cpp
const Identifier myProp("myProperty");  // 한 번만 메모리 할당
ValueTree state("Root");
state.setProperty(myProp, 42, nullptr);
int value = state.getProperty(myProp);  // 빠른 비교
```

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `String` | `toString()` | 문자열 변환 |
| `const char*` | `getCharPointer()` | C-문자열 포인터 반환 |
| `bool` | `operator==(const Identifier&)` | 동일 여부 (포인터 비교, 매우 빠름) |
| `bool` | `operator!=(const Identifier&)` | 다른지 여부 |
| `size_t` | `hash()` | 해시값 반환 |

---

### `juce::Listener`

**설명**: 다양한 이벤트 리스너의 기반 인터페이스.

```cpp
class MyListener : public Button::Listener {
    void buttonClicked(Button* button) override {
        // 버튼 클릭 처리
    }
};
```

**흔한 리스너 타입**:
- `Button::Listener` - 버튼 클릭
- `Slider::Listener` - 슬라이더 값 변경
- `Label::Listener` - 레이블 텍스트 변경
- `ComboBox::Listener` - 콤보박스 선택 변경
- `TextEditor::Listener` - 텍스트 에디터 변경
- `Timer::Listener` - (Timer는 직접 콜백)
- `ChangeBroadcaster::ChangeListener` - 범용 변경 알림

---

### `juce::dsp::ProcessContext` / `juce::dsp::ProcessContextReplacing`

**설명**: DSP 프로세서에 전달되는 오디오 데이터 및 컨텍스트.

| 멤버 | 타입 | 설명 |
|---|---|---|
| `getInputBlock()` | `const AudioBlock<const float>&` | 입력 블록 (읽기 전용) |
| `getOutputBlock()` | `AudioBlock<float>&` | 출력 블록 |
| `isBypassed` | `bool` | 바이패스 상태 |
| `numSamples` | `size_t` | 샘플 수 (AudioBlock::getNumSamples()로도 접근) |

---

### `juce::GenericScopedPointer<T>` / `juce::ScopedPointer<T>`

**설명**: RAII 기반 자동 메모리 정리 스마트 포인터.

```cpp
void processAudio() {
    ScopedPointer<MyObject> obj(new MyObject());
    // obj 사용
}  // obj 스코프 벗어나면 자동 delete
```

| 반환 타입 | 메서드 | 설명 |
|---|---|---|
| `T*` | `get()` | 포인터 반환 |
| `T&` | `operator*()` | 역참조 |
| `T*` | `operator->()` | 멤버 접근 |
| `T*` | `release()` | 소유권 포기 및 포인터 반환 (삭제 안함) |
| `void` | `reset(T*)` | 기존 객체 삭제 후 새 포인터 설정 |
| `bool` | `operator!()` | nullptr 여부 |

---

### `juce::Array<T>`

**설명**: 동적 배열. 템플릿 컨테이너.

```cpp
Array<String> items;
items.add("First");
items.add("Second");
items.add("Third");

for (const auto& item : items) {
    std::cout << item << std::endl;
}
```

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `add(T)` | `newElement` | 원소 추가 |
| `void` | `insert(int, T)` | `index, newElement` | 특정 위치 삽입 |
| `void` | `set(int, T)` | `index, newElement` | 인덱스 원소 변경 |
| `T&` | `getReference(int)` | `index` | 참조 반환 (경계 검사 없음) |
| `T*` | `getObjectPointer(int)` | `index` | 포인터 반환 |
| `int` | `size()` | — | 원소 수 |
| `bool` | `isEmpty()` | — | 비어있는지 |
| `void` | `clear()` | — | 전체 삭제 |
| `void` | `remove(int)` | `index` | 인덱스 원소 제거 |
| `void` | `removeObject(T)` | `object` | 특정 객체 제거 |
| `void` | `removeRange(int, int)` | `startIndex, numToRemove` | 범위 제거 |
| `int` | `indexOf(T)` | `element` | 인덱스 검색 (-1 = 없음) |
| `bool` | `contains(T)` | `element` | 포함 여부 |
| `void` | `sort(Comparator&, bool)` | `comparator, retainOrderOfEquivalentElements` | 정렬 |
| `T&` | `operator[](int)` | `index` | 배열 접근 (경계 검사 없음) |
| `T*` | `begin()`, `end()` | — | 반복자 (range-for 지원) |

---

### `juce::StringArray`

**설명**: String 전용 배열. Array<String>의 편의 버전.

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `void` | `add(const String&)` | `newString` | 문자열 추가 |
| `void` | `addLines(const String&)` | `multiLineText` | 줄 단위로 추가 |
| `void` | `addTokens(const String&, const String&, const String&)` | `text, breakCharacters, quoteCharacters` | 토큰화 추가 |
| `void` | `mergeArray(const StringArray&)` | `otherArray` | 다른 배열과 병합 |
| `void` | `removeString(const String&)` | `stringToRemove` | 특정 문자열 제거 |
| `String` | `joinIntoString(const String&, int, int)` | `separator, startIndex, numberOfElements` | 구분자로 결합 |
| `void` | `sort(bool)` | `ignoreCase` | 정렬 |
| `void` | `removeDuplicates(bool)` | `ignoreCase` | 중복 제거 |
| `int` | `indexOf(const String&, bool)` | `stringToLookFor, ignoreCase` | 인덱스 검색 |

---

### `juce::PropertiesFile`

**설명**: INI 스타일 설정 파일 읽기/쓰기.

```cpp
PropertiesFile::Options opts;
opts.applicationName = "MyApp";
opts.folderName = "MyCompany";
opts.filenameSuffix = "settings";
opts.osxLibrarySubFolder = "Application Support";

PropertiesFile props(opts);
props.setValue("lastOpenedFile", "/path/to/file");
props.saveIfNeeded();
```

| 반환 타입 | 메서드 | 인자 | 설명 |
|---|---|---|---|
| `String` | `getValue(const String&, const String&)` | `keyName, defaultReturnValue` | 값 읽기 |
| `int` | `getIntValue(const String&, int)` | `keyName, defaultReturnValue` | 정수값 읽기 |
| `bool` | `getBoolValue(const String&, bool)` | `keyName, defaultReturnValue` | 불리언값 읽기 |
| `double` | `getDoubleValue(const String&, double)` | `keyName, defaultReturnValue` | double 읽기 |
| `void` | `setValue(const String&, const String&)` | `keyName, value` | 값 쓰기 |
| `void` | `setIntValue(const String&, int)` | `keyName, value` | 정수값 쓰기 |
| `void` | `setBoolValue(const String&, bool)` | `keyName, value` | 불리언값 쓰기 |
| `void` | `removeValue(const String&)` | `keyName` | 키 제거 |
| `void` | `removeAllValues()` | — | 모든 설정 제거 |
| `bool` | `saveIfNeeded()` | — | 변경사항 저장 |
| `void` | `save()` | — | 강제 저장 |
| `StringArray` | `getAllKeys()` | — | 모든 키 반환 |

---

## 부록: 자주 사용되는 Color IDs

### Button Color IDs
```
buttonColourId = 0x1000100
buttonOnColourId = 0x1000101
```

### Label Color IDs
```
labelTextColourId = 0x1001200
outlineColourId = 0x1001201
```

### TextEditor Color IDs
```
backgroundColourId = 0x1002200
textColourId = 0x1002201
highlightColourId = 0x1002202
outlineColourId = 0x1002203
focusedOutlineColourId = 0x1002204
```

### Slider Color IDs
```
backgroundColourId = 0x1001300
thumbColourId = 0x1001301
trackColourId = 0x1001302
rotarySliderFillColourId = 0x1001303
rotarySliderOutlineColourId = 0x1001304
```

---

*이 문서는 https://docs.juce.com/master/index.html 공식 HTML 문서를 기반으로 직접 분석하여 작성되었습니다. 모든 API는 master 브랜치 기준이며 deprecated API는 제외되었습니다.*

*⭐ = 핵심 API | 총 API 수: 500+ | 커버 모듈: 25개*

*최종 업데이트: 2026년 4월 22일*
