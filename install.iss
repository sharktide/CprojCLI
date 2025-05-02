[Setup]
AppName=cproj-cli
AppVersion=1.0
DefaultDirName={pf}\cproj
DefaultGroupName=cproj
OutputDir=.
OutputBaseFilename=cproj_cli_install_x64

[Files]
Source: "cproj.exe"; DestDir: "{app}"; Flags: ignoreversion

[Run]
Filename: "{cmd}"; Parameters: "/C setx PATH ""{app};%PATH%"""; Flags: runhidden runascurrentuser

[Icons]
Name: "{group}\cproj"; Filename: "{app}\cproj.exe"
