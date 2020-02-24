using Gaffeine.Data.Models;
using GameUpdateService.Updaters.US4Updater.US4UpdateMode;
using Mono.Cecil;
using MonoMod.RuntimeDetour;
using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime;
using System.Windows;
using CustomAttributeNamedArgument = Mono.Cecil.CustomAttributeNamedArgument;

namespace Gaffeine.Data.XmlSerializers
{
  internal static class Hooks
  {
    //[MonoModHook(typeof(FileInfoMap),
    //  BindingFlags = BindingFlags.Instance | BindingFlags.Public | BindingFlags.FlattenHierarchy)]
    //[SuppressMessage("Style", "IDE0060:Remove unused parameter")]
    //public static List<KeyValuePair<string, FileInfo>> GetList(
    //  Func<FileInfoMap, List<KeyValuePair<string, FileInfo>>> @delegate,
    //  FileInfoMap @this)
    //{
    //  var list = new List<KeyValuePair<string, FileInfo>>();
    //  foreach ( var item in @this._FileInfoMap ) {
    //    if ( !Utils.ShouldIgnoreFile(item.Key) )
    //      list.Add(item);
    //  }
    //  return list;
    //}

    //[MonoModHook(typeof(PatchInfoMap),
    //  BindingFlags = BindingFlags.Instance | BindingFlags.Public | BindingFlags.FlattenHierarchy)]
    //[SuppressMessage("Style", "IDE0060:Remove unused parameter")]
    //public static List<KeyValuePair<string, PatchInfo>> GetList(
    //  Func<PatchInfoMap, List<KeyValuePair<string, PatchInfo>>> @delegate,
    //  PatchInfoMap @this)
    //{
    //  var list = new List<KeyValuePair<string, PatchInfo>>();
    //  foreach ( var item in @this._FileInfoMap ) {
    //    if ( !Utils.ShouldIgnoreFile(item.Key) )
    //      list.Add(item);
    //  }
    //  return list;
    //}

    [MonoModHook(typeof(US4UpdateModeBase),
      BindingFlags = BindingFlags.Instance | BindingFlags.NonPublic)]
    public static void FileReplace(Action<US4UpdateModeBase, string, string> @delegate,
      US4UpdateModeBase @this, string src, string dest)
    {
      if ( Guid.TryParse(@this._Game.AppId, out var AppId)
        && AppId.Equals(new Guid(0x0D726F91, 0x202E, 0x4158, 0x9B, 0x9D, 0xCA, 0x0B, 0xF9, 0x44, 0x6B, 0x36))
        && string.Equals(Path.GetFileName(dest), "Gaffeine.Data.dll", StringComparison.OrdinalIgnoreCase) ) {

        var executingAssembly = Assembly.GetExecutingAssembly();
        using ( var parentAssemblyDef = AssemblyDefinition.ReadAssembly(src) )
        using ( var assemblyDef = AssemblyDefinition.ReadAssembly(executingAssembly.Location) ) {
          if ( assemblyDef.HasCustomAttributes ) {
            var customAttributes = assemblyDef.CustomAttributes;
            for ( int i = 0; i < customAttributes.Count; ++i ) {
              if ( customAttributes[i].AttributeType.FullName == "System.Xml.Serialization.XmlSerializerVersionAttribute" ) {
                var constructor = customAttributes[i].Constructor;
                customAttributes[i] = new CustomAttribute(constructor) {
                  Properties = {
                    new CustomAttributeNamedArgument("ParentAssemblyId",
                      new CustomAttributeArgument(assemblyDef.MainModule.TypeSystem.String, parentAssemblyDef.GetAssemblyId())),
                    new CustomAttributeNamedArgument("Version",
                      new CustomAttributeArgument(assemblyDef.MainModule.TypeSystem.String, "4.0.0.0"))
                  }
                };
                assemblyDef.Write(Path.Combine(Path.GetDirectoryName(dest), Path.GetFileName(executingAssembly.Location)));
                break;
              }
            }
          }
        }
      }
      @delegate(@this, src, dest);
    }

