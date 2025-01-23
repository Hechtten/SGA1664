# EPIX 图像采集卡开发环境配置说明

1.配置mingw64环境，在.vscode中添加EPIX的inc和lib路径以及XCLIBW64.dll的路径
2.运行xclibex1.c成功，需要对cext.h进行修改，加入uint,ushort,ulong的定义,其原因可能是由于较新版本的 sys/types.h 可能不再自动定义这些类型。
3.在文件目录中加入XCAP的配置文件xcvidset.fmt，修改xclibel1.c中的FORMAT为"default"为FORMATFILE的"xcvidset.fmt"

# 图像数据的处理

1. 使用50us的EN信号，获得帧频为20K的图像数据，每个像素为12bit的TDC数据，将TDC数据转换为firemap
2. 对多幅firemap进行叠加
3. 质心提取
