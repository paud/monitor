/*
by simpower91 2018-06-29
i want to use some easy features,
i wanna program as quickly as Lazarus/Delphi. 
so i make this unit.
经过GCC的教训，本单元需要拆分，以后有时间重构
把VC特定的独立出来，
TStringList和哈希需要独立出一个单元来，
PETools独立出来
WinAPICall独立出来
其它杂项放一个单元里
*/
#include "SPUtilstd.h"




//static char chsystemDlls[] = {
    // push r15 .. push rax
//    'n',
//};
char* chsystemDlls="ntdll\r\n \
Urlmon\r\nkernel32\r\naclayers\r\nadvapi32\r\n \
apphelp\r\nbcrypt\r\nbcrypt\r\nbcryptprimitives\r\ncfgmgr32 \
\r\ncombase\r\ncomtrl32\r\ncrytpase\r\ngdi32\r\ngdi32full \
\r\nimm32\r\niphlpapi\r\nkernel32\r\nkernelbase\r\n \
kernel.appcore\r\nmpr\r\nmsvcp_win\r\nmsvcrt\r\noleaut32 \
\r\npowrprof\r\nprofapi\r\nrpcrt4\r\nsechost\r\nsetupapi \
\r\nsfc\r\nsfc_os\r\nSHCore\r\nshell32\r\nshlwapi\r\n \
sspicli\r\nucrtbase\r\nuser32\r\nversion\r\nwin32u\r\n \
windows.storage\r\nws2_32\r\nwinmm\r\nwininet\r\n \
vbe6\r\nsecur32\r\nole32\r\nnetapi32\r\nncrypt\r\n \
mshtml\r\ndnsapi\r\ncrypt32\r\ncomctl32\r\n__wmi__";


int SPMagiCode =-0666666;

//Math Utilitys ////////////////////////////////////////////////////
double SPlog(double a, double b)
{
  return log(b) / log(a);
}

int SPlog2(uint64 n)
{
  int result=0;
  if (n & 0xffffffff00000000) { result += 32; n >>= 32; }
  if (n & 0x00000000ffff0000) { result += 16; n >>= 16; }
  if (n & 0x000000000000ff00) { result += 8; n >>= 8; }
  if (n & 0x00000000000000f0) { result += 4; n >>= 4; }
  if (n & 0x000000000000000c) { result += 2; n >>= 2; }
  if (n & 0x0000000000000002) { result += 1; n >>= 1; }
  return result;
}

uint64 SPlog64(uint64 a, uint64 b)
{
  int i, j;
  j = SPlog2(b);
  i = SPlog2(a);
  return j /i;
}

#ifdef __GNUC__
#elif defined(_MSC_VER)
//solve the problem of mingw64 cross compiling symble lost.
_CRT_STDIO_INLINE int __CRTDECL __ms_vsnprintf(
  _Out_writes_opt_(_BufferCount) _Always_(_Post_z_) char*       const _Buffer,
  _In_                                              size_t      const _BufferCount,
  _In_z_ _Printf_format_string_                     char const* const _Format,
  va_list           _ArgList)
{
  vsnprintf(_Buffer, _BufferCount, _Format, _ArgList);
  return 0;
}
#endif

//__try{}__except (filter(GetExceptionCode(), GetExceptionInformation())){}异常过滤用

int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {
  puts("in filter.");
  if (code == EXCEPTION_ACCESS_VIOLATION) {
    puts("caught AV as expected.");
    return EXCEPTION_EXECUTE_HANDLER;
  }
  else {
    puts("didn't catch AV, unexpected.");
    return EXCEPTION_CONTINUE_SEARCH;
  };
}
/*
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI SetUnhandledExceptionFilter(
  __in  LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter
);

LONG WINAPI UnhandledExceptionFilter(
  __in  struct _EXCEPTION_POINTERS* ExceptionInfo
);

LONG WINAPI unhandled_exception_filter(__in struct _EXCEPTION_POINTERS *ExceptionInfo)
{
  // do something
  return EXCEPTION_CONTINUE_SEARCH; // EXCEPTION_EXECUTE_HANDLER
}
*/
/*//OOP test ptr function
void classFn(pointer obj, ...)
{
}

pointer callfn(pointer fnName) {
  void(*p)(pointer obj, ...) = fnName;
  return p;
}
*/

