using System.Reflection;
using System.Runtime.CompilerServices;
using System.Security;
using System.Xml.Serialization;

#if _DYNAMIC_XMLSERIALIZER_COMPILATION
[assembly: AllowPartiallyTrustedCallers()]
[assembly: SecurityTransparent()]
[assembly: SecurityRules(SecurityRuleSet.Level1)]
#endif
[assembly: AssemblyVersion("1.0.0.0")]
//[assembly: XmlSerializerVersion(ParentAssemblyId = "b9abbec6-9f94-4591-b883-558fa3d740ad,", Version = "4.0.0.0")] // NCW 2.2.0.510
//[assembly: XmlSerializerVersion(ParentAssemblyId = "1c8e5cf8-4044-4ad1-8b56-390330429d09,", Version = "4.0.0.0")] // NCW 2.2.0.517
//[assembly: XmlSerializerVersion(ParentAssemblyId = "94fc48a8-90ce-4863-be72-6ee7aebc0c42,", Version = "4.0.0.0")] // NCW 2.2.0.530
//[assembly: XmlSerializerVersion(ParentAssemblyId = "79216c71-7706-4ce0-a2b3-886909e7a009,", Version = "4.0.0.0")] // NCW 2.2.0.563
//[assembly: XmlSerializerVersion(ParentAssemblyId = "d69cbfbc-530a-4e08-9052-53a7b4ca26e1,", Version = "4.0.0.0")] // NCW 2.2.0.567
[assembly: XmlSerializerVersion(ParentAssemblyId = "567131ee-5c2a-48a6-b619-48590a1bc931,", Version = "4.0.0.0")]   // NCW 2.2.0.570

//[assembly: XmlSerializerVersion(ParentAssemblyId = "cb2fa5bc-9291-4935-9c15-47665636a644,", Version = "4.0.0.0")] // NCS 2.4.2.730
//[assembly: XmlSerializerVersion(ParentAssemblyId = "a232db4f-edf7-49ad-a81b-e8bd66b9c1d4,", Version = "4.0.0.0")] // NCS 2.4.2.733
//[assembly: XmlSerializerVersion(ParentAssemblyId = "50e05b5e-5e7a-4ee1-b836-276de9339613,", Version = "4.0.0.0")] // NCS 2.4.2.751
//[assembly: XmlSerializerVersion(ParentAssemblyId = "2453b8a9-962f-49f3-ad07-6897dff3f07a,", Version = "4.0.0.0")] // NCS 2.4.2.806
//[assembly: XmlSerializerVersion(ParentAssemblyId = "c0278585-2e9d-4e1c-a507-86f4193d747a,", Version = "4.0.0.0")] // NCS 2.4.2.811
//[assembly: XmlSerializerVersion(ParentAssemblyId = "d8747580-4e6e-4775-84a9-afc1017304bc,", Version = "4.0.0.0")] // NCS 2.4.2.839
[assembly: IgnoresAccessChecksTo("Gaffeine.Data")]
[assembly: IgnoresAccessChecksTo("GameUpdateService")]
