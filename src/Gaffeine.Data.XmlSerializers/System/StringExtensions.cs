using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace System
{
  public static class StringExtensions
  {
    /// <summary>Returns a value indicating whether a specified substring occurs within this string. A parameter specifies the type of search to use for the specified string.</summary>
    /// <param name="value">The string to seek.</param>
    /// <param name="comparisonType">One of the enumeration values that specifies the rules for the search.</param>
    /// <returns>
    ///   <see langword="true" /> if the <paramref name="value" /> parameter occurs within this string, or if <paramref name="value" /> is the empty string (""); otherwise, <see langword="false" />.</returns>
    /// <exception cref="T:System.ArgumentNullException">
    ///   <paramref name="value" /> is <see langword="null" />.</exception>
    /// <exception cref="T:System.ArgumentException">
    ///   <paramref name="comparisonType" /> is not a valid <see cref="T:System.StringComparison" /> value.</exception>
    public static bool Contains(this string @this, string value, StringComparison comparisonType)
    {
      return @this.IndexOf(value, comparisonType) > -1;
    }

    /// <summary>Performs text string comparison given two strings.</summary>
    /// <param name="pattern">The string against which this string is being compared.</param>
    /// <returns>
    ///   <see langword="true" /> if the strings match; otherwise, <see langword="false" />.</returns>
    public static bool Like(this string @this, string pattern)
    {
      return LikeOperator.LikeString(@this, pattern, CompareMethod.Text);
    }

    /// <summary>Performs binary string comparison given two strings.</summary>
    /// <param name="pattern">The string against which this string is being compared.</param>
    /// <returns>
    ///   <see langword="true" /> if the strings match; otherwise, <see langword="false" />.</returns>
    public static bool LikeOrdinal(this string @this, string pattern)
    {
      return LikeOperator.LikeString(@this, pattern, CompareMethod.Binary);
    }
  }
}