using Gaffeine.Data.Models;
using GameUpdateService.Updaters.US4Updater.US4UpdateMode;
using MonoMod.RuntimeDetour.HookGen;
using SevenZip.Compression.LZMA;
using System;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Windows;

namespace Gaffeine.Data.XmlSerializers
{
  internal static class ModuleInitializer
  {
    internal static void Initialize()
    {
      HookEndpointManager.Add(
        typeof(US4UpdateModeBase).GetMethod("FileReplace", BindingFlags.Instance | BindingFlags.NonPublic),
        (Action<Action<US4UpdateModeBase, string, string>, US4UpdateModeBase, string, string>)Hooks.FileReplace);

      HookEndpointManager.Add(
        Type.GetType("Microsoft.ApplicationInsights.Extensibility.TelemetryConfiguration, Microsoft.ApplicationInsights")
            .GetProperty("DisableTelemetry", BindingFlags.Instance | BindingFlags.Public).GetGetMethod(),
        (Func<Func<object, bool>, object, bool>)Hooks.get_DisableTelemetry);

      HookEndpointManager.Add(
        typeof(GameInfo).GetProperty("ExeArgument", BindingFlags.Instance | BindingFlags.Public).GetGetMethod(),
        (Func<Func<GameInfo, string>, GameInfo, string>)Hooks.get_ExeArgument);

      HookEndpointManager.Add(
        typeof(Game).GetProperty("AllowMultiClient", BindingFlags.Instance | BindingFlags.Public).GetGetMethod(),
        (Func<Func<Game, string>, Game, string>)Hooks.get_AllowMultiClient);

      HookEndpointManager.Add(
        Type.GetType("Gaffeine.Controls.Helpers.ShortcutHelper, Gaffeine.Controls")
            .GetMethod("MakeGameShortcut", BindingFlags.Static | BindingFlags.Public),
        (Func<Func<Game, bool>, Game, bool>)Hooks.MakeGameShortcut);

      HookEndpointManager.Add(
        Type.GetType("Gaffeine.Controls.Helpers.ShortcutHelper, Gaffeine.Controls")
            .GetMethod("MakeGameShortcut", BindingFlags.Static | BindingFlags.Public),
        (Func<Func<Game, bool>, Game, bool>)Hooks.MakeGameShortcut);

      HookEndpointManager.Add(
        typeof(LanguagePackageFiles).GetMethod("Exists", BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Public),
        (Func<Func<LanguagePackageFiles, string, bool>, LanguagePackageFiles, string, bool>)Hooks.Exists);

      HookEndpointManager.Add(
        Type.GetType("NCLauncherW.Views.SignInWindow, NCLauncher2")
            .GetMethods(BindingFlags.Instance | BindingFlags.NonPublic)
            .Single(x => x.ReturnType == typeof(void)
                         && x.GetParameters()
                             .Select(y => y.ParameterType)
                             .SequenceEqual(new[] { typeof(UIElement), typeof(bool) })),
        (Action<Action<object, UIElement, bool>, object, UIElement, bool>)Hooks.ca8de357b76a2339a41ee639eb04cc454);
    }

    internal static Assembly OnAssemblyResolve(object sender, ResolveEventArgs args)
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
