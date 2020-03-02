$directoryInfo = [System.IO.DirectoryInfo]::new('..\lib')

$assemblies = $directoryInfo.EnumerateFiles('*.dll', [System.IO.SearchOption]::AllDirectories)

foreach ( $assembly in $assemblies ) {
    $assembly.FullName
    & '..\tools\zopfli\zopfli.exe' --deflate --i1000 $assembly.FullName

    $fileInfo = [System.IO.FileInfo]::new("$($assembly.FullName).deflate")
    $ostream = [System.IO.File]::Open("..\Resources\$($fileInfo.Name)", `
                                      [System.IO.FileMode]::Create, `
                                      [System.IO.FileAccess]::Write)
    $istream = $fileInfo.OpenRead()
    $writer = [System.IO.BinaryWriter]::new($ostream)
    $writer.Write([System.Convert]::ToInt32($fileInfo.Length))
    $istream.CopyTo($ostream)
    $istream.Dispose()
    $fileInfo.Delete()
    $writer.Dispose()
}
