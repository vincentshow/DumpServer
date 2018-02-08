:for /F %%I in ('dir /b *.dmp') do windbg  -QY -c ".logopen /d;!analyze -v;q" -z %%I

windbg  -QY -c ".logopen /d;!analyze -v;q" -z %1