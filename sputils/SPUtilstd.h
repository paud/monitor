#pragma once
/*
by simpower91 2018-06-29
i want to use some easy features,
i wanna program as quickly as Lazarus/Delphi.
so i make this unit.
*/

//实际上只需要在cpp的头文件包含c头文件的地方加extern "C"就可以了
/*比如SPUtilsAttach.h，像这样：
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "SPUtils.h" //注意头文件也要在extern "C"里面，否则找不到C函数

arstr split(string s, string sprt, PINT len);

#ifdef __cplusplus
}
#endif

*/

//其它地方不需要，所以这里全都注释掉
//#ifdef __cplusplus
//extern "C" {
//#endif

//导出函数同名,如果用这个就不需要 export
//#pragma comment(linker, "/export:begin=_begin@0")
//以上仅适用于32位，x64可以直接export使用，无需#pragma,无需stdcall
#define export extern "C" _declspec(dllexport)
#define stdcall __stdcall
#define uses extern "C" 
//格式为：export int stdcall function(); 

//内存对齐1byte
#pragma pack(1)
//以下解决VC SDL检查报错,强制使用 xxx_s 安全函数，
//害的我之前用了好几个啰嗦的_s函数，而且还gcc不兼容
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <Windows.h>
#include <malloc.h>
#include <stdlib.h>
#include <excpt.h>
//#include <setjmp.h>
#include<math.h>
#include "ntapi.h"


//#ifdef __cplusplus
//}
//#endif

//int setjmp(jmp_buf env);
//void longjmp(jmp_buf env, int val);
// x64 SEH，没作用
/*
#define TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define ETRY } }while(0)
#define THROW longjmp(ex_buf__, 1)
*/

//基本类型定义
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64;
//基本类型定义完毕



typedef INT64 int64;
typedef UINT64 uint64;
typedef UINT32 cardinal;
typedef INT32 integer;
typedef UINT32 dword;
typedef UINT32 uint;
typedef UINT16 word;
typedef UINT8 byte;

#ifdef _M_X64
typedef UINT64 nativeuint;
typedef UINT64 pointer;
typedef INT64 nativeint;
#endif // _M_X64
#ifdef _M_IX86 
typedef UINT32 nativeuint;
typedef UINT32 pointer;
typedef INT32 nativeint;
#endif // _M_IX86
typedef pointer pchar;
typedef pointer THandle;

#define mod %
#define inttostr intToStr
#define strtoint strToInt

//其实delphi的string有点类似与COM的机制，引用为0的时候自动释放
//实际上malloc和free也有这种思想在里面，使得free能知道释放的大小。
//动态数组其实也使用了这里的一些机制。
//Delphi编译器做了支持，不容易完美实现，暂时先搁置
struct sAnsiString
{
  word  codeTP; //编码类型
  word  chSize; //字符大小
  integer quoteTimes; //引用次数
  integer len; //字符串长度
  byte ch[1]; //字符串内容，以00结尾
};

struct sWideString
{
  word  codeTP; //编码类型
  word  chSize; //字符大小，改这里的大小，就可以当作动态赎罪使用
  integer quoteTimes; //引用次数
  integer len; //字符串长度
  word ch[1]; //字符串内容，以00 00结尾,need to be dynamic 
};

typedef pchar string;

struct stArrayString
{
  pchar strings[1024];
}; 
typedef struct stArrayString arstr,*parstr;

struct stReplaceFlag
{
  byte rfReplaceAll;
};
typedef struct stReplaceFlag TReplaceFlag;

struct stTObject
{
  pointer classTable;
};
typedef struct stTObject TObject,*PObject;



//THassMap Class//////////////////////////////////////
typedef struct stHashMap
{ 
  //本例中由于要覆盖0xffffffffffffffff大小的内存地址区域，因此假如有1024个元素，则最多需要移动指针6+2次命中
  //因此本例中hashTable大小为(sizeof(pointer)+sizeof(word))*1024*(6+2)个byte 其中word用于容纳下标值[0..1023],超过65535个元素就要换dword，超过0xffffffff个元素就要换为8byte
  //hash矩阵长宽高如下：
  /*
  width:(log elementQt range)  + 2 
  height:elementQt
  length:elementSize+indexSize
  */
  //设置好范围和元素个数之后，随着插入元素不断增加，查找步骤及时间恒定，时间复杂度O(1)
  //本哈希虽然为hookAPI所写，但也能通用，属于除法散列算法
  uint64 elementQt; //默认1024个
  uint64 elementSize;//默认8个字节，64位地址长度
  uint64 indexSize; //下标占用的字节数
  uint64 matrixWidth; //矩阵宽度 log elementQt range
  pointer pHashTable ;//矩阵占用内存大小为(元素size+标号size)*元素个数*((log元素个数 取值范围大小)+2) 
  pointer self;
  pointer(*hashAddKV)(pointer obj, ...);
  pointer(*hashValues)(pointer obj, ...);
} THashMap,*PHashMap;
PHashMap THashMap_Create(uint64 range,uint64 elementQt,uint64 elementSize);
void THashMap_Free(PHashMap obj);



