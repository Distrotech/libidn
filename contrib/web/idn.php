<html>

  <head>
    <title>Try GNU Libidn</title>
    <meta http-equiv='Content-Type' content='text/html; charset="<?php print $charset ? $charset : "ISO-8859-1" ?>"' />
  </head>

  <body>

    <h1>Try GNU Libidn</h1>

    <p>This page enable you to try the Punycode encode/decode and IDNA
    ToASCII/ToUnicode operations of <a
    href="http://www.gnu.org/software/libidn/">GNU Libidn</a>. Note
    that libidn contains more than this, for example a generic
    stringprep implementation with support for several stringprep
    profiles.

    <p>Report bugs to <A
    HREF="mailto:bug-libidn@gnu.org">bug-libidn@gnu.org</A>, but first
    please make sure your browser really is encoding the data you type
    in the charset you select.  If not, incorrect output or an error
    is the "correct" output.

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
      Force charset to: <select name=charset>
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
    <h2>Error codes</h2>

<pre>

  enum
  {
    PUNYCODE_SUCCESS = 0,
    PUNYCODE_BAD_INPUT,		/* Input is invalid.                       */
    PUNYCODE_BIG_OUTPUT,	/* Output would exceed the space provided. */
    PUNYCODE_OVERFLOW		/* Input needs wider integers to process.  */
  };

  /* Error codes. */
  enum
  {
    IDNA_SUCCESS = 0,
    IDNA_STRINGPREP_ERROR = 1,
    IDNA_PUNYCODE_ERROR = 2,
    IDNA_CONTAINS_LDH = 3,
    IDNA_CONTAINS_MINUS = 4,
    IDNA_INVALID_LENGTH = 5,
    IDNA_NO_ACE_PREFIX = 6,
    IDNA_ROUNDTRIP_VERIFY_ERROR = 7,
    IDNA_CONTAINS_ACE_PREFIX = 8,
    IDNA_ICONV_ERROR = 9,
    /* Internal errors. */
    IDNA_MALLOC_ERROR = 201
  };
