#pragma once
#include <stdint.h>

namespace v13
{
  class XmlReaderIO
  {
  public:
    enum ErrCode {};

    virtual enum ErrCode Open(wchar_t const *, wchar_t const *, bool) = 0;
    virtual enum ErrCode Read(unsigned char *, unsigned int *)const = 0;
    virtual unsigned int GetFileSize(void)const = 0;
    virtual wchar_t const *GetFileName(void)const = 0;
    virtual enum ErrCode Next(void) = 0;
    virtual void Close(void) = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlReaderIO) == 0x8);
#else
  static_assert(sizeof(XmlReaderIO) == 0x4);
#endif

  class XmlReaderLog
  {
  public:
    virtual void Error(wchar_t const *, ...)const = 0;
    virtual void Debug(wchar_t const *, ...)const = 0;
    virtual void Trace(wchar_t const *, ...)const = 0;
  }; /* size = 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlReaderLog) == 0x8);
#else
  static_assert(sizeof(XmlReaderLog) == 0x4);
#endif

  class XmlElement
  {
  public:
    virtual int ChildElementCount(void)const = 0;
    virtual class XmlElement const *FirstChildElement(void)const = 0;
    virtual class XmlElement const *NextElement(void)const = 0;
    virtual wchar_t const *Name(void)const = 0;
    virtual long LineNumber(void)const = 0;
    virtual int AttributeCount(void)const = 0;
    virtual wchar_t const *Attribute(unsigned int, wchar_t const *)const = 0;
    virtual wchar_t const *Attribute(int)const = 0;
    virtual wchar_t const *Attribute(wchar_t const *)const = 0;
    virtual wchar_t const *AttributeName(int)const = 0;
    virtual int AttributeIndex(wchar_t const *)const = 0;
    virtual class XmlNode const *ToXmlNode(void)const = 0;
    virtual class XmlNode *ToXmlNode(void) = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlElement) == 0x8);
#else
  static_assert(sizeof(XmlElement) == 0x4);
#endif

  class XmlDoc
  {
  public:
    virtual bool IsValid(void) const = 0;
    virtual wchar_t const *Name(void) const = 0;
    virtual class XmlElement *Root(void) = 0;
    virtual int BinarySize(void) const = 0;
    virtual void SerializeTo(char *, int) const = 0;
    virtual void SerializeFrom(char *, int) = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlDoc) == 0x8);
#else
  static_assert(sizeof(XmlDoc) == 0x4);
#endif

  class XmlReader
  {
  public:
    virtual bool Initialize(class XmlReaderIO *, class XmlReaderLog const *, bool)const = 0;
    virtual class XmlReaderLog const *SetLog(class XmlReaderLog const *)const = 0;
    virtual class XmlReaderLog const *GetLog(void)const = 0;
    virtual void Cleanup(bool)const = 0;
    virtual class XmlReaderIO *GetIO(void)const = 0;
    virtual bool Read(wchar_t const *, class _XmlSaxHandler &)const = 0;
    virtual class XmlDoc *Read(wchar_t const *)const = 0;
    virtual class XmlDoc *Read(unsigned char const *, unsigned int, wchar_t const *)const = 0;
    virtual void Close(class XmlDoc *)const = 0;
    virtual class XmlDoc *NewDoc(void)const = 0;
    virtual bool IsBinary(wchar_t const *)const = 0;
    virtual bool IsBinary(unsigned char const *, unsigned int)const = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlReader) == 0x8);
#else
  static_assert(sizeof(XmlReader) == 0x4);
#endif

  struct XmlCfgDef
  {
    /* 0x0000 */ size_t offset;
    /* 0x0008 */ const wchar_t *name;
    /* 0x0010 */ int32_t type;
    /* 0x0014 */ int32_t reqOption;
    /* 0x0018 */ const void *defaultValue;
    /* 0x0020 */ const wchar_t *desc;
  }; /* size: 0x0028 */
#ifdef _M_X64
  static_assert(sizeof(XmlCfgDef) == 0x28);
#else
  static_assert(sizeof(XmlCfgDef) == 0x18);
#endif

  struct XmlCfgDef2
  {
    /* 0x0000 */ size_t offset;
    /* 0x0008 */ const wchar_t *name;
    /* 0x0010 */ int32_t type;
    /* 0x0014 */ int32_t reqOption;
    /* 0x0018 */ const void *defaultValue;
    /* 0x0020 */ const wchar_t *desc;
    /* 0x0028 */ const wchar_t *derivedFrom;
    /* 0x0030 */ int32_t firstChildIndex;
    /* 0x0034 */ int32_t nextSiblingIndex;
  }; /* size: 0x0038 */
#ifdef _M_X64
  static_assert(sizeof(XmlCfgDef2) == 0x38);
#else
  static_assert(sizeof(XmlCfgDef2) == 0x24);
