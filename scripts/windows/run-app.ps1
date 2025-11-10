$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition

$Config = "Debug"
$ExecutableName = "Game.exe"

if ($args.Count -ge 1) {
    switch -Regex ($args[0].ToLower()) {
        "debug"   { $Config = "Debug" }
        "release" { $Config = "Release" }
        default {
            Write-Host "Error: Invalid configuration '$($args[0])'. Expected 'Debug' or 'Release'." -ForegroundColor Red
            exit 1
        }
    }
}

$BuildOutputDir = Join-Path $ScriptDir "..\..\bin\$Config"
$ExecutablePath = Join-Path $BuildOutputDir $ExecutableName

if (Test-Path $ExecutablePath) {
    Set-Location (Join-Path $ScriptDir "..\..")
    & $ExecutablePath
} else {
    Write-Host "Error: $ExecutablePath can't be executed or doesn't exist!" -ForegroundColor Red
    exit 1
}