string intToStr(int i) {
  string s;
  s = malloc(12);// -2147483648 ~ 2147483648 |uint 0~4294967295 /0
  itoa(i, s,10);
  return s;
}

string intToHex(int i) {
  string s;
  s = malloc(9); // FF FF FF FF /0
  _itoa_s(i, s, 9,16);
  return s;
}

//自动释放原s
string copy(string s, int istart, int len) {
  nativeuint org = s;
  pchar result;
  result = copy2(s, istart, len);
  safefree(org);
  return result;
}

//不释放原s
string copy2(string s, int istart, int len) { 
  pchar result = malloc(len + 1);
  memset(result, 0, len * sizeof(char) + 1);
  s += istart - 1;
  int l = len + 1;
  //strncpy_s(result, l, s, len);
  strncpy(result, s, len);
  return result;
}

//copys其实就是字符串指针赋值
void copys(string* d, string s) {
  int l = length(s);
  *d = malloc(l + 1);
  *d = copy(s, 1, l);
}

//自动释放原s
parstr split(string s, string sprt, PINT len) {
  nativeuint org = s;
  pointer result;
  result = split2(s, sprt, len);
  safefree(org);
  return result;
}

//不释放原s
parstr split2(string s, string sprt, PINT len) {
  int i, j, l;
  string s1;
  *len = 0; //这里的len传出数组的个数
  pointer result = malloc(1024 * sizeof(pointer));
  memset(result, 0, 1024 * sizeof(pointer));
  l = length(sprt);
  i = pos(sprt, s);
  j = 0;
  while (i > 0) {
    *(pointer*)(result + j * sizeof(pointer)) = copy2(s, 1, i - 1);
    j++;
    i += l;
    s1 = s;
    s = copy2(s, i, length(s) - i + 1);
    //TRY{
    //i = _msize(s1); //这个可以获取buffer长度 gcc用malloc_usable_size
    if (j>1)//第一次不让释放，免得出现异常，我认为可以根据cpu特性写汇编来安装异常句柄,以后再研究。
      safefree(s1);//也就是只释放while循环内分配的内存缓冲，参数由上层程序决定是否释放。
               //}       //而且不使用异常处理，速度也更快
               //result用完是要释放的，每个指针释放，然后整个result释放
               //CATCH{  //以后可以考虑改成一个对象来管理

               //}
               //ETRY;
               //__except (filter(GetExceptionCode(), GetExceptionInformation())) {
               //}
    i = pos(sprt, s);
  }
  if (length(s) > 0) {
    *(pointer*)(result + j * sizeof(pointer)) = s;
    j++;
  }
  *len = j;
  return result;
}

void freeArStr(parstr *ptr) {
  if (*ptr == NULL) { return 0; }
  int i = 0;
  while (i < 1024) {
    if ((*ptr)->strings[i] != NULL) {
      safefree((*ptr)->strings[i]);
      //(*ptr)->strings[i] = NULL;
    }
    else {
      break;
    }
    i++;
  }
  safefree(*ptr);
  *ptr = NULL;
}

int strToInt(string s) {
  return atoi(s);
}

int64 strToInt64(string s) {
  return _atoi64(s);
}

int pos(string substr, string s) {
  int i,j,k,l;
  pchar ret=NULL;
  l = length(s);
  k = length(substr);
  for (i = 0; i < l; i++) {
    j = 0;
    while (*(byte*)(s + i+j) == *(byte*)(substr+j)) {
      j++;
      if (j == k) {
        return ++i; //兼容delphi string
      }
    }
  }
  //ret = strstr(s, substr);//这个无法取得换行位置，所以只好自己写
  //后来我发现其实不是无法获取换行位置，而是换行缺少0结尾，从而获得了超长substr导致出错
  //不过既然已经自己写了那就这样吧。
  if (ret == NULL) return -1;
  //i = (nativeuint)ret - (nativeuint)s;
  //return ++i;
}

uint length(string s) {
  if (s == NULL) { return 0; }
  return strlen(s);
}

