using Gaffeine.Data.Models;
using GameUpdateService.Updaters.US4Updater.US4UpdateMode;
using MonoMod.RuntimeDetour.HookGen;
using System;
using System.Linq;
using System.Reflection;
using System.Security.Cryptography;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;
using System.Windows.Media;

namespace Gaffeine.Data.XmlSerializers
{
  internal static class Bootstrapper
  {
    static Bootstrapper()
    {
      AppDomain.CurrentDomain.AssemblyResolve += (sender, e) => {
        var assemblyName = new AssemblyName(e.Name);
        return LzmaResource.Load(assemblyName.Name + ".dll.lzma", s => Assembly.Load(s.GetBuffer()));
      };
    }

    internal static void Setup()
    {
      var splashWindow = Type.GetType("NCLauncherW.App, NCLauncher2")
                             .GetField("_SplashWindow", BindingFlags.Static | BindingFlags.NonPublic)
                             .GetValue(null) as Window;
      if ( splashWindow != null ) {
        var logoImage = splashWindow.FindName("LogoImage") as Image;
        if ( logoImage != null ) {
          logoImage.MaxHeight = logoImage.ActualHeight;
          logoImage.Margin = new Thickness(1);
          logoImage.Stretch = Stretch.Uniform;
          logoImage.Source = LzmaResource.Load("whitespy.xaml.lzma", s => XamlReader.Load(s) as DrawingImage);
        }
      }

      HookEndpointManager.Add(
        typeof(US4UpdateModeBase).GetMethod("FileReplace", BindingFlags.Instance | BindingFlags.NonPublic),
        new Action<Action<US4UpdateModeBase, string, string>, US4UpdateModeBase, string, string>(Hooks.FileReplace));

      HookEndpointManager.Add(
        Type.GetType("Microsoft.ApplicationInsights.Extensibility.TelemetryConfiguration, Microsoft.ApplicationInsights")
            .GetProperty("DisableTelemetry", BindingFlags.Instance | BindingFlags.Public).GetGetMethod(),
        new Func<Func<object, bool>, object, bool>(Hooks.get_DisableTelemetry));

      HookEndpointManager.Add(
        typeof(GameInfo).GetProperty("ExeArgument", BindingFlags.Instance | BindingFlags.Public).GetGetMethod(),
        new Func<Func<GameInfo, string>, GameInfo, string>(Hooks.get_ExeArgument));

      HookEndpointManager.Add(
        typeof(Game).GetProperty("AllowMultiClient", BindingFlags.Instance | BindingFlags.Public).GetGetMethod(),
        new Func<Func<Game, string>, Game, string>(Hooks.get_AllowMultiClient));

      //HookEndpointManager.Add(
      //  typeof(Game).GetProperty("PresenceId", BindingFlags.Instance | BindingFlags.Public).GetGetMethod(),
      //  new Func<Func<Game, string>, Game, string>(Hooks.get_PresenceId));

      //HookEndpointManager.Add(
      //  typeof(Game).GetProperty("PresenceId", BindingFlags.Instance | BindingFlags.Public).GetSetMethod(),
      //  new Action<Action<Game, string>, Game, string>(Hooks.set_PresenceId));

      //HookEndpointManager.Add(
      //  typeof(Game).GetProperty("IsRunning", BindingFlags.Instance | BindingFlags.Public).GetGetMethod(),
      //  new Func<Func<Game, bool>, Game, bool>(Hooks.get_IsRunning));

      HookEndpointManager.Add(
        Type.GetType("Gaffeine.Controls.Helpers.ShortcutHelper, Gaffeine.Controls")
            .GetMethod("MakeGameShortcut", BindingFlags.Static | BindingFlags.Public),
        new Func<Func<Game, bool>, Game, bool>(Hooks.MakeGameShortcut));

      HookEndpointManager.Add(
        typeof(LanguagePackageFiles).GetMethod("Exists", BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Public),
        new Func<Func<LanguagePackageFiles, string, bool>, LanguagePackageFiles, string, bool>(Hooks.Exists));

      HookEndpointManager.Add(
        Type.GetType("NCLauncherW.Views.SignInWindow, NCLauncher2")
            .GetMethods(BindingFlags.Instance | BindingFlags.NonPublic)
            .Single(x => x.Name.Length == 0x21
                         && x.Name.StartsWith("c")
                         && x.ReturnType == typeof(void)
                         && x.GetParameters()
                             .Select(y => y.ParameterType)
                             .SequenceEqual(new[] { typeof(UIElement), typeof(bool) })),
        new Action<Action<object, UIElement, bool>, object, UIElement, bool>(Hooks.SetFocusAndSelectText));
    }
  }
}
