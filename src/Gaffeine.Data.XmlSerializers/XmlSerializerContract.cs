using Gaffeine.Data;
using Gaffeine.Data.Models;
using Microsoft.ApplicationInsights.Extensibility;
using MonoMod.RuntimeDetour;
using NCLauncherW;
using NCLauncherW.Views;
using NCLog;
using System;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Windows;
using static MoreLinq.Extensions.PrependExtension;

namespace Microsoft.Xml.Serialization.GeneratedAssembly
{
  public partial class XmlSerializerContract : global::System.Xml.Serialization.XmlSerializerImplementation
  {
    [DllImport("kernel32.dll")]
    static extern void OutputDebugString(string lpOutputString);

    static XmlSerializerContract()
    {
      AppDomain.CurrentDomain.AssemblyLoad += (sender, args) =>
        OutputDebugString("Assembly loaded: " + args.LoadedAssembly.FullName);

      AppDomain.CurrentDomain.AssemblyResolve += OnAssemblyResolve;
      RegisterRuntimeDetours();
    }

    static void RegisterRuntimeDetours()
    {
      /* Disable Application Insights telemetry
       * Important note: Exactly one event will be tracked before this hook is able to disable it,
       * which is just "NCLauncher2 Startup". To completely disable telemetry you need to overwrite
       * ApplicationInsights.config with this:
     
         <?xml version="1.0" encoding="utf-8"?>
         <ApplicationInsights xmlns="http://schemas.microsoft.com/ApplicationInsights/2013/Settings">
           <DisableTelemetry>true</DisableTelemetry>
         </ApplicationInsights>
      */
      _ = new Hook(typeof(TelemetryConfiguration).GetProperty(nameof(TelemetryConfiguration.DisableTelemetry))
                                                 .GetGetMethod(),
        new Func<Func<TelemetryConfiguration, bool>, TelemetryConfiguration, bool>((fn, @this) => true));

      /* Fixes issue where if you click on the password field without filling in the email address field,
       * it will be focused instead of the password field. 
       */
      var from = typeof(SignInWindow).GetMethods(BindingFlags.Instance | BindingFlags.NonPublic)
                                     .FirstOrDefault(x => x.GetParameters()
                                                           .Select(y => y.ParameterType)
                                                           .SequenceEqual(new[] { typeof(UIElement), typeof(bool) }));
      if ( from != null ) {
        _ = new Hook(from,
          new Action<Action<SignInWindow, UIElement, bool>, SignInWindow, UIElement, bool>((fn, @this, A_1, A_2) => { }));
      }
      /* Forward command line arguments that probably aren't for NC Launcher 2, to the game specified by 
       * the GameID launcher argument.
       */
      _ = new Hook(typeof(GameInfo).GetProperty(nameof(GameInfo.ExeArgument))
                                   .GetGetMethod(),
        new Func<Func<GameInfo, string>, GameInfo, string>((fn, @this) =>
            string.Equals(@this.GameId, AppArgs.Instance.GameId, StringComparison.OrdinalIgnoreCase)
              ? string.Join(" ", Environment.GetCommandLineArgs()
                                            .Skip(1)
                                            .Where(x => !string.IsNullOrEmpty(x)
                                                        && !x.StartsWith("nc-launcher2://")
                                                        && !x.StartsWith("nc-launcher2beta://")
                                                        && !(x.StartsWith("/") && x.Contains(":")))
                                            .Prepend(fn(@this)))
              : fn(@this)));

      /* Fixes bug where unnecessary localization files are downloaded. The funny thing is, it seems
       * like they intended for these files to be skipped, but the code was bugged from the start and it
       * has never been fixed. 
       */
      _ = new Hook(typeof(LanguagePackageFiles).GetMethod(nameof(LanguagePackageFiles.Exists), new[] { typeof(string) }),
        new Func<Func<LanguagePackageFiles, string, bool>, LanguagePackageFiles, string, bool>((fn, @this, fileName) =>
          !string.IsNullOrEmpty(fileName) && @this.Exists(x => fileName.Contains(x.FileName, StringComparison.OrdinalIgnoreCase))));
      
      /* Send unencrypted log messages to OutputDebugString 
       */
      _ = new Hook(typeof(Logger).GetMethod(nameof(Logger.Debug)),
        new Action<Action<Logger, string>, Logger, string>((fn, @this, message) => {
          OutputDebugString("DEBUG - " + message);
          fn(@this, message);
        }));

      _ = new Hook(typeof(Logger).GetMethod(nameof(Logger.Info)),
        new Action<Action<Logger, string>, Logger, string>((fn, @this, message) => {
          OutputDebugString("INFO - " + message);
          fn(@this, message);
        }));

      _ = new Hook(typeof(Logger).GetMethod(nameof(Logger.Error), new[] { typeof(string) }),
        new Action<Action<Logger, string>, Logger, string>((fn, @this, message) => {
          OutputDebugString("ERROR - " + message);
          fn(@this, message);
        }));
    }

    private static Assembly OnAssemblyResolve(object sender, ResolveEventArgs args)
    {
      var assemblyName = new AssemblyName(args.Name);
      var stream = Assembly.GetExecutingAssembly()
                           .GetManifestResourceStream(assemblyName.Name + ".dll.deflate");
      if ( stream != null ) {
        try {
          using ( var binaryReader = new BinaryReader(stream) )
          using ( var deflateStream = new DeflateStream(stream, CompressionMode.Decompress) )
          using ( var memoryStream = new MemoryStream(binaryReader.ReadInt32()) ) {
            deflateStream.CopyTo(memoryStream);
            var assembly = Assembly.Load(memoryStream.GetBuffer());
            if ( assembly != null )
              return assembly;
          }
        } finally {
          stream.Dispose();
        }
      }
      OutputDebugString("Assembly not resolved: " + args.Name);
      return null;
    }
  }
}
