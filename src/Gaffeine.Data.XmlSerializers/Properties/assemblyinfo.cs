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
[assembly: XmlSerializerVersion(ParentAssemblyId = "567131ee-5c2a-48a6-b619-48590a1bc931,", Version = "4.0.0.0")]
[assembly: IgnoresAccessChecksTo("Gaffeine.Data")]
[assembly: IgnoresAccessChecksTo("GameUpdateService")]

// NCW 2.2.0.510: b9abbec6-9f94-4591-b883-558fa3d740ad, 
// NCW 2.2.0.517: 1c8e5cf8-4044-4ad1-8b56-390330429d09, 
// NCW 2.2.0.530: 94fc48a8-90ce-4863-be72-6ee7aebc0c42, 
// NCW 2.2.0.563: 79216c71-7706-4ce0-a2b3-886909e7a009, 
// NCW 2.2.0.567: d69cbfbc-530a-4e08-9052-53a7b4ca26e1,
// NCW 2.2.0.570: 567131ee-5c2a-48a6-b619-48590a1bc931,

// NCS 2.4.2.730: cb2fa5bc-9291-4935-9c15-47665636a644,
// NCS 2.4.2.733: a232db4f-edf7-49ad-a81b-e8bd66b9c1d4,
// NCS 2.4.2.751: 50e05b5e-5e7a-4ee1-b836-276de9339613,
// NCS 2.4.2.806: 2453b8a9-962f-49f3-ad07-6897dff3f07a,
// NCS 2.4.2.811: c0278585-2e9d-4e1c-a507-86f4193d747a,
// NCS 2.4.2.839: d8747580-4e6e-4775-84a9-afc1017304bc,
// NCS 2.4.2.840: 26c05023-30cd-4b2b-9b7a-6d8de7c67fc1,