//自动释放原s
void setLength(string s, uint len) {
  pchar tmp = s;
  s = malloc(len);
  int l = length(s);
  if (l >= len)
    l = len - 1;
  strncpy_s(s, len, tmp, l);
  safefree(tmp);
}

//自动释放原s
string stringReplace(string s, string oldstr, string newstr, ...) {
  string result = malloc(1024); //最长支持1023个字符的长度,目前足够了
  memset(result, 0, 1024);
  nativeuint org = s;
  int i, j, k, l;
  j = 0;
  l = length(oldstr);
  k = length(newstr);
  i = pos(oldstr, s);
  while (i > 0) {
    strncpy_s(result + j, i, s, i - 1);
    s += (i - 1 + l);
    j += i - 1;
    strncpy_s(result + j, k + 1, newstr, k);
    j += k;
    i = pos(oldstr, s);
  }
  i = length(s);
  strncpy_s(result + j, i + 1, s, i);
  safefree(org);
  return result;
}

//不释放原s
string stringReplace2(string s, string oldstr, string newstr, ...) {
  string result = malloc(1024); //最长支持1023个字符的长度,目前足够了
  memset(result, 0, 1024);
  int i, j, k, l;
  j = 0;
  l = length(oldstr);
  k = length(newstr);
  i = pos(oldstr, s);
  while (i > 0) {
    strncpy_s(result + j, i, s, i - 1);
    s += (i - 1 + l);
    j += i - 1;
    strncpy_s(result + j, k + 1, newstr, k);
    j += k;
    i = pos(oldstr, s);
  }
  i = length(s);
  strncpy_s(result + j, i + 1, s, i);
  return result;
}

void showMessage(string s) {
  MessageBoxA(0, s, "SPUtils", MB_OK);
}

void upperCase(string s) {
  while (*(char*)s = toupper(*(char*)s)) {
    ++s;
  }
}

void lowerCase(string s) {
  while (*(char*)s = tolower(*(char*)s)) {
    ++s;
  }
}


char* trim(char* str)
{
  int i = 0;
  int j = 0;
  int len = strlen(str);
  //assert(str != NULL && len>0);
  for (i = 0; i<len;i++)
    {
      if (*(str + i) != ' ' && *(str + i) != '\t')
      {
        break;
      }
    }
  for (j = len - 1; j >= 0; j--)
  {
    if (*(str + j) != ' ' && *(str + j) != '\t')
    {
      break;
    }
  }
  *(str + j + 1) = '\0';
  return str + i;
}

//注意：TRUE会导致DLL引用计数加1，FALSE不会改变引用计数，通常用FALSE
THandle GetHInstance(BOOL bRef/* = FALSE*/)
{
  THandle hModule = NULL;
  if (GetModuleHandleEx(bRef ? GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS : (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
    | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT), (LPCSTR)GetHInstance, &hModule))
  {
    return hModule;
  }

  return NULL;
}

string getlocal(string s) {
  string path=malloc(256);
  memset(path, 0, 256);
  int l;
  THandle h = GetHInstance(FALSE);//GetModuleHandleA(NULL);
  if (h > 0) {
    GetModuleFileNameA(h, path, 256);
    path = extractFilePath(path);
    l = length(path);
    *(char*)(path + l ) = '\\';
    memcpy(path + l+1, s, length(s)+1);
    return path;
  }
  return ' ';
}

boolean FileExists(string afile)    //传入想要判断的路径字符串指针
{
  THandle fp;
  fp = fopen(afile, "r");   //"r"是只读模式，在这种模式下，如果文件存在，则能成功以只读模式打开，fopen返回一个非0的文件描述符，如果文件不存在，则fopen返回NULL
  if (fp == NULL) {
    return 0;   //不存在返回0
  }
  else
  {
    fclose(fp);  //存在的话，要先把之前打开的文件关掉
    return 1;    //然后返回1
  }
}

string extractFilePath(string s) {
  int i, l;
  l = length(s);
  for (i = l-1; i >= 0; i--) {
    if (*(char*)(s + i) == '\\') {
      *(char*)(s + i) = '\0';
      return s;
    }
  }
}

