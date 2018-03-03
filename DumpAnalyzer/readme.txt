1. This will auto call windbg to finish analyze dump files in specified dir.
2. Change basePath in DumpAnalyzer.properties to customize dump path yourself.
3. Change analyze.bat if you have to reset symbol path or anything windbg param.
4. This will generate {guid}.dmp.log file that record windbg result in the dir where dump file exists.
5. Catch FOLLOWUP_IP and SYMBOL_NAME sections as the summary analyzing result and update db history with this info.
