using System.Text;
using System.Linq;

namespace Mono.Cecil
{
  public static class AssemblyDefinitionExtensions
  {
    public static string GetAssemblyId(this AssemblyDefinition @this)
    {
      var list = @this.Modules.Select(x => x.Mvid.ToString()).ToList();
      list.Sort();
      
      var sb = new StringBuilder();
      foreach ( var item in list ) {
        sb.Append(item);
        sb.Append(',');
      }
      return sb.ToString();
    }
  }
}
