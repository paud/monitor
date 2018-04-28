:changedir
@echo off
::set tmppath=%1
::set _T=%1
::在cygwin的 /home/yourID/.bash_profile 末尾加上如下代码
::export _T=${_T//\\//}   # replace backslash to fowardslash 
::if [[ $_T == "" ]];then
::export _T="c:/"
::fi
::cd "$_T"

::set _T=%cd% 
set _T=%1
::%tmppath:~0,2%
::cd %tmppath%
::以下bash命令是为了从Dos环境进入cygwin环境
bash --login -i -c make
::运行编译脚本MakeFile
::make