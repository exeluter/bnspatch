function Get-BNSBaseDirectory {
  $localMachine = [Microsoft.Win32.RegistryKey]::OpenBaseKey([Microsoft.Win32.RegistryHive]::LocalMachine, [Microsoft.Win32.RegistryView]::Registry32)
  if ( $localMachine -ne $null ) {
    try {
      $key = $localMachine.OpenSubKey('SOFTWARE\NCWest\BnS')
      try {
        if ( $key -ne $null ) {
          $value = [string]$key.GetValue('BaseDir')
          if ( [System.IO.Directory]::Exists($value) ) {
            return $value
          }
        }
      } finally {
        if ( $key -ne $null ) {
          $key.Dispose()
        }
      }

      $key = $localMachine.OpenSubKey('SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{9C7ADD9B-0F54-4526-87E8-E739FBB91FD4}')
      if ( $key -ne $null ) {
        try {
            $value = [string]$key.GetValue('InstallLocation')
            if ( [System.IO.Directory]::Exists($value) ) {
              return $value
            }
        } finally {
          $key.Dispose()
        }
      }

      $default = "$env:MSBuildProgramFiles32\NCSOFT\BNS"
      if ( [System.IO.Directory]::Exists($default) ) {
        return $default
      }
    } finally {
      if ( $localMachine -ne $null ) {
        $localMachine.Dispose()
      }
    }
  }
  return $null
}

function Get-BNSRBaseDirectory {
  $localMachine = [Microsoft.Win32.RegistryKey]::OpenBaseKey([Microsoft.Win32.RegistryHive]::LocalMachine, [Microsoft.Win32.RegistryView]::Registry32)
  if ( $localMachine -ne $null ) {
    try {
      $key = $localMachine.OpenSubKey('SOFTWARE\plaync\BNSR_LIVE')
      try {
        if ( $key -ne $null ) {
        $value = [System.IO.Path]::Combine([string]$key.GetValue('BaseDir'), 'BNSR')
          if ( [System.IO.Directory]::Exists($value) ) {
            return $value
          }
        }
      } finally {
        if ( $key -ne $null ) {
          $key.Dispose()
        }
      }

      $key = $localMachine.OpenSubKey('SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{9C7ADD9B-0F54-4526-87E8-E739FBB91FD4}')
      if ( $key -ne $null ) {
        try {
          $value = [System.IO.Path]::Combine([string]$key.GetValue('InstallLocation'), 'BNSR')
            if ( [System.IO.Directory]::Exists($value) ) {
              return $value
            }
        } finally {
          $key.Dispose()
        }
      }

      $default = "$env:MSBuildProgramFiles32\plaync\BNSR"
      if ( [System.IO.Directory]::Exists($default) ) {
        return $default
      }
    } finally {
      if ( $localMachine -ne $null ) {
        $localMachine.Dispose()
      }
    }
  }
  return $null
}

if ( $env:MSBuildConfiguration.StartsWith('Release') ) {
  & "$env:MSBuildProjectDir\tools\upx\upx.exe" "$env:MSBuildTargetPath" --lzma
}
if ( !$env:CI ) {
  if ( $env:MSBuildConfiguration.EndsWith('-BNSR') ) { 
    $BaseDir = Get-BNSRBaseDirectory 
    if ( $BaseDir ) {
      Copy-Item "$env:MSBuildTargetPath" -Destination "$BaseDir\Binaries\Win64\" -Force
    }
  } else { 
    $BaseDir = Get-BNSBaseDirectory 
    if ( $BaseDir ) {
      if ( $env:MSBuildPlatformTarget -eq 'x64' ) {
        Copy-Item "$env:MSBuildTargetPath" -Destination "$BaseDir\bin64\" -Force
      } else {
        Copy-Item "$env:MSBuildTargetPath" -Destination "$BaseDir\bin\" -Force
      }
    }
  }
}
