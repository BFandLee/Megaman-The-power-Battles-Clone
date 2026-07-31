---
name: diagram
description: 클래스 다이어그램 자동 업데이트 및 PNG 이미지 변환 스킬. 사용자가 /diagram (또는 /diagrame)을 입력하거나, 질문 답변/버그 수정 후 클래스 다이어그램 작성을 요청할 때 활성화됩니다.
---

# Class Diagram Generation Skill

이 스킬은 프로젝트의 최신 소스 코드(`Core`, `Entity`, `Manager`, `Renderer` 등)를 스캔하여 **클래스 다이어그램(Class Diagram)**을 Mermaid 및 고화질 **PNG 이미지**로 자동 작성/업데이트하는 가이드라인입니다.

## 🎯 사용 시점 (Triggers)
- 사용자가 `/diagram` 또는 `/diagrame` 슬래시 커맨드를 입력했을 때
- 버그 수정이나 주요 기능 구현이 끝난 후, 구조 변화를 시각화해야 할 때

## 📁 파일 저장 위치 (Output Paths)
- **클래스 다이어그램 문서 (Artifact)**: `architecture_diagram.md`
- **고화질 PNG 이미지 (Skill Directory)**: `.agents/skills/diagram/architecture_diagram.png`

## 📋 처리 절차 (Execution Steps)

1. **코드베이스 최신 구조 스캔**:
   - `MegaManWinAPI` 프로젝트 내부의 주요 헤더/소스 파일들을 검색합니다.
   - 클래스 상속 관계 (`Inheritance`), 소유 및 보유 관계 (`Composition / Aggregation`), 참조 관계 (`Reference`)를 분석합니다.

2. **Mermaid 다이어그램 및 소유권 지도 작성**:
   - 클래스 간 관계를 Mermaid `classDiagram` 표준 규격으로 작성합니다.
   - Raw Pointer 기반 소유권(Creation, Ownership, Weak Reference) 표를 정리합니다.
   - 아티팩트 문서(`architecture_diagram.md`)를 작성 및 갱신합니다.

3. **고화질 PNG 이미지 자동 생성 및 저장**:
   - 작성된 Mermaid 코드를 base64로 인코딩하여 `mermaid.ink` API로 고화질 이미지를 렌더링합니다.
   - 생성된 PNG 바이너리를 프로젝트 내부 스킬 디렉토리인 `.agents/skills/diagram/architecture_diagram.png` 경로에 직접 저장합니다.

4. **사용자에게 결과 가이드**:
   - 생성된 마크다운 아티팩트 문서 및 `.agents/skills/diagram/architecture_diagram.png` 파일 링크를 사용자에게 안내합니다.