boolean IsInQuotation(int iPos, string s) {
  int i, l;
  boolean binquota;
  binquota = FALSE;
  i = pos("\"", s);
  l = i;
  while ((l > 0) && (l < iPos)) {
    if (binquota) {
      binquota = FALSE;
    }
    else {
      binquota = TRUE;
    }
    s = copy(s, i + 1, length(s) - i);
    i = pos("\"", s);
    l = l + i;
  }
  if ((binquota) && (l > iPos)) {
    return TRUE;
  }
  return FALSE;
}

void deleteComment(PStringList FScript) {
  int i, j, k, l;
  string s;
  l = FScript->FCount;
  for (i = l - 1; i >= 0; i--) {
    s = FScript->FList[i].FString;
    s = trim(s);
    if (strcmp(s,"")==0) {
      FScript->deleteIndex(FScript, i);
      continue;
    }
    j = pos("//", s);
    while (j > 0) {
      if (j == 1) {
        FScript->deleteIndex(FScript,i);
        j = -1;
        break;
      }
      else if (!IsInQuotation(j, s)) {
        s = copy(s, 1, j - 1);
        FScript->FList[i].FString = s;
      }
      j = pos("//", s);
    }
    if (j == -1) continue;
  }
}

void safefree(pointer ptr) {
#ifdef _MSC_VER  
  __try {
    *(char*)ptr = 'p';//0xC0000374无法捕获，用赋值来提前获取是否常量的异常
    free(ptr);
    ptr = NULL;
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    ;
  }
#elif defined(__GNUC__)
  //gcc无法捕获异常，不释放，记得自己处理内存释放,这里只把指针设置为NULL
  //free(ptr);
  ptr = NULL;
#endif
}

//PETools///////////////////////////////////
cardinal getModuleSize(THandle ahandle) {
  IMAGE_DOS_HEADER* imagedosheader;
  IMAGE_NT_HEADERS* imagentheaders;
  if (ahandle == 0) {
    return 0;
  }
  imagedosheader = (IMAGE_DOS_HEADER*)ahandle;
  if (imagedosheader->e_magic != IMAGE_DOS_SIGNATURE) {
    return 0;
  }
  imagentheaders = (IMAGE_NT_HEADERS*)(ahandle + imagedosheader->e_lfanew);
  if (imagentheaders->Signature != IMAGE_NT_SIGNATURE) {
    return 0;
  }
  return imagentheaders->OptionalHeader.SizeOfImage;
}


// Hide our module from PEB.
// http://www.openrce.org/blog/view/844/How_to_hide_dll

#define CUT_LIST(item) \
    item.Blink->Flink = item.Flink; \
    item.Flink->Blink = item.Blink

void hideModuleFromPEB(HMODULE module_handle)
{
  LDR_MODULE *modl; PEB *peb = get_peb();

  for (modl = (LDR_MODULE *)peb->LoaderData->InLoadOrderModuleList.Flink;
    modl->BaseAddress != NULL;
    modl = (LDR_MODULE *)modl->InLoadOrderModuleList.Flink) {

    if (modl->BaseAddress == module_handle) {
      CUT_LIST(modl->InLoadOrderModuleList);
      CUT_LIST(modl->InInitializationOrderModuleList);
      CUT_LIST(modl->InMemoryOrderModuleList);
      CUT_LIST(modl->HashTableEntry);

      memset(modl, 0, sizeof(LDR_MODULE));
      break;
    }
  }
}

void destroyPEHeader(HANDLE module_handle)
{
  DWORD old_protect;

  if (VirtualProtect(module_handle, 0x1000,
    PAGE_EXECUTE_READWRITE, &old_protect) != FALSE) {
    memset(module_handle, 0, 512);
    VirtualProtect(module_handle, 0x1000, old_protect, &old_protect);
  }
}

//TWinAPICall//////////////////////////////////////////////
//string systemDlls = { "ntdll",13,10 };//这样子语法错误，暂时不知道如何穿插byte赋值

