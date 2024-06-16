# Wizard Magic Key Cyber
> `Wizard Magic Key Cyber` by SN-Grotesque

此库已在以下环境下测试通过：
1. Ubuntu/Debian系Linux操作系统 - GCC套件
2. Windows 10操作系统 - MinGW64 - Visual Studio (2019~2022)

### Author(s):
 - [SN-Grotesque](https://github.com/sngrotesque)
 - [R7](https://github.com/r7z7)

### 开放源代码许可（Open Source License）
 - [OpenSSL](https://github.com/openssl/openssl)
 - [libpng](http://libpng.org/pub/png/)
 - [Python](https://www.python.org/)
 - [cJSON](https://github.com/DaveGamble/cJSON/)
 - [Zlib](https://github.com/madler/zlib)
 - [iconv](https://www.gnu.org/software/libiconv/)

### 关于（About）
0.  Q：为什么想开发这个库？  
    A：因为我在未来会制作一个属于自己的即时通讯软件，想以这个库为基础进行开发。  
    Q：为什么使用C++？  
    A：其实最开始使用的是C，但是后来因为一些功能的实现难度，我最终考虑使用了C++，至于为什么不是C#或者Java以及Python，那是因为出于性能考虑。  
    Q：有些功能，目前市面上一些库已经包含了，为什么要自主开发一个类似的？  
    A：人嘛，有时候总是想要有一些自己能掌握的东西，自己决定更新进度，同时也可以学习这些功能的底层实现原理。
1.  此库拥有多套加密算法，其中包含了FEA（Fast encryption algorithm），由[SN-Grotesque](https://github.com/sngrotesque)自制研发的一款分组对称加密算法。
2.  此库还在逐步不断完善中。

### 使用（Usage）
1.  此库支持64位操作系统（Windows，Linux），并支持MingW64与MSVC（Visual Studio 2019+）与支持C++14标准的GCC套件。
2.  对于`MingW64`与`GCC套件`的使用，可以直接调用根目录的[run.py](run.py)代码来执行（功能基本上等同于make）。  
    而对于`Visual Studio`，需要自行将库编译为dll文件或者在主程序代码文件中包含`*.cpp`文件。

### 开发文档
 -  开发标准：
>   应使用至少支持C++14的编译器来对代码进行编译，同时为了保证Linux系统的可移植性。应尽量避免使用C++20及以上标准以兼容低版本GCC套件。
 -  代码风格：
>   每行不超过90个字符，注释请添加在可能会引起困惑或误会的部分。
 -  代码层级：
>   尽可能的少，如果可以，请按照Linux开发标准中的3级编写代码。

### 版本更新日志
> 修改了版本后需要更改的文件  
> upload.py  
> READMKE.md

#### v7.1.0
1.  重写了wmkc::Exception（抛弃原先的函数调用std::runtime_error的形式），将其作为一个完整的异常类使用，继承于std::exception。
2.  增加了对于wmkc::crypto::fea::ctr_xcrypt的更多支持，现在请使用counter类来构建一个计数器。
3.  将所有类的名称使用大驼峰命名法，跟随编码规范。
4.  ~~更新了[终端字体颜色库](includes/config/color.hpp)对于C++的支持，抛弃了原先的C语法。~~
5.  将Python代码移动到了[WMKC_for_Python](https://github.com/sngrotesque/WMKC_Python)仓库，如果要使用，请去对应仓库查看，谢谢。
6.  添加了[ChaCha20](https://github.com/marcizhu/ChaCha20)加密算法的实现，如果你要使用其他实现，请自行调用（如OpenSSL库提供的ChaCha20）。

等待添加的功能
- 为socket库增加简易版的setsockopt与getsockopt方法，以及待完善的Socket库和SSL库。（将逐步完成Socket库）

#### v7.0.0
1.  所有C++代码重新组织代码结构，将所有功能列入wmkc命名空间，并将原有的类更名。  
    例如：wmkcBase64变为`wmkc::base64`，其中的方法encode变为`wmkc::base64().encode()`
2.  重新优化所有代码。
3.  修复了FEA加密算法的一个密钥长度隐患。
4.  完成了`wmkc::structure::pack`，但是需要使用者注意`C/C++`的特性，解释在[struct.hpp](includes/struct.hpp)头部。  

#### v6.6.6
1.  C++代码为完全复刻C代码的内容，但是修复了各种问题与难用性。
2.  C代码暂时停止维护，其中存在内存泄露等问题，如需使用，请尽可能使用C++代码。

#### v6.5.0
1.  新增C++代码

#### v6.4.0-dev
1.  重构wmkcNet库与wmkcError结构。
2.  优化了wmkc_obj结构。
3.  优化了wmkc_file库的编码转换功能。
4.  新增了wmkcSSL库用于进行HTTPS通信。
5.  新增了C++代码，以便于实现各种面向对象的功能。

#### v6.3.0-canary
1.  预计将所有实现改为内部实现，然后将函数作为类成员的方式使用（由于效率原因已弃之）。
2.  修复了network/wmkc_net库中对于wmkcNet_timeout函数的实现，  
    傻逼微软你妈死了，弄你妈的毫秒单位啊。

#### v6.2.0-canary
1.  将修改wmkcNet库，以支持一定情况下的OpenSSL组件，以便于进行HTTPS通信。
2.  将完善wmkcFile库，更好的支持对于不同操作系统的文件操作函数。
3.  将针对所有WMKC库进行优化。

#### v6.1.0-dev
1.  将wmkc_chat库中的对象指针成员修改为数组成员，  
    否则会导致无意义的代码过长。
2.  重构了wmkc_file库，删除了wmkcFile对象。
3.  放弃使用icu4c库对编码进行转换，改用iconv库。
4.  小幅度修改了wmkc_pad，wmkc_base64库。
5.  删除了wmkcFloat，wmkcFile类型，多余而且扰乱思绪。
6.  将wmkcMisc_PRINT_RAW函数修改为了类似Python中的print bytes类型的实现。

#### v6.0.0-dev
1.  将wmkc_base修改为wmkc_base64，因考虑到  
    不会添加base16（因为此编码可以使用wmkc_binascii库实现）  
    不会添加Base32（因为此编码实在没有必要添加）  
    不会添加Base85（因为此编码（又称Ascii85）目前还没有一个完美的标准）  
    并将Base64的编码与解码库重新设计，去掉了基于wmkc对象的形式。
2.  添加了wmkc_dict库，更好的操作键值对类型的数据。
3.  修改了关于颜色的宏。
4.  修复了wmkc_misc库的BUG。
5.  部署了libpng库，重新构建了zlib库文件。
6.  修复了wmkc_stream库中wmkc_scanf函数的隐藏BUG。
7.  将wmkcStream库中的wmkcStream_scanf函数移动到wmkc_common库并更名为wmkc_scanf。

#### v5.1.0-dev
1.  修改了wmkc_random，wmkc_time库的实现。
2.  修复了wmkc_base库的错误信息不全的问题。
3.  修改了对于系统宏的名称标识。
4.  增加了适当的文档与注释。
5.  增加了wmkc_thread库用于多线程编程。

#### v5.0.0-dev
1.  将库更名为Wizard Magic Key Cyber，原因是与目前某一组织名称重合。
2.  将库函数与一般变量类型与宏函数做了更名。
3.  修改了一些针对不同操作系统的支持。
