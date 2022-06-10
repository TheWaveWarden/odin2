[Setup]
AppName=Odin2 VST3 Synthesizer
AppVersion=2.3.2
DefaultDirName={cf}
DefaultGroupName=Odin2
OutputBaseFilename=Odin2
DisableDirPage=yes
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
InfoBeforeFile=infobefore.txt

 
[Files]
Source: "..\..\build\Odin2_artefacts\Release\VST3\Odin2.vst3\Contents\x86_64-win\Odin2.vst3"; DestDir: "{cf}\VST3"; Flags: ignoreversion

[Dirs]
Name: "C:\ProgramData\odin2\Soundbanks"; Permissions: everyone-full
Name: "C:\ProgramData\odin2\Soundbanks\User Patches"; Permissions: everyone-full

