param([string]$Configuration = "Debug", [string]$Platform = "x64")
$ErrorActionPreference = "Stop"
$repo = Split-Path $PSScriptRoot -Parent
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vs = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (!$vs) { throw "Visual Studio C++ tools not found" }
$arch = if ($Platform -eq "Win32") { "x86" } else { "amd64" }
$runtime = if ($Configuration -eq "Debug") { "/MDd /Od /Zi" } else { "/MD /O2" }
$lib = if ($Platform -eq "Win32") { "$repo\$Configuration\opEngine_Window.lib" } else { "$repo\x64\$Configuration\opEngine_Window.lib" }
$output = Join-Path (Split-Path $lib) "Tests"
New-Item -ItemType Directory -Force -Path $output | Out-Null
$command = '@echo off' + "`r`n" + 'call "' + $vs + '\Common7\Tools\VsDevCmd.bat" -no_logo -arch=' + $arch + "`r`n" +
    'cl /nologo /EHsc /std:c++20 ' + $runtime + ' /I"' + $repo + '\opEngine_SOURCE" "' + $PSScriptRoot + '\CollisionTests.cpp" /Fo"' + $output + '\CollisionTests.obj" /Fe"' + $output + '\CollisionTests.exe" /link "' + $lib + '" user32.lib gdi32.lib /PDB:"' + $output + '\CollisionTests.pdb"' + "`r`n" +
    'if errorlevel 1 exit /b 1' + "`r`n" + '"' + $output + '\CollisionTests.exe"'
$script = Join-Path $output "run.cmd"
[IO.File]::WriteAllText($script, $command, [Text.Encoding]::Default)
Push-Location (Join-Path $repo "Editor_Window")
try { & $env:ComSpec /c $script; if ($LASTEXITCODE -ne 0) { throw "Collision tests failed" } }
finally { Pop-Location }
