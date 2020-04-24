#pragma once
#include <stdint.h>

namespace v13
{
  class XmlReaderIO
  {
  public:
    enum ErrCode
    {
      ERR_NO_ERROR,
      ERR_UNKNOWN,
      ERR_SYSTEM,
      ERR_NO_MORE_FILE,
      ERR_NOT_IMPLEMENTED,
      ERR_INVALID_PARAM,
      ERR_INSUFFICIENT_BUFFER
    };

    virtual enum ErrCode Open(wchar_t const *path, wchar_t const *xml, bool recursive) = 0;
    virtual enum ErrCode Read(unsigned char *buf, unsigned int *bufsize) const = 0;
    virtual unsigned int GetFileSize() const = 0;
    virtual wchar_t const *GetFileName() const = 0;
    virtual enum ErrCode Next() = 0;
    virtual void Close() = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlReaderIO) == 0x8);
#else
  static_assert(sizeof(XmlReaderIO) == 0x4);
#endif

  class XmlReaderLog
  {
  public:
    virtual void Error(wchar_t const *format, ...) const = 0;
    virtual void Debug(wchar_t const *format, ...) const = 0;
    virtual void Trace(wchar_t const *format, ...) const = 0;
  }; /* size = 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlReaderLog) == 0x8);
#else
  static_assert(sizeof(XmlReaderLog) == 0x4);
#endif

  class XmlElement
  {
  public:
    virtual int ChildElementCount() const = 0;
    virtual class XmlElement const *FirstChildElement() const = 0;
    virtual class XmlElement const *NextElement() const = 0;
    virtual wchar_t const *Name() const = 0;
    virtual long LineNumber() const = 0;
    virtual int AttributeCount() const = 0;
    virtual wchar_t const *Attribute(unsigned int nameHash, wchar_t const *name) const = 0;
    virtual wchar_t const *Attribute(int index) const = 0;
    virtual wchar_t const *Attribute(wchar_t const *name) const = 0;
    virtual wchar_t const *AttributeName(int index) const = 0;
    virtual int AttributeIndex(wchar_t const *name) const = 0;
    virtual class XmlNode const *ToXmlNode() const = 0;
    virtual class XmlNode *ToXmlNode() = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlElement) == 0x8);
#else
  static_assert(sizeof(XmlElement) == 0x4);
#endif

  class XmlDoc
  {
  public:
    virtual bool IsValid() const = 0;
    virtual wchar_t const *Name() const = 0;
    virtual class XmlElement *Root() = 0;
    virtual int BinarySize() const = 0;
    virtual void SerializeTo(char *buf, int size) const = 0;
    virtual void SerializeFrom(char *buf, int size) = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlDoc) == 0x8);
#else
  static_assert(sizeof(XmlDoc) == 0x4);
#endif

  class XmlReader
  {
  public:
    virtual bool Initialize(class XmlReaderIO *io, class XmlReaderLog const *log, bool useExpat) const = 0;
    virtual class XmlReaderLog const *SetLog(class XmlReaderLog const *log) const = 0;
    virtual class XmlReaderLog const *GetLog() const = 0;
    virtual void Cleanup(bool clearMemory) const = 0;
    virtual class XmlReaderIO *GetIO() const = 0;
    virtual bool Read(wchar_t const *xml, class _XmlSaxHandler &handler) const = 0;
    virtual class XmlDoc *Read(wchar_t const *xml) const = 0;
    virtual class XmlDoc *Read(unsigned char const *mem, unsigned int size, wchar_t const *xmlFileNameForLogging) const = 0;
    virtual void Close(class XmlDoc *doc) const = 0;
    virtual class XmlDoc *NewDoc() const = 0;
    virtual bool IsBinary(wchar_t const *xml) const = 0;
    virtual bool IsBinary(unsigned char const *mem, unsigned int size) const = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlReader) == 0x8);
#else
  static_assert(sizeof(XmlReader) == 0x4);
#endif

  struct XmlCfgDef
  {
    enum
    {
      CFG_NONE,
      CFG_INT,
      CFG_INT64,
      CFG_FLOAT,
      CFG_BOOL,
      CFG_IP,
      CFG_STR,
      CFG_DIR,
      CFG_ANSI,
      CFG_GROUP,
      CFG_TYPE_MAX
    };
    enum
    {
      CFG_OPTIONAL,
      CFG_REQUIRED,
      CFG_DERIVED,
      CFG_DEV
    };

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
    enum
    {
      CFG_NONE,
      CFG_INT,
      CFG_INT64,
      CFG_FLOAT,
      CFG_BOOL,
      CFG_IP,
      CFG_STR,
      CFG_DIR,
      CFG_ANSI,
      CFG_GROUP,
      CFG_TYPE_MAX
    };
    enum
    {
      CFG_OPTIONAL,
      CFG_REQUIRED,
      CFG_DERIVED,
      CFG_DEV
    };

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
    virtual bool Initialize(struct XmlCfgDef2 const *def2, int cnt, void *(*allocFunc)(size_t), size_t structSize) const = 0;
    virtual bool Initialize(struct XmlCfgDef const *def, int cnt, void *(*allocFunc)(size_t), size_t structSize) const = 0;
    virtual void Cleanup() const = 0;
    virtual void *Read(size_t &bufferSize, unsigned char *configXmlString, unsigned int configXmlStringSize, wchar_t const *filename, wchar_t const *cfgname, bool versionVerification, int configDefinitionMajorVersion, int configDefinitionMinorVersion, bool loadDefaultForUnspecified, bool readOnly) const = 0;
    virtual void *Read(size_t &bufferSize, wchar_t const *filename, wchar_t const *cfgname, bool versionVerification, int configDefinitionMajorVersion, int configDefinitionMinorVersion, bool loadDefaultForUnspecified, bool readOnly) const = 0;
    virtual void *Read(wchar_t const *filename, wchar_t const *cfgname, bool readOnly) const = 0;
    virtual wchar_t const *GetLastErrorString() const = 0;
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
      XML_NONE,
      XML_ELEMENT,
      XML_TEXT,
    };

    virtual enum TYPE Type() const = 0;
    virtual bool IsValid() const = 0;
    virtual wchar_t const *Name() const = 0;
    virtual class XmlDoc const *GetDoc() const = 0;
    virtual class XmlNode const *Parent() const = 0;
    virtual int ChildCount() const = 0;
    virtual class XmlNode const *FirstChild() const = 0;
    virtual class XmlNode const *Child(int) const = 0;
    virtual class XmlNode const *Next() const = 0;
    virtual long LineNumber() const = 0;
    virtual wchar_t const *GetURI() const = 0;
    virtual int MemSize() const = 0;
    virtual int Clone(char *buf, int size) const = 0;
    virtual class XmlNode *CloneNode(char *buf, int size) const = 0;
    virtual int BinarySize() const = 0;
    virtual void SerializeTo(char *&buf, int &size) const = 0;
    virtual void SerializeFrom(char *&buf, int &size) = 0;
    virtual class XmlElement const *ToXmlElement() const = 0;
    virtual class XmlElement *ToXmlElement() = 0;
    virtual class XmlTextNode const *ToXmlTextNode() const = 0;
    virtual class XmlTextNode *ToXmlTextNode() = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlNode) == 0x8);
#else
  static_assert(sizeof(XmlNode) == 0x4);
#endif

  class XmlTextNode
  {
  public:
    virtual wchar_t const *Value() const = 0;
    virtual class XmlNode const *ToXmlNode() const = 0;
    virtual class XmlNode *ToXmlNode() = 0;
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

  class XmlPieceReader
  {
  public:
    virtual bool Read(class XmlDoc *) = 0;
    virtual int GetMaxNodeCountPerPiece() = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlPieceReader) == 0x8);
#else
  static_assert(sizeof(XmlPieceReader) == 0x4);
#endif

  class XmlReader
  {
  public:
    virtual bool Initialize(class XmlReaderIO *io, class XmlReaderLog const *log, bool useExpat) const = 0;
    virtual class XmlReaderLog const *SetLog(class XmlReaderLog const *) const = 0;
    virtual class XmlReaderLog const *GetLog() const = 0;
    virtual void Cleanup(bool) const = 0;
    virtual class XmlReaderIO *GetIO() const = 0;
    virtual bool Read(wchar_t const *xml, class _XmlSaxHandler &handler) const = 0;
    virtual class XmlDoc *Read(wchar_t const *xml, class XmlPieceReader *xmlPieceReader) const = 0;
    virtual class XmlDoc *Read(unsigned char const *mem, unsigned int size, wchar_t const *xmlFileNameForLogging, class XmlPieceReader *xmlPieceReader) const = 0;
    virtual void Close(class XmlDoc *doc) const = 0;
    virtual class XmlDoc *NewDoc() const = 0;
    virtual bool IsBinary(wchar_t const *xml) const = 0;
    virtual bool IsBinary(unsigned char const *mem, unsigned int size) const = 0;
  }; /* size: 0x0008 */
#ifdef _M_X64
  static_assert(sizeof(XmlReader) == 0x8);
#else
  static_assert(sizeof(XmlReader) == 0x4);
#endif

  using v13::XmlCfgDef;
  using v13::XmlCfgDef2;
  using v13::XmlConfigReader;
  using v13::XmlNode;
  using v13::XmlTextNode;
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
