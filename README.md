# 使用说明(限Windows环境)
## 环境依赖
+ Visual Studio 2022
+ cmake
## 使用方法
```terminal
mkdir build
cd build
cmake ..
```
完成上面的步骤后，
```terminal
cmake --build .
```
此时在`build/Debug`目录下会生成可执行文件`learn_opengl.exe`. 将根目录下的`images`和`shaders`文件夹移动到此处，打开`learn_opengl.exe`, 如果顺利的话，会看到一系列转动的宝可梦箱子。