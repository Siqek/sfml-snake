$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProfileFile = "$HOME\Documents\WindowsPowershell\profile.ps1"

if (-not (Test-Path $ProfileFile)) {
    New-Item -ItemType File -Path $ProfileFile -Force
}

function AddFunctionIfMissing {
    param (
        [string]$FunctionName,
        [string]$FunctionBody
    )

    $ProfileContent = Get-Content -Path $ProfileFile -ErrorAction SilentlyContinue

    if (-not ($ProfileContent | Select-String -Pattern ("function\s+$([regex]::Escape($FunctionName))"))) {
        Add-Content -Path $ProfileFile -Value "function $FunctionName $FunctionBody"
    } else {
        Write-Warning "Function '$FunctionName' is already defined in $ProfileFile"
    }
}

AddFunctionIfMissing -FunctionName "run"   -FunctionBody "{ & `"$ScriptDir/run-app.ps1`" @args }"
AddFunctionIfMissing -FunctionName "build" -FunctionBody @"
{
    `$jobs = [Math]::Max(1, [int]`$env:NUMBER_OF_PROCESSORS - 2)
    make --no-print-directory -C `"$ScriptDir\..\..\`" -j `$jobs @args
}
"@
AddFunctionIfMissing -FunctionName "clean" -FunctionBody "{ make --no-print-directory -C `"$ScriptDir\..\..\`" clean @args }"

. $ProfileFile
