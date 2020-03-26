function Get-BaseDir {
  param(
    [Parameter(Mandatory=$true)] [string]$SubKey
  )

  $localMachine = [Microsoft.Win32.RegistryKey]::OpenBaseKey([Microsoft.Win32.RegistryHive]::LocalMachine, [Microsoft.Win32.RegistryView]::Registry32)
  try {
    $key = $localMachine.OpenSubKey($SubKey)
    try {
        return [string]$key.GetValue('BaseDir')
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
    [Parameter(Mandatory=$true)] [string]$ProductCode
  )

  $localMachine = [Microsoft.Win32.RegistryKey]::OpenBaseKey([Microsoft.Win32.RegistryHive]::LocalMachine, [Microsoft.Win32.RegistryView]::Registry32)
  try {
    $key = $localMachine.OpenSubKey("SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$ProductCode")
    try {
        return [string]$key.GetValue('InstallLocation')
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
      "Would you like to automatically copy $env:MSBuildTargetFileName to your NC Launcher 2 folder after compilation? You may need to copy it once manually and adjust its ACL.`nYou won't be asked about this again.",`
      "$env:MSBuildTargetPath",`
      [System.Windows.Forms.MessageBoxButtons]::YesNo,`
      [System.Windows.Forms.MessageBoxIcon]::Question) ) {
      {[System.Windows.Forms.DialogResult]::Yes} { '' > "$env:MSBuildProjectDir\scripts\postbuild.yes"; break }
      {[System.Windows.Forms.DialogResult]::No} { '' > "$env:MSBuildProjectDir\scripts\postbuild.no" }
      default { exit }
    }
  }
  $NCLauncherWBaseDir = Get-BaseDir -SubKey 'SOFTWARE\plaync\NCLauncherW'
  if ( ![System.IO.Directory]::Exists($NCLauncherWBaseDir) ) {
    $NCLauncherWBaseDir = Get-InstallLocation -ProductCode 'NCLauncherW_plaync'
  }
  if ( [System.IO.Directory]::Exists($NCLauncherWBaseDir) ) {
    Copy-Item "$env:MSBuildTargetPath" -Destination "$NCLauncherWBaseDir\" -Force
  }
}
