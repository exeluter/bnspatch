using System.Reflection;
using System.Runtime.CompilerServices;
using System.Security;
using System.Xml.Serialization;
using Gaffeine.Data.XmlSerializers;

#if _DYNAMIC_XMLSERIALIZER_COMPILATION
[assembly: AllowPartiallyTrustedCallers()]
[assembly: SecurityTransparent()]
[assembly: SecurityRules(SecurityRuleSet.Level1)]
#endif
[assembly: AssemblyVersion("1.0.0.0")]
[assembly: XmlSerializerVersion(ParentAssemblyId = "567131ee-5c2a-48a6-b619-48590a1bc931,", Version = "4.0.0.0")]
[assembly: IgnoresAccessChecksTo("Gaffeine.Data")]
