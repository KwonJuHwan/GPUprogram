# GPUprogram

GPU Programming 수업을 통해, 만든 3D 모델링 생활관.
c++를 기반으로 둔 OpenGL를 통해, 처음으로 모델링을 해보았다. 처음 해보는 프로젝트라 어려움이 많았다. 
먼저 3D모델링이 되어있는 파일을 다운을 받고, 이를 OpenGL로 열어야하는데, 무료로 배포하는 파일 중 열리지 않는 파일이 많았다. 그래서 프로젝트를 준비하면서 원하는 이미지가 없어, Texture를 texture mapping 기법을 이용하여 PNG를 Texture에 입히는 방식(빌보드 기법)을 채택하여, PNG를 따오고, Texture를 제작 후, PNG를 입혔다.
Light_cube를 통해, 취침등을 만들어 directoinal light를 구현하였고, light_caster를 통해, 취침등과 손전등(Soft Point light)를 구현하였다. 
또한, 생활관 안에 있는 느낌을 내기 위해 camera를 구현하였다.