#endif

  class XmlConfigReader
  {
  public:
    virtual bool Initialize(struct XmlCfgDef2 const *, int, void *(*)(size_t), size_t)const = 0;
    virtual bool Initialize(struct XmlCfgDef const *, int, void *(*)(size_t), size_t)const = 0;
    virtual void Cleanup(void)const = 0;
    virtual void *Read(size_t &, unsigned char *, unsigned int, wchar_t const *, wchar_t const *, bool, int, int, bool, bool)const = 0;
    virtual void *Read(size_t &, wchar_t const *, wchar_t const *, bool, int, int, bool, bool)const = 0;
    virtual void *Read(wchar_t const *, wchar_t const *, bool)const = 0;
    virtual wchar_t const *GetLastErrorString(void)const = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlConfigReader) == 0x8);
#else
  static_assert(sizeof(XmlConfigReader) == 0x4);
#endif

  class XmlNode
  {
  public:
    enum TYPE
    {
      XML_NONE = 0,
      XML_ELEMENT = 1,
      XML_TEXT = 2,
    };

    virtual enum XmlNode::TYPE Type(void)const = 0;
    virtual bool IsValid(void)const = 0;
    virtual wchar_t const *Name(void)const = 0;
    virtual class XmlDoc const *GetDoc(void)const = 0;
    virtual class XmlNode const *Parent(void)const = 0;
    virtual int ChildCount(void)const = 0;
    virtual class XmlNode const *FirstChild(void)const = 0;
    virtual class XmlNode const *Child(int)const = 0;
    virtual class XmlNode const *Next(void)const = 0;
    virtual long LineNumber(void)const = 0;
    virtual wchar_t const *GetURI(void)const = 0;
    virtual int MemSize(void)const = 0;
    virtual int Clone(char *, int)const = 0;
    virtual class XmlNode *CloneNode(char *, int)const = 0;
    virtual int BinarySize(void)const = 0;
    virtual void SerializeTo(char *&, int &)const = 0;
    virtual void SerializeFrom(char *&, int &) = 0;
    virtual class XmlElement const *ToXmlElement(void)const = 0;
    virtual class XmlElement *ToXmlElement(void) = 0;
    virtual class XmlTextNode const *ToXmlTextNode(void)const = 0;
    virtual class XmlTextNode *ToXmlTextNode(void) = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlNode) == 0x8);
#else
  static_assert(sizeof(XmlNode) == 0x4);
#endif

  class XmlTextNode
  {
  public:
    virtual wchar_t const *Value(void)const = 0;
    virtual class XmlNode const *ToXmlNode(void)const = 0;
    virtual class XmlNode *ToXmlNode(void) = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlTextNode) == 0x8);
#else
  static_assert(sizeof(XmlTextNode) == 0x4);
#endif
}

namespace v14
{
  using v13::XmlReaderIO;
  using v13::XmlReaderLog;
  using v13::XmlElement;
  using v13::XmlDoc;
  using v13::XmlCfgDef;
  using v13::XmlCfgDef2;
  using v13::XmlConfigReader;
  using v13::XmlNode;
  using v13::XmlTextNode;

  class XmlPieceReader
  {
  public:
    virtual bool Read(class XmlDoc *) = 0;
    virtual int GetMaxNodeCountPerPiece(void) = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlPieceReader) == 0x8);
#else
  static_assert(sizeof(XmlPieceReader) == 0x4);
#endif

  class XmlReader
  {
  public:
    virtual bool Initialize(class XmlReaderIO *, class XmlReaderLog const *, bool)const = 0;
    virtual class XmlReaderLog const *SetLog(class XmlReaderLog const *)const = 0;
    virtual class XmlReaderLog const *GetLog(void)const = 0;
    virtual void Cleanup(bool)const = 0;
    virtual class XmlReaderIO *GetIO(void)const = 0;
    virtual bool Read(wchar_t const *, class _XmlSaxHandler &)const = 0;
    virtual class XmlDoc *Read(wchar_t const *, class XmlPieceReader *)const = 0;
    virtual class XmlDoc *Read(unsigned char const *, unsigned int, wchar_t const *, class XmlPieceReader *)const = 0;
    virtual void Close(class XmlDoc *)const = 0;
    virtual class XmlDoc *NewDoc(void)const = 0;
    virtual bool IsBinary(wchar_t const *)const = 0;
    virtual bool IsBinary(unsigned char const *, unsigned int)const = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlReader) == 0x8);
#else
  static_assert(sizeof(XmlReader) == 0x4);
#endif
}

namespace v15
{
  // unknown difference between v14
  using v14::XmlReaderIO;
  using v14::XmlReaderLog;
  using v14::XmlElement;
  using v14::XmlDoc;
  using v14::XmlPieceReader;
  using v14::XmlReader;
  using v14::XmlCfgDef;
  using v14::XmlCfgDef2;
  using v14::XmlConfigReader;
  using v14::XmlNode;
  using v14::XmlTextNode;
}
