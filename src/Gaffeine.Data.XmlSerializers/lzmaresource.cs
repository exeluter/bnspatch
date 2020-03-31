using SevenZip.Compression.LZMA;
using System;
using System.IO;
using System.Reflection;

namespace Gaffeine.Data.XmlSerializers
{
  internal static class LzmaResource
  {
    internal static TResult Load<TResult>(string name, Func<MemoryStream, TResult> func)
    {
      var executingAssembly = Assembly.GetExecutingAssembly();
      var stream = executingAssembly.GetManifestResourceStream(name);
      if ( stream != null ) {
        using ( var binaryReader = new BinaryReader(stream) ) {
          var lzmaDecoder = new Decoder();
          lzmaDecoder.SetDecoderProperties(binaryReader.ReadBytes(5));
          long decompressedSize = binaryReader.ReadInt64();
          int capacity = decompressedSize > 0 ? Convert.ToInt32(decompressedSize) : 0;
          using ( var memoryStream = new MemoryStream(capacity) ) {
            lzmaDecoder.Code(stream, memoryStream, -1, decompressedSize, null);
            memoryStream.Seek(0, SeekOrigin.Begin);
            return func(memoryStream);
          }
        }
      }
      return default;
    }
  }
}
