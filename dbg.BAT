SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Program Files\MATLAB\R2016b\bin;C:\Program Files\Microsoft SQL Server\Client SDK\ODBC\110\Tools\Binn\;C:\Program Files (x86)\Microsoft SQL Server\120\Tools\Binn\;C:\Program Files\Microsoft SQL Server\120\Tools\Binn\;C:\Program Files\Microsoft SQL Server\120\DTS\Binn\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;C:\Program Files\MATLAB\R2016b\polyspace\bin;C:\Program Files (x86)\GNU Tools ARM Embedded\7 2017-q4-major\bin;C:\Users\Hosam Yonis\AppData\Local\Microsoft\WindowsApps;
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=TM4C123GH6PM
SET CPU_VENDOR=Texas Instruments
SET UV2_TARGET=dbg
SET CPU_CLOCK=0x00F42400
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\dbg\bsp.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\dbg\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\dbg\system_tm4c123gh6pm.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\dbg\startup_tm4c123gh6pm._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\dbg\miros.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\dbg\lesson.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\dbg\lesson.axf" --i32combined --output ".\dbg\lesson.hex"
