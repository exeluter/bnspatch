using System;
using System.Linq;
using System.Reflection;
using MonoMod.RuntimeDetour;

namespace MonoMod.RuntimeDetour
{
  [AttributeUsage(AttributeTargets.Method, AllowMultiple = true)]
  public class MonoModHookAttribute : Attribute
  {
    public Type Type { get; }
    public BindingFlags BindingFlags { get; set; } = BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public;
    public bool IgnoreName { get; set; }

    public MonoModHookAttribute(Type type)
    {
      Type = type;
    }

    public MonoModHookAttribute(string typeName)
      : this(Type.GetType(typeName))
    {
    }

    public bool TryHook(MethodInfo to, out object hook)
    {
      hook = null;

      if ( Type is null )
        return false;

      var parameterTypes = to.GetParameters()
                             .Select(x => x.ParameterType)
                             .SkipWhile((x, i) => i == 0 && typeof(Delegate).IsAssignableFrom(x))
                             .SkipWhile((x, i) => i == 0
                                                  && !BindingFlags.HasFlag(BindingFlags.Static)
                                                  && x.IsAssignableFrom(Type))
                             .ToList();

      var comparisonType = BindingFlags.HasFlag(BindingFlags.IgnoreCase)
                             ? StringComparison.OrdinalIgnoreCase
                             : StringComparison.Ordinal;

      var from = Type.GetMethods(BindingFlags)
                     .SingleOrDefault(x => {
                       if ( (IgnoreName || x.Name.Equals(to.Name, StringComparison.OrdinalIgnoreCase))
                         && to.ReturnType == x.ReturnType ) {

                         var parameters = x.GetParameters();
                         return parameters.Length == parameterTypes.Count
                           && parameterTypes.SequenceEqual(parameters.Select(y => y.ParameterType));
                       }
                       return false;
                     });

      if ( from is null )
        return false;

      hook = new Hook(from, to);
      return true;
    }
  }
}