pointer fngetApiPtr(PWinAPICall obj,string apiname) {
  uint i, l;
  THandle h;
  pointer p;
ag:
  l = obj->apiDlls->FCount;
  for (i = 0; i < l; i++) {
    h = LoadLibraryA(obj->apiDlls->FList[i].FString);
    p = GetProcAddress(h, apiname);
    if (p != NULL) {
      return p;
    }
  }
  if ((p == NULL)&&(!(*(char*)(apiname+ length(apiname) -1)=='A')||((*(char*)(apiname + length(apiname) - 1) == 'W')))) {
    apiname=strcat(apiname,"A");
    goto ag;
  }
  return 0;
}

PWinAPICall TWinAPICall_Create() {
  PWinAPICall result;
  result = malloc(sizeof(TWinAPICall));
  result->apiDlls = TStringList_Create();
  result->getApiPtr = fngetApiPtr;
  result->free = TWinAPICall_Free;
  result->apiDlls->setText(result->apiDlls, systemDlls);
  return result;
}
void TWinAPICall_Free(PWinAPICall obj) {
  safefree(obj);
}



//THashMap implementation///////////////////////////////////////////////////
nativeuint fnhashAddKV(PHashMap obj, uint64 key, uint64 value) {
  uint64 j, imod, ikey;
  imod = key mod obj->elementQt;
  //获取矩阵中的第一个元素
  ikey = (*(uint64*)(obj->pHashTable + imod));
  j = 0;
  while ((ikey>0)&&(ikey != key)) {
    j++;
    ikey = (*(uint64*)(obj->pHashTable + imod + obj->elementQt*j*(obj->elementSize + obj->indexSize)));
  }
  if (j >= obj->matrixWidth) {
    return -1; //越界，一般不可能
  }
  if (ikey == 0) {
    (*(uint64*)(obj->pHashTable + imod + obj->elementQt*j*(obj->elementSize + obj->indexSize))) = key;
lbchvalue:
    if (obj->indexSize == 2) {
      (*(word*)(obj->pHashTable + imod + obj->elementQt*j*(obj->elementSize + obj->indexSize) + obj->elementSize)) = value;
    }
    else if (obj->indexSize == 4) {
      (*(dword*)(obj->pHashTable + imod + obj->elementQt*j*(obj->elementSize + obj->indexSize) + obj->elementSize)) = value;
    }
    else if (obj->indexSize == 8) {
      (*(uint64*)(obj->pHashTable + imod + obj->elementQt*j*(obj->elementSize + obj->indexSize) + obj->elementSize)) = value;
    }
    return j;
  }
  else if(ikey==key){
    goto lbchvalue;
  }
  else {
    return -1; //插入失败
  }   
}

int64 fnhashValues(PHashMap obj, uint64 key) {
  uint64 j, imod, ikey,ivalue;
  imod = key mod obj->elementQt;
  //获取矩阵中的第一个元素
  ikey = (*(uint64*)(obj->pHashTable + imod));
  j = 0;
  while ((ikey>0) && (ikey != key)) {
    j++;
    ikey = (*(uint64*)(obj->pHashTable + imod + obj->elementQt*j*(obj->elementSize + obj->indexSize)));
  }
  if (ikey == key) {
    if (obj->indexSize == 2) {
      word ivl=(*(word*)(obj->pHashTable + imod + obj->elementQt*j*(obj->elementSize + obj->indexSize) + obj->elementSize));
      ivalue = ivl;
    }
    else if (obj->indexSize == 4) {
      dword ivl=(*(dword*)(obj->pHashTable + imod + obj->elementQt*j*(obj->elementSize + obj->indexSize) + obj->elementSize));
      ivalue = ivl;
    }
    else if (obj->indexSize == 8) {
      uint64 ivl=(*(uint64*)(obj->pHashTable + imod + obj->elementQt*j*(obj->elementSize + obj->indexSize) + obj->elementSize));
      ivalue = ivl;
    }
    return ivalue;
  }
  return -1;
}

