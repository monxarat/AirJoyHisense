@ECHO OFF
@echo ===================================================================
@echo    ���svn,vss,cvs����ļ�   author:Yusheng date:2009.4.24
@echo    filename : clearSvnCvsVss.bat
@echo ===================================================================
@ECHO ��Ctrl + Cȡ����
@pause
@echo ִ��������%0 %1 %2 %3
@echo ת����%1 
cd /d %1

@echo (1)��ʼ�����.SVN���ļ���
@rem for /r %%d in (.) do if exist "%%d\.svn" echo /s /q "%%d\.svn"
@for /r %%d in (.) do if exist "%%d\.svn" rd /s /q "%%d\.svn"

@echo (2)��ʼ���CVS���
@for /r %%d in (.) do if exist "%%d\CVS\Root" rd /s/q "%%d\CVS"

@echo (3)��ʼ���vssver.scc��vssver2.scc�ļ�
attrib -s -h -r -a vssver?.scc /s
for /r . %%i in (vssver?.scc) do if exist %%i del %%i

@echo (4)��ʼ���VisualStudio�е�VSS����ļ�
attrib -s -h -r -a *.vssscc /s
attrib -s -h -r -a *.csproj.vspscc /s

for /r . %%i in (*.vssscc) do if exist %%i del %%i
for /r . %%i in (*.csproj.vspscc) do if exist %%i del %%i

@echo ɾ����������Ƿ�����ɾ������ǲ�ͬ�ı��빤�ߴ����Լ��������־���ɸ��ݾ�������޸��������ļ��е������С�
pause
exit