    [MonoModHook("Microsoft.ApplicationInsights.Extensibility.TelemetryConfiguration, Microsoft.ApplicationInsights",
      BindingFlags = BindingFlags.Instance | BindingFlags.Public)]
    [SuppressMessage("Style", "IDE0060:Remove unused parameter")]
    public static bool get_DisableTelemetry(Func<object, bool> @delegate,
      object @this) => true;

    [MonoModHook(typeof(GameInfo),
      BindingFlags = BindingFlags.Instance | BindingFlags.Public)]
    public static string get_ExeArgument(Func<GameInfo, string> @delegate,
      GameInfo @this)
    {
      var dict = new Dictionary<string, string>();
      var list = new List<string>();

      foreach ( var s in Environment.GetCommandLineArgs().Skip(1) ) {
        if ( string.IsNullOrEmpty(s) )
          continue;

        int n;
        if ( Uri.TryCreate(s, UriKind.Absolute, out var uri)
          && (uri.Scheme == "nc-launcher2" || uri.Scheme == "nc-launcher2beta") ) {

          var fields = UrlUtility.ParseQueryString(uri.Query);
          foreach ( var key in fields.Cast<string>() ) {
            if ( !dict.ContainsKey(key) )
              dict.Add(key, fields[key]);
          }
        } else if ( s[0] == '/' && (n = s.IndexOf(':', 1)) != -1 ) {
          dict.Add(s.Substring(1, n - 1), s.Substring(n + 1, s.Length - n - 1));
        } else {
          list.Add(s);
        }
      }

      if ( dict.TryGetValue("GameID", out var id) && @this.GameId == id )
        return string.Join(" ", new[] { @delegate(@this) }.Concat(list));

      return @delegate(@this);
    }

    [MonoModHook(typeof(Game),
      BindingFlags = BindingFlags.Instance | BindingFlags.Public)]
    public static string get_AllowMultiClient(Func<Game, string> @delegate,
      Game @this) => "1";

    [MonoModHook("Gaffeine.Controls.Helpers.ShortcutHelper, Gaffeine.Controls",
      BindingFlags = BindingFlags.Static | BindingFlags.Public)]
    public static bool MakeGameShortcut(Func<Game, bool> @delegate,
      Game game)
    {
      string path = Path.Combine(
        Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory), game.GameName + ".lnk");

      return File.Exists(path) || @delegate(game);
    }

    [MonoModHook(typeof(LanguagePackageFiles),
      BindingFlags = BindingFlags.Instance | BindingFlags.Public)]
    [SuppressMessage("Style", "IDE0060:Remove unused parameter")]
    public static bool Exists(Func<LanguagePackageFiles, string, bool> @delegate,
      LanguagePackageFiles @this, string fileName)
    {
      return !string.IsNullOrEmpty(fileName)
        && @this.Exists(x => fileName.Contains(x.FileName, StringComparison.OrdinalIgnoreCase));
    }

    [MonoModHook("NCLauncherW.Views.SignInWindow, NCLauncher2",
      IgnoreName = true,
      BindingFlags = BindingFlags.Instance | BindingFlags.NonPublic)]
    [SuppressMessage("Style", "IDE0060:Remove unused parameter")]
    [SuppressMessage("Style", "IDE1006:Naming Styles")]
    public static void ca8de357b76a2339a41ee639eb04cc454(Action<object, UIElement, bool> @delegate,
      object @this, UIElement A_1, bool A_2)
    {
    }

    [MonoModHook("NCLog.Logger, NCLog",
      BindingFlags = BindingFlags.Instance | BindingFlags.Public)]
    public static void CleanUp(Action<object, string, string, string, DateTime> @delegate,
      object @this, string logDirectory, string fileName, string extension, DateTime date)
    {
      if ( string.IsNullOrEmpty(logDirectory) )
        throw new ArgumentException(nameof(logDirectory);
      if ( string.IsNullOrEmpty(fileName) )
        throw new ArgumentException(nameof(fileName));
      if ( string.IsNullOrEmpty(extension) )
        throw new ArgumentException(nameof(extension));

      var directory = new DirectoryInfo(logDirectory);
      if ( !directory.Exists )
        return;

      foreach ( var file in directory.GetFiles(fileName + "*" + extension)
                                     .Where(x => x.LastWriteTime < date) ) {
        try {
          file.Delete();
        } catch ( UnauthorizedAccessException ) {
        }
      }
    }
  }
}
