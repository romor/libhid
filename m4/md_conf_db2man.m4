AC_DEFUN([MD_CONF_DB2MAN],
  [
    for file in \
      /usr/share/sgml/docbook/stylesheet/xsl/nwalsh/manpages/docbook.xsl \
      /usr/share/sgml/docbook/xsl-stylesheets-\*/manpages/docbook.xsl \
      /sw/share/xml/xsl/docbook-xsl/manpages/docbook.xsl; do

      test -r $file && db2man_file=$file
    done

    if [[ -n "$db2man_file" ]]; then
      AC_MSG_NOTICE([Using db2man stylesheet: $db2man_file])
    else
      LOCATE=$(command -v locate)
      if test -x $LOCATE; then
        db2man_file=$($LOCATE docbook.xsl)
      fi

      if [[ -z "$db2man_file" -o ! -r "$db2man_file" ]]; then
        AC_ERROR([No appropriate db2man stylesheet found.])
      fi
    fi
    AM_CONDITIONAL(HAVE_DB2MAN, test -n "$db2man_file")
    AC_SUBST(DB2MAN_FILE, "$db2man_file")
  ])
