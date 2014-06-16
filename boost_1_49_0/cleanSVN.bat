@ECHO OFF
@echo ===================================================================
@echo    清除svn,vss,cvs标记文件   author:Yusheng date:2009.4.24
@echo    filename : clearSvnCvsVss.bat
@echo ===================================================================
@ECHO 按Ctrl + C取消。
@pause
@echo 执行批处理：%0 %1 %2 %3
@echo 转到：%1 
cd /d %1

@echo (1)开始清除“.SVN”文件夹
@rem for /r %%d in (.) do if exist "%%d\.svn" echo /s /q "%%d\.svn"
@for /r %%d in (.) do if exist "%%d\.svn" rd /s /q "%%d\.svn"

@echo (2)开始清除CVS标记
@for /r %%d in (.) do if exist "%%d\CVS\Root" rd /s/q "%%d\CVS"

@echo (3)开始清除vssver.scc和vssver2.scc文件
attrib -s -h -r -a vssver?.scc /s
for /r . %%i in (vssver?.scc) do if exist %%i del %%i

@echo (4)开始清除VisualStudio中的VSS标记文件
attrib -s -h -r -a *.vssscc /s
attrib -s -h -r -a *.csproj.vspscc /s

for /r . %%i in (*.vssscc) do if exist %%i del %%i
for /r . %%i in (*.csproj.vspscc) do if exist %%i del %%i

@echo 删除完后请检查是否清楚干净。考虑不同的编码工具带有自己的特殊标志，可根据具体情况修改批处理文件中的命令行。
pause
exit

