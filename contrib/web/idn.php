<html>

  <head>
    <title>Try GNU Libidn</title>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
  </head>

  <body>

    <h1>Try GNU Libidn</h1>

    <p>This page enable you to try the Punycode encode and decode and
    IDNA ToASCII and ToUnicode operations of <a
    href="http://www.gnu.org/software/libidn/">GNU Libidn</a>. Note
    that libidn contains more than this, for example a generic
    stringprep implementation with support for several stringprep
    profiles.

    <hr>
    <h2>Input</h2>
    <form>
      <input type=text name=data size=40 value="<?php print $data ? $data : "räksmörgås.josefsson.org" ?>"><br>
      <input type=radio name=mode value=punyencode <?php if ($mode == "punyencode") { print "checked"; } ?>>Punycode encode<br>
      <input type=radio name=mode value=punydecode <?php if ($mode == "punydecode") { print "checked"; } ?>> Punycode decode<br>
      <input type=radio name=mode value=toascii <?php if ($mode == "toascii" || !$mode) { print "checked"; } ?>>IDNA ToASCII<br>
      <input type=radio name=mode value=tounicode <?php if ($mode == "tounicode") { print "checked"; } ?>>IDNA ToUnicode<br>
      <input type=checkbox name=allowunassigned <?php if ($allowunassigned) { print "checked"; } ?>>Allow Unassigned<br>
      <input type=checkbox name=usestd3asciirules <?php if ($usestd3asciirules) { print "checked"; } ?>>UseSTD3ASCIIRules<br>
      Force input charset to: <select name=charset>
	<option <?php if ($charset == "ANSI_X3.4-1968") { print "selected"; } ?>>ANSI_X3.4-1968
	<option <?php if ($charset == "ANSI_X3.110-1983") { print "SELECTED"; } ?>>ANSI_X3.110-1983
	<option <?php if ($charset == "ANSI_X3.4-1968") { print "SELECTED"; } ?>>ANSI_X3.4-1968
	<option <?php if ($charset == "ASMO_449") { print "SELECTED"; } ?>>ASMO_449
	<option <?php if ($charset == "BIG5") { print "SELECTED"; } ?>>BIG5
	<option <?php if ($charset == "BIG5-HKSCS") { print "SELECTED"; } ?>>BIG5-HKSCS
	<option <?php if ($charset == "BS_4730") { print "SELECTED"; } ?>>BS_4730
	<option <?php if ($charset == "BS_VIEWDATA") { print "SELECTED"; } ?>>BS_VIEWDATA
	<option <?php if ($charset == "CP10007") { print "SELECTED"; } ?>>CP10007
	<option <?php if ($charset == "CP1250") { print "SELECTED"; } ?>>CP1250
	<option <?php if ($charset == "CP1251") { print "SELECTED"; } ?>>CP1251
	<option <?php if ($charset == "CP1252") { print "SELECTED"; } ?>>CP1252
	<option <?php if ($charset == "CP1253") { print "SELECTED"; } ?>>CP1253
	<option <?php if ($charset == "CP1254") { print "SELECTED"; } ?>>CP1254
	<option <?php if ($charset == "CP1255") { print "SELECTED"; } ?>>CP1255
	<option <?php if ($charset == "CP1256") { print "SELECTED"; } ?>>CP1256
	<option <?php if ($charset == "CP1257") { print "SELECTED"; } ?>>CP1257
	<option <?php if ($charset == "CP1258") { print "SELECTED"; } ?>>CP1258
	<option <?php if ($charset == "CP737") { print "SELECTED"; } ?>>CP737
	<option <?php if ($charset == "CP775") { print "SELECTED"; } ?>>CP775
	<option <?php if ($charset == "CP949") { print "SELECTED"; } ?>>CP949
	<option <?php if ($charset == "CSA_Z243.4-1985-1") { print "SELECTED"; } ?>>CSA_Z243.4-1985-1
	<option <?php if ($charset == "CSA_Z243.4-1985-2") { print "SELECTED"; } ?>>CSA_Z243.4-1985-2
	<option <?php if ($charset == "CSA_Z243.4-1985-GR") { print "SELECTED"; } ?>>CSA_Z243.4-1985-GR
	<option <?php if ($charset == "CSN_369103") { print "SELECTED"; } ?>>CSN_369103
	<option <?php if ($charset == "CWI") { print "SELECTED"; } ?>>CWI
	<option <?php if ($charset == "DEC-MCS") { print "SELECTED"; } ?>>DEC-MCS
	<option <?php if ($charset == "DIN_66003") { print "SELECTED"; } ?>>DIN_66003
	<option <?php if ($charset == "DS_2089") { print "SELECTED"; } ?>>DS_2089
	<option <?php if ($charset == "EBCDIC-AT-DE") { print "SELECTED"; } ?>>EBCDIC-AT-DE
	<option <?php if ($charset == "EBCDIC-AT-DE-A") { print "SELECTED"; } ?>>EBCDIC-AT-DE-A
	<option <?php if ($charset == "EBCDIC-CA-FR") { print "SELECTED"; } ?>>EBCDIC-CA-FR
	<option <?php if ($charset == "EBCDIC-DK-NO") { print "SELECTED"; } ?>>EBCDIC-DK-NO
	<option <?php if ($charset == "EBCDIC-DK-NO-A") { print "SELECTED"; } ?>>EBCDIC-DK-NO-A
	<option <?php if ($charset == "EBCDIC-ES") { print "SELECTED"; } ?>>EBCDIC-ES
	<option <?php if ($charset == "EBCDIC-ES-A") { print "SELECTED"; } ?>>EBCDIC-ES-A
	<option <?php if ($charset == "EBCDIC-ES-S") { print "SELECTED"; } ?>>EBCDIC-ES-S
	<option <?php if ($charset == "EBCDIC-FI-SE") { print "SELECTED"; } ?>>EBCDIC-FI-SE
	<option <?php if ($charset == "EBCDIC-FI-SE-A") { print "SELECTED"; } ?>>EBCDIC-FI-SE-A
	<option <?php if ($charset == "EBCDIC-FR") { print "SELECTED"; } ?>>EBCDIC-FR
	<option <?php if ($charset == "EBCDIC-IS-FRISS") { print "SELECTED"; } ?>>EBCDIC-IS-FRISS
	<option <?php if ($charset == "EBCDIC-IT") { print "SELECTED"; } ?>>EBCDIC-IT
	<option <?php if ($charset == "EBCDIC-PT") { print "SELECTED"; } ?>>EBCDIC-PT
	<option <?php if ($charset == "EBCDIC-UK") { print "SELECTED"; } ?>>EBCDIC-UK
	<option <?php if ($charset == "EBCDIC-US") { print "SELECTED"; } ?>>EBCDIC-US
	<option <?php if ($charset == "ECMA-CYRILLIC") { print "SELECTED"; } ?>>ECMA-CYRILLIC
	<option <?php if ($charset == "ES") { print "SELECTED"; } ?>>ES
	<option <?php if ($charset == "ES2") { print "SELECTED"; } ?>>ES2
	<option <?php if ($charset == "EUC-JP") { print "SELECTED"; } ?>>EUC-JP
	<option <?php if ($charset == "EUC-KR") { print "SELECTED"; } ?>>EUC-KR
	<option <?php if ($charset == "EUC-TW") { print "SELECTED"; } ?>>EUC-TW
	<option <?php if ($charset == "GB18030") { print "SELECTED"; } ?>>GB18030
	<option <?php if ($charset == "GB2312") { print "SELECTED"; } ?>>GB2312
	<option <?php if ($charset == "GBK") { print "SELECTED"; } ?>>GBK
	<option <?php if ($charset == "GB_1988-80") { print "SELECTED"; } ?>>GB_1988-80
	<option <?php if ($charset == "GEORGIAN-ACADEMY") { print "SELECTED"; } ?>>GEORGIAN-ACADEMY
	<option <?php if ($charset == "GEORGIAN-PS") { print "SELECTED"; } ?>>GEORGIAN-PS
	<option <?php if ($charset == "GOST_19768-74") { print "SELECTED"; } ?>>GOST_19768-74
	<option <?php if ($charset == "GREEK-CCITT") { print "SELECTED"; } ?>>GREEK-CCITT
	<option <?php if ($charset == "GREEK7") { print "SELECTED"; } ?>>GREEK7
	<option <?php if ($charset == "GREEK7-OLD") { print "SELECTED"; } ?>>GREEK7-OLD
	<option <?php if ($charset == "HP-ROMAN8") { print "SELECTED"; } ?>>HP-ROMAN8
	<option <?php if ($charset == "IBM037") { print "SELECTED"; } ?>>IBM037
	<option <?php if ($charset == "IBM038") { print "SELECTED"; } ?>>IBM038
	<option <?php if ($charset == "IBM1004") { print "SELECTED"; } ?>>IBM1004
	<option <?php if ($charset == "IBM1026") { print "SELECTED"; } ?>>IBM1026
	<option <?php if ($charset == "IBM1047") { print "SELECTED"; } ?>>IBM1047
	<option <?php if ($charset == "IBM256") { print "SELECTED"; } ?>>IBM256
	<option <?php if ($charset == "IBM273") { print "SELECTED"; } ?>>IBM273
	<option <?php if ($charset == "IBM274") { print "SELECTED"; } ?>>IBM274
	<option <?php if ($charset == "IBM275") { print "SELECTED"; } ?>>IBM275
	<option <?php if ($charset == "IBM277") { print "SELECTED"; } ?>>IBM277
	<option <?php if ($charset == "IBM278") { print "SELECTED"; } ?>>IBM278
	<option <?php if ($charset == "IBM280") { print "SELECTED"; } ?>>IBM280
	<option <?php if ($charset == "IBM281") { print "SELECTED"; } ?>>IBM281
	<option <?php if ($charset == "IBM284") { print "SELECTED"; } ?>>IBM284
	<option <?php if ($charset == "IBM285") { print "SELECTED"; } ?>>IBM285
	<option <?php if ($charset == "IBM290") { print "SELECTED"; } ?>>IBM290
	<option <?php if ($charset == "IBM297") { print "SELECTED"; } ?>>IBM297
	<option <?php if ($charset == "IBM420") { print "SELECTED"; } ?>>IBM420
	<option <?php if ($charset == "IBM423") { print "SELECTED"; } ?>>IBM423
	<option <?php if ($charset == "IBM424") { print "SELECTED"; } ?>>IBM424
	<option <?php if ($charset == "IBM437") { print "SELECTED"; } ?>>IBM437
	<option <?php if ($charset == "IBM500") { print "SELECTED"; } ?>>IBM500
	<option <?php if ($charset == "IBM850") { print "SELECTED"; } ?>>IBM850
	<option <?php if ($charset == "IBM851") { print "SELECTED"; } ?>>IBM851
	<option <?php if ($charset == "IBM852") { print "SELECTED"; } ?>>IBM852
	<option <?php if ($charset == "IBM855") { print "SELECTED"; } ?>>IBM855
	<option <?php if ($charset == "IBM857") { print "SELECTED"; } ?>>IBM857
	<option <?php if ($charset == "IBM860") { print "SELECTED"; } ?>>IBM860
	<option <?php if ($charset == "IBM861") { print "SELECTED"; } ?>>IBM861
	<option <?php if ($charset == "IBM862") { print "SELECTED"; } ?>>IBM862
	<option <?php if ($charset == "IBM863") { print "SELECTED"; } ?>>IBM863
	<option <?php if ($charset == "IBM864") { print "SELECTED"; } ?>>IBM864
	<option <?php if ($charset == "IBM865") { print "SELECTED"; } ?>>IBM865
	<option <?php if ($charset == "IBM866") { print "SELECTED"; } ?>>IBM866
	<option <?php if ($charset == "IBM868") { print "SELECTED"; } ?>>IBM868
	<option <?php if ($charset == "IBM869") { print "SELECTED"; } ?>>IBM869
	<option <?php if ($charset == "IBM870") { print "SELECTED"; } ?>>IBM870
	<option <?php if ($charset == "IBM871") { print "SELECTED"; } ?>>IBM871
	<option <?php if ($charset == "IBM874") { print "SELECTED"; } ?>>IBM874
	<option <?php if ($charset == "IBM875") { print "SELECTED"; } ?>>IBM875
	<option <?php if ($charset == "IBM880") { print "SELECTED"; } ?>>IBM880
	<option <?php if ($charset == "IBM891") { print "SELECTED"; } ?>>IBM891
	<option <?php if ($charset == "IBM903") { print "SELECTED"; } ?>>IBM903
	<option <?php if ($charset == "IBM904") { print "SELECTED"; } ?>>IBM904
	<option <?php if ($charset == "IBM905") { print "SELECTED"; } ?>>IBM905
	<option <?php if ($charset == "IBM918") { print "SELECTED"; } ?>>IBM918
	<option <?php if ($charset == "IEC_P27-1") { print "SELECTED"; } ?>>IEC_P27-1
	<option <?php if ($charset == "INIS") { print "SELECTED"; } ?>>INIS
	<option <?php if ($charset == "INIS-8") { print "SELECTED"; } ?>>INIS-8
	<option <?php if ($charset == "INIS-CYRILLIC") { print "SELECTED"; } ?>>INIS-CYRILLIC
	<option <?php if ($charset == "INVARIANT") { print "SELECTED"; } ?>>INVARIANT
	<option <?php if ($charset == "ISIRI-3342") { print "SELECTED"; } ?>>ISIRI-3342
	<option <?php if ($charset == "ISO-8859-1" || !$charset) { print "SELECTED"; } ?>>ISO-8859-1
	<option <?php if ($charset == "ISO-8859-10") { print "SELECTED"; } ?>>ISO-8859-10
	<option <?php if ($charset == "ISO-8859-13") { print "SELECTED"; } ?>>ISO-8859-13
	<option <?php if ($charset == "ISO-8859-14") { print "SELECTED"; } ?>>ISO-8859-14
	<option <?php if ($charset == "ISO-8859-15") { print "SELECTED"; } ?>>ISO-8859-15
	<option <?php if ($charset == "ISO-8859-16") { print "SELECTED"; } ?>>ISO-8859-16
	<option <?php if ($charset == "ISO-8859-2") { print "SELECTED"; } ?>>ISO-8859-2
	<option <?php if ($charset == "ISO-8859-3") { print "SELECTED"; } ?>>ISO-8859-3
	<option <?php if ($charset == "ISO-8859-4") { print "SELECTED"; } ?>>ISO-8859-4
	<option <?php if ($charset == "ISO-8859-5") { print "SELECTED"; } ?>>ISO-8859-5
	<option <?php if ($charset == "ISO-8859-6") { print "SELECTED"; } ?>>ISO-8859-6
	<option <?php if ($charset == "ISO-8859-7") { print "SELECTED"; } ?>>ISO-8859-7
	<option <?php if ($charset == "ISO-8859-8") { print "SELECTED"; } ?>>ISO-8859-8
	<option <?php if ($charset == "ISO-8859-9") { print "SELECTED"; } ?>>ISO-8859-9
	<option <?php if ($charset == "ISO-IR-197") { print "SELECTED"; } ?>>ISO-IR-197
	<option <?php if ($charset == "ISO-IR-209") { print "SELECTED"; } ?>>ISO-IR-209
	<option <?php if ($charset == "ISO-IR-90") { print "SELECTED"; } ?>>ISO-IR-90
	<option <?php if ($charset == "ISO_10367-BOX") { print "SELECTED"; } ?>>ISO_10367-BOX
	<option <?php if ($charset == "ISO_10646") { print "SELECTED"; } ?>>ISO_10646
	<option <?php if ($charset == "ISO_2033-1983") { print "SELECTED"; } ?>>ISO_2033-1983
	<option <?php if ($charset == "ISO_5427") { print "SELECTED"; } ?>>ISO_5427
	<option <?php if ($charset == "ISO_5427-EXT") { print "SELECTED"; } ?>>ISO_5427-EXT
	<option <?php if ($charset == "ISO_5428") { print "SELECTED"; } ?>>ISO_5428
	<option <?php if ($charset == "ISO_646.BASIC") { print "SELECTED"; } ?>>ISO_646.BASIC
	<option <?php if ($charset == "ISO_646.IRV") { print "SELECTED"; } ?>>ISO_646.IRV
	<option <?php if ($charset == "ISO_6937") { print "SELECTED"; } ?>>ISO_6937
	<option <?php if ($charset == "ISO_6937-2-25") { print "SELECTED"; } ?>>ISO_6937-2-25
	<option <?php if ($charset == "ISO_6937-2-ADD") { print "SELECTED"; } ?>>ISO_6937-2-ADD
	<option <?php if ($charset == "ISO_8859-1,GL") { print "SELECTED"; } ?>>ISO_8859-1,GL
	<option <?php if ($charset == "ISO_8859-SUPP") { print "SELECTED"; } ?>>ISO_8859-SUPP
	<option <?php if ($charset == "IT") { print "SELECTED"; } ?>>IT
	<option <?php if ($charset == "JIS_C6220-1969-JP") { print "SELECTED"; } ?>>JIS_C6220-1969-JP
	<option <?php if ($charset == "JIS_C6220-1969-RO") { print "SELECTED"; } ?>>JIS_C6220-1969-RO
	<option <?php if ($charset == "JIS_C6229-1984-A") { print "SELECTED"; } ?>>JIS_C6229-1984-A
	<option <?php if ($charset == "JIS_C6229-1984-B") { print "SELECTED"; } ?>>JIS_C6229-1984-B
	<option <?php if ($charset == "JIS_C6229-1984-B-ADD") { print "SELECTED"; } ?>>JIS_C6229-1984-B-ADD
	<option <?php if ($charset == "JIS_C6229-1984-HAND") { print "SELECTED"; } ?>>JIS_C6229-1984-HAND
	<option <?php if ($charset == "JIS_C6229-1984-HAND-ADD") { print "SELECTED"; } ?>>JIS_C6229-1984-HAND-ADD
	<option <?php if ($charset == "JIS_C6229-1984-KANA") { print "SELECTED"; } ?>>JIS_C6229-1984-KANA
	<option <?php if ($charset == "JIS_X0201") { print "SELECTED"; } ?>>JIS_X0201
	<option <?php if ($charset == "JOHAB") { print "SELECTED"; } ?>>JOHAB
	<option <?php if ($charset == "JUS_I.B1.002") { print "SELECTED"; } ?>>JUS_I.B1.002
	<option <?php if ($charset == "JUS_I.B1.003-MAC") { print "SELECTED"; } ?>>JUS_I.B1.003-MAC
	<option <?php if ($charset == "JUS_I.B1.003-SERB") { print "SELECTED"; } ?>>JUS_I.B1.003-SERB
	<option <?php if ($charset == "KOI-8") { print "SELECTED"; } ?>>KOI-8
	<option <?php if ($charset == "KOI8-R") { print "SELECTED"; } ?>>KOI8-R
	<option <?php if ($charset == "KOI8-T") { print "SELECTED"; } ?>>KOI8-T
	<option <?php if ($charset == "KOI8-U") { print "SELECTED"; } ?>>KOI8-U
	<option <?php if ($charset == "KSC5636") { print "SELECTED"; } ?>>KSC5636
	<option <?php if ($charset == "LATIN-GREEK") { print "SELECTED"; } ?>>LATIN-GREEK
	<option <?php if ($charset == "LATIN-GREEK-1") { print "SELECTED"; } ?>>LATIN-GREEK-1
	<option <?php if ($charset == "MAC-CYRILLIC") { print "SELECTED"; } ?>>MAC-CYRILLIC
	<option <?php if ($charset == "MAC-IS") { print "SELECTED"; } ?>>MAC-IS
	<option <?php if ($charset == "MAC-SAMI") { print "SELECTED"; } ?>>MAC-SAMI
	<option <?php if ($charset == "MAC-UK") { print "SELECTED"; } ?>>MAC-UK
	<option <?php if ($charset == "MACINTOSH") { print "SELECTED"; } ?>>MACINTOSH
	<option <?php if ($charset == "MSZ_7795.3") { print "SELECTED"; } ?>>MSZ_7795.3
	<option <?php if ($charset == "NATS-DANO") { print "SELECTED"; } ?>>NATS-DANO
	<option <?php if ($charset == "NATS-DANO-ADD") { print "SELECTED"; } ?>>NATS-DANO-ADD
	<option <?php if ($charset == "NATS-SEFI") { print "SELECTED"; } ?>>NATS-SEFI
	<option <?php if ($charset == "NATS-SEFI-ADD") { print "SELECTED"; } ?>>NATS-SEFI-ADD
	<option <?php if ($charset == "NC_NC00-10") { print "SELECTED"; } ?>>NC_NC00-10
	<option <?php if ($charset == "NEXTSTEP") { print "SELECTED"; } ?>>NEXTSTEP
	<option <?php if ($charset == "NF_Z_62-010") { print "SELECTED"; } ?>>NF_Z_62-010
	<option <?php if ($charset == "NF_Z_62-010_(1973)") { print "SELECTED"; } ?>>NF_Z_62-010_(1973)
	<option <?php if ($charset == "NF_Z_62-010_1973") { print "SELECTED"; } ?>>NF_Z_62-010_1973
	<option <?php if ($charset == "NS_4551-1") { print "SELECTED"; } ?>>NS_4551-1
	<option <?php if ($charset == "NS_4551-2") { print "SELECTED"; } ?>>NS_4551-2
	<option <?php if ($charset == "PT") { print "SELECTED"; } ?>>PT
	<option <?php if ($charset == "PT2") { print "SELECTED"; } ?>>PT2
	<option <?php if ($charset == "SAMI") { print "SELECTED"; } ?>>SAMI
	<option <?php if ($charset == "SAMI-WS2") { print "SELECTED"; } ?>>SAMI-WS2
	<option <?php if ($charset == "SEN_850200_B") { print "SELECTED"; } ?>>SEN_850200_B
	<option <?php if ($charset == "SEN_850200_C") { print "SELECTED"; } ?>>SEN_850200_C
	<option <?php if ($charset == "SHIFT_JIS") { print "SELECTED"; } ?>>SHIFT_JIS
	<option <?php if ($charset == "T.101-G2") { print "selected"; } ?>>T.101-G2
	<option <?php if ($charset == "T.61-7BIT") { print "SELECTED"; } ?>>T.61-7BIT
	<option <?php if ($charset == "T.61-8BIT") { print "SELECTED"; } ?>>T.61-8BIT
	<option <?php if ($charset == "TIS-620") { print "SELECTED"; } ?>>TIS-620
	<option <?php if ($charset == "UTF-8") { print "SELECTED"; } ?>>UTF-8
	<option <?php if ($charset == "VIDEOTEX-SUPPL") { print "SELECTED"; } ?>>VIDEOTEX-SUPPL
	<option <?php if ($charset == "VISCII") { print "SELECTED"; } ?>>VISCII
	<option <?php if ($charset == "WIN-SAMI-2") { print "SELECTED"; } ?>>WIN-SAMI-2
	</select><br>
      <input type=submit><br>
    </form>

    <hr>
    <h2>Output</h2>

    <pre>
<?php
   if ($charset) {
     print "$ CHARSET=" .  escapeshellarg($charset) . "; export CHARSET\n";
     putenv("CHARSET=" . escapeshellarg($charset));
   }
   $cmd = "echo " . escapeshellarg( $data ? $data : "räksmörgås.josefsson.org") . " | /usr/local/bin/idn --debug" . ($allowunassigned ? " --allow-unassigned" : "") . ($usestd3asciirules ? " --usestd3asciirules" : "") . ($mode == "punydecode" ? " --punycode-decode" : "") . ($mode == "punyencode" ? " --punycode-encode" : "") . ($mode == "toascii" || !$mode ? " --idna-to-ascii" : "") . ($mode == "tounicode" ? " --idna-to-unicode" : "") . " 2>&1";
   $h = popen($cmd, "r");
   while($s = fgets($h, 1024)) { $out .= $s; };
   pclose($h);
   print "$ $cmd\n";
   print $out;
?>
</pre>

    <hr>
    <address>Simon Josefsson</address>
  </body>
</html>
