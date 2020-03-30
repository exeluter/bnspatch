using Gaffeine.Data.XmlSerializers;
using System;
using System.Collections;
using System.Xml.Serialization;

namespace Microsoft.Xml.Serialization.GeneratedAssembly
{
  public class XmlSerializerContract : XmlSerializerImplementation
  {
    static XmlSerializerContract()
    {
      Bootstrapper.Setup();
    }

    public override XmlSerializationReader Reader => null;

    public override XmlSerializationWriter Writer => null;

    public override Hashtable ReadMethods => null;

    public override Hashtable WriteMethods => null;

    public override Hashtable TypedSerializers => null;

    public override bool CanSerialize(Type type) => false;

    public override XmlSerializer GetSerializer(Type type) => null;
  }
}