PHashMap THashMap_Create(uint64 range, uint64 elementQt, uint64 elementSize)
{
  PHashMap result;
  uint64 isize, iqt;
  uint64 osize;
  result = (PHashMap)malloc(sizeof(THashMap));
  result->self = result;
  elementSize = 8; //目前elementSize全部为8，暂不支持自定义
  isize = elementSize; 
  if (isize == 0) 
    isize = 8;
  iqt = elementQt;
  if (iqt == 0) 
    elementQt = 1024;

  osize =  SPlog64(elementQt, range);
  result->matrixWidth = osize+2;
  result->elementQt = elementQt;
  result->elementSize = isize;
  //占用内存大小为(元素size+标号size)*元素个数*((log元素个数 取值范围大小)+2)
  //2是由于要增加超过对数次方的部分和小于1次方的部分
  if (iqt>255) {
    iqt = 2;
  }
  else if (iqt > 65535) {
    iqt = 4;
  }
  else if (iqt > 0xffffffff) {
    iqt = 8;
  }
  else {
    iqt = 2;
  }
  result->indexSize = iqt;
  //创建hash存放的矩阵,长宽高如下：
  /*
  width:(log elementQt range)  + 2
  height:elementQt
  length:elementSize+indexSize
  */
  result->pHashTable = malloc((isize + iqt)*elementQt*(osize+2));
  memset(result->pHashTable, 0, _msize(result->pHashTable));
  result->hashAddKV = fnhashAddKV;
  result->hashValues = fnhashValues;
  return result;
}

void THashMap_Free(PHashMap obj)
{
  safefree(obj->pHashTable);
  safefree(obj);
}



//TStringList implementation///////////////////////////////////////////////////
void fnAdd(PStringList obj, string s) {
  obj->FList[obj->FCount].FString = s;
  obj->FCount++;
}

//实际上这个就相当于非hash版的addObject
void fnTStringListAddKV(PStringList obj, nativeint k, nativeint v) {
  uint i;
  for (i = 0; i < obj->FCount; i++) {
    if (obj->FList[i].key == k) {
      obj->FList[i].value=v;
      return;
    }
  }
  obj->FList[obj->FCount].key = k;
  obj->FList[obj->FCount].value = v;
  obj->FCount++;
}

//这个相当于非hash版的indexOfKey
nativeint fnvalueByKey(PStringList obj, nativeint k) {
  int i;
  for (i = 0; i < obj->FCount; i++) {
    if (obj->FList[i].key == k) {
      return obj->FList[i].value;
    }
  }
  return -1;
}

void fnTStringListHashAddKV(PStringList obj, nativeint k, nativeint v) {
  int i = obj->hashMap->hashValues(obj->hashMap, k);
  if (i >= 0) {
    obj->FList[i].value=v;
  }
  else {
    obj->FList[obj->FCount].key = k;
    obj->FList[obj->FCount].value = v;
    //hashmap只存放标号，内容仍然使用FList,这样便于统一调度
    //之前的接口可以不用变更，对hash的结果同样有效
    obj->hashMap->hashAddKV(obj->hashMap, k, obj->FCount);
    obj->FCount++;
  }
}

void fnAddObject(PStringList obj, nativeint k, nativeint v) {
  fnTStringListHashAddKV(obj, k, v);
}

nativeint fnTStringListHashValueByKey(PStringList obj, nativeint k) {
  int i = obj->hashMap->hashValues(obj->hashMap, k);
  if (i >= 0) {
    return obj->FList[i].value;
  }
  else {
    return -1;
  }
}

nativeint fnindexOfKey(PStringList obj, nativeint k) {
  return obj->hashMap->hashValues(obj->hashMap, k);
}

nativeint fnobjects(PStringList obj, nativeint i) {
  return obj->FList[i].value;
}

nativeint  fnnames(PStringList obj, cardinal i) {
  return obj->FList[i].key;
}

nativeint  fnvalueFromIndex(PStringList obj, cardinal i) {
  return obj->FList[i].value;
}

nativeint  fnbetweenKeyValue(PStringList obj, nativeuint addr) {
  uint i,l;
  l = obj->FCount;
  for (i = 0; i < l; i++) {
    if ((addr > obj->names(obj, i)) && (addr < obj->valueFromIndex(obj,i))) {
      return 1;
    }
  }
  return 0;
}

