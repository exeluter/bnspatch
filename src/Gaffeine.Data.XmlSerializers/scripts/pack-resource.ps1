param (
    [string]$InFile,
    [string]$OutFile
)

& '..\tools\zopfli\zopfli.exe' --deflate --i1000 $InFile
$ostream = [System.IO.File]::Open("$OutFile", `
                                  [System.IO.FileMode]::Create, `
                                  [System.IO.FileAccess]::Write)
$fileInfo = [System.IO.FileInfo]::new("$InFile.deflate")
$istream = $fileInfo.OpenRead()
$writer = [System.IO.BinaryWriter]::new($ostream)
$writer.Write([System.Convert]::ToInt32($fileInfo.Length))
$istream.CopyTo($ostream)
$istream.Dispose()
$fileInfo.Delete()
$writer.Dispose()
