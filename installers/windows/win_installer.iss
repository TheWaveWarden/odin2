[Setup]
AppName=Odin2 Synthesizer Plugin
AppVersion=2.4.0
DefaultDirName={cf}
DefaultGroupName=Odin2
OutputBaseFilename=Odin2
DisableDirPage=yes
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
InfoBeforeFile=infobefore.txt

[Types]
Name: "full"; Description: "Full installation"
Name: "vst3only"; Description: "VST3"
Name: "claponly"; Description: "CLAP"; Flags: iscustom

[Components]
Name: "vst3"; Description: "VST3 Plugin"; Types: full vst3only
Name: "clap"; Description: "CLAP Plugin"; Types: full claponly
 
[Files]
Source: "..\..\build\Odin2_artefacts\Release\VST3\Odin2.vst3\Contents\x86_64-win\Odin2.vst3"; DestDir: "{cf}\VST3"; Flags: ignoreversion; Components: vst3
Source: "..\..\build\Odin2_artefacts\Release\CLAP\Odin2.clap"; DestDir: "{cf}\CLAP"; Flags: ignoreversion; Components: clap

[Dirs]
Name: "{commonappdata}\odin2\Soundbanks"; Permissions: everyone-full
Name: "{commonappdata}\odin2\Soundbanks\User Patches"; Permissions: everyone-full

