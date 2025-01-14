rem **Builds and runs the arduino IDE for testing Jacob Smith May 31 2019 Personal Study
rem reference 1 https://superuser.com/questions/757146/windows-7-cmd-command-to-keep-cmd-open-after-executing
rem reference 2 https://www.wikihow.com/Delete-a-File-Using-Command-Prompt
rem reference 3 https://superuser.com/questions/1189975/how-can-i-force-stop-a-program-without-using-the-mouse-in-windows-10
rem reference 4 https://superuser.com/questions/1179510/wait-for-a-process-to-complete-in-cmd
rem reference 5 https://www.tutorialspoint.com/batch_script/batch_script_if_statement.htm
rem reference 6 https://stackoverflow.com/questions/734598/how-do-i-make-a-batch-file-terminate-upon-encountering-an-error
rem reference 7 https://superuser.com/questions/101974/play-a-sound-maybe-wav-from-windows-line-command/528541
rem reference 8 https://stackoverflow.com/questions/4983508/can-i-have-an-if-block-in-dos-batch-file
rem reference 9 https://stackoverflow.com/questions/40610241/automatically-answer-to-input-prompt-in-windows-batch

rem sets the file paths to copy to and from
SET startLoc=C:\Users\jsmit\Documents\
SET destLoc=C:\Users\jsmit\Pictures\

rem do not change these, they are true for any installation
SET folderStart=%startLoc%ArduinoClassMaker\src\cc\arduinoclassmaker
SET folderDest=%destLoc%Arduino\app\src\cc\arduinoclassmaker
SET fileStart=%startLoc%ArduinoClassMaker\src\processing\app\ClassMakerInterface.java
SET fileDest=%destLoc%Arduino\app\src\processing\app\ClassMakerInterface.java
SET problemFile=%destLoc%Arduino\app\lib
SET build=%destLoc%Arduino\build

rem temp, sets computer to shutdown so I have to take a break
shutdown /s /t 1600

rem copy files from eclipse workspace to arduino ide and allow all files to be copied, see ref 9 for auto respond
call echo all|xcopy /E /I  %folderStart% %folderDest% 
rem add interface file if isn't in dest
call echo f|xcopy /E /I  %fileStart% %fileDest%
rem overwrite interface file if it is in dest
call echo y|xcopy /E /I  %fileStart% %fileDest%
rem **delete file that results in compilation error ref 3
cd %problemFile%
taskkill /F /IM "javaw.exe" /T
rem delete the file, see ref 2
del jnidispatch-4.2.2-win32-x86.dll
rem  go to directory, compile run the arduino package
cd %build%
rem see 4 for call, which forces commands to be sequential
call ant build
rem these two lines stop the script and play a sound, see reference 1,4,5,6,7,8
call :holdError
call ant run
call :holdError
exit
:holdError
 if %errorlevel% neq 0 (
	start wmplayer "C:\Windows\Media\Alarm10.wav" && timeout 4 && taskkill /im wmplayer.exe
	pause
	exit
)
 EXIT /B 0