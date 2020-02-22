function Get-BnSBaseDirectory {
    $localMachine = [Microsoft.Win32.RegistryKey]::OpenBaseKey(
        [Microsoft.Win32.RegistryHive]::LocalMachine,
        [Microsoft.Win32.RegistryView]::Registry32)
    try {
        $key = $localMachine.OpenSubKey('SOFTWARE\NCWest\BnS')
        try {
            $BaseDir = [string]$key.GetValue('BaseDir')
            if ( Test-Path $BaseDir ) {
                return $BaseDir
            }
        } finally {
            if ( $key -ne $null ) {
                $key.Dispose()
            }
        }

        $key = $localMachine.OpenSubKey(
            'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{9C7ADD9B-0F54-4526-87E8-E739FBB91FD4}')
        try {
            $InstallLocation = [string]$key.GetValue('InstallLocation')
            if ( Test-Path $InstallLocation ) {
                return $InstallLocation
            }
        } finally {
            if ( $key -ne $null ) {
                $key.Dispose()
            }
        }

        $default = "$env:MSBuildProgramFiles32\NCSOFT\BNS"
        if ( Test-Path $default ) {
            return $default
        }
    } finally {
        if ( $localMachine -ne $null ) {
            $localMachine.Dispose()
        }
    }
    return $null
}

if ( $env:MSBuildConfiguration -ne "Debug" ) {
    & "$env:MSBuildProjectDir\tools\upx\upx.exe" "$env:MSBuildTargetPath" --lzma
}
if ( !$env:CI ) {
    $BaseDir = Get-BnSBaseDirectory
    if ( $BaseDir ) {
        Copy-Item "$env:MSBuildTargetPath" -Destination "$BaseDir\bin64\" -Force
    }
}
