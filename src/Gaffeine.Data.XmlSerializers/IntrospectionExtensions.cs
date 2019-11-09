using System;
using System.Reflection;
using MonoMod.Utils;

public static class IntrospectionExtensions
{
  static readonly FastReflectionDelegate GetSimpleNameDelegate;

  static IntrospectionExtensions()
  {
    GetSimpleNameDelegate = Type.GetType("System.Reflection.RuntimeAssembly")?
                                .GetMethod("GetSimpleName", BindingFlags.Instance | BindingFlags.NonPublic)?
                                .GetFastDelegate();
  }

  public static string GetSimpleName(this Assembly @this) =>
    GetSimpleNameDelegate?.Invoke(@this) as string;
}