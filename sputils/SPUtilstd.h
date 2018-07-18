#pragma once
/*
by simpower91 2018-06-29
i want to use some easy features,
i wanna program as quickly as Lazarus/Delphi.
so i make this unit.
*/

//ʵ����ֻ��Ҫ��cpp��ͷ�ļ�����cͷ�ļ��ĵط���extern "C"�Ϳ�����
/*����SPUtilsAttach.h����������
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "SPUtils.h" //ע��ͷ�ļ�ҲҪ��extern "C"���棬�����Ҳ���C����

arstr split(string s, string sprt, PINT len);

#ifdef __cplusplus
}
#endif

*/

//�����ط�����Ҫ����������ȫ��ע�͵�
//#ifdef __cplusplus
//extern "C" {
//#endif

//��������ͬ��,���������Ͳ���Ҫ export
//#pragma comment(linker, "/export:begin=_begin@0")
//���Ͻ�������32λ��x64����ֱ��exportʹ�ã�����#pragma,����stdcall
#define export extern "C" _declspec(dllexport)
#define stdcall __stdcall
#define uses extern "C" 
//��ʽΪ��export int stdcall function(); 

//�ڴ����1byte
#pragma pack(1)
//���½��VC SDL��鱨��,ǿ��ʹ�� xxx_s ��ȫ������
//������֮ǰ���˺ü������µ�_s���������һ�gcc������
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
// x64 SEH��û����
/*
#define TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define ETRY } }while(0)
#define THROW longjmp(ex_buf__, 1)
*/

//�������Ͷ���
typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64;
//�������Ͷ������



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

//��ʵdelphi��string�е�������COM�Ļ��ƣ�����Ϊ0��ʱ���Զ��ͷ�
//ʵ����malloc��freeҲ������˼�������棬ʹ��free��֪���ͷŵĴ�С��
//��̬������ʵҲʹ���������һЩ���ơ�
//Delphi����������֧�֣�����������ʵ�֣���ʱ�ȸ���
struct sAnsiString
{
  word  codeTP; //��������
  word  chSize; //�ַ���С
  integer quoteTimes; //���ô���
  integer len; //�ַ�������
  byte ch[1]; //�ַ������ݣ���00��β
};

struct sWideString
{
  word  codeTP; //��������
  word  chSize; //�ַ���С��������Ĵ�С���Ϳ��Ե�����̬����ʹ��
  integer quoteTimes; //���ô���
  integer len; //�ַ�������
  word ch[1]; //�ַ������ݣ���00 00��β,need to be dynamic 
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
  //����������Ҫ����0xffffffffffffffff��С���ڴ��ַ������˼�����1024��Ԫ�أ��������Ҫ�ƶ�ָ��6+2������
  //��˱�����hashTable��СΪ(sizeof(pointer)+sizeof(word))*1024*(6+2)��byte ����word���������±�ֵ[0..1023],����65535��Ԫ�ؾ�Ҫ��dword������0xffffffff��Ԫ�ؾ�Ҫ��Ϊ8byte
  //hash���󳤿�����£�
  /*
  width:(log elementQt range)  + 2 
  height:elementQt
  length:elementSize+indexSize
  */
  //���ú÷�Χ��Ԫ�ظ���֮�����Ų���Ԫ�ز������ӣ����Ҳ��輰ʱ��㶨��ʱ�临�Ӷ�O(1)
  //����ϣ��ȻΪhookAPI��д����Ҳ��ͨ�ã����ڳ���ɢ���㷨
  uint64 elementQt; //Ĭ��1024��
  uint64 elementSize;//Ĭ��8���ֽڣ�64λ��ַ����
  uint64 indexSize; //�±�ռ�õ��ֽ���
  uint64 matrixWidth; //������ log elementQt range
  pointer pHashTable ;//����ռ���ڴ��СΪ(Ԫ��size+���size)*Ԫ�ظ���*((logԪ�ظ��� ȡֵ��Χ��С)+2) 
  pointer self;
  pointer(*hashAddKV)(pointer obj, ...);
  pointer(*hashValues)(pointer obj, ...);
} THashMap,*PHashMap;
PHashMap THashMap_Create(uint64 range,uint64 elementQt,uint64 elementSize);
void THashMap_Free(PHashMap obj);



//TStringList class///////////////////////////////////
struct TStringItem
{
  string FString;//�����string��name=value�Ļ�������ͨ���ƶ�ָ������ȡ������ŵ�KV����
  nativeuint key;
  nativeuint value; // PObject FObject;��������������obj
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
  pointer(*addObject)(pointer obj, ...);  //ֱ�ӵ���hashAddKV
  pointer(*addKV)(pointer obj, ...);        //����kv�൱��addObject
  pointer(*valueByKey)(pointer obj, ...);   //����kv�൱��addObject
  pointer(*hashAddKV)(pointer obj, ...);        //ʹ��hash����kv�൱��addObject
  pointer(*hashValueByKey)(pointer obj, ...);   //ʹ��hash����kv�൱��addObject
  pointer(*loadFromFile)(pointer obj, ...);
  pointer(*saveToFile)(pointer obj, ...);
  pointer(*values)(pointer obj, ...);
  pointer(*names)(pointer obj, ...);
  pointer(*betweenKeyValue)(pointer obj, ...);
  pointer(*valueFromIndex)(pointer obj, ...);
  pointer(*objects)(pointer obj, ...);  
  pointer(*indexOfKey)(pointer obj, ...); //kv type, hash version
  pointer(*indexOf)(pointer obj, ...); // by string δʵ��
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


//ʵ����ֻ��Ҫ��cpp��ͷ�ļ�����cͷ�ļ��ĵط���extern "C"�Ϳ�����
//�����ط�����Ҫ����������ȫ��ע�͵�
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
  //ע���������stringReplace2�����s�����ã�����Ҫ�ͷ��ڴ�ģ����s�ǳ������������ͷ�
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