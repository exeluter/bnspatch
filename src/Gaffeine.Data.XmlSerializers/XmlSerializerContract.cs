using Gaffeine.Data;
using Gaffeine.Data.Models;
using Mono.Cecil;
using MonoMod.RuntimeDetour;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;
using System.Xml.Serialization;
using static MoreLinq.Extensions.PrependExtension;

namespace Microsoft.Xml.Serialization.GeneratedAssembly
{
  public class XmlSerializerContract : XmlSerializerImplementation
  {
    static readonly string EntryAssemblySimpleName;
    static readonly Dictionary<string, string> AppArgs;

    static XmlSerializerContract()
    {
      AppDomain.CurrentDomain.AssemblyLoad += (sender, args) =>
        NativeMethods.OutputDebugString("Assembly loaded: " + args.LoadedAssembly.FullName);
      AppDomain.CurrentDomain.AssemblyResolve += OnAssemblyResolve;

      EntryAssemblySimpleName = Assembly.GetEntryAssembly().GetSimpleName();
      string typeName = null;
      if ( EntryAssemblySimpleName == "NCLauncher2" )
        typeName = "NCLauncherW.AppArgs, NCLauncher2";
      else if ( EntryAssemblySimpleName == "SelfUpdater" )
        typeName = "SelfUpdater.Settings.AppArgs, SelfUpdater";

      if ( typeName != null ) {
        AppArgs = Type.GetType(typeName)?
                      .GetProperty("Instance", BindingFlags.Static | BindingFlags.Public)?
                      .GetValue(null)?
                      .GetPropertyValues<string>();
        if ( AppArgs != null )
          RegisterRuntimeDetours();
      }
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
      /* Disable Application Insights telemetry
       * Important note: Exactly one event will be tracked before this hook is able to disable it,
       * which is just "NCLauncher2 Startup". To completely disable telemetry you need to overwrite
       * ApplicationInsights.config with this:

         <?xml version="1.0" encoding="utf-8"?>
         <ApplicationInsights xmlns="http://schemas.microsoft.com/ApplicationInsights/2013/Settings">
           <DisableTelemetry>true</DisableTelemetry>
         </ApplicationInsights>
      */
      _ = new Hook(Type.GetType("Microsoft.ApplicationInsights.Extensibility.TelemetryConfiguration, Microsoft.ApplicationInsights")
                       .GetProperty("DisableTelemetry")
                       .GetGetMethod(),
        new Func<Func<object, bool>, object, bool>((fn, @this) => true));

      /* Forward command line arguments that probably aren't for NC Launcher 2, to the game specified by 
       * the GameID launcher argument.
       */
      _ = new Hook(typeof(GameInfo).GetProperty(nameof(GameInfo.ExeArgument))
                                   .GetGetMethod(),
        new Func<Func<GameInfo, string>, GameInfo, string>((fn, @this) =>
            AppArgs.TryGetValue("GameId", out string value) && string.Equals(@this.GameId, value, StringComparison.OrdinalIgnoreCase)
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
      _ = new Hook(Type.GetType("NCLog.Logger, NCLog")
                       .GetMethod("Debug"),
        new Action<Action<object, string>, object, string>((fn, @this, message) => {
          NativeMethods.OutputDebugString("DEBUG - " + message);
          fn(@this, message);
        }));

      _ = new Hook(Type.GetType("NCLog.Logger, NCLog")
                       .GetMethod("Info"),
        new Action<Action<object, string>, object, string>((fn, @this, message) => {
          NativeMethods.OutputDebugString("INFO - " + message);
          fn(@this, message);
        }));

      _ = new Hook(Type.GetType("NCLog.Logger, NCLog")
                       .GetMethod("Error", new[] { typeof(string) }),
        new Action<Action<object, string>, object, string>((fn, @this, message) => {
          NativeMethods.OutputDebugString("ERROR - " + message);
          fn(@this, message);
        }));

      if ( EntryAssemblySimpleName == "NCLauncher2" ) {
        /* Fixes issue where if you click on the password field without filling in the email address field,
         * it will be focused instead of the password field. 
         */
        var from = Type.GetType("NCLauncherW.Views.SignInWindow, NCLauncher2")
                       .GetMethods(BindingFlags.Instance | BindingFlags.NonPublic)
                       .FirstOrDefault(x => x.ReturnType == typeof(void)
                                         && x.GetParameters()
                                             .Select(y => y.ParameterType)
                                             .SequenceEqual(new[] { typeof(UIElement), typeof(bool) }));
        if ( from != null ) {
          _ = new Hook(from,
            new Action<Action<object, UIElement, bool>, object, UIElement, bool>((fn, @this, A_1, A_2) => { }));
        };
      } else if ( EntryAssemblySimpleName == "SelfUpdater" ) {

      }
    }

    private static string GenerateAssemblyId(AssemblyDefinition assemblyDef)
    {
      var sb = new StringBuilder();
      foreach ( var mvid in assemblyDef.Modules
                                       .Select(x => x.Mvid.ToString())
                                       .OrderBy(x => x) ) {
        sb.Append(mvid);
        sb.Append(',');
      }
      return sb.ToString();
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

    private static class NativeMethods
    {
      [DllImport("kernel32.dll")]
      internal static extern void OutputDebugString(string lpOutputString);
    }
  }
}
