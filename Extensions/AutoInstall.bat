echo MFX File: %1
echo MFX Destination: %2

:mfd
set status=ERROR
for /F "tokens=*" %%A in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Clickteam\Multimedia Fusion Developer 2\Settings" /v "ProPath"') do set status=%%A
set status=%status:~0,5%
if %status%==ERROR goto nmfd

for /F "tokens=3* delims= " %%A in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Clickteam\Multimedia Fusion Developer 2\Settings" /v "ProPath" ^| find "REG_SZ"') do set mmfpath=%%A %%B

echo MMF2.0 Dev Path: %mmfpath%

copy /y "%1" "%mmfpath%%2"

set mmfpath=
set status=

goto mfs

:nmfd
echo You don't have MMF2.0 Developer :(

:mfs
set status=ERROR
for /F "tokens=1 delims=:" %%A in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Clickteam\Multimedia Fusion 2\Settings" /v "StdPath"') do set status=%%A
set status=%status:~0,5%
if %status%==ERROR goto nmfs

for /F "tokens=3* delims= " %%A in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Clickteam\Multimedia Fusion 2\Settings" /v "StdPath" ^| find "REG_SZ"') do set mmfpath=%%A %%B

echo MMF2.0 Std Path: %mmfpath%

copy /y "%1" "%mmfpath%%2"

set mmfpath=
set status=

goto cfd

:nmfs
echo You don't have MMF2.0 Standard

:cfd
set status=ERROR
for /F "tokens=1 delims=:" %%A in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Clickteam\Fusion Developer 2.5\Settings" /v "InstallPath"') do set status=%%A
set status=%status:~0,5%
if %status%==ERROR goto ncfd

for /F "tokens=3* delims= " %%A in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Clickteam\Fusion Developer 2.5\Settings" /v "InstallPath" ^| find "REG_SZ"') do set mmfpath=%%A %%B

echo CTF2.5 Dev Path: %mmfpath%

copy /y "%1" "%mmfpath%%2"

set mmfpath=
set status=

goto cfs

:ncfd
echo You don't have CTF2.5 Developer :(

:cfs
set status=ERROR
for /F "tokens=1 delims=:" %%A in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Clickteam\Fusion 2.5\Settings" /v "InstallPath"') do set status=%%A
set status=%status:~0,5%
if %status%==ERROR goto ncfs

for /F "tokens=3* delims= " %%A in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Clickteam\Fusion 2.5\Settings" /v "InstallPath" ^| find "REG_SZ"') do set mmfpath=%%A %%B

echo CTF2.5 Std Path: %mmfpath%

copy /y "%1" "%mmfpath%%2"

set mmfpath=
set status=

goto end

:ncfs
echo You don't have CTF2.5 Standard

:end

(call )
exit /b 0
