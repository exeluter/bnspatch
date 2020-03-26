using MonoMod.RuntimeDetour;
using SevenZip.Compression.LZMA;
using System;
using System.IO;
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

    static Assembly OnAssemblyResolve(object sender, ResolveEventArgs args)
    {
      var assemblyName = new AssemblyName(args.Name);
      var executingAssembly = Assembly.GetExecutingAssembly();
      var stream = executingAssembly.GetManifestResourceStream(assemblyName.Name + ".dll.lzma");
      if ( stream != null ) {
        using ( var binaryReader = new BinaryReader(stream) ) {
          var lzmaDecoder = new Decoder();
          lzmaDecoder.SetDecoderProperties(binaryReader.ReadBytes(5));
          ulong decompressedSize = binaryReader.ReadUInt64();
          long compressedSize = stream.Length - stream.Position;
          int capacity = decompressedSize != ulong.MaxValue ? Convert.ToInt32(decompressedSize) : 0;
          using ( var memoryStream = new MemoryStream(capacity) ) {
            lzmaDecoder.Code(stream, memoryStream, compressedSize, Convert.ToInt64(decompressedSize), null);
            return Assembly.Load(memoryStream.GetBuffer());
          }
        }
      }
      return null;
    }
  }
}
