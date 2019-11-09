using Gaffeine.Data;
using Gaffeine.Data.Models;
using MonoMod.RuntimeDetour;
using System;
using System.Collections;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Windows;
using System.Xml.Serialization;
using MoreLinq;
using Microsoft.CSharp.RuntimeBinder;

namespace Microsoft.Xml.Serialization.GeneratedAssembly
{
  public class XmlSerializerContract : XmlSerializerImplementation
  {

    static XmlSerializerContract()
    {
      AppDomain.CurrentDomain.AssemblyLoad += (sender, args) =>
        NativeMethods.OutputDebugString("Assembly loaded: " + args.LoadedAssembly.FullName);
      AppDomain.CurrentDomain.AssemblyResolve += OnAssemblyResolve;

      RegisterRuntimeDetours();
    }

    public override XmlSerializationReader Reader => null;
    public override XmlSerializationWriter Writer => null;
    public override Hashtable ReadMethods => null;
    public override Hashtable WriteMethods => null;
    public override Hashtable TypedSerializers => null;
    public override bool CanSerialize(Type type) => false;
    public override XmlSerializer GetSerializer(Type type) => null;

    private static void RegisterRuntimeDetours()
    {
      // Disable Application Insights telemetry
      // Important note: Exactly one event will be tracked before this hook is able to disable it,
      // which is just "NCLauncher2 Startup". To completely disable telemetry you need to overwrite
      // ApplicationInsights.config with the one in the Resources folder. 
      _ = new Hook(Type.GetType("Microsoft.ApplicationInsights.Extensibility.TelemetryConfiguration, Microsoft.ApplicationInsights")
                       .GetProperty("DisableTelemetry")
                       .GetGetMethod(),
        new Func<Func<object, bool>, object, bool>((_, @this) => true));

      // Forward command line arguments that probably aren't for NC Launcher 2, to the game specified by 
      // the GameID launcher argument. 
      _ = new Hook(typeof(GameInfo).GetProperty(nameof(GameInfo.ExeArgument))
                                   .GetGetMethod(),
        new Func<Func<GameInfo, string>, GameInfo, string>((fn, @this) => {
          try {
            if ( @this.GameId.Equals(AppArgs.Instance.GameId, StringComparison.OrdinalIgnoreCase) )
              return string.Join(" ", Environment.GetCommandLineArgs()
                                                 .Skip(1)
                                                 .Where(x => !string.IsNullOrEmpty(x)
                                                             && !x.StartsWith("nc-launcher2://")
                                                             && !x.StartsWith("nc-launcher2beta://")
                                                             && !(x.StartsWith("/") && x.Contains(":")))
                                                 .Prepend(fn(@this)));
          } catch ( RuntimeBinderException ) {
          }
          return fn(@this);
        }));

      // Don't overwrite the game shortcut if it already exists.
      _ = new Hook(Type.GetType("Gaffeine.Controls.Helpers.ShortcutHelper, Gaffeine.Controls")
                       .GetMethod("MakeGameShortcut"),
        new Func<Func<Game, bool>, Game, bool>((fn, game) =>
          File.Exists(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory), game.GameName + ".lnk"))
          || fn(game)));

      // Fixes bug where unnecessary localization files are downloaded. The funny thing is, it seems
      // like they intended for these files to be skipped, but the code was bugged from the start and it
      // has never been fixed.
      _ = new Hook(typeof(LanguagePackageFiles).GetMethod(nameof(LanguagePackageFiles.Exists), new[] { typeof(string) }),
        new Func<Func<LanguagePackageFiles, string, bool>, LanguagePackageFiles, string, bool>((_, @this, fileName) =>
          !string.IsNullOrEmpty(fileName) && @this.Exists(x => fileName.Contains(x.FileName, StringComparison.OrdinalIgnoreCase))));

      // Turn off encryption of log messages
      Type.GetType("NCLog.Logger, NCLog")
          .GetProperty("UseEncryption")
          .SetValue(null, false);

      if ( Assembly.GetEntryAssembly().GetSimpleName() == "NCLauncher2" ) {
        // Fixes annoying behavior when you click on the password field without filling in the email address field,
        // it will be focused instead of the password field (and vice versa). 
        var from = Type.GetType("NCLauncherW.Views.SignInWindow, NCLauncher2")
                       .GetMethods(BindingFlags.Instance | BindingFlags.NonPublic)
                       .FirstOrDefault(x => x.ReturnType == typeof(void)
                                            && x.GetParameters()
                                                .Select(y => y.ParameterType)
                                                .SequenceEqual(new[] { typeof(UIElement), typeof(bool) }));
        if ( from != null ) {
          _ = new Hook(from,
            new Action<Action<object, UIElement, bool>, object, UIElement, bool>((_, @this, arg1, arg2) => { }));
        };
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
          var assembly = Assembly.Load(memoryStream.GetBuffer());
          if ( assembly != null )
            return assembly;
        }
      }
      NativeMethods.OutputDebugString("Assembly not resolved: " + args.Name);
      return null;
    }
    private class AppArgs
    {
      public static readonly dynamic Instance;

      static AppArgs()
      {
        // Super hack-ish, but whatever.
        string typeName = null;
        switch ( Assembly.GetEntryAssembly().GetSimpleName() ) {
        case "NCLauncher2":
          typeName = "NCLauncherW.AppArgs, NCLauncher2";
          break;
        case "SelfUpdater":
          typeName = "SelfUpdater.Settings.AppArgs, SelfUpdater";
          break;
        }
        if ( typeName != null ) {
          Instance = Type.GetType(typeName)?
                         .GetProperty("Instance")?
                         .GetGetMethod()
                         .Invoke(null, null);
        }
      }
    }

    private static class NativeMethods
    {
      [DllImport("kernel32.dll")]
      internal static extern void OutputDebugString(string lpOutputString);
    }
  }
}
