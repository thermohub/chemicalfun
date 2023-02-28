call build\tests\test_main.exe
call conda activate chemicalfun
call python -c "import chemicalfun"
REM call pytest -ra -vv %TEST_DIR% --color=yes
