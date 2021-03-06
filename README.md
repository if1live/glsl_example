# 셰이더 프로그래밍 입문 for GLSL

[셰이더 프로그래밍 입문](http://www.hanb.co.kr/book/look.html?isbn=978-89-7914-949-4)을 참고해서 예제를 GLSL로 구현한 소스입니다. Shadow Mapping의 경우는 [ShadowMapping with GLSL][fabiensanglard]를 참고해서 구현했습니다.

## Build
1. ```cmake .```를 실행
2. ```haruna_exe```를 실행 프로젝트로 설정
4. Color Conversion, Edge Detection의 경우는 실행중에 1/2/3 을 눌러서 모드를 바꿀 수 있다.

## Screen Shot / Video
### GLSL Color Shader Example
[Video](http://youtu.be/UXAQF222bhQ)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/color_shader.jpg)

### GLSL Texture2D Mapping
[Video](http://youtu.be/FujIAOStZ54)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/texture_mapping.jpg)

### GLSL Lighting Example
[Video](http://youtu.be/WYu2eOt7TSM)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/lighting.jpg)

### GLSL Diffuse/Specular Mapping Example
[Video](http://youtu.be/xZ3KtjtmWLw)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/diffuse_specular_mapping.jpg)

### GLSL Toon Shader Example
[Video](http://youtu.be/bLChCT_nzuU)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/toon_shader.jpg)

### GLSL Normal Mapping Example
[Video](http://youtu.be/kz0rjRE7AKU)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/normal_mapping.jpg)

### GLSL Environment Mapping Example
[Video](http://youtu.be/kz0rjRE7AKU)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/env_mapping.jpg)

### GLSL UV Animation Example
[Video](http://youtu.be/9-J4WDu-BfU)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/uv_animation.jpg)

### GLSL Shadow Mapping Example
[Video](http://youtu.be/g1dlrTpxYIc)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/shadow_mapping.jpg)

### GLSL Color Conversion Example
Default / Gray / Sepia

[Video](http://youtu.be/Bttrulb0KfQ)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/gray.jpg)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/sepia.jpg)

### GLSL Edge Detection Example
Default / Edge Detection / Emboss 

[Video](http://youtu.be/kpPKTpR0Z3A)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/edge_detection.jpg)

![ScreenShot](https://raw.github.com/if1live/glsl_example/master/screenshot/emboss.jpg)

## Reference
* [셰이더 프로그래밍 입문][pope_book]
* [ShadowMapping with GLSL][fabiensanglard]
* [iPhone 3D Programming][iphone3d]

## Changelog
* 2013/09/05 : cmake 리펙토링. glfw3로 버전 올림. 기타 자잘한 수정. cross platform 가능한거같은 기분이 들게 수정(...)
* 2013/04/27 : 최초 Release

[pope_book]: http://www.hanb.co.kr/book/look.html?isbn=978-89-7914-949-4
[fabiensanglard]: http://fabiensanglard.net/shadowmapping/index.php
[iphone3d]: http://ofps.oreilly.com/titles/9780596804824/