$exePath = "$($env:USERPROFILE)\nsis-2.46-setup.exe"

(New-Object Net.WebClient).DownloadFile('http://ufpr.dl.sourceforge.net/project/nsis/NSIS%202/2.46/nsis-2.46-setup.exe', $exePath)

cmd /c start /wait $exePath /S /D=C:\nsis

$zipPath = "$env:APPVEYOR_BUILD_FOLDER\gsutil.zip"

$gsutilPath = "$env:APPVEYOR_BUILD_FOLDER"

(New-Object System.Net.WebClient).DownloadFile('https://pub.storage.googleapis.com/gsutil.zip', $zipPath)

7z x $zipPath -y | Out-Null

$bytes = [System.Convert]::FromBase64String($env:gsu_json)

[System.IO.File]::WriteAllBytes("$env:USERPROFILE\service.json", $bytes)  