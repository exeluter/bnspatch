using System;
using System.Linq;
using System.Reflection;
using MonoMod.RuntimeDetour;

namespace Gaffeine.Data.XmlSerializers
{
  [AttributeUsage(AttributeTargets.Method, AllowMultiple = true)]
  public class MonoModHookAttribute : Attribute
  {
    public Type Type { get; }
    public BindingFlags BindingFlags { get; set; } = BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public;
    public bool IgnoreName { get; set; }

    public MonoModHookAttribute(Type type)
    {
      this.Type = type;
    }

    public MonoModHookAttribute(string typeName)
      : this(Type.GetType(typeName))
    {
    }

    public bool TryHook(MethodInfo to, out object hook)
    {
      hook = null;

      if ( this.Type is null )
        return false;

      var parameterTypes = to.GetParameters()
                             .Select(x => x.ParameterType)
                             .SkipWhile((x, i) => i == 0 && typeof(Delegate).IsAssignableFrom(x))
                             .SkipWhile((x, i) => i == 0
                                                  && !this.BindingFlags.HasFlag(BindingFlags.Static)
                                                  && x.IsAssignableFrom(this.Type))
                             .ToList();

      var comparisonType = this.BindingFlags.HasFlag(BindingFlags.IgnoreCase)
                             ? StringComparison.OrdinalIgnoreCase
                             : StringComparison.Ordinal;

      var from = this.Type.GetMethods(this.BindingFlags)
                          .SingleOrDefault(x => {
                            if ( this.IgnoreName || x.Name.Equals(to.Name, StringComparison.OrdinalIgnoreCase) ) {
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
