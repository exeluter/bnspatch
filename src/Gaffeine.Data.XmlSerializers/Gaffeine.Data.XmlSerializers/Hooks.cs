using Gaffeine.Data.Models;
using Gaffeine.Data.Models.Update;
using GameUpdateService.Updaters.US4Updater.US4UpdateMode;
using Mono.Cecil;
using MoreLinq;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Windows;
using System.Xml.Serialization;
using CustomAttributeNamedArgument = Mono.Cecil.CustomAttributeNamedArgument;
using FileInfo = Gaffeine.Data.Models.Update.FileInfo;

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
            var item = assemblyDef.CustomAttributes.SingleOrDefault(x => x.AttributeType.FullName == "System.Xml.Serialization.XmlSerializerVersionAttribute");
            assemblyDef.CustomAttributes.Remove(item);

            assemblyDef.CustomAttributes.Add(new CustomAttribute(item.Constructor) {
              Properties = {
                new CustomAttributeNamedArgument("ParentAssemblyId",
                  new CustomAttributeArgument(assemblyDef.MainModule.TypeSystem.String, parentAssemblyDef.GetAssemblyId())),
                new CustomAttributeNamedArgument("Version",
                  new CustomAttributeArgument(assemblyDef.MainModule.TypeSystem.String, "4.0.0.0"))
              }
            });
            assemblyDef.Write(Path.Combine(Path.GetDirectoryName(dest), Path.GetFileName(executingAssembly.Location)));
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
      var args = Environment.GetCommandLineArgs().Skip(1);

      // To do: Find a better way to detect GameId arg and compare to current game
      return string.Join(" ", args.Where(x => !string.IsNullOrEmpty(x)
                                              && !x.StartsWith("nc-launcher2://")
                                              && !x.StartsWith("nc-launcher2beta://")
                                              && !(x.StartsWith("/") && x.Contains(":")))
                                  .Prepend(@delegate(@this)));
    }

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
  }
}
