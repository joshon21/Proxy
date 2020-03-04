# 代理管理工具使用介绍

### 一. 项目介绍

- 该项目是基于C/S架构的一个代理管理工具，目的是通过服务端灵活管控并查看各客户端IE代理配置，方便切换客户端主机走不通代理上网

### 二. 文件介绍

- server: 该文件夹为服务的编译后的可执行文件，以及相应依赖库文件，监听6666端口，若修改，需从新编译源码

- client: 该文件夹为客户端编译后的可执行文件，目前编译配置的服务端IP为192.168.8.17:6666，如若修改，需要重新编译客户端源码

- code: 客户端和服务端的源代码路径

- code/server/main.cpp: 服务端程序的入口函数

- code/client/client/main.cpp: 客户端程序的入口函数

### 三. 界面介绍

- Server: 客户端IE默认设置的代理IP

- Port: 客户端IE默认设置的代理IP的端口

- IE刷新频率: 每隔多久同步IE的代理设置

- 连接频率: 每隔多久客户端向服务端发送自己的代理设置

- 全选，可批量针对全部客户端操作，单选/多选，可对选中的客户端操作

- 代理状态: 默认都会帮客户端开启，如果客户禁用后1分钟会自动启用，我们双击按钮可以禁用客户端使用代理