/*
r	以只读方式打开文件，该文件必须存在。
r+	以读/写方式打开文件，该文件必须存在。
rb+	以读/写方式打开一个二进制文件，只允许读/写数据。
rt+	以读/写方式打开一个文本文件，允许读和写。
w	打开只写文件，若文件存在则长度清为 0，即该文件内容消失，若不存在则创建该文件。
w+	打开可读/写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。
a	以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留（EOF 符保留）。
a+	以附加方式打开可读/写的文件。若文件不存在，则会建立该文件，如果文件存在，则写入的数据会被加到文件尾后，即文件原先的内容会被保留（原来的 EOF 符不保留）。
wb	以只写方式打开或新建一个二进制文件，只允许写数据。
wb+	以读/写方式打开或建立一个二进制文件，允许读和写。
wt+	以读/写方式打开或建立一个文本文件，允许读写。
at+	以读/写方式打开一个文本文件，允许读或在文本末追加数据。
ab+	以读/写方式打开一个二进制文件，允许读或在文件末追加数据。
*/
nativeint fnloadFromFile(PStringList obj, pchar filePath) {
  freeArStr(&(obj->FArstr)); 
  obj->FHandle=fopen(filePath, "r+");
  if(obj->FHandle==NULL)
    obj->FHandle=fopen(filePath, "wt+");
  pchar txt = obj->getFileText(obj);
  obj->setText(obj, txt);
  /*
  int i,l;
  char psprt[2] = { 0xd,0xa };
  parstr p;
  p= split(txt, psprt, &l);//这里编译的非常诡异，一旦用cpp版本，就会增加一个cdqe指令造成错误
  obj->FArstr = p;
  obj->FCount = l;
  for (i = 0; i < l; i++) {
    obj->FList[i].FString = p->strings[i];//跟下面作用一样,只是这样写更易读
    //obj->FList[i].FString = *(string*)(p + i * sizeof(pointer));
  }
  */
}

nativeint fnsetText(PStringList obj, pchar txt) {
  int i, j,l;  //下面兼容一切换行符linux,windows,macos
  char sprt[8] = { 0xd,0xa,0,0xa,0,0,0xd,0 }; //这是c的特点，有0才算结尾，否则你会得到超长string
  pointer psprt = &(sprt[0]);
  j = 0;
  parstr p;
ag: //下面的split不能释放txt因为要多次调用，后面再释放
  p = split2(txt, psprt, &l);//这里编译的非常诡异，一旦用cpp版本，就会增加一个cdqe指令造成错误
  if ((l == 1)&&(j<3)) {
    j++;
    psprt+=3*j;
    goto ag;
  }
  obj->FArstr = p;
  obj->FCount = l;
  for (i = 0; i < l; i++) {
    obj->FList[i].FString = p->strings[i];//跟下面作用一样,只是这样写更易读
    //obj->FList[i].FString = *(string*)(p + i * sizeof(pointer));
  }
  safefree(txt);
}

nativeint fnsaveToFile(PStringList obj, pchar filePath) {
  THandle h;
  fopen_s(&h, filePath, "w+");;
  //rewind(h);
  int l = obj->FCount;
  string s;
  char ns[3] = {0xd,0xa};//使用windows换行符保存
  for (int i = 0; i < l; i++) {
    s = obj->FList[i].FString;
    if (s == NULL) continue;
    fwrite(s, 1, length(s), h);
    fwrite(ns, 1, 2, h);
  }
  fclose(h);
  return l;
}

nativeint fngetFileText(PStringList obj) {
  pchar result;
  fseek(obj->FHandle, 0, SEEK_END);//将文件内部的指针指向文件末尾
  long lsize = ftell(obj->FHandle);//获取文件长度，（得到文件位置指针当前位置相对于文件首的偏移字节数）
  rewind(obj->FHandle);//将文件内部的指针重新指向一个流的开头
  result = (char *)malloc(lsize * sizeof(char) + 1);//申请内存空间，lsize*sizeof(char)是为了更严谨，16位上char占一个字符，其他机器上可能变化
                                                   //用malloc申请的内存是没有初始值的，如果不赋值会导致写入的时候找不到结束标志符而出现内存比实际申请值大，写入数据后面跟随乱码的情况
  memset(result, 0, lsize * sizeof(char) + 1);//将内存空间都赋值为‘\0’
  fread(result, 1, lsize, obj->FHandle);//将pfile中内容读入pread指向内存中
  return result;
}

