:changedir
@echo off
set _T=%1
set _makefile="Makefile"
set "a=%2"

if "%_T%"=="" (
    set _T=%cd%
)
if "%a%"=="" (
    echo %a% is not assigned
) else (
    set _makefile=%a%
)
::实际上交叉编译，直接用x64的cygwin也能编译32位的，唯一需要区别的就是gdb，其它的都可以混用
F:\cygwin86\bin\bash --login -i -c "make -f %_makefile%"
