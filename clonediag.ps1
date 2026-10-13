# Clone diagnostics: what in a cloned Windows still names the disk's old IDs.
# Run elevated, with the clone mounted:  .\clonediag.ps1 -W E   (E: = the clone's Windows partition)
# Writes clonediag.txt next to this script. Only reads; the hives are loaded and unloaded again.
param([Parameter(Mandatory)][string]$W)
$W = $W.TrimEnd(':', '\')
$out = Join-Path $PSScriptRoot 'clonediag.txt'
$old = @{
    'disk'      = '94bc6f54-db7a-4a6d-81d1-63580471b845'
    'windows'   = '1852a955-87fd-418f-9a3a-c2d1605a7698'
    'recovery'  = '4e476dd8-0021-4676-852d-ad92bd983077'
    'esp'       = 'bf8d5179-dcd7-4e9a-8d8f-1e2cc1eaf5ea'
}
function Say {
    param([Parameter(ValueFromPipeline)]$s)
    process { if ($null -eq $s -or "$s".Trim() -eq '') { return }; "$s" | Out-File -FilePath $out -Append -Encoding utf8 }
}
function Section($s) { Say ''; Say ('=' * 70); Say $s; Say ('=' * 70) }
Remove-Item $out -ErrorAction SilentlyContinue

Section "Clone at ${W}:  $(Get-Date)"
Get-Partition -DriveLetter $W | Format-List DiskNumber, PartitionNumber, Guid, Size | Out-String | Say
Get-Disk -Number (Get-Partition -DriveLetter $W).DiskNumber | Format-List Number, Guid, FriendlyName | Out-String | Say

Section 'Loading hives'
reg load HKLM\CL_SYS "${W}:\Windows\System32\config\SYSTEM" 2>&1 | Out-String | Say
reg load HKLM\CL_SW "${W}:\Windows\System32\config\SOFTWARE" 2>&1 | Out-String | Say
try {
    Section 'Current control set'
    reg query HKLM\CL_SYS\Select 2>&1 | Out-String | Say

    foreach ($name in $old.Keys) {
        $g = [guid]$old[$name]
        $hex = ($g.ToByteArray() | ForEach-Object { $_.ToString('X2') }) -join ''
        foreach ($hive in 'HKLM\CL_SYS', 'HKLM\CL_SW') {
            foreach ($pat in @($old[$name], $hex)) {
                Section "Old $name ID  $pat  in $hive"
                reg query $hive /s /f $pat 2>&1 | Out-String | Say
            }
        }
    }

    Section 'Volume and partition devices Windows knows (STORAGE\Volume)'
    reg query HKLM\CL_SYS\ControlSet001\Enum\STORAGE\Volume 2>&1 | Out-String | Say
    Section 'partmgr'
    reg query HKLM\CL_SYS\ControlSet001\Services\partmgr /s 2>&1 | Out-String | Say
    Section 'Page file settings'
    reg query "HKLM\CL_SYS\ControlSet001\Control\Session Manager\Memory Management" 2>&1 | Out-String | Say
    Section 'MountedDevices'
    reg query HKLM\CL_SYS\MountedDevices 2>&1 | Out-String | Say
    Section 'Crash control / last boot'
    reg query "HKLM\CL_SYS\ControlSet001\Control\CrashControl" 2>&1 | Out-String | Say
}
finally {
    [gc]::Collect()
    Start-Sleep -Milliseconds 500
    reg unload HKLM\CL_SYS | Out-Null
    reg unload HKLM\CL_SW | Out-Null
}

Section 'System event log, newest 150'
try {
    Get-WinEvent -Path "${W}:\Windows\System32\winevt\Logs\System.evtx" -MaxEvents 150 -ErrorAction Stop |
        Format-Table TimeCreated, Id, LevelDisplayName, ProviderName, @{n='Message';e={($_.Message -split "`n")[0]}} -Wrap -Auto |
        Out-String -Width 250 | Say
} catch { Say "could not read: $_" }

Section 'ntbtlog.txt, last 60 lines'
if (Test-Path "${W}:\Windows\ntbtlog.txt") { Get-Content "${W}:\Windows\ntbtlog.txt" -Tail 60 | Say } else { Say 'none (boot logging not on, or it never got that far)' }

Section 'setupapi.dev.log, last 120 lines'
if (Test-Path "${W}:\Windows\INF\setupapi.dev.log") { Get-Content "${W}:\Windows\INF\setupapi.dev.log" -Tail 120 | Say } else { Say 'none' }

Write-Host "Wrote $out"
