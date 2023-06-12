@echo off
setlocal enabledelayedexpansion

set appPath=..\CircuitMaster\circuitMaster_main\release\circuitMaster_main.exe
set testsPath=tests

if exist !appPath! (
	for /L %%i in (1, 1, 5) do ( 
	set "inputPath=%testsPath%\test%%i_input.xml"
	set "outputPath=%testsPath%\test%%i_output.txt"
	set "expOutputPath=%testsPath%\test%%i_expected.txt"

	echo ---------Запуск теста №%%i---------

	"%appPath%" "!inputPath!" "!outputPath!" 
	
	if errorlevel 1 (
	echo В процессе выполнения теста произошла ошибка. 
	 ) else (
	echo В файл записано:
		type "!outputPath!"
	echo Ожидаемый результат:
		type "!expOutputPath!"
		echo Тест успешно завершен.
	 ) 
	
	echo.
)

	echo Выполнение всех тестов завершено.
	
) else (
	echo Файл .exe не найден, проверьте указанный путь к файлу.
)

pause