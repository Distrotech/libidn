/**
 * STRINGPREP_VERSION
 *
 * String defined via CPP denoting the header file version number.
 * Used together with stringprep_check_version() to verify header file
 * and run-time library consistency.
 */
#define STRINGPREP_VERSION

/**
 * stringprep_nameprep:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to the nameprep profile.
 * The AllowUnassigned flag is true, use
 * stringprep_nameprep_no_unassigned() for false AllowUnassigned.
 * Returns 0 iff successful, or an error code.
 **/
int
stringprep_nameprep (char *in, int maxlen)
{
}

/**
 * stringprep_nameprep_no_unassigned:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to the nameprep profile.
 * The AllowUnassigned flag is false, use stringprep_nameprep() for
 * true AllowUnassigned.  Returns 0 iff successful, or an error code.
 **/
int
stringprep_nameprep_no_unassigned (char *in, int maxlen)
{
}

/**
 * stringprep_kerberos5:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to the draft Kerberos5
 * stringprep profile.  Returns 0 iff successful, or an error code.
 **/
int
stringprep_kerberos5 (char *in, int maxlen)
{
}


/**
 * stringprep_generic:
 * @in: input/ouput array with string to prepare.
 * @maxlen: maximum length of input/output array.
 *
 * Prepare the input UTF-8 string according to a hypotetical "generic"
 * stringprep profile. This is mostly used for debugging or when
 * constructing new stringprep profiles. Returns 0 iff successful, or
 * an error code.
 **/
int
stringprep_generic (char *in, int maxlen)
{
}
