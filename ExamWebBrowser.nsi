; Includes ---------------------------------
!include MUI2.nsh
Unicode true

; Settings ---------------------------------
Name "SMB33 ExamWebBrowser"
OutFile "ExamWebBrowserInstaller.exe"
RequestExecutionLevel user
InstallDir "$PROGRAMFILES\ExamWebBrowser"
Page Directory
Page InstFiles

!define MUI_ICON "media/logo-128x128.ico"

; Pages ------------------------------------
;!insertmacro MUI_PAGE_COMPONENTS 
;!insertmacro MUI_PAGE_DIRECTORY
;!insertmacro MUI_PAGE_INSTFILES
;!insertmacro MUI_PAGE_WELCOME
; Encoding = ANSI :
;!insertmacro MUI_PAGE_LICENSE "ExamWebBrowser.txt"

; Languages --------------------------------
!insertmacro MUI_LANGUAGE "French"

; Sections ---------------------------------
Section "ExamWebBrowser" section_index

  # your code here
  SetOutPath $InstDir
  File /nonfatal /a /r "bin\Windows\"

SectionEnd

; Descriptions -----------------------------
;LangString DESC_section_index ${LANG_English} "section_description"

;!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
;	!insertmacro MUI_DESCRIPTION_TEXT ${section_index} $(DESC_section_index)
;!insertmacro MUI_FUNCTION_DESCRIPTION_END

; Functions --------------------------------
