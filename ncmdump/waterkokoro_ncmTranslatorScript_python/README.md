# ncmTranslatorScript_python
网易云音乐.ncm音乐格式转换python脚本

使用方法：

方法一：
  1、将 ncmTranslator.py 脚本放到需要处理的文件夹下    
     2、执行 ./ncmTranslator.py   
方法二：
  ./ncmTranslator.py {参数：指定要处理的绝对路径}  
    
ps：只在ubuntu下使用过, 使用的python3   
ps2: 顺便会下载专辑图片到音乐文件所在的路径    
引用代码：https://github.com/lianglixin/ncmdump/blob/master/folder_dump.py    
    
    
后台使用：nohup ./ncmTranslator.py > ./process.log 2>&1 &    
后台使用的方法查看日志：tail -f process.log    
