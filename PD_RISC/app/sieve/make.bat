@rem PD_RISC_32p6 make batch file

@setlocal

@rem First determine PD installation directory (PD_BASE)
@call :GetPDInstallDir
@rem and get the tool version (PD_VERSION)
@for /F "tokens=1*" %%v in ('%PD_BASE%\bin\lmake --ver') DO @set PD_VERSION=%%v

@rem init some variables
@set APP=sieve
@set APP_DIR=%~dp0
@set MODEL_DIR=%APP_DIR%\..\..
@set BIN=%APP_DIR%\sieve.out
@set MCFG= %MODEL_DIR%\hdl_gen\hdl_memory_configuration.txt

@rem The targets:
@if "%1"=="" call :doDebug
@if "%1"=="run" call :doRun
@if "%1"=="debug" call :doDebug
@if "%1"=="clean" call :doClean
@goto end

@rem --- doRun ---
:doRun
@rem Ensure that 3rdparty libs are found:
@set "PATH=%PD_BASE%\lib\msvc-9.0\3rdparty;%PATH%"
@%MODEL_DIR%\contrib\pdriscrun\run.exe %BIN%
@ exit /B 0

@rem --- doDebug ---
:doDebug
@%PD_BASE%\bin\pdbg -a %MODEL_DIR%\%PD_VERSION%\lib\architecture.msvc-9.0.dll -e %BIN%
@ exit /B 0

@rem --- doClean ---
:doClean
@echo %APP%: clean is not supported on windows systems
@ exit /B 0

	
@rem --- GetPDInstallDir ---
:GetPDInstallDir
@set PD_BASE=
@rem check if PD_INST_DIR is set
@if not "%PD_INST_DIR%" == "" (
    @if not exist %PD_INST_DIR%\bin\lmake.exe (
        @echo ERROR: %PD_INST_DIR%\bin\lmake does not exist
        @ exit /B 1
    )
    @echo INFO: Using PD_INST_DIR from environment: %PD_INST_DIR%
    @set PD_BASE=%PD_INST_DIR%
    @exit /B 0
)
@rem check if lmake is in the path
@for /F "tokens=1*" %%v in ('lmake --ltinstnative') DO @set PD_BASE=%%v
@if not "%PD_BASE%" == "" (
    @echo INFO: Found lmake in PATH, using %PD_BASE%\bin\lmake
    @exit /B 0
)
@if "%PD_BASE%" == "" (
    @echo ERROR: Could not determine ProcessorDesigner installation directory
    @echo        Ensure that either the environment variable PD_INST_DIR points to
    @echo        the PD installation directory or that it is in your PATH!
    @ exit /B 1
)
@ exit /B 0

:end
@endlocal
