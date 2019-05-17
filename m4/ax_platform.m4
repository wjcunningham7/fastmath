###########################
#(C) Will Cunningham 2017 #
#         DK Lab          #
# Northeastern University #
###########################

AC_DEFUN([AX_PLATFORM],
[
AC_PROVIDE([AX_PLATFORM])

AC_ARG_WITH([platform], AS_HELP_STRING([--with-platform=PLATFORM], [Name of the platform added to the binary name]),
  [platform=$with_platform],
  [ if [[[ "$#" -ge 1 && "$1" != "" ]]] ; then
      platform="$1"
    else
      platform=""
    fi
  ])

AC_MSG_CHECKING([platform])
if [[[ $platform != "" ]]] ; then
  AC_MSG_RESULT([$platform])
  AC_SUBST([PLATFORM], [_$platform])
else
  AC_MSG_RESULT([generic])
fi
])
