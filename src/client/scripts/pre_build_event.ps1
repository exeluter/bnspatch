Set-Location -Path "$env:MSBuildProjectDir\stub"
& cl /c /nologo "$env:MSBuildTargetName.cpp" /WX
if ( [System.IO.File]::Exists("$($env:MSBuildTargetName)_$($env:MSBuildPlatformTarget).def") ) {
  & link /lib `
         /def:"$($env:MSBuildTargetName)_$($env:MSBuildPlatformTarget).def" `
         "$env:MSBuildTargetName.obj" `
         /out:"$($env:MSBuildTargetName)_$($env:MSBuildPlatformTarget).lib" `
         /errorreport:none `
         /machine:"$env:MSBuildPlatformTarget" `
         /nologo `
         /WX
} else {
  & link /lib `
         /def:"$env:MSBuildTargetName.def" `
         "$env:MSBuildTargetName.obj" `
         /out:"$env:MSBuildTargetName.lib" `
         /errorreport:none `
         /machine:"$env:MSBuildPlatformTarget" `
         /nologo `
         /WX
}
