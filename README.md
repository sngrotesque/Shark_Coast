# The wizard's universal key

**在未阅读完此文档且有异议的情况下请不要参与此代码库的编写。**
**并且此库随时可能在不同操作系统中进行测试，如遇编译无法通过，请自行修改编译指令。**

 - [作者 / Authors](#authors)
 - [开放源代码许可 / OpenSource license](#开放源代码许可open-source-license)
 - [提问与解答 / Q&A](#提问与解答qa)
 - [关于 / About](#关于about)
 - [使用 / Usage](#使用usage)
 - [开发规范 / Development Specifications](#开发规范development-specifications)
 - [版本更新日志 / ChangeLogs](#版本更新日志change-log)
 - [鸣谢 / Acknowledgement](#鸣谢acknowledgement)

### Author(s):
 - [SN-Grotesque](https://github.com/sngrotesque)
 - [R7](https://github.com/r7z7)

### 开放源代码许可（Open Source License）
 - [OpenSSL](https://github.com/openssl/openssl) - 使用OpenSSL的各种算法库
 - [libpng](http://libpng.org/pub/png/) - 未来将用于PNG图像的读取和写入
 - [Zlib](https://github.com/madler/zlib) - 进行CRC32校验与后续可能的压缩功能
 - [iconv](https://www.gnu.org/software/libiconv/) - 作为字符串编码转换时使用的库，如果可能后续考虑自行实现

### 鸣谢（Acknowledgement）
 - [Python](https://github.com/python/cpython) - 参考并借鉴了Python的C源代码（包括语法和部分功能的实现），可以说我的开发者生涯都深受Python的影响。

### 提问与解答（Q&A）
**Q**: 此库的（明确）依赖项有哪些？
A：内部依赖项：[WukConfig](wuk/includes/config/WukConfig.hh)/[WukException](wuk/includes/config/WukException.hh)/[WukMemory](wuk/includes/WukMemory.hh)。外部依赖项：[OpenSSL](https://github.com/openssl/openssl)/[Zlib](https://github.com/madler/zlib)。

**Q**：为什么想开发这个库？  
A：因为我在未来会制作一个属于自己的即时通讯软件，想以这个库为基础进行开发。

**Q**：为什么使用C++？  
A：其实最开始使用的是C，但是后来因为一些功能的实现难度，我最终考虑使用了C++，至于为什么不是C#或者Java以及Python，那是因为出于[性能](https://goodmanwen.github.io/Programming-Language-Benchmarks-Visualization/)考虑。

**Q**：有些功能，目前市面上一些库已经包含了，为什么要自主开发一个类似的？  
A：人嘛，有时候总是想要有一些自己能掌握的东西，自己决定更新进度，同时也可以学习这些功能的底层实现原理，其实说白了就是可以自己定义很多市面上的库没有的功能。

**Q**：为什么没有`master`这种主分支？  
A：在功能还没完全成型以及Bug还没排除掉绝大多数的情况下，我并不想污染主分支。

**Q**：为什么没有明确支持`Mac OS`系统？  
A：因为我从未真正使用过`Mac OS`[^macos]，并且也不熟悉它的操作和工作原理，不过正常来说，既然它是[POSIX标准](https://pubs.opengroup.org/onlinepubs/9699919799/)[^what_is_posix]的产物，那其实应该也是支持的。

### 关于（About）
1.  此库拥有多套加密算法，其中包含了[FEA（Fast encryption algorithm）](wuk/includes/crypto/fea.hpp)，由[SN-Grotesque](https://github.com/sngrotesque)自制研发的一款分组对称加密算法。更多关于此算法的描述请[查看](doc/Reference.md#L53)。
2.  此库还在逐步不断完善中，缺少开发者的支持，如果你有能力，请和我一起开发这个库，非常感谢。

此库已在以下环境下测试通过：
1. `Ubuntu/Debian`系 - **GNU C**
2. `Windows 7/8(.1)/10/11` - **MinGW64 (MSYS2)** or **Visual Studio (2019~2022)**

### 使用（Usage）
1.  此库仅支持64位操作系统（Windows，Linux），请不要在使用32位操作系统的情况下使用此库，否则可能会出现意想不到的情况。
2.  对于`MingW64 (MSYS2)`与`GNUC`的使用，可以直接调用根目录的[make.py](make.py)代码来执行（功能基本上等同于make）。  
    而对于`Visual Studio`，需要自行将库编译为dll文件或者在主程序代码文件中包含对应的代码文件。  
    未来会使用[CMakeList.txt (Cmake)](https://cmake.org/)来强化编译步骤。
3.  目前暂未对[`Clang`](https://clang.llvm.org/)这款编译器做直接测试，请使用Clang作为编译器的开发者自行判断此库在Clang中的可行性。

### 开发规范（Development specifications）
 -  开发标准：
1.  C++ standard >= `C++11`，目前已使用的最高标准为`C++20`。
    - 针对低版本（或未完全支持C++20标准）的编译器，应使用宏`#if __cplusplus >= 202002`（在此库的编写中请使用`WUK_STD_CPP_20`宏）来判断是否编译时指定了`C++20`，如果是就转到`C++20`的代码。而不应该直接舍弃`C++20`。
2.  对于`reinterpret_cast`[^reinterpret_cast]关键字的使用，在**Windows-MingW64-VisualStudio2022**和**Linux-GNUC**中  
    测试无问题，相当于C代码中的强制类型转换[^forced_conversion]，目前测试未发现问题，继续使用。
3.  为避免产生混淆和误解，请在内部实现中使用类的成员时加上显式`this`调用，防止理解上与参数或变量混淆。
    - 比如`MyClass::func`中使用了自身的`val`成员，应使用`this->val`而不是`val`。

 -  代码风格：
1.  单行不超过90个英文字符（非强制，但尽可能不要超过100个字符）。
2.  对于类型的初始化，只使用两种初始化方法。
    - 第一种是[int a = 0](https://en.cppreference.com/w/cpp/language/copy_initialization)，第二种是[int a{0};](https://en.cppreference.com/w/cpp/language/list_initialization)。
3.  仅使用三种主要命名方式（不限制其他方式，但请不要违反此条）：
    1. 大驼峰命名法（仅用于命名类），示例：`class MyClass`。
    2. 小驼峰命名法（仅可用于命名变量），示例：`int keyLength`。
    3. 下划线命名法（可用于函数命名和变量命名），示例：`void get_file()` or `Data data_set`。

    对于变量，不应该参杂太多名词，比如在循环中的`index`，能写为`i`就不要写为`process_result_index`。
4.  所有宏命名以及枚举类型应全部为大写并使用下划线命名法，例如：`WUK_SUPPORT`，`GAME_DEVELOP`，`enum class mode {CFB, CBC, CTR}`。
5.  注释请添加在可能会引起困惑或误会的部分（或者如果你需要在文件的起始部分想说明什么东西）。  
    ~~对于较少代码却需要很多注释，那么你应该考虑重新编码，因为你已经写出了垃圾代码。~~  
    并且只使用三种注释方式：
    1. 单行注释，仅用于解释某个变量到底是做什么的。
    2. [Doxygen注释](https://www.doxygen.nl/manual/docblocks.html)用于解释一个比较复杂的函数或类在做什么，以及它的参数代表什么。
    3. Linux开发规范中定义的[注释形式](https://www.kernel.org/doc/html/latest/translations/zh_CN/process/coding-style.html#id10)。
6.  尽可能避免对结构体使用`typedef`关键字，如果必须，那么你应该考虑是否将它改为类。
7.  代码层级应尽可能的少，如果可以，请按照Linux开发标准中建议的最多3层嵌套来编写代码[^linux_coding_style]。  
    由于C++不同于C，很多语法不能完全按照C的语法来实现，针对于嵌套层级这点不强求。
8.  ~~即便此库使用的是C++，你用C的语法来写也是完全没有问题的，不过唯一需要注意的是，目前所有内存指针全部使用`new`来申请内存，在
    发生巨大改动之前，请不要使用`malloc`与`realloc`这类C内存管理函数。需要确保释放内存时的语法一致性。~~  
    对于基本类型（如`int`, `char`, `unsigled char`, `(u)int{x}_t`, `pointer （不含类类型指针）`等），应使用[WukMemory](wuk/includes/WukMemory.hh)中的`wuk::m_alloc`与`wuk::m_realloc`等处理内存空间，对于类类型，请使用C++关键字`new ([])`与`delete ([])`等处理内存空间。
9.  使用`空格(Space)`作为缩进符！缩进单位：**4**个空格符。
10. 对于类型转换[^diff_type_conversion]：对于数字类型（整数和浮点数）使用C风格转换或C++风格转换皆可，其他类型请全部使用C++风格的转换。
11. 为了保证可读性和可维护性，请在实现时按照声明顺序实现一个类类型。例如类中的方法顺序是`funcA(); funcB(); funcC();`，那么在实现文件中也必须是这个顺序，不能是比如`funcB(); funcA(); funcC();`。

### 版本更新日志（Change log）
> 在未来实现爬虫库时要查阅的文档：
> 1. [Hypertext Transfer Protocol -- HTTP/1.1](https://datatracker.ietf.org/doc/html/rfc2616)
> 2. [HTTP - Wikipedia](https://en.wikipedia.org/wiki/HTTP)
> 3. [HTTP resources and specifications - HTTP | MDN](https://developer.mozilla.org/en-US/docs/Web/HTTP/Resources_and_specifications)
>
> 修改了版本后需要更改的文件  
> [upload.py](upload.py)  
> [READMKE.md](README.md)

#### v0.8
1.  修改了代码文件的格式，以防止使用时与其他头文件冲突。
2.  ~~逐步将所有`new`方法改为C方法的`malloc`函数，后续考虑是否加入智能指针。~~
    - 此条请看[开发规范 / Development Specifications](#开发规范development-specifications)
3.  修复了关于初始化列表中的一个可能存在的指针指向错误的问题。
4.  优化了wuk::net中的代码。
5.  将网络库中的使用堆空间的指针全部改为[wuk::Buffer](wuk/includes/WukBuffer.hh)。  
    暂时不会考虑使用智能指针，因其无法达成我的要求。
6.  完善了[wuk::Buffer](wuk/includes/WukBuffer.hh)模块的功能，未来也许会添加更多功能。
7.  完善并优化了一些细节与修复了一个空指针指向问题。

#### v0.7.2
1.  修复了[Base64解码函数](wuk/includes/base64.hpp)在`v0.7.1`版本中的BUG，并修复了`v0.7.1`版本中解码函数未针对填充符进行处理的问题，现已经可以使用。  
    ~~但目前并未完全按照[RFC4648](https://datatracker.ietf.org/doc/html/rfc4648)实现，请等待后续的进一步完善。~~  
    使用Python中的方法来完善了Base64的解码功能，但存在一个处理受污染Base64编码串的小问题。  
    即：最终指针实际使用的内存空间会比解码后应实际占用的内存空间略大（这取决于Base64编码串被污染的程度），将在后续版本中修复此问题。  
    ~~目前又发现一个问题，一种极为特殊的被污染的Base64编码串会导致直接的内存泄露或者程序闪退。~~
    问题解决，问题的根源是`bin_data`指针指向错误，应使用`bin_data_start`指针来释放内存。**妈的终于把心头的一个大石头移开了，我就说这段时间怎么感觉胸闷。**
2.  提供了用于[公开库的API调用](wuk/includes/config/wuk.hpp#L36)，现在可以更好的支持编译器导入和导出库函数了。  
    请在编译时加入宏**WUK_EXPORTS**，否则很可能将编译失败。
3.  已修复一些编译器的警告项。
4.  此版本将着重检查和修复一些可能的隐患。
5.  完善了[密码库的计数器](wuk/includes/crypto/utils.hpp#L9)。
6.  完善了部分[SSL库](wuk/includes/network/ssl.hpp)的功能。
7.  完善了[Binascii](wuk/includes/binascii.hpp)，修复了[Base64](sources/base64.cpp)中的异常捕获隐患。
8.  修改并完善了[WUK库的代码和宏定义](wuk/includes/config/wuk.hpp)，以便未来更好的支持各个平台以及各个编译器。
9.  逐渐将所有以前的C语法改为C++语法。
10. 优化了[make.py](make.py)的代码。

#### v0.7.1
1.  重写了wuk::Exception（抛弃原先的函数调用std::runtime_error的形式），将其作为一个完整的异常类使用，继承于std::exception。
2.  增加了对于wuk::crypto::fea::ctr_xcrypt的更多支持，现在请使用counter类来构建一个计数器。
3.  将所有类的名称使用大驼峰命名法，跟随编码规范。
4.  ~~更新了[终端字体颜色库](wuk/includes/config/color.hpp)对于C++的支持，抛弃了原先的C语法。~~
5.  添加了[ChaCha20](https://github.com/marcizhu/ChaCha20)加密算法的实现，如果你要使用其他实现，请自行调用（如OpenSSL库提供的ChaCha20）。
6.  **如果使用此库中提供的[Base64](wuk/includes/base64.hpp)进行解码操作，有可能会导致内存溢出或内存泄漏甚至更严重的后果，请不要使用！！！**  
    将重新实现有关Base64编码中的解码函数，目前发现了一个问题，具体描述：  
    在[base64.cpp 第93行](sources/base64.cpp#L93)与[base64.cpp 第94行](sources/base64.cpp#L94)，将值与解码表进行置换后，值甚至会大于`800`，并且
    让这个值与`0xff`进行与运算`v & 0xff`后依旧无法得出正确的值。  
    这个问题是我在使用Python进行此库的重新实现时发现的，不确定是否为Python的问题，但是此库会进行检查和修复，并重新实现，并且将完全按照规范进行编写。  
    此Base64算法的最初实现是[CSDN - C语言实现Base64编码/解码](https://blog.csdn.net/qq_26093511/article/details/78836087)，估计就是这个时候就
    留下了隐患，当初是直接拿这个代码然后对其进行优化和重新编码后得到目前的Base64版本。

等待添加的功能
- 为socket库增加简易版的setsockopt与getsockopt方法，以及待完善的Socket库和SSL库。（将逐步完成Socket库）

#### v0.7.0
1.  所有C++代码重新组织代码结构，将所有功能列入wuk命名空间，并将原有的类更名。  
    例如：wukBase64变为`wuk::base64`，其中的方法encode变为`wuk::base64().encode()`
2.  重新优化所有代码。
3.  修复了FEA加密算法的一个密钥长度隐患。
4.  完成了`wuk::structure::pack`，但是需要使用者注意`C/C++`的特性，解释在[struct.hpp](wuk/includes/struct.hpp)头部。  

#### v0.6.6
1.  C++代码为完全复刻C代码的内容，但是修复了各种问题与难用性。
2.  C代码暂时停止维护，其中存在内存泄露等问题，如需使用，请尽可能使用C++代码。

#### v0.6.5
1.  新增C++代码

#### v0.6.4
1.  重构wukNet库与wukError结构。
2.  优化了wuk_obj结构。
3.  优化了wuk_file库的编码转换功能。
4.  新增了wukSSL库用于进行HTTPS通信。
5.  新增了C++代码，以便于实现各种面向对象的功能。

#### v0.6.3
1.  预计将所有实现改为内部实现，然后将函数作为类成员的方式使用（由于效率原因已弃之）。
2.  修复了network/wuk_net库中对于wukNet_timeout函数的实现，  
    傻逼微软你妈死了，弄你妈的[毫秒单位](https://learn.microsoft.com/zh-cn/windows/win32/api/winsock/nf-winsock-setsockopt)啊。

#### v0.6.2
1.  将修改wukNet库，以支持一定情况下的OpenSSL组件，以便于进行HTTPS通信。
2.  将完善wukFile库，更好的支持对于不同操作系统的文件操作函数。
3.  将针对所有WUK库进行优化。

#### v0.6.1
1.  将wuk_chat库中的对象指针成员修改为数组成员，  
    否则会导致无意义的代码过长。
2.  重构了wuk_file库，删除了wukFile对象。
3.  放弃使用icu4c库对编码进行转换，改用iconv库。
4.  小幅度修改了wuk_pad，wuk_base64库。
5.  删除了wukFloat，wukFile类型，多余而且扰乱思绪。
6.  将wukMisc_PRINT_RAW函数修改为了类似Python中的print bytes类型的实现。

#### v0.6.0
1.  将wuk_base修改为wuk_base64，因考虑到  
    不会添加base16（因为此编码可以使用wuk_binascii库实现）  
    不会添加Base32（因为此编码实在没有必要添加）  
    不会添加Base85（因为此编码（又称Ascii85）目前还没有一个完美的标准）  
    并将Base64的编码与解码库重新设计，去掉了基于wuk对象的形式。
2.  添加了wuk_dict库，更好的操作键值对类型的数据。
3.  修改了关于颜色的宏。
4.  修复了wuk_misc库的BUG。
5.  部署了libpng库，重新构建了zlib库文件。
6.  修复了wuk_stream库中wuk_scanf函数的隐藏BUG。
7.  将wukStream库中的wukStream_scanf函数移动到wuk_common库并更名为wuk_scanf。

#### v0.0.51
1.  修改了wuk_random，wuk_time库的实现。
2.  修复了wuk_base库的错误信息不全的问题。
3.  修改了对于系统宏的名称标识。
4.  增加了适当的文档与注释。
5.  增加了wuk_thread库用于多线程编程。

#### v0.0.50
1.  将库更名为Wizard Magic Key Cyber，原因是与目前某一组织名称重合。
2.  将库函数与一般变量类型与宏函数做了更名。
3.  修改了一些针对不同操作系统的支持。

[^macos]: [Is Mac OS X a POSIX OS?](https://stackoverflow.com/questions/5785516/is-mac-os-x-a-posix-os), [OS X for UNIX Users](https://images.apple.com/media/us/osx/2012/docs/OSX_for_UNIX_Users_TB_July2011.pdf), [Programming MacOS-X and the Linux API - POSIX compatible?](https://stackoverflow.com/questions/9013717/programming-macos-x-and-the-linux-api-posix-compatible)

[^what_is_posix]: [What is the meaning of "POSIX"?](https://stackoverflow.com/questions/1780599/what-is-the-meaning-of-posix), [POSIX - Wiki](https://en.wikipedia.org/wiki/POSIX)

[^linux_coding_style]: [Linux 内核代码风格](https://www.kernel.org/doc/html/latest/translations/zh_CN/process/coding-style.html)

[^reinterpret_cast]: [reinterpret_cast conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast), [C++标准转换运算符reinterpret_cast - 博客园](https://www.cnblogs.com/ider/archive/2011/07/30/cpp_cast_operator_part3.html), [reinterpret_cast in C++ | Type Casting operators](https://www.geeksforgeeks.org/reinterpret_cast-in-c-type-casting-operators/)

[^forced_conversion]: `const char *p = "1234"; uint8_t *p1 = (uint8_t *)p;`

[^diff_type_conversion]: [What is the difference between static_cast and reinterpret_cast? [duplicate]](https://stackoverflow.com/questions/6855686/what-is-the-difference-between-static-cast-and-reinterpret-cast), [When should static_cast, dynamic_cast, const_cast, and reinterpret_cast be used?](https://stackoverflow.com/a/332086/21376217), [What is the C equivalent for reinterpret_cast?](https://stackoverflow.com/questions/1382051/what-is-the-c-equivalent-for-reinterpret-cast)
