### ArtnetPlayer

具体原理为加载MOV（512*X）影片，根据设定帧率刷新影片，读取每一帧影片，读取每一行Pixels数据读取第一通道（R），将数据通过Art net发送出去
已经测试过512universe 60fps实时发送，配合ArtnetRecorder可以实现数据还原。

![](./screenshot.png)

data文件夹内Setting.xml文件为配置文件，可以设置需播放的视频文件名称、输出帧率、是否自动播放、输出Artnet数据首个Universe和总计多少个Universe，启动窗口是否显示UI控件

快捷键g，显示或隐藏UI；快捷键S，停止播放；快捷键空格，播放或停止播放