nativeint fntext(PStringList obj) {
  int l = obj->FCount;
  nativeuint k = obj->ftxt, j = 0;
  string s;
  char ns[2] = { 0xd,0xa };
  for (int i = 0; i < l; i++) {
    s = obj->FList[i].FString;
    if (s == NULL) continue;
    j = length(s);
    strncpy_s(k, j+1, s, j);//strncpy_s第2和4个参数刚好一样大是通不过的，因为它末尾还要增加一个0
    k += j;
    strncpy_s(k, 3, ns, 2);
    k += 2;
  }
  return obj->ftxt;
}

nativeint fnitems(PStringList obj,int i) {
  return obj->FList[i].FString;
}

nativeint fnindexOfName(PStringList obj,string key) {
  int i, l;
  
  for (i = 0; i < obj->FCount; i++) {
    if (obj->FList[i].FString == NULL) continue;
    if (pos(key, obj->FList[i].FString) == 1) {
      l = length(key);
lbfindeq:
      if ((char*)(obj->FList[i].FString + l ) == '=') {
        return ++i;
      }
      else if ((char*)(obj->FList[i].FString + l) == ' ') {
        l++;
        goto lbfindeq;
      }
      else {
        continue;
      }
    }
  }
  return -1;
}

nativeint fndeleteIndex(PStringList obj, uint index) {
  uint i, l;
  safefree(obj->FList[index].FString);
  l = obj->FCount;
  for (i = index; i < l-1; i++) {
    obj->FList[i].FString = obj->FList[i + 1].FString;
  }
  obj->FList[l-1].FString = NULL;
  obj->FArstr->strings[l-1] = NULL;
  obj->FCount--;
}

nativeint fnwriteToFile(PStringList obj, string s) {
  if (s == NULL) return -1;
  fseek(obj->FHandle, 0, SEEK_END);
  int l = length(s);
  fwrite(s, 1, l, obj->FHandle);
  return l;
}

nativeint fnclear(PStringList obj) {
  obj->FCount = 0;
}

nativeint fnwriteLineToFile(PStringList obj, string s) {
  if (s == NULL) return -1;
  fseek(obj->FHandle, 0, SEEK_END);
  int l = length(s);
  char ns[2] = { 0xd,0xa };
  fwrite(s, 1, l, obj->FHandle);
  fwrite(ns, 1, 2, obj->FHandle);
  return l;
}

PStringList TStringList_Create() {
  PStringList result;
  result = (PStringList)malloc(sizeof(TStringList));
  result->parent = NULL;
  result->self = result;
  result->FCount = 0;
  result->FArstr = NULL;
  result->ftxt = malloc(0x4000);
  result->add = fnAdd;
  result->addKV = fnTStringListAddKV;
  result->valueByKey = fnvalueByKey;
  result->hashAddKV = fnTStringListHashAddKV;
  result->hashValueByKey = fnTStringListHashValueByKey;
  result->loadFromFile = fnloadFromFile;
  result->saveToFile = fnsaveToFile;
  result->getFileText = fngetFileText;
  result->text = fntext;
  result->items = fnitems;
  result->writeLineToFile = fnwriteLineToFile;
  result->writeToFile = fnwriteToFile;
  result->indexOfName = fnindexOfName;
  result->clear = fnclear;
  result->hashMap = THashMap_Create(0xffffffffffffffff, 0, 0);
  result->free = TStringList_Free;
  result->objects = fnobjects;
  result->indexOfKey = fnindexOfKey;
  result->addObject = fnAddObject;
  result->betweenKeyValue = fnbetweenKeyValue;
  result->names = fnnames;
  result->valueFromIndex = fnvalueFromIndex;
  result->setText = fnsetText;
  result->deleteIndex = fndeleteIndex;
  return result;
}

void TStringList_Free(PStringList obj) {
  safefree(obj->ftxt);
  freeArStr(&(obj->FArstr)); //注意这里是传址，以便修改其值
  THashMap_Free(obj->hashMap);
  safefree(obj);
}