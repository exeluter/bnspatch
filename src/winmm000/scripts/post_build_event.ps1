function Get-BaseDir {
  param(
    [Parameter(Mandatory=$true)] [string]$SubKey,
    [Parameter(Mandatory=$false)] [string]$Combine
  )

  $localMachine = [Microsoft.Win32.RegistryKey]::OpenBaseKey([Microsoft.Win32.RegistryHive]::LocalMachine, [Microsoft.Win32.RegistryView]::Registry32)
  try {
    $key = $localMachine.OpenSubKey($SubKey)
    try {
        return [System.IO.Path]::Combine([string]$key.GetValue('BaseDir'), $Combine)
    } catch {
    } finally {
      if ( $key -ne $null ) {
        $key.Dispose()
      }
    }
  } catch {
  } finally {
    if ( $localMachine -ne $null ) {
      $localMachine.Dispose()
    }
  }
  return $null
}

function Get-InstallLocation {
  param(
    [Parameter(Mandatory=$true)] [string]$ProductCode,
    [Parameter(Mandatory=$false)] [string]$Combine
  )

  $localMachine = [Microsoft.Win32.RegistryKey]::OpenBaseKey([Microsoft.Win32.RegistryHive]::LocalMachine, [Microsoft.Win32.RegistryView]::Registry32)
  try {
    $key = $localMachine.OpenSubKey("SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$ProductCode")
    try {
        return [System.IO.Path]::Combine([string]$key.GetValue('InstallLocation'), $Combine)
    } catch {
    } finally {
      if ( $key -ne $null ) {
        $key.Dispose()
      }
    }
  } catch {
  } finally {
    if ( $localMachine -ne $null ) {
      $localMachine.Dispose()
    }
  }
  return $null
}

if ( !$env:CI -or [System.IO.File]::Exists("$env:MSBuildProjectDir\scripts\postbuild.no") ) {
  if ( ![System.IO.File]::Exists("$env:MSBuildProjectDir\scripts\postbuild.yes") ) {
    Add-Type -AssemblyName 'System.Windows.Forms'
    switch ( [System.Windows.Forms.MessageBox]::Show(`
      "Would you like to automatically copy $env:MSBuildTargetFileName to your BNS folder(s) after compilation? You may need to copy it once manually and adjust its ACL.`nYou won't be asked about this again.",`
      "$env:MSBuildTargetPath",`
      [System.Windows.Forms.MessageBoxButtons]::YesNo,`
      [System.Windows.Forms.MessageBoxIcon]::Question) ) {
      {[System.Windows.Forms.DialogResult]::Yes} { '' > "$env:MSBuildProjectDir\scripts\postbuild.yes"; break }
      {[System.Windows.Forms.DialogResult]::No} { '' > "$env:MSBuildProjectDir\scripts\postbuild.no" }
      default { exit }
    }
  }

  # do something
}
