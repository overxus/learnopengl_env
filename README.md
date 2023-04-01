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
此时在`build/Debug`目录下会生成可执行文件`learn_opengl.exe`. 将文件夹`shaders`、文件夹`nanosuit`以及文件夹`dll`下的`assimp-vc143-mtd.dll`文件移动到此处，打开`learn_opengl.exe`, 如果顺利的话，会看到机器人的脚。

通过按键A(左), D(右), W(前), S(后), ↑(上), ↓(下)调节镜头位置，观察整个机器人。