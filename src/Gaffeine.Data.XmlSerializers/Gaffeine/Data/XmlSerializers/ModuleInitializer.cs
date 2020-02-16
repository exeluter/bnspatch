using System;
using System.IO;
using System.IO.Compression;
using System.Reflection;

namespace Gaffeine.Data.XmlSerializers
{
  internal static class ModuleInitializer
  {
    internal static void Initialize()
    {
      AppDomain.CurrentDomain.AssemblyResolve += OnAssemblyResolve;

      foreach ( var to in typeof(Hooks).GetMethods(BindingFlags.Static | BindingFlags.Public) ) {
        var attributes = to.GetCustomAttributes<MonoModHookAttribute>();
        if ( attributes is null )
          continue;

        foreach ( var attribute in attributes )
          attribute.TryHook(to, out var hook);
      }
    }

    private static Assembly OnAssemblyResolve(object sender, ResolveEventArgs args)
    {
      var assemblyName = new AssemblyName(args.Name);
      var stream = Assembly.GetExecutingAssembly()
                           .GetManifestResourceStream(assemblyName.Name + ".dll.deflate");
      if ( stream != null ) {
        using ( var binaryReader = new BinaryReader(stream) )
        using ( var deflateStream = new DeflateStream(stream, CompressionMode.Decompress) )
        using ( var memoryStream = new MemoryStream(binaryReader.ReadInt32()) ) {
          deflateStream.CopyTo(memoryStream);
          return Assembly.Load(memoryStream.GetBuffer());
        }
      }
      return null;
    }
  }
}
