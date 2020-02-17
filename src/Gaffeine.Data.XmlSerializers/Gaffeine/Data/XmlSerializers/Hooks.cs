using Gaffeine.Data.Models;
using Gaffeine.Data.Models.Update;
using MoreLinq;
using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Windows;
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
      if ( !args.Contains(@this.GameId, StringComparer.OrdinalIgnoreCase) )
        return @delegate(@this);

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
