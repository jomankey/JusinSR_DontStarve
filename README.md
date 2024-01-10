쥬신5개월차SR프로젝트
======================

## 1. Client 설정


### 1.1. General
1. 속성 -> 일반 : output directory ../Bin\
2. 추가디렉토리 : ../../Base; ../Header; ../../Reference/Header
3. 링커-> 일반 : 추가포함디렉토 ../../Reference/Library
4. 입력 :시스템라이브러리

### 1.2. VC++Directory 
* Include Directories
  - $(DXSDK_DIR)Include
* Library Directories
  - $(DXSDK_DIR)lib\x86

### 1.3. C/C++
* General
  - Additonal Include Directries
    + ../Header
    - ../../Base
    - ../../Reference/Header

### 1.4. Linker
* General
  - Additional Library Directories
    ../../Reference/Library
* Input
  - Additional Dependencies(추가종속성)
    + d3d9.lib
    + d3dx9.lib
    + dxguid.lib
    + System.lib
    + Utility.lib

함수
----
* ProtoMgr
  - 컴포넌트 (리소스매니저와같음)
  - 