</pre>

    <hr>
    <h2>Examples</h2>

    <ul>
	<li><a href="http://josefsson.org/idn.php?data=r%E4ksm%F6rg%E5s.josefsson.org&mode=toascii&charset=ISO-8859-1">ISO-8859-1 example</a>
	<li><a href="http://josefsson.org/idn.php?data=r%84ksm%94rg%86s.josefsson.org&mode=toascii&charset=IBM850">IBM850 (aka codepage 850) example (same as previous)</a>
	<li><a href="http://josefsson.org/idn.php?data=%E2%82%ACcu&mode=toascii&charset=UTF-8">UTF-8 Euro example</a>
	<li><a href="http://josefsson.org/idn.php?data=%A4cu&mode=toascii&charset=ISO-8859-15">ISO-8859-15 Euro example (same as previous)</a>
	<li><a href="http://josefsson.org/idn.php?data=%B0%C2%BC%BC%C6%E0%C8%FE%B7%C3-with-SUPER-MONKEYS&mode=toascii&charset=EUC-JP">EUC-JP example</a>
	<li><a href="http://josefsson.org/idn.php?data=%B9%CC%BC%FA&mode=toascii&charset=EUC-KR">EUC-KR example</a>
	<li><a href="http://josefsson.org/idn.php?data=%D9%84%D9%8A%D9%87%D9%85%D8%A7%D8%A8%D8%AA%D9%83%D9%84%D9%85%D9%88%D8%B4%D8%B9%D8%B1%D8%A8%D9%8A%D8%9F&mode=toascii&charset=UTF-8">UTF-8 Arabic (Egyptian)</a>
	<li><a href="http://josefsson.org/idn.php?data=%E4%BB%96%E4%BB%AC%E4%B8%BA%E4%BB%80%E4%B9%88%E4%B8%8D%E8%AF%B4%E4%B8%AD%E6%96%87&mode=toascii&charset=UTF-8">UTF-8 Simplified Chinese</a>
	<li><a href="http://josefsson.org/idn.php?data=%CB%FB%C3%C7%CE%AA%CA%B2%C3%B4%B2%BB%CB%B5%D6%D0%CE%C4&mode=toascii&charset=GB18030">GB18030 Simplified Chinese (same as previous)</a>
	<li><a href="http://josefsson.org/idn.php?data=%D7%9C%D7%9E%D7%94%D7%94%D7%9D%D7%A4%D7%A9%D7%95%D7%98%D7%9C%D7%90%D7%9E%D7%93%D7%91%D7%A8%D7%99%D7%9D%D7%A2%D7%91%D7%A8%D7%99%D7%AA&mode=toascii&charset=UTF-8">UTF-8 Hebrew</a>
	<li><a href="http://josefsson.org/idn.php?data=%D0%BF%D0%BE%D1%87%D0%B5%D0%BC%D1%83%D0%B6%D0%B5%D0%BE%D0%BD%D0%B8%D0%BD%D0%B5%D0%B3%D0%BE%D0%B2%D0%BE%D1%80%D1%8F%D1%82%D0%BF%D0%BE%D1%80%D1%83%D1%81%D1%81%D0%BA%D0%B8&mode=toascii&charset=UTF-8">UTF-8 Russian (Cyrillic)</a>
	<li><a href="http://josefsson.org/idn.php?data=%D0%CF%DE%C5%CD%D5%D6%C5%CF%CE%C9%CE%C5%C7%CF%D7%CF%D2%D1%D4%D0%CF%D2%D5%D3%D3%CB%C9&mode=toascii&charset=KOI8-R">KOI8-R Russian Cyrillic (same as previous)</a>
	<li><a href="http://josefsson.org/idn.php?data=%D0%CF%DE%C5%CD%D5%D6%C5%CF%CE%C9%CE%C5%C7%CF%D7%CF%D2%D1%D4%D0%CF%D2%D5%D3%D3%CB%C9&mode=toascii&charset=ECMA-CYRILLIC">ECMA-CYRILLIC Russian Cyrillic (same as previous)</a>
	<li><a href="http://josefsson.org/idn.php?data=T%E1%BA%A1isaoh%E1%BB%8Dkh%C3%B4ngth%E1%BB%83ch%E1%BB%89n%C3%B3iti%E1%BA%BFngVi%E1%BB%87t&mode=toascii&charset=UTF-8">UTF-8 Vietnamese</a>
	<li><a href="http://josefsson.org/idn.php?data=%E3%81%B2%E3%81%A8%E3%81%A4%E5%B1%8B%E6%A0%B9%E3%81%AE%E4%B8%8B2&mode=toascii&charset=UTF-8">UTF-8 Japanese</a>
	<li><a href="http://josefsson.org/idn.php?data=%A4%D2%A4%C8%A4%C4%B2%B0%BA%AC%A4%CE%B2%BC2&mode=toascii&charset=EUC-JP">EUC-JP Japanese (same as previous)</a>
	<li><a href="http://josefsson.org/idn.php?data=%82%D0%82%C6%82%C2%89%AE%8D%AA%82%CC%89%BA2&mode=toascii&charset=SHIFT_JIS">SHIFT_JIS Japanese (same as previous)</a>
	<li><a href="http://josefsson.org/idn.php?data=Pro%C4%8Dprost%C4%9Bnemluv%C3%AD%C4%8Desky&mode=toascii&charset=UTF-8">UTF-8 Czech</a>
	<li><a href="http://josefsson.org/idn.php?data=Pro%E8prost%ECnemluv%ED%E8esky&mode=toascii&charset=ISO-8859-2">ISO-8859-2 Czech (same as previous)</a>
	<li><a href="http://josefsson.org/idn.php?data=%E0%A4%AF%E0%A4%B9%E0%A4%B2%E0%A5%8B%E0%A4%97%E0%A4%B9%E0%A4%BF%E0%A4%A8%E0%A5%8D%E0%A4%A6%E0%A5%80%E0%A4%95%E0%A5%8D%E0%A4%AF%E0%A5%8B%E0%A4%82%E0%A4%A8%E0%A4%B9%E0%A5%80%E0%A4%82%E0%A4%AC%E0%A5%8B%E0%A4%B2%E0%A4%B8%E0%A4%95%E0%A4%A4%E0%A5%87%E0%A4%B9%E0%A5%88%E0%A4%82&mode=toascii&charset=UTF-8">UTF-8 Hindi Devanagari</a>
    </ul>

    <hr>
  </body>
</html>
