# irrlicht0.1
irrlicht version 0.1 first commit

Irrlicht引擎共分为五部分：
1）Core
该部分由一些容器类及数学库组成，如string、vector等。
对应的名字空间为：namespace irr::core
2）Scene
该部分主要负责三维场景的绘制及管理，包括场景节点，摄像机，粒子系统、mesh 资源，公告板，灯光，动画器，天空体，地形等。
Irrlicht的场景中的所有的东西都是场景节点，统一由场景管理器来管理。
对应的名字空间为：namespace irr::scene
3）Video
该部分主要负责图片纹理的载入及管理，包括纹理，材质，灯光，图片，顶点等渲染属性的控制。
对应的名字空间为：namespace irr::video
4）GUI
该部分包括了一些二维GUI控件
对应的名字空间为：namespace irr::gui
5）FileSystem
该部分负责文件系统的读写。
对应的名字空间为：namespace irr::io

相关修改：
1）direct8改为direct9
1）删除line2d.h，没有被引用