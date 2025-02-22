# expects file "languages2test.txt" with each language name on a line

# ----- SETUP start -------------
$exePath = ".\opte35.exe"
$layoutfile = "Layout_symmetrical.txt"
    $description = "symmetrical" # short description of the general layout type
$configfile = "key-arrangement_ansi_symmetrical.cfg"
# $xgram = -3   # -3 for trigram and -2 for bigram
# ====== SETUP stop =============


# ensure codepage is 65001 (UTF-8 encoding)
& chcp 65001

$lineNumber = 0

Get-Content -Path "languages2test.txt" | ForEach-Object {
    $lineNumber++
    # write number of the line and the line content
    Write-Output "${lineNumber}: $_"
    $language_file = "$_.txt"
    $outputPostScript = "${description}_$_.ps"
    $outputPDF = "${description}_$_.pdf"
    $outputFile = "${description}_$_.txt"

    # Write-Output "$exePath -3 $language_file -r $layout -g $outputPostScript -K $configfile > $outputFile"
    # & $exePath -3 $language_file -r $layoutfile -g $outputPostScript -K $configfile > $outputFile
    & ps2pdf -dALLOWPSTRANSPARENCY $outputPostScript  $outputPDF
}



# opt35.exe -3 german.txt -r Layout_symmetrical.txt -g symmetrical_german.ps -K key-arrangement_ansi_symmetrical.cfg >symmetrical_german.txt

# & $exePath $arguments > $outputFile