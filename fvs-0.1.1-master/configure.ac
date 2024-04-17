dnl Process this file with autoconf to produce a configure script
AC_INIT(fvs)

########################################################################################
# Include version information
########################################################################################
. ${srcdir}/version.sh

echo "configuring ${PACKAGE_NAME} ${PACKAGE_VERSION}${PACKAGE_VERSION_ADDENDUM}"
AC_CANONICAL_SYSTEM
AC_SUBST(LIBRARY_CURRENT)
AC_SUBST(LIBRARY_REVISION)
AC_SUBST(LIBRARY_AGE)
AC_SUBST(PACKAGE_NAME)
AC_SUBST(PACKAGE_VERSION)
AC_SUBST(PACKAGE_LIB_VERSION)
AC_SUBST(PACKAGE_LIB_VERSION_NUMBER)
AC_SUBST(PACKAGE_RELEASE_DATE)
AC_SUBST(PACKAGE_VERSION_ADDENDUM)

# Ensure that make can run correctly
AM_SANITY_CHECK

# Initializing automake
AM_INIT_AUTOMAKE($PACKAGE_NAME,"${PACKAGE_VERSION}${PACKAGE_VERSION_ADDENDUM}", ' ')


########################################################################################
# Headers and other configuration files
########################################################################################
AC_CONFIG_HEADERS(include/config.h)
AC_CONFIG_FILES(Makefile src/Makefile include/Makefile demos/Makefile)

########################################################################################
# Checks for programs
########################################################################################
AC_PROG_CC
AC_PROG_LIBTOOL

########################################################################################
# Checks for libraries
########################################################################################
AC_CHECK_LIB(m, cos, , echo "math lib not found"; exit 1)

########################################################################################
# Checks for headers
########################################################################################
AC_STDC_HEADERS
AC_HAVE_HEADERS(string.h math.h)

########################################################################################
# Checks for ImageMagick
########################################################################################
AC_CHECKING(for ImageMagick support ...)
AC_DEFINE(HAVE_LIBMAGICK, 1, Defined if ImageMagick was installed on this system)
failed=0;
passed=0;
AC_CHECK_HEADER(magick/api.h,passed=`expr $passed + 1`,failed=`expr $failed + 1`)
AC_CHECK_LIB(Magick,ExportImagePixels, passed=`expr $passed + 1`, failed=`expr $failed + 1`)
AC_CHECK_LIB(Magick,ImportImagePixels, passed=`expr $passed + 1`, failed=`expr $failed + 1`)
AC_MSG_CHECKING(if ImageMagick package is complete)
if test $passed -gt 0
then
    if test $failed -gt 0
    then
	AC_MSG_RESULT(no -- maybe should a newer version of ImageMagick be installed)
	exit 1;
    else
	AC_DEFINE(HAVE_LIBMAGICK)
	AC_MSG_RESULT(yes)
    fi
else
    AC_MSG_RESULT(no, you have to install ImageMagick)
    exit 1;
fi


########################################################################################
# TYPES AND STRUCTS
########################################################################################

# Obtain size of an 'short' and define as SIZEOF_SHORT
AC_CHECK_SIZEOF(short)

# Obtain size of an 'unsigned short' and define as SIZEOF_UNSIGNED_SHORT
AC_CHECK_SIZEOF(unsigned short)

# Obtain size of an 'int' and define as SIZEOF_INT
AC_CHECK_SIZEOF(int)

# Obtain size of an 'unsigned int' and define as SIZEOF_UNSIGNED_INT
AC_CHECK_SIZEOF(unsigned int)

# Obtain size of a 'long' and define as SIZEOF_LONG
AC_CHECK_SIZEOF(long)

# Obtain size of a 'unsigned long' and define as SIZEOF_UNSIGNED_LONG
AC_CHECK_SIZEOF(unsigned long)

# Obtain size of a 'long long' and define as SIZEOF_LONG_LONG.  If
# 'long long' is not supported then the value defined is zero.
AC_CHECK_SIZEOF(long long)

# Obtain size of a 'unsigned long long' and define as
# SIZEOF_UNSIGNED_LONG_LONG.  If 'unsigned long long' is not
# supported then the value defined is zero.
AC_CHECK_SIZEOF(unsigned long long)

# Check types and define defaults if they are not defined
AC_CHECK_TYPE(uint, unsigned int)
AC_CHECK_TYPE(byte, char)
AC_CHECK_TYPE(ubyte, unsigned char)
AC_CHECK_TYPE(short, short)
AC_CHECK_TYPE(ushort, unsigned short)
AC_CHECK_TYPE(ulong, unsigned long)
AC_CHECK_TYPE(int8, char)
AC_CHECK_TYPE(uint8, unsigned char)
AC_CHECK_TYPE(int16, short)
AC_CHECK_TYPE(uint16, unsigned short)
AC_CHECK_TYPE(int32, int)
AC_CHECK_TYPE(uint32, unsigned int)
AC_CHECK_TYPE(int64, long)
AC_CHECK_TYPE(__int64, long long)
AC_CHECK_TYPE(uint64, unsigned long)
AC_CHECK_TYPE(float32, float)
AC_CHECK_TYPE(float64, double)

########################################################################################
# Generate files
########################################################################################
AC_OUTPUT

echo "

  Configure :
  -----------
    Source files in :     ${srcdir}
    Compiler :            ${CC}
    Preprocessor :        ${CPP}
    Compiler flags :      ${CFLAGS}
    Linker flags :        ${LDFLAGS}
    ImageMagick version : `Magick-config --version`
"


echo "Now type in make to build the files..."

