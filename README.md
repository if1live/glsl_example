# glsl-example

셰이더 프로그래밍 입문 for GLSL

[셰이더 프로그래밍 입문](http://www.hanb.co.kr/book/look.html?isbn=978-89-7914-949-4)을 참고해서 예제를 GLSL로 구현한 소스입니다. Shadow Mapping의 경우는 [ShadowMapping with GLSL][fabiensanglard]를 참고해서 구현했습니다.

## Build
1. ```build/runCmake.exe```를 실행
2. ```haruna_exe```를 실행 프로젝트로 설정
2. haruna_exe -> Properties -> Debugging -> Working Directory : ```$(SolutionDir)..\assets```

## Reference
* [셰이더 프로그래밍 입문][pope_book]
* [ShadowMapping with GLSL][fabiensanglard]
* [iPhone 3D Programming][iphone3d]

[pope_book]: http://www.hanb.co.kr/book/look.html?isbn=978-89-7914-949-4
[fabiensanglard]: http://fabiensanglard.net/shadowmapping/index.php
[iphone3d]: http://ofps.oreilly.com/titles/9780596804824/