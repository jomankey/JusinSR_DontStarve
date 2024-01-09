쥬신5개월차SR프로젝트
======================

1. Client 설정
-------------

1.1. General
1. 속성 -> 일반 : output directory ../Bin\
2. 추가디렉토리 : ../../Base; ../Header; ../../Reference/Header
3. 링커-> 일반 : 추가포함디렉토 ../../Reference/Library
4. 입력 :시스템라이브러리

1.2. VC++Directory 
* Include Directories
  - $(DXSDK_DIR)Include
* Library Directories
  - $(DXSDK_DIR)lib\x86

1.3. C/C++
General
* Additonal Include Directries
  - ../Header
  - ../../Base
  - ../../Reference/Header

1.4. Linker


