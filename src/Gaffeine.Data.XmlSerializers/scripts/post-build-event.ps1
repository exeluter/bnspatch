function Get-NCLauncherBaseDirectory {
    $localMachine = [Microsoft.Win32.RegistryKey]::OpenBaseKey(
        [Microsoft.Win32.RegistryHive]::LocalMachine,
        [Microsoft.Win32.RegistryView]::Registry32)
    try {
        $key = $localMachine.OpenSubKey(
            'SOFTWARE\plaync\NCLauncherW')
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
            'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\NCLauncherW_plaync')
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

        $default = "$env:MSBuildProgramFiles32\NCSOFT\NC Launcher 2"
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

$BaseDir = Get-NCLauncherBaseDirectory
if ( $BaseDir ) {
    Copy-Item "$env:MSBuildTargetPath" -Destination "$BaseDir\" -Force
}
