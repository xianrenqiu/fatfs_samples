# fatfs samples

![layers](http://www.elm-chan.org/fsw/ff/res/layers.png)

> 示例为 `fatfs` 在 `linux` 平台的示例代码，基于 [`fatfs R0.13a`](http://www.elm-chan.org/fsw/ff/00index_e.html) 版本修改了 `diskio.c/diskio.h` 文件, 示例中使用系统 `RAM` 或 `DISK` 作为设备载体，可作为移植到嵌入式设备的前期调试环境。

## 编译

```t
$ mkdir build && cd build
$ cmake ..
$ make
```

## 运行

```t
$ ./sample

  fatfs sample use RAM.

  0. creat fatfs system success

  1. mkdir success, dir name: /abc

  2. open /abc/file1.txt file success
  3. write 1234567 to /abc/file1.txt file success

  4. dir name: ABC
  5. file name: FILE1.TXT

  6. open /abc/file1.txt file success
  7. read 1234567 from /abc/file1.txt file success
```