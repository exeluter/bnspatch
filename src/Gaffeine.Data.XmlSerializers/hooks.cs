using Gaffeine.Data.Models;
using GameUpdateService.Updaters.US4Updater.US4UpdateMode;
using Mono.Cecil;
using System;
using System.Collections.Generic;
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
    public static void FileReplace(Action<US4UpdateModeBase, string, string> @delegate, US4UpdateModeBase @this, string src, string dest)
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
              if ( customAttributes[i].AttributeType.FullName != "System.Xml.Serialization.XmlSerializerVersionAttribute" )
                continue;
              var constructor = customAttributes[i].Constructor;
              customAttributes[i] = new CustomAttribute(constructor) {
                Properties = {
                  new CustomAttributeNamedArgument("ParentAssemblyId", new CustomAttributeArgument(assemblyDef.MainModule.TypeSystem.String, parentAssemblyDef.GetAssemblyId())),
                  new CustomAttributeNamedArgument("Version", new CustomAttributeArgument(assemblyDef.MainModule.TypeSystem.String, "4.0.0.0"))
                }
              };
              assemblyDef.Write(Path.Combine(Path.GetDirectoryName(dest), Path.GetFileName(executingAssembly.Location)));
              break;
            }
          }
        }
      }
      @delegate(@this, src, dest);
    }

    public static bool get_DisableTelemetry(Func<object, bool> @delegate, object @this) => true;

    public static string get_ExeArgument(Func<GameInfo, string> @delegate, GameInfo @this)
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

    //public static string get_PresenceId(Func<Game, string> @delegate, Game @this)
    //{
    //  var presenceId = @delegate(@this);
    //  if ( !string.IsNullOrEmpty(presenceId) ) {
    //    var gameRegion = @this.SelectedGameRegion ?? @this.GameRegions.FirstOrDefault();
    //    if ( gameRegion != null )
    //      presenceId = string.Join(":", presenceId, gameRegion.Code.ToString("d"));
    //  }
    //  return presenceId;
    //}

    //public static void set_PresenceId(Action<Game, string> @delegate, Game @this, string value)
    //{
    //  var array = value.Split(':');
    //  if ( array.Length > 2 )
    //    value = string.Join(":", array[0], array[1]);

    //  @delegate(@this, value);
    //}

    //public static bool get_IsRunning(Func<Game, bool> @delegate, Game @this)
    //{
    //  if ( @this.GameUpdater.CheckInstalled()
    //    && Mutex.TryOpenExisting(@this.PresenceId, out var mtx) ) {
    //    mtx.Close();
    //    return true;
    //  }
    //  return @delegate(@this);
    //}

    public static string get_AllowMultiClient(Func<Game, string> @delegate, Game @this) => "1";

    public static bool MakeGameShortcut(Func<Game, bool> @delegate, Game game)
    {
      string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory), game.GameName + ".lnk");

      return File.Exists(path) || @delegate(game);
    }

    public static bool Exists(Func<LanguagePackageFiles, string, bool> @delegate, LanguagePackageFiles @this, string fileName)
    {
      return !string.IsNullOrEmpty(fileName)
        && @this.Exists(x => fileName.Contains(x.FileName, StringComparison.OrdinalIgnoreCase));
    }

    public static void SetFocusAndSelectText(Action<object, UIElement, bool> @delegate, object @this, UIElement element, bool A_2)
    {
    }
  }
}