//TStringList class///////////////////////////////////
struct TStringItem
{
  string FString;//如果是string的name=value的话，可以通过移动指针来获取，并存放到KV里面
  nativeuint key;
  nativeuint value; // PObject FObject;这个可以用来存放obj
};


//the base version,
//it will be extend to hash version later.
struct stTStringList
{
  PObject parent;
  pointer self;
  struct TStringItem FList[1024]; //static array temporary,only can hook 1024 apis, but enough already.
  int FCount;
  string ftxt;
  PHashMap hashMap;
  parstr FArstr;
  THandle FHandle;
  pointer(*text)(pointer obj, ...);
  pointer(*getFileText)(pointer obj, ...);
  pointer(*add)(pointer obj, ...);
  pointer(*addObject)(pointer obj, ...);  //直接调用hashAddKV
  pointer(*addKV)(pointer obj, ...);        //数字kv相当于addObject
  pointer(*valueByKey)(pointer obj, ...);   //数字kv相当于addObject
  pointer(*hashAddKV)(pointer obj, ...);        //使用hash数字kv相当于addObject
  pointer(*hashValueByKey)(pointer obj, ...);   //使用hash数字kv相当于addObject
  pointer(*loadFromFile)(pointer obj, ...);
  pointer(*saveToFile)(pointer obj, ...);
  pointer(*values)(pointer obj, ...);
  pointer(*names)(pointer obj, ...);
  pointer(*betweenKeyValue)(pointer obj, ...);
  pointer(*valueFromIndex)(pointer obj, ...);
  pointer(*objects)(pointer obj, ...);  
  pointer(*indexOfKey)(pointer obj, ...); //kv type, hash version
  pointer(*indexOf)(pointer obj, ...); // by string 未实现
  pointer(*indexOfName)(pointer obj, ...);
  pointer(*indexOfObject)(pointer obj, ...);
  pointer(*valueOfIndex)(pointer obj, ...);
  pointer(*clear)(pointer obj, ...);

  pointer(*writeToFile)(pointer obj, ...);
  pointer(*writeLineToFile)(pointer obj, ...);
  pointer(*items)(pointer obj, ...);
  pointer(*free)(pointer obj, ...);
  pointer(*setText)(pointer obj, ...);
  pointer(*deleteIndex)(pointer obj, ...);

};
typedef struct stTStringList TStringList;
typedef TStringList *PStringList;


//实际上只需要在cpp的头文件包含c头文件的地方加extern "C"就可以了
//其它地方不需要，所以这里全都注释掉
//#ifdef __cplusplus
//extern "C" {
//#endif
  PStringList TStringList_Create();
  void TStringList_Free(PStringList obj);



extern char* chsystemDlls;
#define systemDlls (string)chsystemDlls

typedef struct stTWinAPICall {
  PStringList apiDlls;
  pointer(*getApiPtr)(pointer obj, ...);
  pointer(*free)(pointer obj, ...);
}TWinAPICall,*PWinAPICall;

PWinAPICall TWinAPICall_Create();
void TWinAPICall_Free(PWinAPICall obj);



#ifdef __GNUC__
#elif defined(_MSC_VER)
//solve the problem of mingw64 cross compiling symble lost.
//eg. capstone cross compile...
extern _CRT_STDIO_INLINE int __CRTDECL __ms_vsnprintf(
  _Out_writes_opt_(_BufferCount) _Always_(_Post_z_) char*       const _Buffer,
  _In_                                              size_t      const _BufferCount,
  _In_z_ _Printf_format_string_                     char const* const _Format,
  va_list           _ArgList);
#endif

double SPlog(double a, double b);
int SPlog2(uint64 n);
uint64 SPlog64(uint64 a, uint64 b);


  string intToStr(int i);
  string intToHex(int i);
  int strToInt(string s);
  int64 strToInt64(string s);
  int pos(string substr, string s);
  uint length(string s);
  void setLength(string s, uint len);
  void showMessage(string s);
  void upperCase(string s);
  void lowerCase(string s);
  parstr split(string s, string sprt, PINT len);
  parstr split2(string s, string sprt, PINT len);
  string stringReplace(string s, string oldstr, string newstr, /*TReplaceFlag rflag*/...);
  //注意下面这个stringReplace2，如果s不再用，是需要释放内存的，如果s是常量，则无需释放
  string stringReplace2(string s, string oldstr, string newstr, /*TReplaceFlag rflag*/...);
  void freeArStr(parstr *ptr);
  string copy(string s, int istart, int len);
  string copy2(string s, int istart, int len);
  void copys(string* d, string s);
  THandle GetHInstance(BOOL bRef);
  string getlocal(string s);
  string extractFilePath(string s);
  cardinal getModuleSize(THandle ahandle);
  char* trim(char* str);
  boolean IsInQuotation(int iPos, string s);
  void deleteComment(PStringList FScript);
  void safefree(pointer ptr);


  //arstr split(string s, string sprt, PINT len);




//#ifdef __cplusplus
//}
//#endif